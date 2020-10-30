#ifndef HUFFMAN
#define HUFFMAN
#define charToInt(c) ((256 + (int)c))%256
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

extern long long int charFrequencies[256];

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

typedef struct codeSearchTreeNode
{
	char c;
	struct codeSearchTreeNode *leftChild;
	struct codeSearchTreeNode *rightChild;
	bool aValidCharacter;	
} codeSearchTreeNode;

Node* newNode(char, long long int, bool);

bool isSmallerNode(Node*, Node*);

Code* newCode(char, char*);

void formCodes(Node*, char*);

codeSearchTreeNode* newCodeTreeNode(char, bool);

extern Code* codes[270];

void writeBitToOutputFile(int, FILE*);

void writeCode(Code*, FILE*);

void readCode(Code*, FILE*);

#endif