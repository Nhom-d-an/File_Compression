#pragma once
#include "List.h"
#include <iostream>
#include <fstream>
#include <vector>

enum {COMPRESS, DECOMPRESS};

typedef struct {
	unsigned char len;
	unsigned short int pos;
} compressData;

class PackedData {
public:
	unsigned short int data;

	PackedData(unsigned char len, unsigned short int pos);
	PackedData() : data(0) {};

	const unsigned char getLen();
	const unsigned short int getPos();
};

class LZ77
{
public:
	int option;
	static const int WIN_BUFSIZE = 4096;
	static const int NUM_POS_BITS = 12; // kiem tra
	static const int WIN_MASK = 4095;
	static const int PAT_BUFSIZE = 16;
	static const int NUM_LEN_BITS = 4; // kiem tra
	static const int MIN_MATCH_LEN = 2;
	static const int GET_BUFSIZE = 8192;
	static const int PUT_BUFSIZE = 8192;

	unsigned char windowBuffer[WIN_BUFSIZE];
	unsigned char pattern[PAT_BUFSIZE];
	unsigned char* getBuffer;
	unsigned char* getBufferStart;
	unsigned char* getBufferEnd;
	PackedData* putBuffer;
	PackedData* putBufferEnd;

	unsigned int windowCnt = 0, patCnt = 0, bufCnt = 0, putBufCnt = 0;
	unsigned int numberOfReadBytes;
	List* searchList;
	std::string fileInPath;
	std::string fileOutPath;
	std::ifstream in;
	std::ofstream out;

	LZ77(std::string fileInPath, std::string fileOutPath = "");
	~LZ77();

	void initBufferCompress();
	void initBufferDecompress();

	const compressData search(unsigned char window[], unsigned char pattern[]);
	void putEncodeData(compressData& data);
	int getCharFromFile();
	void compress();

	const compressData getEncodeData();
	void decompress();
};

