#include "LZ77.h"
#include <fstream>
#include <algorithm>
#include <string>

PackedData::PackedData(unsigned char len, unsigned short int pos) {
	data = len << LZ77::NUM_POS_BITS;
	data += pos;
}

// get len data from packed data
const unsigned char PackedData::getLen() {
	unsigned char len;
	len = (data >> LZ77::NUM_POS_BITS);
	return len;
}

// get pos data from packed data
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

// initialize all the buffer which is needed for compress operation
void LZ77::initBufferCompress() {
	in.read((char*)pattern, PAT_BUFSIZE);
	bufCnt = in.gcount();

	getBuffer = new unsigned char[GET_BUFSIZE];
	in.read((char*)getBuffer, GET_BUFSIZE);
	numberOfReadBytes = in.gcount(); 
	getBufferStart = getBuffer;
	getBufferEnd = getBuffer + numberOfReadBytes;

	putBuffer = new PackedData[PUT_BUFSIZE];
	putBufferEnd = putBuffer;
}

// initialize all the buffer which is needed for decompress operation
void LZ77::initBufferDecompress() {
	getBuffer = new unsigned char[GET_BUFSIZE];
	in.read((char*)getBuffer, GET_BUFSIZE);
	numberOfReadBytes = in.gcount(); 
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

// search for the longest matching string of window and pattern
// return the len and pos of the string
const compressData LZ77::search(unsigned char window[], unsigned char pattern[]) {
	int i, j, k;
	compressData data = { 1, 0 };
	int* list = searchList->list;
	int* next = searchList->next;

	// get the index of the character of pattern in list
	i = list[pattern[patCnt]];
	if (bufCnt > 1) {
		while (i != -1) {
			j = patCnt + 1;
			if (j == PAT_BUFSIZE) {
				j = 0;
			}

			k = 1;

			do {
				// if the j-th character is different with the adjacent character of the pattern[patCnt]
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
				// turn i, k into len and pos of the longest matching string
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

// write encoded data to compressed file
void LZ77::putEncodeData(compressData& data) {
	unsigned char len;
	unsigned short int pos;

	// if len < MIN_MATCH_LEN, write len = 0 and the character to file
	// if len > MIN_MATCH_LEN, write len and pos to file
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
		len = data.len - MIN_MATCH_LEN + 1;
		pos = data.pos;

		PackedData packed(len, pos);

		putBuffer[putBufCnt] = packed;
		putBufCnt++;

		if (putBufCnt >= PUT_BUFSIZE) {
			out.write((char*)putBuffer, PUT_BUFSIZE * sizeof(PackedData));
			putBufCnt = 0;
		}
	}

	int j = windowCnt;
	int i = 0;

	// delete the matching string from list and window buffer
	for (i = 0; i < data.len; i++, j++) {
		searchList->del(*(windowBuffer + (j & WIN_MASK)), j & WIN_MASK);

		*(windowBuffer + (j & WIN_MASK)) = *(pattern + (patCnt + i) % PAT_BUFSIZE);

		searchList->insert(*(windowBuffer + (j & WIN_MASK)), j & WIN_MASK);
	}

	// get new character into pattern buffer
	for (i = 0; i < data.len; i++) {
		int getData = getCharFromFile();
		if (getData != EOF) {
			*(pattern + (patCnt + i) % PAT_BUFSIZE) = (unsigned char)getData;
		}
		else {
			break;
		}
	}

	// update bufCnt with the number of byte that is read into pattern
	// update windowCnt using circular access
	bufCnt += i;
	windowCnt = (windowCnt + data.len) & WIN_MASK;
	patCnt = (patCnt + data.len);
	if (patCnt >= PAT_BUFSIZE) {
		patCnt -= PAT_BUFSIZE;
	}
}

// get char data from file
int LZ77::getCharFromFile() {
	int charData;

	if (numberOfReadBytes != 0) {
		charData = (int)(*getBuffer++);

		// if running out of get buffer, read more data from file into the get buffer
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

// compress the file
void LZ77::compress() {
	out = std::ofstream(fileOutPath, std::ios::binary | std::ios::out);
	option = COMPRESS;

	out << fileInPath << std::endl;

	initBufferCompress();

	compressData data;
	while (bufCnt > 0) {
		data = search(windowBuffer, pattern);
		
		putEncodeData(data);
	}

	// write the remain data in the put buffer into file
	if (putBufCnt > 0) {
		out.write((char*)putBuffer, putBufCnt * sizeof(PackedData));
	}
}

// decompress the file
void LZ77::decompress() {
	option = DECOMPRESS;

	std::string temp;

	// get the original file path
	getline(in, temp);

	fileOutPath = temp.substr(0, temp.size());
	int check = fileOutPath.find(".enFo");

	// check the folder name
	if (check != std::string::npos) {
		std::string token = ".deFo";
		
		fileOutPath.replace(check, 5, token);
		check = fileOutPath.find(".enFo");
	}

	std::ofstream out(fileOutPath, std::ios::binary | std::ios::out);

	compressData data;
	unsigned char i;

	// read until the end of file
	while (!in.eof()) {
		data = getEncodeData();

		if (data.len != 0) {
			data.len += (MIN_MATCH_LEN - 1);

			i = data.len;
			// read the matching string from window buffer at pos with length len
			while (i--) {
				pattern[i] = windowBuffer[(data.pos + i) & WIN_MASK];
			}
			i = data.len;
			// add the matching string to the window buffer
			while (i--) {
				windowBuffer[(windowCnt + i) & WIN_MASK] = pattern[i];
			}

			// write the matching string to file
			// update the windowCnt with circular access
			out.write((char*)pattern, data.len);
			windowCnt = (windowCnt + data.len) & WIN_MASK;
		}
		else {
			// get the character from the pos data
			unsigned char saveCharacter = (unsigned char)data.pos;
			out.put(saveCharacter);

			// add the character to window buffer
			// update the windowCnt
			windowBuffer[windowCnt] = saveCharacter;
			if ((++windowCnt) >= WIN_BUFSIZE) {
				windowCnt = 0;
			}
		}
	}
}

// read encoded data from file
const compressData LZ77::getEncodeData() {
	compressData data;
	PackedData packed;
	
	in.read((char*)&packed, 2);
	data.len = packed.getLen();
	data.pos = packed.getPos();

	return data;
}

