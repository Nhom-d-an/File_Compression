#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <queue> 
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stack>
#include <algorithm>

using namespace std;

//node for huffman structure
class huffmanNode {
public:
	char letter = '\0';
	double freq = 0;//the frequency of letter's apprearance
	bool leafNode = true;
	string code;//code to replace letter
	huffmanNode* left = NULL;
	huffmanNode* right = NULL;

	//constructor
	huffmanNode(char letter, double freq=0, huffmanNode* left=NULL, huffmanNode* right=NULL,bool leafNode=true) {
		this->letter = letter;
		this->freq = freq;
		this->left = left;
		this->right = right;
		this->leafNode = leafNode;
	}

	//default constructor
	huffmanNode() {

	}

	bool operator < (const huffmanNode& other) {
		return freq < other.freq;
	}

	//swap two node
	static void swapNode(huffmanNode*& a, huffmanNode* &b) {
		char letter = a->letter;
		double freq = a->freq;

		a->letter = b->letter;
		a->freq = b->freq;

		b->letter = letter;
		b->freq = freq;
	}

	//create a new node what is the same as
	huffmanNode* clone() {
		return new huffmanNode(letter, freq);
	}
};


class huffman {
	string inPath;//name file to compress
	string outPath;//name file to write encode what compressed

	huffmanNode* root;
	vector<huffmanNode*> frequency;//array have 256 elements indicate 256 character
	int size = 256;

	long long int bitWrited = 0;//number of bit what written to encoding file
	long long int byteWrited = 0; // number of byte what written to encoding file
	int bitPadded; // number of bit what written to encoding file

	//struct to compare two elements of the priority queue
	struct cmp {
		bool operator()(const huffmanNode* a, const huffmanNode* b) {
			return a->freq > b->freq;//compare frequency of letter of node
		}
	};

	//encode
	char* writebit(char*& res, int& beginbit, string codetowrite);
	void encodingTable(huffmanNode* node, string code = "");
	void writeTree(huffmanNode* node, ofstream& out);
	void writeEncodeText(ofstream& out);
	void writeEncodeTail(ofstream& out);

	void statistic();
	void buildHuffmanTree();
	void encodingTable();
	void writeTree();
	void writeEncoding();

	//decode
	void reBuilTree(huffmanNode*& node, char letter, ifstream& in);
	void readTail(ifstream& in);
	void writeDecode(ifstream& in);
public:

	void encodedFile();//compress file
	void decodedFile();//decompress file
	
	huffman(string inPath, string outPath);//constructor

	
};



