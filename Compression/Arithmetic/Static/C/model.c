#include "model.h"
#include "filehandler.h"
#include <stdlib.h>
#include <stdio.h>

#define MASK(x) (1L << (PRECISION - ((x) + 1)))

void initTable()
{
    for (int x = 0; x < 256; x++)
    {
        table[x] = (prob_t *)malloc(sizeof(prob_t));
        table[x]->ulimit = table[x]->llimit = table[x]->freq = table[x]->sfreq = 0;
        table[x]->ch = (unsigned char)x;
    }
    total_size = 0;
}
void insertChar(const unsigned char ch) // how to make sure it is static insertion??
{
    table[ch]->freq++;
    total_size++;
}

void WriteHeader(file_manager *bfpOut)
{
    int c;
    ul previous = 0; /* symbol count so far */
    int bit;

    for (c = 0; c <= (256 - 1); c++)
    {
        if (table[c]->ulimit > previous)
        {
            /* some of these symbols will be encoded */
            insertBit((char)c, bfpOut);
            previous = (table[c]->ulimit - previous); /* symbol count */

            /* write count PRECISION - 2 bits, one at a time (LSB first) */
            for (bit = 0; bit < (PRECISION - 2); bit++)
            {
                insertBit((previous & 1), bfpOut);
                previous >>= 1;
            }

            /* current upper range is previous for the next character */
            previous = table[c]->ulimit;
        }
    }

    /* now write end of table (zero count) */
    insertBit(0x00, bfpOut);
    previous = 0;
    ul i = 0;
    while (i < PRECISION - 2)
        insertBit((unsigned char)(MASK(i++) & previous != 0), bfpOut);
}

void scaleTable()
{
    int scaleFactor = 1;
    if (total_size > (1 << (PRECISION - 2)))
    {
        // fprintf(stderr,"Here\n");
        scaleFactor = total_size / (1 << (PRECISION - 2)) + 1;
    }
    for (int x = 0; x < 256; x++)
    {
        table[x]->sfreq = table[x]->freq / scaleFactor;
        if (x == 0)
        {
            table[x]->llimit = 0;
        }
        else
        {
            table[x]->llimit = table[x - 1]->ulimit;
        }
        table[x]->ulimit = table[x]->llimit + table[x]->sfreq;
    }
}
