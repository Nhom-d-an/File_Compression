#include "functionUtilities.h"

bool checkInPathEnFormat(string inPath, string& outPath) {
	string format = inPath.substr(inPath.size() - 4, inPath.size());//take the end of path: .txt

	if (format != ".txt") return false;//format of file is wrong

	outPath = inPath.substr(0, inPath.size() - 4) + ".enFi";
	return true;
}

bool checkInPathDeFormat(string inPath, string& outPath) {
	string format = inPath.substr(inPath.size() - 5, inPath.size());//take the end of path: .txt

	if (format != ".enFi") return false;//format of file is wrong

	outPath = inPath.substr(0, inPath.size() - 4) + ".txt";
	return true;
}

bool checkInPathOpen(string inPath) {
	ifstream in(inPath);

	if (!in) {//cannot open file
		return false;
	}

	in.close();
	return true;
}

void encodingFile() {

	cout << "Enter the path to the File needed to encode" << endl;

	string inPath;
	string outPath;
	cin >> inPath;

	if (!checkInPathEnFormat(inPath, outPath)) {
		cout << "File have no right Format" << endl;
		return;
	}

	if (!checkInPathOpen(inPath)) {
		cout << "File not Found" << endl;
		return;
	}

	cout <<endl<< "Encoding..." << endl;

	huffman algorithm;
	string text = algorithm.readTextFile(inPath);//read text
	map<char, double> stat = algorithm.statistic(text);//statistic the appearances's frequently of each character
	huffmanNode* root = algorithm.buildHuffmanTree(stat);//build the tree of algorithm
	map<char, string> table;
	algorithm.encodingTable(table, root);//build the table indicating code of each character
	string encode = algorithm.encodedText(table, text);//transfering text to encode
	algorithm.writeCodingText(table, encode, outPath);//write encoding 

	cout << "Encoding Successfully" << endl;

}


void decodingFile() {

	cout << "Enter the path to the File needed to decode:" << endl;

	string inPath;
	string outPath;
	cin >> inPath;

	if (!checkInPathDeFormat(inPath, outPath)) {
		cout << "File have no right Format" << endl;
		return;
	}

	if (!checkInPathOpen(inPath)) {
		cout << "File not Found" << endl;
		return;
	}

	cout <<endl<< "Decoding..." << endl;

	huffman algorithm;
	map<char, string> table; //the table to indicate code of each character
	string code;//save the encoded
	algorithm.readEncodedFile(table, code, inPath);
	string text = algorithm.decodedText(table, code);//transfer code to text
	algorithm.writeDecodingText(text, outPath);

	cout << "Decoding Succesfully" << endl;
}
