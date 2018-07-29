#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <fstream>
#include <cstring>
#include <iostream>
using namespace std;

const int SIZE = 10; // the size of the buffer
int alreadyRead = 0; // [times we wrote to buffer] x [buffer size]
int previous = 0; // the value of prefix function for the last index of previous buffer

// Simple prefix function
int* prefixFunction(char* s) {
	int s_length = strlen(s);
	int* p = new int[s_length];
	for (int i = 0; i < s_length; i++)
		p[i] = 0;
	for (int i = 0; i < s_length; i++) {
		for (int k = 1; k < i; k++) {
			if (strncmp(s, s + i - k + 1, k) == 0) {
				p[i] = k;
			}
		}
	}
	return p;
}

// KMP that prints index every time it finds occurrence
void kmp(char* s, char* w) {
	int s_length = SIZE;
	int w_length = strlen(w);
	char a[SIZE*2];
	strcpy(a, w);
	strcat(a, "#");
	strncat(a, w, previous);
	strcat(a, s);
	int* p = prefixFunction(a);
	for (int i = 0; i < s_length + previous; i++) {
		if (p[i + w_length + 1] == w_length) {
			cout << i - w_length - previous + 1 + alreadyRead << endl;
		}
	}
	previous = p[w_length + s_length + previous];
}


int main(int argc, char *argv[]) {
	if (argc < 3) {
		cout << "Not enough arguments" << endl;
	}
	else {
		FILE* file = fopen(argv[1], "r");
		char buf[SIZE] = { ' ' };
		char * word = argv[2];

		while (!feof(file)) {
			fread(buf, sizeof(char), sizeof(buf), file);
			kmp(buf, word);
			alreadyRead += SIZE;
		}
		fclose(file);
	}
	return 0;
}
