#include "huffman.h"

string huffman::readTextFile(string inpath) {
	ifstream in(inpath); //open file

	string text;
	string currentLine;

	while (getline(in, currentLine)) {//get content of file by reading one by one row of file
		text += currentLine + '\n';
	}

	in.close();
	return text.substr(0, text.size() - 1);//the character in the end of text is residual
}

map<char, double> huffman::statistic(string text) {
	map<char, double> result;

	for (int i = 0; i < text.size(); i++) {
		result[text[i]]++;
	}

	return result;
}

huffmanNode* huffman::buildHuffmanTree(const map<char, double>& symbolTable) {
	priority_queue<huffmanNode*, vector<huffmanNode*>, cmp> priorityQueue;
	//firstly, construct leaf node;
	for (auto it = symbolTable.begin(); it != symbolTable.end(); it++) {
		priorityQueue.push(new huffmanNode(it->first, it->second));
	}
	//modify tree into one Node
	while (priorityQueue.size() > 1) {
		huffmanNode* left = priorityQueue.top(); priorityQueue.pop();
		huffmanNode* right = priorityQueue.top(); priorityQueue.pop();

		//create new node
		priorityQueue.push(new huffmanNode('\0', left->freq + right->freq, left, right, false));
	}
	//last node
	huffmanNode* root = priorityQueue.top(); priorityQueue.pop();

	return root;
}


void huffman::encodingTable(map<char, string>& table, huffmanNode* node, string code) {
	if (node == NULL) {
		return;
	}

	//leaf node
	if (node->leafNode) {
		table[node->letter] = code;
		return;
	}
	//To execute recursion if that is not leaf node
	encodingTable(table, node->left, code + "0");
	encodingTable(table, node->right, code + "1");
}

string huffman::encodedText(map<char, string>& table, string& text) {
	string code;
	
	for (int i = 0; i < text.size(); i++) {
		
		code += table[text[i]];
	}

	return code;
}

string huffman::convertBinary(int x) {

	string result = itoaBin(x);

	return result;
}

char huffman::convertedChar(string& s) {
	for (int i = 0; i < s.size(); i++) {
		char c = s[i];

	}

	return 'c';
}

string huffman::itoaBin(int x){
	string result;

	bool isNegative = (x < 0) ? 1 : 0;
	while(x){
		if (x % 2 == 0) result = '0' + result;
		else result = '1' + result;

		x = x / 2;
	}

	int size = result.size();

	for (int i = 0; i < 8 - size; i++) {
		result = "0" + result;
	}

	if (isNegative) {
		for (int i = 0; i < 8; i++) {
			if (result[i] == '0') result[i] = '1';
			else result[i] = '0';
		}
		int i = 8;
		while (result[i] != '0')
			i--;
		for (; i < 8; i++) {
			if (result[i] == '0') result[i] = '1';
			else result[i] = '0';
		}
	}

	return result;
}

string huffman::convertBinary(char c) {
	if (c >= 65 && c <= 70) {
		return convertBinary(10 + (c - 65));
	}
	else {
		return convertBinary(atoi(&c));
	}
}
void huffman::paddedEncodingText(string &code) {
	//string result;
	int extraPadding = 8 - code.size() % 8;

	for (int i = 0; i < extraPadding; i++) {
		code += '0';
	}

	string paddedInfor = convertBinary(extraPadding);
	code = paddedInfor + code;

	

}

int huffman::convertedDecimal(string s) {
	int exponent = 0;
	int result = 0;

	for (int i = s.size() - 1; i >= 0; i--) {
		char c = s[i];
		result += atoi(&c) * pow(2, exponent);
		exponent++;
	}

	return result;
}

string huffman::substr(string s, int a, int b) {
	string result;
	for (int i = a; i < b; i++) {
		result += s[i];
	}

	return result;
}


void huffman::encodingFile(string& s, ofstream& out) {
	paddedEncodingText(s);
	char c = s.size() / 8;
	out.write((char*)&c, 1);

	for (int i = 0; i < s.size(); i += 8) {
		string code = substr(s, i, i + 8);
		
		int it= convertedDecimal(code);
		unsigned char c = convertedDecimal(code);
	
		out.write((char*)&c, 1);
	}

}



void huffman::writeCodingText(map<char, string>& table, string& code, string outPath) {

	ofstream out(outPath, ios::binary);
	if (!out)
	{
		cout << "Can't open file to write encoding" << endl;
		return;
	}

	unsigned char size = table.size();//number of characters encoded
	out.write((char*)&size, 1);

	for (auto it = table.begin(); it != table.end(); it++) {
		char c = it->first;
		string character;

		character += c;
		

		out << character;//write character of table
		//cout << character <<endl << it->second << endl;
		encodingFile(it->second, out); //write code of that character into file
	}

	encodingFile(code, out);//mainly encoding the content of file opened before

	out.close();
	
}

string huffman::decodingCodeFile(ifstream& in) {
	unsigned char ch;
	string result;
	while (in.read((char*)&ch,1)) {
		if (ch == '\n')
			break;

		string check;
		
		check += ch;
		if (ch != '\0')
			result = result + convertedBit(check);
		else
			result = result + itoaBin(0);
	}

	//result = convertedBit(result);
	result = removePadding(result);
	//cout << result << endl;
	return result;
}




void huffman::readEncodedFile(map<char, string>& table, string& code,string inPath) {
	string s;
	string encoded;
	char character;
	char c;

	ifstream in(inPath, ios::binary);
	unsigned char ch;

	in.read((char*)&ch, 1);
	int size = ch;//size of characters



	for (int i = 0; i < size; i++) {
		//read character

		in.get(character);
		char c;
		in.read((char*)&c, 1);
		int size = c;//size of code indicating character


		string result;
		for (int j = 0; j < size; j++) {
			in.read((char*)&ch, 1);
		
			result = result + convertBinary(ch);
		}

		//remove the code residual
		encoded = removePadding(result);
	
		//set encoded of character
		table[character] = encoded;
	}
	
	string result;
	int count = 0;

	in.read((char*)&ch, 1);//read size of encode to ignore it
	

	while (in.read((char*)&ch, 1)) {
		result += convertedBit(ch);//read and decode each character
	}

	//remove the code residual
	code = removePadding(result);
	
	in.close();
}

string huffman::convertedBit(char c) {

	string result;

	if (c == '\0') {
		result = result + itoaBin(0);

	}
	else
	{
		int decimal = char(c);
		result += convertBinary(decimal);
	}

	return result;
}

string huffman::convertedBit(string& code) {
	int i = 0;
	string result;
	int count = 0;
	//while(code[i])
	for(int i=0;i<code.size();i++)
	{
		char c = code[i];

		if (c == '\0') {
			result = result + itoaBin(0);
			
		}
		else
		{
			int decimal = char(c);
			result += convertBinary(decimal);
		}
	}

	return result;

}

string huffman::removePadding(string& code) {
	int extraPadding = convertedDecimal(code.substr(0, 8));

	//cout << code.size() - extraPadding << endl;
	//string result = code.substr(8, code.size() - extraPadding);
	string result = substr(code, 8, code.size() - extraPadding);
	//cout << result;
	return result;
}

string huffman::decodedText(map<char, string>& table, string& code) {
	string currentCode;
	string decodeText;

	for (int i = 0; i < code.size(); i++) {
		currentCode += code[i];
		
		for (auto it = table.begin(); it != table.end(); it++) {
			if (currentCode == it->second) {
				decodeText += it->first;
				currentCode = "";
				break;
			}
		}
	}

	return decodeText;
}

void huffman::writeDecodingText(string& text, string outPath) {
	ofstream out(outPath);

	if (!out) {
		cout << "Can't open file to write Decode" << endl;
		return;
	}

	out << text;
	out.close();
}