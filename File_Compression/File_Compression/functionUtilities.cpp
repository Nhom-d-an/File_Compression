#include "functionUtilities.h"

//check the path to encoding file
bool checkInPathEnFormat(string inPath, string& outPath) {
	string format = inPath.substr(inPath.size() - 4, inPath.size());//take the end of path: .txt

	if (format != ".txt") return false;//format of file is wrong

	outPath = inPath.substr(0, inPath.size() - 4) + ".enFi";//change tail
	return true;
}

//check the path to decoding file
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

	outPath = inPath.substr(0, inPath.size() - 5) + decode;//change tail
	return true;
}
//check if file can open
bool checkInPathOpen(string inPath) {
	ifstream in(inPath);

	if (!in) {//cannot open file
		return false;
	}

	in.close();
	return true;
}
//encoding file
void encodingFile() {

	cout << "Enter the path to the Text File needed to encode (.txt):";

	string inPath;
	string outPath;
	cin >> inPath;

	cout << "Encoding..." << endl;
	clock_t start = clock();
	encodingFile(inPath);
	clock_t end = clock();
	cout << "Time: " << (end - start) / CLOCKS_PER_SEC <<" s"<< endl;
	cout << "Encoding Successfully" << endl;
}

//decoding file
void decodingFile() {

	cout << "Enter the path to the Text File needed to decode (.enFi):";

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
	clock_t start = clock();
	decodingFile(inPath);
	clock_t end = clock();
	cout << "Time: " << (end - start) / CLOCKS_PER_SEC << " s" << endl;
	cout << "Decoding Succesfully" << endl;
}
//encoding file
void encodingFile(string inPath) {
	string outPath;

	if (!checkInPathEnFormat(inPath, outPath)) {
		cout << "File have no right Format" << endl;
		exit(0);
	}

	if (!checkInPathOpen(inPath)) {
		cout << "File not Found" << endl;
		exit(0);
		
	}

	huffman algorithm(inPath, outPath);
	algorithm.encodedFile();

}
//decoding file
void decodingFile(string inPath) {
	string outPath;
	

	if (!checkInPathDeFormat(inPath, outPath)) {
		cout << "File have no right Format" << endl;
		exit(0);
	}

	if (!checkInPathOpen(inPath)) {
		cout << "File not Found" << endl;
		exit(0);
	}

	huffman algorithm(inPath, outPath);
	algorithm.decodedFile();
	
}
//transfer string to char
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
//copy to the new folder has the same as content
string copyFolder(string inPath, int format=0) {
	string encode = ".enFo";
	string decode = ".deFo";
	string outPath;

	if (format == 0)
		outPath = inPath + encode;
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
//processing interacting with folder
void processFolder(string path, int format=0){
	DIR* dir;
	struct dirent* ent;

	char* cPath = transStringToChar(path);
	dir = opendir(cPath);

	void(*function)(string);
	string tailFile;
	//idenfify tail
	if (format == 0) {
		function = encodingFile;
		tailFile = ".txt";
	}	
	else
	{
		function = decodingFile;
		tailFile = ".enFi";
	}

	if (dir != NULL) {//access the path
		while ((ent = readdir(dir)) != NULL) {
			string filename = ent->d_name;
			if (filename == "." || filename == "..")
				continue;
			if (filename.size() > 4) {
				string tail;
				if(format==0)  tail = filename.substr(filename.size() - 4, filename.size());
				else tail = filename.substr(filename.size() - 5, filename.size());
				if (tail == tailFile) {//find the right format file
					string newPath = path + "\\" + filename;
					function(newPath);//process
					if (remove(transStringToChar(newPath))) {//remove the beggin file
						cout << "Cann't remove the old File" << endl;
					}
					continue;
				}
			}

			string newPath = path + "\\" + filename;
			if (format == 0)
				processFolder(newPath, 0);
			else
				processFolder(newPath, 1);

		}
		closedir(dir);
	}
}
//check the path to folder
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
//encoding folder
void encodingFolder() {
	string inPath;
	string outPath;

	cout << "Enter the path to the Folder containing Text File needed to encode:";
	rewind(stdin);
	getline(cin, inPath);


	if (!checkPathFolder(inPath)) {
		cout << "Cann't open the Folder" << endl;
		exit(0);
	}

	cout << "Encoding..." << endl;
	clock_t start = clock();
	outPath = copyFolder(inPath);//copy to the new folder has the same content
	processFolder(outPath);//Encoding these folder
	clock_t end = clock();
	cout << "Time: " << (end - start) / CLOCKS_PER_SEC << " s" << endl;
	cout << "Encoding Successfully" << endl;
}

void decodingFolder() {
	string inPath;
	string outPath;

	cout << "Enter the path to the Folder containing Text File needed to decode (.enFo):";
	rewind(stdin);
	getline(cin, inPath);


	if (!checkPathFolder(inPath)) {
		cout << "Cann't open the Folder" << endl;
		exit(0);
	}

	if (!checkInPathDeFormat(inPath, outPath,1)) {
		cout << "Folder has no right format" << endl;
		exit(0);
	}

	cout << "Decoding..." << endl;
	clock_t start = clock();
	outPath = copyFolder(inPath, 1);//copy to the new folder has the same content
	processFolder(outPath, 1);//Decoding these folder
	clock_t end = clock();
	cout << "Time: " << (end - start) / CLOCKS_PER_SEC << " s" << endl;
	cout << "Decoding Successfully" << endl;

}