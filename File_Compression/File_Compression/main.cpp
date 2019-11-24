#include "huffman.h"

int main() {
	huffman alg;
	//encoding
	ifstream in("in.txt");
	string s = alg.readTextFile(in);
	in.close();

	map<char, double> maps = alg.statistic(s);
	huffmanNode* root = alg.buildHuffmanTree(maps);
	map<char, string> table;
	alg.encodingTable(table, root);
	string code=alg.encodedText(table, s);
	alg.paddedEncodingText(code);
	
	ofstream out("out_encoding.txt");
	alg.writeCodingText(table, code, out);
	cout << "encoding done" << endl;
	out.close();

	//decoding
	map<char, string> ta;
	string en;
	in.open("out_encoding.txt");
	if (!in) return 0;
	alg.readEncodedFile(ta, en, in);
	en = alg.convertedBit(en);
	en = alg.removePadding(en);
	string text = alg.decodedText(ta, en);
	in.close();

	out.open("out_decoding.txt");
	alg.writeDecodingText(text, out);
	cout << "decoding done" << endl;
	out.close();

	return 0;
}