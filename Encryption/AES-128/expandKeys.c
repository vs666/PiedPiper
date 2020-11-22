#include "standardDefinitions.h"
#include "expandKeys.h"

void expandKey(unsigned char key[16], unsigned char expandedKeys[176]){

	// First 4 columns of the expanded keys are same as the original key
	for(int i=0; i<16; i++){
		expandedKeys[i] = key[i];
	}

	int bytesDone = 16;
	int rconColumn = 1;
	unsigned char keyColumn[4];

	while(bytesDone < 176){
		
		for(int i=0; i<4; i++){
			keyColumn[i] = expandedKeys[i + bytesDone - 4];
		}

		// Get the next column after each 4 columns
		if(bytesDone % 16 == 0){

			// Rotate the column
			unsigned char firstChar = keyColumn[0];
			for(int i=0; i<3; i++){
				keyColumn[i] = keyColumn[i+1];
			}
			keyColumn[3] = firstChar;

			// Undergo the substitution box
			for(int i=0; i<4; i++){
				keyColumn[i] = substitutionBox[keyColumn[i]];
			}

			// XOR with rcon columns
			keyColumn[0] = keyColumn[0] ^ rconColumns[rconColumn++];

		}

		for(int i=0; i<4; i++){
			expandedKeys[bytesDone] = expandedKeys[bytesDone - 16] ^ keyColumn[i];
			bytesDone++;
		}
	}

}
