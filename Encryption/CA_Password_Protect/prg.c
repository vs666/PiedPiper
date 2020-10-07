/**
 * Pseudo Random Number Generator using Cellular Automaton
 * Code is to be include as libraru in the naive encryption/decryption algorithm
 * This method is space inefficient, but has more application in image compression than data compression
 * 
 * Created By: Varul Srivastava
 * Reviewed by: Varul Srivastava
 * 
 * This algorithm uses Wolfram's Rule of Cellular Automaton
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "prg.h"

const long long int MULT_NUM = (long long int)1092342049;
const long long int ADD_NUM = (long long int)133942323;
const long long int MOD = (long long int)256;
int column_size = 100 * 1024 * 1024 / 8;
const int RULE = 30;
int row_size = 1;
unsigned char **hasharray;
int c_x = 0, c_y = 0;

void initMain(unsigned char *pswd, int len)
{
    row_size = max(len, strlen(pswd));
    column_size = column_size / row_size;
    hasharray = (unsigned char **)malloc((column_size + 1) * sizeof(unsigned char *));
    for (int x = 0; x < column_size + 1; x++)
    {
        hasharray[x] = (unsigned char *)malloc(row_size);
    }
    hasharray = initPrg(pswd);
}

unsigned char conversion(int x, int y)
{
    unsigned char bit_seq = 0;
    for (int a = 0; a < 8; a++)
    {
        int num = 0;
        if (a == 0)
        {
            num = (hasharray[x][(y - 1 + row_size) % row_size] & (1 << 7)) != 0 ? 1 : 0;
        }
        else
        {
            num = (hasharray[x][y] & (1 << (a - 1))) != 0 ? 1 : 0;
        }

        num += (hasharray[x][y] & (1 << a)) != 0 ? 2 : 0;

        if (a == 7)
        {
            num += (hasharray[x][(y + 1) % row_size] & (1)) != 0 ? 4 : 0;
        }
        else
        {
            num += (hasharray[x][y] & (1 << (a + 1))) != 0 ? 4 : 0;
        }
        bit_seq += (RULE & (1 << num)) != 0 ? (1 << a) : 0;
    }
    return bit_seq;
}

/**
 *  prevents dictionary attacks by removing unsigned character bias
 *  NOTE : Check for uniqueness of passwords or rate of hash collisions 
*/
unsigned char **removeBias(unsigned char *ar)
{
    int ar_size = strlen(ar);
    unsigned char **arr = hasharray;
    for (int x = 0; x < row_size; x++)
    {
        if (x == 0)
        {
            arr[0][x] = (ar[x % ar_size] * MULT_NUM + ADD_NUM);
        }
        else
        {
            arr[0][x] = ((ar[x % ar_size] * MULT_NUM + ADD_NUM) ^ arr[0][x - 1]);
        }
    }
    return arr;
}

unsigned char **initPrg(unsigned char *pswd)
{
    unsigned char **base = removeBias(pswd);
    for (int x = 1; x < column_size + 1; x++)
    {
        for (int y = 0; y < row_size; y++)
        {
            base[x][y] = conversion(x - 1, y);
        }
    }
    return base;
}

unsigned char getNum()
{
    unsigned char val = 0;
    for (int x = 0; x < 8; x++)
    {
        val += (hasharray[c_x + x][c_y / 8] & (1 << (c_y % 8))) != 0 ? (1 << x) : 0;
    }
    c_x += 8;
    if (c_x >= column_size)
    {
        // x-overflow of grid
        c_x = 0;
        c_y++;
    }
    if (c_y / 8 >= row_size)
    {
        // reinitialization of the grid
        hasharray = initPrg(hasharray[column_size]);
        c_x = 0;
        c_y = 0;
    }
    return val;
}
