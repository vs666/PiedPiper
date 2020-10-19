#include "huffman.h"

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