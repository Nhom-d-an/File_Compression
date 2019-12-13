#include "functionUtilities.h"

int main() {
	bool exit = false;

	while (!exit) {
		system("cls");
		cout << "1. Encoding" << endl;
		cout << "2. Decoding" << endl;
		cout << "3. Encoding Folder" << endl;
		cout << "4. Decoding Folder" << endl;
		cout << "5. Exit" << endl;

		int choice; cin >> choice;

		switch (choice)
		{
		case 1:
			encodingFile();
			break;
		case 2:
			decodingFile();
			break;
		case 3:
			encodingFolder();
			break;
		case 4:
			decodingFolder();
			break;
		case 5:
			exit = true;
			return 0;
		default:
			return 0;
		}

		system("pause");
	}

	return 0;
}

