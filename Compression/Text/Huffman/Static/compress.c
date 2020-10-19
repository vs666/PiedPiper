#include "huffman.h"
#include "minHeap.h"
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
			printf("Usage: %s <path_to_input_file> <path_to_the_output_file>\n", argv[0]);
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

	printf("Input file given: %s\n", inputPath);
	printf("\nReading from input file...\n");

	FILE *inputFile;

	inputFile = fopen(inputPath, "r");

	if(inputFile == NULL)
	{
		printf("Error: The input file could not be read.\n");
		return 1;
	}
	else
	{
		for(int charCode=0; charCode < 256; charCode++)
			charFrequencies[charCode] = 0LL;

		printf("Input file successfully opened.\n");
		printf("Generating frequency table...\n");

		char c = fgetc(inputFile);
		long long int totalFreq = 0;

		while( c != EOF )
		{
			charFrequencies[charToInt(c)] += 1;
			totalFreq++;
			c = fgetc(inputFile);
		}

		printf("Generating codes...\n");

		for(int index=0; index < 257; index++)
			minHeap[index] = NULL;

		for(int i=0; i<256; i++)
		{
			if(charFrequencies[i] > 0)
			{
				Node* node = newNode((char)i, charFrequencies[i], true);
				insertNode(node);
			}
		}

		while(minHeapSize > 1)
		{
			Node* smallestNode = getMinimumNode();
			removeMinimumNode();
			Node* nextSmallestNode = getMinimumNode();
			removeMinimumNode();

			/*
			*	Combined nodes have @ as the character.
			*	Original nodes can be identified by isLeafNode.
			*/

			Node* combinedNode = newNode('@', (smallestNode->frequency + nextSmallestNode->frequency), false);
			combinedNode->leftChild = smallestNode;
			combinedNode->rightChild = nextSmallestNode;
			insertNode(combinedNode);
		}

		fclose(inputFile);

		Node* head = minHeap[1];
		char startCode[] = "";
		formCodes(head, startCode);
		printf("Codes generated successfully\n");
		printf("Generating Output...\n");

		int countOfCodes = 0;
		int countOfExcessBits = 0;

		for(int i=0; i<256; i++)
		{
			if(codes[i] != NULL)
			{
				countOfCodes++;
				countOfExcessBits += charFrequencies[i] * strlen(codes[i]->code);
				countOfExcessBits %= 8;
			}
		}

		int countOfBitsToIgnore = 8 - countOfExcessBits;

		FILE *outputFile;

		outputFile = fopen(outputPath, "wb+");

		if(outputFile == NULL)
		{
			printf("The program was not able to open the output file...\n");
			return 1;
		}
		else
		{
			fwrite(&countOfCodes, sizeof(int), 1, outputFile);
			fwrite(&countOfBitsToIgnore, sizeof(int), 1, outputFile);
			for(int i=0; i<256; i++)
			{
				if(codes[i] != NULL)
				{
					// fwrite(codes[i], sizeof(Code), 1, outputFile);
				}
			}

			inputFile = fopen(inputPath, "r");

			if(inputFile == NULL)
			{
				printf("The program ran into an error!");
				return 1;
			}
			else
			{
				c = fgetc(inputFile);
				while( c != EOF )
				{
					int codeLength = strlen((codes[charToInt(c)]->code));
					for(int i=0; i<codeLength; i++)
					{
						char bit = codes[charToInt(c)]->code[i];
						dataToBeWritten = dataToBeWritten << 1;
						bitsInData += 1;
						switch(bit)
						{
							case '0':
							dataToBeWritten = dataToBeWritten | 0;
							break;
							case '1':
							dataToBeWritten = dataToBeWritten | 1;
							break;
							default:
							printf("We ran into some unexpected error!\n");
							return 1; 
						}
						if(bitsInData == 8)
						{
							dataBuffer[bufferLength++] = dataToBeWritten;
							dataBuffer[bufferLength] = '\0';
							dataToBeWritten = 0;
							bitsInData = 0;
						}
						if(bufferLength == 17){
							fprintf(outputFile, "%s", dataBuffer);
							bufferLength = 0;
						}
					}
					c = fgetc(inputFile);
				}
				fclose(inputFile);
				for(int i=0; i<countOfBitsToIgnore; i++){
					dataToBeWritten = dataToBeWritten << 1;
				}
				dataBuffer[bufferLength++] = dataToBeWritten;
				dataBuffer[bufferLength++] = '\0';
				fprintf(outputFile, "%s", dataBuffer);
			}
			fclose(outputFile);
		}
	}
	printf("Successfully compressed!\n");
	printf("Output File: %s\n", outputPath);
	printf("Good Bye.\n");
	return 0;
}