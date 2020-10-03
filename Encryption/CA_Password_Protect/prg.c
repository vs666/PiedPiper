/**
 * Pseudo Random Number Generator using Cellular Automaton
 * Code is to be include as libraru in the naive encryption/decryption algorithm
 * This method is space inefficient, but has more application in image compression than data compression
 * 
 * Created By : Varul Srivastava
 * Reviewed by : Ashwin Mittal
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
int column_size = 100 * 1024 * 1024 / 8;
const int RULE = 150;
int row_size = 1;
char **hasharray;
int c_x = 0, c_y = 0;
void initMain(char *pswd, int len)
{
    row_size = max(len, strlen(pswd));
    column_size = column_size / row_size;
    hasharray = (char *)malloc(column_size * sizeof(char *));
    hasharray = initPrg(pswd);
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

/**
 *  prevents dictionary attacks by removing character bias
 *  NOTE : Check for uniqueness of passwords or rate of hash collisions 
*/
char **removeBias(char *ar)
{
    char arr[column_size][row_size];
    for (int x = 0; x < row_size; x++)
    {
        if (x == 0)
        {
            arr[0][x] = (ar[x] * MULT_NUM + ADD_NUM) % MOD;
        }
        else
        {
            arr[0][x] = ((ar[x] * MULT_NUM + ADD_NUM) ^ arr[0][x - 1]) % MOD;
        }
    }
    return arr;
}

char **initPrg(char *pswd)
{
    row_size = max(row_size, strlen(pswd));
    char **base = removeBias(pswd);
    for (int x = 1; x < column_size + 1; x++)
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

char getNum()
{
    char val = 0;
    for (int x = 0; x < 8; x++)
    {
        val = val | (((hasharray[x + c_x][c_y / 8] & (1 << (c_y % 8))) != 0) << x);
    }
    return val;
    c_x += 8;
    if (c_x >= column_size)
    {
        // x-overflow of grid
        c_x = 0;
        c_y++;
    }
    if (c_y >= row_size)
    {
        // reinitialization of the grid
        hasharray = initPrg(hasharray[column_size]);
        c_x = 0;
        c_y = 0;
    }
}
