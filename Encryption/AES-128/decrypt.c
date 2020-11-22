#include "standardDefinitions.h"
#include "expandKeys.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void SubRoundKey(unsigned char * state, unsigned char * roundKey) {
	for (int i = 0; i < 16; i++) {
		state[i] ^= roundKey[i];
	}
}

/* InverseMixColumns uses mul9, mul11, mul13, mul14 look-up tables
 * Unmixes the columns by reversing the effect of MixColumns in encryption
 */
void InverseMixColumns(unsigned char * state) {
	unsigned char tmp[4][4];
	unsigned char multAns[4][4];

	for(int i=0; i<16; i++) {
		tmp[i%4][i/4] = state[i];
		multAns[i%4][i/4] = 0;
	}

	for(int colNum=0; colNum<4; colNum++) {
		multAns[0][colNum] = multiplyWithE[tmp[0][colNum]] ^ multiplyWithB[tmp[1][colNum]] ^ multiplyWithD[tmp[2][colNum]] ^ multiplyWith9[tmp[3][colNum]];
		multAns[1][colNum] = multiplyWith9[tmp[0][colNum]] ^ multiplyWithE[tmp[1][colNum]] ^ multiplyWithB[tmp[2][colNum]] ^ multiplyWithD[tmp[3][colNum]];
		multAns[2][colNum] = multiplyWithD[tmp[0][colNum]] ^ multiplyWith9[tmp[1][colNum]] ^ multiplyWithE[tmp[2][colNum]] ^ multiplyWithB[tmp[3][colNum]];
		multAns[3][colNum] = multiplyWithB[tmp[0][colNum]] ^ multiplyWithD[tmp[1][colNum]] ^ multiplyWith9[tmp[2][colNum]] ^ multiplyWithE[tmp[3][colNum]];
	}

	for (int i = 0; i < 16; i++) {
		state[i] = multAns[i%4][i/4];
	}
}

// Shifts rows right (rather than left) for decryption
void ShiftRows(unsigned char * state) {
	unsigned char tmp[4][4];
	unsigned char shiftedMatrix[4][4];

	for(int i=0; i<16; i++) {
		tmp[i%4][i/4] = state[i];
	}

	for(int rowNum=0; rowNum<4; rowNum++) {
		int rightShiftAmt = rowNum;
		for(int colNum=0; colNum<4; colNum++) {
			shiftedMatrix[rowNum][colNum] = tmp[rowNum][(colNum-rightShiftAmt+4)%4];
		}
	}
	
	for (int i = 0; i < 16; i++) {
		state[i] = shiftedMatrix[i%4][i/4];
	}
}

/* Perform substitution to each of the 16 bytes
 * Uses inverse S-box as lookup table
 */
void SubBytes(unsigned char * state) {
	for (int i = 0; i < 16; i++) { // Perform substitution to each of the 16 bytes
		state[i] = inverseSubstitutionBox[state[i]];
	}
}

/* Each round operates on 128 bits at a time
 * The number of rounds is defined in AESDecrypt()
 * Not surprisingly, the steps are the encryption steps but reversed
 */
void Round(unsigned char * state, unsigned char * key, unsigned char isFirstRound) {
	SubRoundKey(state, key);
    if(isFirstRound != (unsigned char)1){
        InverseMixColumns(state);
    }
	ShiftRows(state);
	SubBytes(state);
}

// Same as Round() but no InverseMixColumns

/* The AES decryption function
 * Organizes all the decryption steps into one function
 */
void decrypt(unsigned char * encryptedMessage, unsigned char * expandedKey, unsigned char * decryptedMessage)
{
	unsigned char state[16]; // Stores the first 16 bytes of encrypted message

	for (int i = 0; i < 16; i++) {
		state[i] = encryptedMessage[i];
	}

	Round(state, expandedKey+160, 1);

	int numberOfRounds = 9;

	for (int i = numberOfRounds-1; i >= 0; i--) {
		Round(state, expandedKey + (16 * (i + 1)), 0);
	}

	SubRoundKey(state, expandedKey); // Final round

	// Copy decrypted state to buffer
	for (int i = 0; i < 16; i++) {
		decryptedMessage[i] = state[i];
	}
}

long int numBytesToRead(FILE *fp) { 
	if (fp == NULL) { 
		return 0; 
	} 
  
	fseek(fp, 0L, SEEK_END); 
	long int res = ftell(fp); 
	fseek(fp, 0L, SEEK_SET);

	res /= 3;
  
	return res; 
} 

int main() {

	printf("======== Pied Piper AES-128 bit Decryptor =======\n");

	FILE *mssgFile = NULL;
	mssgFile = fopen("message", "rb");

	if(mssgFile == NULL) {
		printf("Error! Encrypted file named 'message' not found!!\n");
		return 1;
	}

	unsigned char *encryptedMessage = NULL;

	int messageLen = numBytesToRead(mssgFile);
	encryptedMessage = (unsigned char*)malloc(messageLen * sizeof(unsigned char));
	for(int i=0; i<messageLen; i++) {
		fscanf(mssgFile, "%x", &encryptedMessage[i]);
	}

	fclose(mssgFile);

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

	unsigned char * decryptedMessage = NULL;
	decryptedMessage = (unsigned char*)malloc(messageLen * sizeof(unsigned char));

	for (int i = 0; i < messageLen; i += 16) {
		decrypt(encryptedMessage + i, expandedKey, decryptedMessage + i);
	}

	printf("Decrypted message: ");
	for (int i = 0; i < messageLen; i++) {
		printf("%c",decryptedMessage[i]);
	}
	printf("\n");

	return 0;
}