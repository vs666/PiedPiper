#include "huffman.h"
#include "codeSearchTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char* argv[])
{
	// Handling Wrong or Help arguments

	if(argc == 1)
	{
		printf("Use -h or --help flag for help.\n");
		return 1;
	}

	else if(argc == 2)
	{	
		if( strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0 )
		{
			printf("Compressor text files using static Huffman algorithm\n\n");
			printf("Usage: %s <path_to_compressed_file> <path_to_the_output_file>\n", argv[0]);
			return 0;
		}
		else
		{
			printf("Use -h or --help flag for help.\n");
			return 1;
		}
	}

	else if(argc > 3)
	{
		printf("Too many arguments! I don't know what to do..\n");
		printf("Use -h or --help flag for help.\n");
		return 1;
	}

	// Now, the actual implementation will begin.

	char* inputPath = argv[1];
	char* outputPath = argv[2];

	FILE *inputFile;
	inputFile = fopen(inputPath, "rb");

	if(inputFile == NULL)
	{
		printf("Unable to open the input compressed file\n");
		return 1;
	}
	printf("Input file given: %s\n\n", inputPath);
	printf("Reading header from input file.\n");

	codeSearchTreeNode* headNodePointer = newCodeTreeNode('@', false);

	long long countCharsToBeRead;
	int countOfCodes, countOfBitsToIgnore;
	fread(&countCharsToBeRead, sizeof(long long), 1, inputFile);
	fread(&countOfCodes, sizeof(int), 1, inputFile);
	fread(&countOfBitsToIgnore, sizeof(int), 1, inputFile);

	Code* tempCode = newCode('@', "@");

	for(int codeCount=0; codeCount<countOfCodes; codeCount++)
	{
		readCode(tempCode, inputFile);
		insertCodeNode(headNodePointer, tempCode, 0, strlen(tempCode->code));
	}
	codeSearchTreeNode* movingPointer = headNodePointer;

	FILE *outputFile;
	outputFile = fopen(outputPath, "w");

	if(outputFile == NULL)
	{
		printf("There was error in creating an output file at given path.\n");
		return 1;
	}

	char charRead;

	printf("Writing out to output file...\n");

	for(long long charsRead=0; charsRead<(countCharsToBeRead-1); charsRead++)
	{
		fread(&charRead, sizeof(char), 1, inputFile);
		int intOfChar = charToInt(charRead);
		int bits[8];
		for(int position=7; position>=0; position--)
		{
			bits[position] = intOfChar % 2;
			intOfChar /= 2;
		}
		for(int position=0; position<8; position++)
		{
			int bit = bits[position];
			// printf("Read %d bit\n", bit);
			if(bit == 0)
			{
				movingPointer = movingPointer->leftChild;
			}
			else
			{
				movingPointer = movingPointer->rightChild;
			}
			if(movingPointer->aValidCharacter == true)
			{
				fprintf(outputFile, "%c", movingPointer->c);
				movingPointer = headNodePointer;
			}
		}
	}
	fread(&charRead, sizeof(char), 1, inputFile);
	int intOfChar = charToInt(charRead);
	int bits[8];
	for(int position=7; position>=0; position--)
	{
		bits[position] = intOfChar % 2;
		intOfChar /= 2;
	}
	for(int position=0; position < 8-countOfBitsToIgnore; position++)
	{
		int bit = bits[position];
		if(bit == 0)
		{
			movingPointer = movingPointer->leftChild;
		}
		else
		{
			movingPointer = movingPointer->rightChild;
		}
		if(movingPointer->aValidCharacter == true)
		{
			fprintf(outputFile, "%c", movingPointer->c);
			movingPointer = headNodePointer;
		}
	}

	fclose(inputFile);
	fclose(outputFile);

	printf("File succesfully decompressed!\n");
	printf("Output file: %s\n", outputPath);
	printf("Good Bye\n");

	return 0;
}
