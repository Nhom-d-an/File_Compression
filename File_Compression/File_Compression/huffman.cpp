#include "huffman.h"

//constructor
huffman::huffman(string inPath, string outPath) {
	this->inPath = inPath;
	this->outPath = outPath;

	frequency.resize(256);

	for (int i = 0; i < 256; i++) {
		frequency[i] = new huffmanNode(i);
	}

	root = NULL;
}


//statistic the frequency of letter
void huffman::statistic() {

	ifstream in(inPath, ios::binary || ios::out);

	char c;
	while (in.read(&c,1)) {
		unsigned char c1 = c;
		frequency[(int)c1]->freq++;//increasing frequency
	}

	in.close();
}

void huffman::buildHuffmanTree() {
	priority_queue<huffmanNode*, vector<huffmanNode*>, cmp> priorityQueue;

	vector<huffmanNode*> t;
	//modify the letters have no appearance
	for (int i = 0; i < size; i++) {
		if (frequency[i]->freq)
			t.push_back(frequency[i]->clone());
	}
	//if there is only one letter , let clone the one has the same content
	if (t.size() == 1) {
		t.push_back(t[0]->clone());
	}
	//arrangeing the array based the frequency of letter
	double min = t[0]->freq;
	int idx = 0;
	for (int i = 0; i < t.size()-1; i++) {
		for (int j = i+1; j < t.size(); j++) {
			if (t[j]->freq < t[i]->freq) {

				huffmanNode::swapNode(t[i], t[j]);
			}

		}
	}
	
	//bulid the priority queue
	for (int i = 0; i < t.size(); i++) {
			priorityQueue.push(t[i]);
	}

	//modify queue to there is only one node
	while (priorityQueue.size() > 1) {
		huffmanNode* left = priorityQueue.top(); priorityQueue.pop();
		huffmanNode* right = priorityQueue.top(); priorityQueue.pop();

		//create new node
		priorityQueue.push(new huffmanNode('\0', left->freq + right->freq, left, right, false));
	}
	//last node is root
	root = priorityQueue.top(); priorityQueue.pop();
}

//build the code to replace each letter have appearanced
void huffman::encodingTable(huffmanNode* node, string code) {
	
	if (node == NULL) {
		return;
	}
	
	//leaf node
	unsigned char idx;
	if (node->leafNode) 
	{
		idx = node->letter; // get index of array
		frequency[(int)idx]->code = code;//set code for letter
	}
	//if node isn't leaf node
	encodingTable(node->left, code+"0");//left node
	encodingTable(node->right, code+"1");//right node
}

void huffman::encodingTable() {
	encodingTable(root);
}

//write code to array character 
char* huffman::writebit(char*& res, int& beginbit, string codetowrite) {
	int j = 0;
	for (int i = 0; i < codetowrite.size(); i++) {

		if (codetowrite[i] == '0')//letter is '0'
		{
			(*res) &= ~(1 << (7 - beginbit));//write 0
		}
		else
		{
			(*res) |= 1 << (7 - beginbit);//write 1
		}
		beginbit++;//increase position

		if (beginbit == 8) {
			beginbit = 0;
			res += 1;//increase pointer
		}

	}

	return res;
}

//write tree to file
void huffman::writeTree(huffmanNode* node,ofstream& out) {
	if (node->leafNode) {//leaf node
		out << "1";
		out << node->letter;//write letter
	}
	else {
		out << "0";

		writeTree(node->left, out);//left node
		writeTree(node->right, out);//right node
	}

}

//write tree to file
void huffman::writeTree() {
	ofstream out(outPath, ios::binary);

	writeTree(root, out);
}

//encode text then writte to encoding file
void huffman::writeEncodeText(ofstream& out) {
	ifstream in(inPath, ios::binary);


	long long int count = 0;
	char c[32] = { 0 };
	char* point = c;
	int beginBit = 0;
	char ch;
	unsigned char idx;

	//encode then write one by one character
	while (in.read(&ch, 1)) {
		idx = ch;

		string encode = frequency[idx]->code;//get code
		count += encode.size();//count bit written

		writebit(point, beginBit, encode);//transfer code

		int numBit = point - c;
		if (numBit != 0) {//write code
			out.write(c, numBit);

			c[0] = *point;
			point = c;
		}

	}

	if (count % 8 != 0) {
		out.write(c, 1);//implementing bit
		byteWrited = count / 8 + 1;
	}
	else
		byteWrited = count / 8;

	bitWrited = count;
	
}

//write information to encoding file
void huffman::writeEncodeTail(ofstream& out) {	
	char ch;

	bitPadded = 8 - (bitWrited % 8);
	if (bitPadded == 8) bitPadded = 0;

	ch = (char)bitPadded;
	out.write(&ch, 1);//write padding bit to encoding file

	int tmp = byteWrited;
	stack<int> s;
	while (tmp > 0) {//seprated byte written to intergers
		s.push(tmp % 10);
		tmp = tmp / 10;
	}
	//write aforementionded to encoding file
	while (s.size()) {
		ch = s.top();
		s.pop();
		out.write(&ch, 1);
	}
	//write the length of byte written to file
	tmp = byteWrited;
	int length = 0;
	while (tmp > 0) {
		length++;
		tmp = tmp / 10;
	}

	ch = length;
	out.write(&ch, 1);

	out.close();
}

//write encoding to file
void huffman::writeEncoding() {
	ofstream out(outPath, ios::binary);

	writeTree(root,out);
	writeEncodeText(out);
	writeEncodeTail(out);
	
}

void huffman::encodedFile() {
	statistic();
	buildHuffmanTree();
	encodingTable();
	writeEncoding();
}

//rebuld the tree from the encoding file
void huffman::reBuilTree(huffmanNode* &node, char letter, ifstream& in) {
	node = new huffmanNode;
	if (root == NULL)
		root = node;

	char c;

	if (letter == '1') {//leaf node
		
		in.read(&c, 1);

		node->leafNode = true;
		node->letter = c;
	}
	else if (letter == '0'){
		node->leafNode = false;

		in.read(&c, 1);
		reBuilTree(node->left, c, in);//left node

		in.read(&c, 1);
		reBuilTree(node->right, c, in);//right node
	}
}
//read information about the encoding file
void huffman::readTail(ifstream& in) {
	char ch;
	long long int begin = in.tellg();
	in.seekg(-1, ios::end);
	long long int end = in.tellg();
	in.read(&ch, 1);

	int length = (int)ch;//length of written byte
	in.seekg(-(length + 2), ios::end);

	in.read(&ch, 1);//read padding bit
	bitPadded = (int)ch;

	int tmp;
	for (int i = 0; i < length; i++) {//read the written byte of encoding file
		in.read(&ch, 1);
		tmp = (int)ch;

		byteWrited = 10 * byteWrited + tmp;
	}

	in.seekg(-(end - begin), ios::cur);
}

//write decode
void huffman::writeDecode(ifstream& in) {
	ofstream out(outPath, ios::out);
	if (!out) return;

	char ch;
	char bit;
	huffmanNode* cur = root;

	for (long long int i = 0; i < byteWrited - 1; i++) {
		in.read(&ch, 1);

		for (int j = 0; j < 8; j++) {//transfer letter to code
			bit = ((ch & (1 << (7 - j))) != 0) > 0 ? '1' : '0';

			if (bit == '0') {
				cur = cur->left;
			}
			else
				cur = cur->right;

			if (cur->leafNode) {
				out << cur->letter;
				cur = root;
			}
		}
	}

	//last byte is treated respectively
	in.read(&ch, 1);

	for (int j = 0; j < 8 - bitPadded; j++) {//remove padding bit
		bit = ((ch & (1 << (7 - j))) != 0) ? '1' : '0';//transfer letter to code


		if (bit == '0') {
			cur = cur->left;
		}
		else
			cur = cur->right;

		if (cur->leafNode) {
			out << cur->letter;
			cur = root;
		}
	}

	out.close();
	in.close();

}

//decoding file that have encoded
void huffman::decodedFile() {

	ifstream in(inPath, ios::binary);
	ofstream out(outPath, ios::out);
	if (!out) return;
	char ch;

	root = NULL;

	in.read((char*)&ch, 1);
	reBuilTree(root, ch, in);//buid tree
	readTail(in);//read information
	writeDecode(in);//write decode

}