#pragma once
#include "List.h"
#include <iostream>
#include <fstream>
#include <vector>

enum {COMPRESS, DECOMPRESS};

// struct that hold the len and pos data
typedef struct {
	unsigned char len;
	unsigned short int pos;
} compressData;

// encoded data
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
	static const int WIN_BUFSIZE = 4096; // size of the window buffer
	static const int NUM_POS_BITS = 12;	// size of the pos data
	static const int WIN_MASK = 4095; // bit masking 
	static const int PAT_BUFSIZE = 16; // size of the pattern buffer (look ahead buffer)
	static const int NUM_LEN_BITS = 4; // size of the len data
	static const int MIN_MATCH_LEN = 2; // the minimum match length for the algorithm
	static const int GET_BUFSIZE = 8192; // the size of the get buffer
	static const int PUT_BUFSIZE = 8192; // the size of the put buffer

	unsigned char windowBuffer[WIN_BUFSIZE];
	unsigned char pattern[PAT_BUFSIZE];
	unsigned char* getBuffer;
	unsigned char* getBufferStart;
	unsigned char* getBufferEnd;
	PackedData* putBuffer;
	PackedData* putBufferEnd;

	// windowCnt, patCnt the current used position of the window buffer and pattern buffer
	// bufCnt the remaining character in pattern buffer
	unsigned int windowCnt = 0, patCnt = 0, bufCnt = 0, putBufCnt = 0; // all index variable which is used
	unsigned int numberOfReadBytes; // the number of bytes which is read from file in the get buffer
	List* searchList; // the list save the character of window buffer, used to improve search algorithm
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

