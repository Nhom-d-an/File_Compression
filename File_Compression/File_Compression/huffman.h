#pragma once
/*#include "priorityQueue.h"
#include <map>
#include <fstream>
class nodeHuffman {
public:
	char letter;
	double freq;
	bool leafNode;
	nodeHuffman* left;
	nodeHuffman* right;
	nodeHuffman* parent;

	nodeHuffman(char Letter, double Freq, nodeHuffman* Left = NULL, nodeHuffman* Right = NULL) {
		letter = Letter;
		freq = Freq;
		left = Left;
		right = Right;
		parent = NULL;
		leafNode = false;

		Left->parent = this;
		Right->parent = this;
	}

	nodeHuffman(double Freq, nodeHuffman* Left = NULL, nodeHuffman* Right = NULL) {
		letter = '\n';
		freq = Freq;
		left = Left;
		right = Right;
		parent = NULL;
		leafNode = true;
	}

	bool operator<=(const nodeHuffman& other) {
		return freq <= other.freq;
	}
};

class huffman
{

public:
	//main factor
	nodeHuffman* root;
	//supported factor;
	string text;
	int length;
	map<char, double> statistics;
	map<char, nodeHuffman*> leaves;

	huffman() {
		root = NULL;
		text = "";
		length = 0;
	}

	

	void decode(string nameIn, string nameOut) {
		ifstream in(nameIn);
		if (!in) {
			cout << "File " << nameIn << " not Found" << endl;
		}


		char chacracter;
		//read coded file already
		while (in.get(chacracter)) {
			text += chacracter;
			length++;
		}
	}

	void buildTree() {
		priorityQueue<nodeHuffman*> queue;

		//build the tree by frequency of character's appearance
		
		//firstly, construct the leaves
		for (auto it = statistics.begin(); it != statistics.end(); it++) {
			leaves[it->first] = new nodeHuffman(it->first, it->second);
			queue.push(leaves[it->first]);
		}
		//modify priority queue by generating children
		while (queue.size > 1) {
			nodeHuffman* left = queue.pop();
			nodeHuffman* right = queue.pop();

			queue.push(new nodeHuffman(left->freq + right->freq, left, right));
		}
		
		root = queue.pop();
	}

	string encode(char c) {
		string code = "";
		nodeHuffman* current = leaves[c];

		while (current->parent )
		{
			if (current == current->parent->left) code += "0";//left child
			else code += "1";//right child

			current = current->parent;
		}

		reverse(code.begin(), code.end());//reverse code

		return code;
	}

	string encode() {
		string code = "";
		int i = 0;
		nodeHuffman* current;

		for (int i = 0; i < length; i++) {
			code += encode(text[i]);
		}

		return code;
	}

	string decode() {
		string code = "";
		int i = 0;
		nodeHuffman* current;


	}
};

void encode(string nameIn, string nameOut) {
	ifstream in(nameIn);
	if (!in) {
		cout << "File " << nameIn << " not Found" << endl;
	}

	huffman algorithm;
	char chacracter;
	//read file and calculate the frequency of each character of file
	while (in.get(chacracter)) {
		algorithm.text += chacracter;
		algorithm.length++;
		algorithm.statistics[chacracter]++;
	}
	// calculate probility the frequency
	for (auto it = algorithm.statistics.begin(); it != algorithm.statistics.end(); it++) {
		algorithm.statistics[it->first] = it->second / algorithm.length;
	}

	algorithm.buildTree();//buld tree for algorithm Huffman
	string code = algorithm.encode(); //actualy encode

	in.close();//close inport file

	ofstream out(nameOut);

	if (!out) {
		cout << "Can't not open file " << nameOut << endl;
		return;
	}

	out << code;
	out.close();
}*/
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
	//huffmanNode* root;
	struct cmp {
		bool operator()(const huffmanNode* a, const huffmanNode* b) {
			return a->freq > b->freq;
		}
	};

	string convertBinary(int x);
	int convertedDecimal(string s);
	string substr(string s,int a, int b);
public:
	string readTextFile(ifstream& in);
	map<char, double> statistic(string text);
	huffmanNode* buildHuffmanTree(const map<char, double>& symbolTable);
	void encodingTable(map<char, string> &table, huffmanNode* node, string code = "");
	string encodedText(map<char, string>& table, string& text);
	void paddedEncodingText(string &code);
	vector<int> decimalCodingArray(string& code);
	void writeCodingText(map<char, string>& table, string& code, ofstream& out);
	char convertedBinToHex(string s);

	void readEncodedFile(map<char, string>& table, string& code, ifstream& in);
	string convertedBit(string& code);
	string removePadding(string& code);
	string decodedText(map<char, string>& table, string& code);
	void writeDecodingText(string& text, ofstream& out);
};



