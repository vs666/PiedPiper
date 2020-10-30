#ifndef HUFFMAN_MIN_HEAP
#define HUFFMAN_MIN_HEAP

#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

// The data gets stored starting from index 1

extern Node* minHeap[257];
extern int minHeapSize;

Node* getMinimumNode();

void removeMinimumNode();

void heapifyTopToBottom(int);

void heapifyBottomToTop(int);

Node* getLeftChild(int);

Node* getRightChild(int);

void insertNode(Node*);

#endif