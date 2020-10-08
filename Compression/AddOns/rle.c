#include <stdlib.h>
#include "rle.h"

typedef unsigned char byte;

char *convert(char *inp)
{
    int index = 0;
    int n = strlen(inp);
    char *len = (char *)malloc(n * 2);
    int x = 0;
    while (x < n)
    {
        byte count = 1;
        while (x < n - 1 && inp[x] == inp[x + 1] && count < (byte)254)
        {
            x++;
            count++;
        }
        len[index++] = count;
        len[index++] = inp[x];
        x++;
    }
    return len;
}
