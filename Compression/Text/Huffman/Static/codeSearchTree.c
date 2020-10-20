#include "codeSearchTree.h"

void insertCodeNode(codeSearchTreeNode* node, Code* toBeAdded, int indx, int codeLength)
{
    printf("In function\n");
    if(indx == codeLength)
    {
        node->c = toBeAdded->c;
        node->aValidCharacter = true;
        return;
    }
    
    char currBit = toBeAdded->code[indx];
    switch(currBit)
    {
        case '0':
        if(node->leftChild == NULL)
        {
            node->leftChild = newCodeTreeNode('@', false);
        }
        insertCodeNode(node->leftChild, toBeAdded, indx+1, codeLength);
        break;

        case '1':
        if(node->rightChild == NULL)
        {
            node->rightChild = newCodeTreeNode('@', false);
        }
        insertCodeNode(node->rightChild, toBeAdded, indx+1, codeLength);
        break;
    }
    return ;
}