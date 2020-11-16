#include "standardDefinitions.h"
#include "expandKeys.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void SubBytes(unsigned char * state) {
	for (int i = 0; i < 16; i++) {
		state[i] = substitutionBox[state[i]];
	}
}

void ShiftRows(unsigned char * state) {
	unsigned char tmp[4][4];

	for(int i=0; i<16; i++) {
		tmp[i%4][i/4] = state[i];
	}

	for(int rowNum=0; rowNum<4; rowNum++) {
		int leftShiftDistance = rowNum;

		unsigned char tempRow[4];
		for(int j=0; j<4; j++) {
			tempRow[j] = tmp[rowNum][j];
		}

		for(int j=0; j<4; j++){
			tmp[rowNum][j] = tempRow[(j+leftShiftDistance)%4];
		}
	}

	for (int i = 0; i < 16; i++) {
		state[i] = tmp[i%4][i/4];
	}
}

void MixColumns(unsigned char * state) {

	unsigned char tmp[4][4];
	unsigned char multAns[4][4];

	for(int i=0; i<16; i++) {
		tmp[i%4][i/4] = state[i];
		multAns[i%4][i/4] = 0;
	}

	for(int colNum=0; colNum<4; colNum++) {
		multAns[0][colNum] = multiplyWith2[tmp[0][colNum]] ^ multiplyWith3[tmp[1][colNum]] ^ tmp[2][colNum] ^ tmp[3][colNum];
		multAns[1][colNum] = multiplyWith2[tmp[1][colNum]] ^ multiplyWith3[tmp[2][colNum]] ^ tmp[0][colNum] ^ tmp[3][colNum];
		multAns[2][colNum] = multiplyWith2[tmp[2][colNum]] ^ multiplyWith3[tmp[3][colNum]] ^ tmp[0][colNum] ^ tmp[1][colNum];
		multAns[3][colNum] = multiplyWith2[tmp[3][colNum]] ^ multiplyWith3[tmp[0][colNum]] ^ tmp[2][colNum] ^ tmp[1][colNum];
	}
	
	for (int i = 0; i < 16; i++) {
		state[i] = multAns[i%4][i/4];
	}
}

void AddRoundKey(unsigned char * state, unsigned char * roundKey) {
	for (int i = 0; i < 16; i++) {
		state[i] = state[i] ^ roundKey[i];
	}
}

void Round(unsigned char * state, unsigned char * key, unsigned char isFinalRound) {
	SubBytes(state);
	ShiftRows(state);
	if(isFinalRound != (unsigned char)1){
		MixColumns(state);
	}
	AddRoundKey(state, key);
}

void encrypt(unsigned char * message, unsigned char * expandedKey, unsigned char * encryptedMessage) {
	unsigned char state[16]; // Stores the first 16 bytes of original message

	for (int i = 0; i < 16; i++) {
		state[i] = message[i];
	}

	AddRoundKey(state, expandedKey); // Initial round

	int numberOfRounds = 10;

	for (int roundNumber = 0; roundNumber < numberOfRounds - 1; roundNumber++) {
		Round(state, expandedKey + (16 * (roundNumber+1)), (unsigned char)0);
	}

	Round(state, expandedKey + 160, (unsigned char)1);

	for (int i = 0; i < 16; i++) {
		encryptedMessage[i] = state[i];
	}
}

int main() {

	printf("======== Pied Piper AES-128 bit Encryptor =======\n");

	char message[1024];

	printf("Enter the message to encrypt: ");
	scanf("%255[^\n]s", message);

	int originalLen = strlen(message);

	int paddedMessageLen = originalLen;

	if ((paddedMessageLen % 16) != 0) {
		paddedMessageLen = 16 * (paddedMessageLen/16 + 1);
	}

	unsigned char *paddedMessage;
	paddedMessage = (unsigned char*)malloc(paddedMessageLen * sizeof(unsigned char));
	for(int i = 0; i < originalLen; i++) {
		paddedMessage[i] = message[i];
	}
	for(int i=originalLen; i<paddedMessageLen; i++){
		paddedMessage[i] = 0;
	}

	unsigned char *encryptedMessage;
	encryptedMessage = (unsigned char*)malloc(paddedMessageLen * sizeof(unsigned char));

	FILE *keyFile = NULL;
	keyFile = fopen("key", "r");

	if(keyFile == NULL){
		printf("There was a problem reading the key file.\n");
		return 1;
	}

	unsigned char key[16];

	for(int keyByteNum=0; keyByteNum<16; keyByteNum++) {
		fscanf(keyFile, "%x", &key[keyByteNum]);
	}

	fclose(keyFile);

	unsigned char expandedKey[176];

	expandKey(key, expandedKey);

	for (int i = 0; i < paddedMessageLen; i += 16) {
		encrypt(paddedMessage+i, expandedKey, encryptedMessage+i);
	}

	printf("Encrypted message hex:\n");
	for (int i = 0; i < paddedMessageLen; i++) {
		printf("%02x ", encryptedMessage[i]);
	}

	printf("\n");

	FILE *outputFile = NULL;

	outputFile = fopen("message", "wb");
	
	if(outputFile == NULL) {
		printf("There was an error while writing ouput to a file\n");
		return 1;
	}

	for(int i=0; i<paddedMessageLen; i++){
		fprintf(outputFile, "%02x ", encryptedMessage[i]);
	}

	printf("Encrypted message written to file named: message\n");
	
	free(paddedMessage);
	free(encryptedMessage);

	return 0;
}