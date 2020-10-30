#include "minHeap.h"

Node* minHeap[257];
int minHeapSize = 0;

Node* getLeftChild(int indx)
{
	return minHeap[2*indx];
}

Node* getRightChild(int indx)
{
	return minHeap[2*indx + 1];
}

Node* getMinimumNode()
{
	if(minHeapSize == 0)
		return NULL;
	else
		return minHeap[1];
}

void removeMinimumNode()
{
	if(minHeapSize == 0)
		return;
	
	else if(minHeapSize == 1)
	{
		minHeap[1] = NULL;
		minHeapSize = 0;
		return;
	}

	else
	{
		minHeap[1] = minHeap[minHeapSize];
		minHeap[minHeapSize] = NULL;
		minHeapSize -= 1;
		heapifyTopToBottom(1);
	}
}

void heapifyTopToBottom(int indx)
{
	if(2*indx > minHeapSize)
		return;

	Node* currentNode = minHeap[indx];
	Node* leftChild = getLeftChild(indx);
	Node* rightChild = getRightChild(indx);

	if(rightChild == NULL)
	{
		if(isSmallerNode(leftChild, currentNode) == true)
		{
			minHeap[indx] = leftChild;
			minHeap[2*indx] = currentNode;
			heapifyTopToBottom(2*indx);
		}
	}

	else if(isSmallerNode(leftChild, currentNode) == true && isSmallerNode(rightChild, currentNode) == true)
	{
		if(isSmallerNode(leftChild, rightChild) == true)
		{
			minHeap[indx] = leftChild;
			minHeap[2*indx] = currentNode;
			heapifyTopToBottom(2*indx);
		}
		else
		{
			minHeap[indx] = rightChild;
			minHeap[2*indx + 1] = currentNode;
			heapifyTopToBottom(2*indx + 1);
		}
	}

	else if(isSmallerNode(leftChild, currentNode) == true)
	{
		minHeap[indx] = leftChild;
		minHeap[2*indx] = currentNode;
		heapifyTopToBottom(2*indx);
	}

	else if(isSmallerNode(rightChild, currentNode) == true)
	{
		minHeap[indx] = rightChild;
		minHeap[2*indx + 1] = currentNode;
		heapifyTopToBottom(2*indx + 1);
	}

	else
		return;
}

void heapifyBottomToTop(int indx)
{
	if(indx == 1)
		return;
	else
	{
		Node* currentNode = minHeap[indx];
		Node* parentNode = minHeap[indx/2];
		if(isSmallerNode(currentNode, parentNode) == true)
		{
			minHeap[indx/2] = currentNode;
			minHeap[indx] = parentNode;
			heapifyBottomToTop(indx/2);
		}
		
		else
			return;
	}
}

void insertNode(Node* node)
{
	minHeapSize++;
	minHeap[minHeapSize] = node;
	heapifyBottomToTop(minHeapSize);
	return;

}
