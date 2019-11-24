#include "huffman.h"

const string inFile = "in.txt";
const string outFile = "in.bin";

string huffman::readTextFile(ifstream &in) {
	string text;
	string currentLine;
	if (!in) {
		cout << "File not Found" << endl;
		exit(0);
	}

	while (getline(in, currentLine)) {
		text += currentLine + '\n';
	}

	return text.substr(0, text.size() - 1);
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

/*huffmanNode* huffman::buildHuffmanTree(const map<char, double>& symbolTable) {
	priorityQueue<huffmanNode*> pq;
	//firstly, construct leaf node
	for (auto it = symbolTable.begin(); it != symbolTable.end(); it++) {
		pq.push(new huffmanNode(it->first, it->second));
	}
	//modify tree into one Node
	while (pq.size > 1) {
		huffmanNode* left = pq.pop();
		huffmanNode* right = pq.pop();

		//create new node
		pq.push(new huffmanNode('\0', left->freq + right->freq, left, right, false));
	}

	//last node
	huffmanNode* root = pq.pop();

	return root;
}*/

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
	char* s = new char[5];
	_itoa(x, s, 2);

	string result = s;
	int size = result.size();

	for (int i = 0; i < 4-size; i++) {
		result = "0" + result;
	}

	return result;
}
void huffman::paddedEncodingText(string &code) {
	int extraPadding = 4 - code.size() % 4;

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

vector<int> huffman::decimalCodingArray(string& code) {
	vector<int> result;

	for (int i = 0; i < code.size(); i += 8) {
		string s = substr(code, i, i + 8);
		result.push_back(convertedDecimal(s));
	}


	return result;
}

char huffman::convertedBinToHex(string s) {
	int result = 0;

	for (int count = 0; count < s.size(); ++count)
	{
		result *= 2;
		result += s[count] == '1' ? 1 : 0;
	}

	char c;
	if (result > 9) {
		c = 65 + (result - 10);
	}
	else
		c = 48 + result;

	return c;

}

void huffman::writeCodingText(map<char, string>& table, string& code, ofstream& out) {
	
	out << table.size() << " ";

	for (auto it = table.begin(); it != table.end(); it++) {
		out << it->first << " " << it->second << " ";
	}
	out << "\n";
	
	for (int i = 0; i < code.size(); i += 4) {
		string s = substr(code, i, i + 4);
		out << convertedBinToHex(s);
	}


}

void huffman::readEncodedFile(map<char, string>& table, string& code, ifstream& in){
	char c;
	string encoded;

	int size;
	in >> size;

	for (int i = 0; i < size; i++) {
		in >> c;
		in >> encoded;
		table[c] = encoded;
	}
	in.get(c); in.get(c);
	
	getline(in, code);
}

string huffman::convertedBit(string& code) {
	int i = 0;
	string result;
	while (code[i]) {
		char c = code[i];
		result += convertBinary(atoi(&c));
		i++;
	}

	return result;

}

string huffman::removePadding(string& code) {
	int extraPadding = convertedDecimal(code.substr(0, 4));

	string result = code.substr(4, code.size() - extraPadding);

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

void huffman::writeDecodingText(string& text, ofstream& out) {
	out << text;
}