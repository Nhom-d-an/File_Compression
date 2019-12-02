#include "functionUtilities.h"


bool checkInPathEnFormat(string inPath, string& outPath) {
	string format = inPath.substr(inPath.size() - 4, inPath.size());//take the end of path: .txt

	if (format != ".txt") return false;//format of file is wrong

	outPath = inPath.substr(0, inPath.size() - 4) + ".enFi";
	return true;
}

bool checkInPathDeFormat(string inPath, string& outPath, int format=0) {
	string encode;
	string decode;

	if (format == 0) {
		encode = ".enFi";
		decode = ".deFi";
	}
	else {
		encode = ".enFo";
		decode = ".deFo";
	}
	string tail = inPath.substr(inPath.size() - 5, inPath.size());//take the end of path: .enFi

	if (tail != encode) return false;//format of file is wrong

	outPath = inPath.substr(0, inPath.size() - 5) + decode;
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

void encodingFile(string inPath) {

	//cout << "Enter the path to the File needed to encode" << endl;

	//string inPath;
	string outPath;
	//cin >> inPath;

	if (!checkInPathEnFormat(inPath, outPath)) {
		cout << "File have no right Format" << endl;
		return;
	}

	if (!checkInPathOpen(inPath)) {
		cout << "File not Found" << endl;
		return;
	}

	//cout << endl << "Encoding..." << endl;

	huffman algorithm;
	string text = algorithm.readTextFile(inPath);//read text
	map<char, double> stat = algorithm.statistic(text);//statistic the appearances's frequently of each character
	huffmanNode* root = algorithm.buildHuffmanTree(stat);//build the tree of algorithm
	map<char, string> table;
	algorithm.encodingTable(table, root);//build the table indicating code of each character
	string encode = algorithm.encodedText(table, text);//transfering text to encode
	algorithm.writeCodingText(table, encode, outPath);//write encoding 

	//cout << "Encoding Successfully" << endl;

}

void decodingFile(string inPath) {

	//cout << "Enter the path to the File needed to decode:" << endl;

	//string inPath;
	string outPath;
	

	if (!checkInPathDeFormat(inPath, outPath)) {
		cout << "File have no right Format" << endl;
		return;
	}

	if (!checkInPathOpen(inPath)) {
		cout << "File not Found" << endl;
		return;
	}

	//cout << endl << "Decoding..." << endl;

	huffman algorithm;
	map<char, string> table; //the table to indicate code of each character
	string code;//save the encoded
	algorithm.readEncodedFile(table, code, inPath);
	string text = algorithm.decodedText(table, code);//transfer code to text
	algorithm.writeDecodingText(text, outPath);

	//cout << "Decoding Succesfully" << endl;
}

char* transStringToChar(string s) {
	int size = s.size();
	char* result = new char[size];

	int i = 0;
	while (s[i]) {
		result[i] = s[i];
		i++;
	}
	result[i] = '\0';

	return result;
}

string copyFolder(string inPath, int format=0) {
	string encode = ".enFo";
	string decode = ".deFo";
	string outPath;

	if (format == 0)
		outPath = inPath.substr(0, inPath.size()-5) + encode;
	else
		outPath = inPath.substr(0, inPath.size()-5) + decode;

	inPath += '\0';
	SHFILEOPSTRUCT sf;
	memset(&sf, 0, sizeof(sf));
	sf.hwnd = 0;
	sf.wFunc = FO_COPY;
	sf.pFrom = (LPCSTR)inPath.c_str();
	sf.pTo = (LPCSTR)outPath.c_str();

	sf.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_NOERRORUI;
	int n = SHFileOperation(&sf);
	if (n != 0) {
		cout << "Cann't copy folder" << endl;
	}
	return outPath;
}

void processFolder(string path, int format=0){
	DIR* dir;
	struct dirent* ent;

	char* cPath = transStringToChar(path);
	dir = opendir(cPath);

	void(*function)(string);
	string tailFile;

	if (format == 0) {
		function = encodingFile;
		tailFile = ".txt";
	}	
	else
	{
		function = decodingFile;
		tailFile = ".enFi";
	}

	if (dir != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {
			string filename = ent->d_name;
			if (filename == "." || filename == "..")
				continue;
			if (filename.size() > 4) {
				string tail;
				if(format==0)  tail = filename.substr(filename.size() - 4, filename.size());
				else tail = filename.substr(filename.size() - 5, filename.size());
				if (tail == tailFile) {
					string newPath = path + "\\" + filename;
					function(newPath);
					if (remove(transStringToChar(newPath))) {
						cout << "Cann't remove the old File" << endl;
					}
					continue;
				}
			}

			//cout << "name of folder: " << filename << endl;
			string newPath = path + "\\" + filename;
			if (format == 0)
				processFolder(newPath, 0);
			else
				processFolder(newPath, 1);

		}
		closedir(dir);
	}

}

bool checkPathFolder(string inPath) {
	DIR* dir;//direction

	char* cPath = transStringToChar(inPath);
	dir = opendir(cPath);

	if (dir == NULL) {//cann't open folder
		return false;
	}

	closedir(dir);
	return true;
}

void encodingFolder() {
	string inPath;
	string outPath;

	cout << "Enter the path to the Folder needed to encode:";
	rewind(stdin);
	getline(cin, inPath);


	if (!checkPathFolder(inPath)) {
		cout << "Cann't open the Folder" << endl;
		return;
	}

	cout << "Encoding..." << endl;

	outPath = copyFolder(inPath);//copy to the new folder has the same content
	processFolder(outPath);//Encoding these folder

	cout << "Encoding Successfully" << endl;
}

void decodingFolder() {
	string inPath;
	string outPath;

	cout << "Enter the path to the Folder needed to decode:";
	rewind(stdin);
	getline(cin, inPath);


	if (!checkPathFolder(inPath)) {
		cout << "Cann't open the Folder" << endl;
		return;
	}

	if (!checkInPathDeFormat(inPath, outPath,1)) {
		cout << "Folder has no right format" << endl;
		return;
	}

	cout << "Decoding..." << endl;

	outPath = copyFolder(inPath, 1);
	processFolder(outPath, 1);

	cout << "Decoding Successfully" << endl;

}