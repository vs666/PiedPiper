#include "huffman.h"

long long int charFrequencies[256];
Code* codes[270];

Node* newNode(char _c, long long int _frequency, bool _isLeafNode)
{
	Node* node = (Node*) malloc(sizeof(Node));
	node -> c = _c;
	node -> frequency = _frequency;
	node -> isLeafNode = _isLeafNode;
	node -> leftChild = NULL;
	node -> rightChild = NULL;

	return node;
}

bool isSmallerNode(Node* node1, Node* node2)
{
	if ((node1 -> frequency) < (node2 -> frequency))
	{
		return true;
	}
	else
	{
		return false;
	}
}

Code* newCode(char _c, char* _code)
{
	Code* code = (Code*) malloc(sizeof(Code));

	code -> c = _c;
	code -> code = _code;
	code->code = (char*)malloc(1 + strlen(_code));
	sprintf(code->code, "%s", _code);

	return code;
}

void formCodes(Node* node, char* code)
{
	#include<stdio.h>
	if(node == NULL)
		return;
	if(node -> isLeafNode == true)
	{
		Code* codeNode = newCode(node->c, code);
		codes[((int)(codeNode->c) + 256) % 256] = codeNode;
		return;
	}

	else
	{
		char zero[] = "0", one[] = "1";
		char* rightChildCode = (char*)malloc(strlen(code) + 2);
		char* leftChildCode = (char*)malloc(strlen(code) + 2);
		sprintf(rightChildCode, "%s1", code);
		sprintf(leftChildCode, "%s0", code);
		formCodes(node->leftChild, leftChildCode);
		free(leftChildCode);
		formCodes(node->rightChild, rightChildCode);
		free(rightChildCode);
	}
	return ;

}

codeSearchTreeNode* newCodeTreeNode(char chararcter, bool isValid)
{
	codeSearchTreeNode* node = (codeSearchTreeNode*)malloc(sizeof(codeSearchTreeNode));
	node->c = chararcter;
	node->aValidCharacter = isValid;
	node->leftChild = NULL;
	node->rightChild = NULL;
	return node;
}

void writeBitToOutputFile(int bit, FILE* file)
{
	static int dataToBeWritten;
	static int bitsInData;

	dataToBeWritten = dataToBeWritten << 1;
	bitsInData++;

	if(bit == 1)
	{
		dataToBeWritten = dataToBeWritten | 1;
	}

	if(bitsInData == 8)
	{
		char data = (char)dataToBeWritten;
		fwrite(&data, sizeof(char), 1, file);
		dataToBeWritten = 0;
		bitsInData = 0;
	}

}

void writeCode(Code* sourceCode, FILE* file)
{
	char codeLength = (char)strlen(sourceCode->code);
	char character = sourceCode->c;
	char *codeString = sourceCode->code;
	fwrite(&character, sizeof(char), 1, file);
	fwrite(&codeLength, sizeof(char), 1, file);
	fwrite(codeString, sizeof(char), charToInt(codeLength), file);
}

void readCode(Code* destinationCode, FILE* file)
{
	char character, codeLength;
	char* codeString = NULL;
	fread(&character, sizeof(char), 1, file);
	fread(&codeLength, sizeof(char), 1, file);
	codeString = (char*)malloc((1+codeLength)*sizeof(char));
	fread(codeString, sizeof(char), charToInt(codeLength), file);
	codeString[charToInt(codeLength)] = '\0';
	destinationCode->c = character;
	destinationCode->code = codeString;
}