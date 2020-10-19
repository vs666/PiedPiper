#ifndef HUFFMAN
#define HUFFMAN

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

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