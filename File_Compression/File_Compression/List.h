#pragma once

// a list class for lz77 algorithm
class List
{
public:
	static const int NULL_VAL = -1;
	static const int MAX_SYM = 256;
	static const int MAX_BUFSIZE = 8192;

	// this is an array of 256 characters
	int list[MAX_SYM];

	// these arrays contain the "previous" and "next" pointers
	int prev[MAX_BUFSIZE];
	int next[MAX_BUFSIZE];

	List() {}
	List(const int size, unsigned char windowBuffer[]) {
		for (int i = 0; i < MAX_SYM; i++) {
			list[i] = NULL_VAL;
		}

		for (int i = 0; i < size; i++) {
			prev[i] = NULL_VAL;
			next[i] = NULL_VAL;
			insert(windowBuffer[i], i);
		}
	}

	void insert(unsigned char c, int i) {
		int temp = list[c];

		list[c] = i;
		prev[i] = NULL_VAL;
		next[i] = temp;

		if (temp != NULL_VAL) {
			prev[temp] = i;
		}
	}

	void del(unsigned char c, int i) {
		if (list[c] == i) {
			list[c] = next[i];
			if (list[c] != NULL_VAL) {
				prev[list[c]] = NULL_VAL;
			}
		}
		else {
			next[prev[i]] = next[i];

			if (next[i] != NULL_VAL) {
				prev[next[i]] = prev[i];
			}
		}
	}
};

