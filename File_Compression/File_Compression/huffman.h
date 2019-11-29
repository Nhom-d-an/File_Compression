#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <queue> 
#include <stdlib.h>
#include <math.h>


using namespace std;

class huffmanNode {
public:
	char letter;
	double freq;
	bool leafNode;
	huffmanNode* left;
	huffmanNode* right;

	huffmanNode(char letter, double freq, huffmanNode* left=NULL, huffmanNode* right=NULL,bool leafNode=true) {
		this->letter = letter;
		this->freq = freq;
		this->left = left;
		this->right = right;
		this->leafNode = leafNode;
	}

	bool operator >= (const huffmanNode& other) {
		return freq >= other.freq;
	}
};

class huffman {
	//struct to compare two elements of the map
	struct cmp {
		bool operator()(const huffmanNode* a, const huffmanNode* b) {
			return a->freq > b->freq;
		}
	};

	string convertBinary(int x);
	string convertBinary(char c);
	int convertedDecimal(string s);
	string substr(string s,int a, int b);
	string itoaBin(int x);
	char convertedChar(string& s);
	string convertedBit(string& code);
public:
	//encode
	string readTextFile(string InPath);
	map<char, double> statistic(string text);
	huffmanNode* buildHuffmanTree(const map<char, double>& symbolTable);
	void encodingTable(map<char, string> &table, huffmanNode* node, string code = "");
	string encodedText(map<char, string>& table, string& text);
	void paddedEncodingText(string &code);
	void encodingFile(string& s, ofstream &out);
	void writeCodingText(map<char, string>& table, string& code, string outPath);
	
	//decode
	void readEncodedFile(map<char, string>& table, string& code, string inPath);
	string decodingCodeFile(ifstream &in);
	string convertedBit(char c);
	string removePadding(string& code);
	string decodedText(map<char, string>& table, string& code);
	void writeDecodingText(string& text, string outPath);
};



