#ifndef HUFFMAN
#define HUFFMAN
#define charToInt(c) ((256 + (int)c))%256
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

char dataToBeWritten = 0;
char dataBuffer[17];
int bufferLength = 0;
int bitsInData = 0;
long long int charFrequencies[256];

typedef struct node
{
	char c;
	long long int frequency;
	bool isLeafNode;
	struct node* leftChild;
	struct node* rightChild;
} Node;

typedef struct code
{
	char c;
	char* code;
} Code;

Node* newNode(char, long long int, bool);

bool isSmallerNode(Node*, Node*);

Code* newCode(char, char*);

void formCodes(Node*, char*);

Code* codes[270];

#endif