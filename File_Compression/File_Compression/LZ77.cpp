#include "LZ77.h"
#include <fstream>
#include <algorithm>
#include <string>

PackedData::PackedData(unsigned char len, unsigned short int pos) {
	data = len << LZ77::NUM_POS_BITS;
	data += pos;
}

const unsigned char PackedData::getLen() {
	unsigned char len;
	len = (data >> LZ77::NUM_POS_BITS);
	return len;
}

const unsigned short int PackedData::getPos() {
	unsigned short int pos;
	pos = (data << LZ77::NUM_LEN_BITS);
	pos = pos >> LZ77::NUM_LEN_BITS;
	return pos;
}

LZ77::LZ77(std::string fileInPath, std::string fileOutPath) {
	std::fill(windowBuffer, windowBuffer + WIN_BUFSIZE, 32);
	searchList = new List(WIN_BUFSIZE, windowBuffer);
	this->fileInPath = fileInPath;
	this->fileOutPath = fileOutPath;

	in = std::ifstream(fileInPath, std::ios::binary | std::ios::in);
}

void LZ77::initBufferCompress() {
	in.read((char*)pattern, PAT_BUFSIZE);
	bufCnt = in.gcount();

	getBuffer = new unsigned char[GET_BUFSIZE];
	in.read((char*)getBuffer, GET_BUFSIZE);
	numberOfReadBytes = in.gcount(); // kiem tra
	getBufferStart = getBuffer;
	getBufferEnd = getBuffer + numberOfReadBytes;

	putBuffer = new PackedData[PUT_BUFSIZE];
	putBufferEnd = putBuffer;
}

void LZ77::initBufferDecompress() {
	getBuffer = new unsigned char[GET_BUFSIZE];
	in.read((char*)getBuffer, GET_BUFSIZE);
	numberOfReadBytes = in.gcount(); // kiem tra
	getBufferStart = getBuffer;
	getBufferEnd = getBuffer + numberOfReadBytes;
}

LZ77::~LZ77() {
	delete searchList;

	if (option == COMPRESS) {
		delete[] getBuffer;
		delete[] putBuffer;
	}
	
	in.close();
	out.close();
}

const compressData LZ77::search(unsigned char window[], unsigned char pattern[]) {
	int i, j, k;
	compressData data = { 1, 0 };
	int* list = searchList->list;
	int* next = searchList->next;

	i = list[pattern[patCnt]];
	if (bufCnt > 1) {
		while (i != -1) {
			j = patCnt + 1;
			if (j == PAT_BUFSIZE) {
				j = 0;
			}

			k = 1;

			do {
				if (pattern[j] != window[(i + k) & WIN_MASK]) {
					break;
				}

				j++;
				if (j == PAT_BUFSIZE) {
					j = 0;
				}
				k++;
			} while (k < bufCnt);

			if (k > data.len) {
				data.pos = (unsigned short int)i;
				data.len = (unsigned char)k;

				if (k == bufCnt) {
					break;
				}
			}

			i = next[i];
		}
	}

	if (data.len >= MIN_MATCH_LEN) {
		bufCnt -= data.len;
	}
	else {
		bufCnt -= 1;
	}

	return data;
}

void LZ77::putEncodeData(compressData& data) {
	unsigned char len;
	unsigned short int pos;

	if (data.len < MIN_MATCH_LEN) {
		data.len = 1;
		len = 0;

		unsigned short int saveCharacter;
		saveCharacter = (unsigned short int)pattern[patCnt];
		PackedData packed(len, saveCharacter);

		putBuffer[putBufCnt] = packed;
		putBufCnt++;

		if (putBufCnt >= PUT_BUFSIZE) {
			out.write((char*)putBuffer, PUT_BUFSIZE * sizeof(PackedData));
			putBufCnt = 0;
		}
	}
	else {
		//len = data.len - MIN_MATCH_LEN + 2
		len = data.len - MIN_MATCH_LEN + 1;
		pos = data.pos;

		PackedData packed(len, pos);
		/*out.write((char*)&packed, sizeof(PackedData));*/

		putBuffer[putBufCnt] = packed;
		putBufCnt++;

		if (putBufCnt >= PUT_BUFSIZE) {
			out.write((char*)putBuffer, PUT_BUFSIZE * sizeof(PackedData));
			putBufCnt = 0;
		}
	}

	int j = windowCnt;
	int i = 0;
	for (i = 0; i < data.len; i++, j++) {
		searchList->del(*(windowBuffer + (j & WIN_MASK)), j & WIN_MASK);

		*(windowBuffer + (j & WIN_MASK)) = *(pattern + (patCnt + i) % PAT_BUFSIZE);

		searchList->insert(*(windowBuffer + (j & WIN_MASK)), j & WIN_MASK);
	}

	for (i = 0; i < data.len; i++) {
		int getData = getCharFromFile();
		if (getData != EOF) {
			*(pattern + (patCnt + i) % PAT_BUFSIZE) = (unsigned char)getData;
		}
		else {
			break;
		}
	}

	bufCnt += i;
	windowCnt = (windowCnt + data.len) & WIN_MASK;
	patCnt = (patCnt + data.len);
	if (patCnt >= PAT_BUFSIZE) {
		patCnt -= PAT_BUFSIZE;
	}
}

int LZ77::getCharFromFile() {
	int charData;

	if (numberOfReadBytes != 0) {
		charData = (int)(*getBuffer++);

		if (getBuffer == getBufferEnd) {
			getBuffer = getBufferStart;
			in.read((char*)getBuffer, GET_BUFSIZE);
			numberOfReadBytes = in.gcount();
			getBufferEnd = getBuffer + numberOfReadBytes;
		}
		return charData;
	}
	else return EOF;
}

void LZ77::compress() {
	out = std::ofstream(fileOutPath, std::ios::binary | std::ios::out);
	option = COMPRESS;

	out << fileInPath << std::endl;

	initBufferCompress();

	//std::ofstream testLog("log.txt", std::ios::out);
	compressData data;
	while (bufCnt > 0) {
		data = search(windowBuffer, pattern);
		//testLog << (unsigned short int)data.len << " " << data.pos;\
		
		putEncodeData(data);
	}

	if (putBufCnt > 0) {
		out.write((char*)putBuffer, putBufCnt * sizeof(PackedData));
	}
	//testLog.close();
}

void LZ77::decompress() {
	option = DECOMPRESS;

	std::string temp;

	getline(in, temp);

	fileOutPath = temp.substr(0, temp.size());
	std::ofstream out(fileOutPath, std::ios::binary | std::ios::out);

	compressData data;
	unsigned char i;

	while (!in.eof()) {
		data = getEncodeData();

		if (data.len != 0) {
			data.len += (MIN_MATCH_LEN - 1);

			i = data.len;
			while (i--) {
				pattern[i] = windowBuffer[(data.pos + i) & WIN_MASK];
			}
			i = data.len;
			while (i--) {
				windowBuffer[(windowCnt + i) & WIN_MASK] = pattern[i];
			}

			out.write((char*)pattern, data.len);
			windowCnt = (windowCnt + data.len) & WIN_MASK;
		}
		else {
			unsigned char saveCharacter = (unsigned char)data.pos;
			out.put(saveCharacter);
			windowBuffer[windowCnt] = saveCharacter;
			if ((++windowCnt) >= WIN_BUFSIZE) {
				windowCnt = 0;
			}
		}
	}

	//data = getEncodeData();
}

const compressData LZ77::getEncodeData() {
	compressData data;
	PackedData packed;
	//std::fstream testFile("test.txt", std::ios::out | std::ios::binary);
	//unsigned char* test = new unsigned char[100];
	//in.read((char*)test, 100);
	//testFile.write((char*)test, 100);
	in.read((char*)&packed, 2);
	data.len = packed.getLen();
	data.pos = packed.getPos();

	return data;
}

