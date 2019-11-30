#include "functionUtilities.h"

int main() {
	bool exit = false;

	while (!exit) {
		cout << endl;
		cout << "1. Encoding File" << endl;
		cout << "2. Decoding File" << endl;
		cout << "3. Exit" << endl;

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
			exit = true;
			break;
		default:
			break;
		}
	}

	return 0;
}
