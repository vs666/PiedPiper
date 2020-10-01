/**
 * Pseudo Random Number Generator using Cellular Automaton
 * Code is to be include as libraru in the naive encryption/decryption algorithm
 * This method is space inefficient, but has more application in image compression than data compression
 * 
 * Created By : Varul Srivastava
 * Reviewed by : 
 * 
 * This algorithm uses Wolfram's Rule of Cellular Automaton
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

const long long int MULT_NUM = (long long int)1000000007;
const long long int ADD_NUM = (long long int)1029831027;
const long long int MOD = (long long int)0xFF;
const int COLUMN_SIZE = 1000000;
const int RULE = 150;

int row_size = 1;
int key;

void initMain(int keyVal)
{
    key = keyVal;
}

char conversion(char **base, int x, int y)
{
    char bit_seq = 0;
    for (int a = 0; a < 8; a++)
    {
        int num = 0;
        if (a == 0)
        {
            num = (base[x][(y - 1 + row_size) % row_size] & (0x90));
        }
        else
        {
            num = (base[x][y] & (1 << (a - 1)));
        }
        num = num | (base[x][y] & (1 << a));
        if (a == 7)
        {
            num = num | (base[x][(y + 1) % row_size] & (1));
        }
        else
        {
            num = num | (base[x][y + 1] & 1);
        }

        bit_seq = bit_seq | (RULE & (1 << num));
    }
    return bit_seq;
}

char **removeBias(char *ar)
{
    char arr[COLUMN_SIZE][row_size];
    for (int x = 0; x < row_size; x++)
    {
        arr[0][x] = (ar[x] * MULT_NUM + ADD_NUM) % MOD;
    }
    return arr;
}

char **initPrg(char *pswd)
{
    row_size = max(row_size, strlen(pswd));
    char **base = removeBias(pswd);
    for (int x = 1; x < COLUMN_SIZE; x++)
    {
        for (int y = 0; y < row_size; y++)
        {
            base[x][y] = conversion(base, x - 1, y);
        }
    }
    return base;
}

char **initPrg(char *pswd, int row)
{
    row_size = row;
    return initPrg(pswd);
}

int getNum()
{
    // generate number from the grid generated
}
