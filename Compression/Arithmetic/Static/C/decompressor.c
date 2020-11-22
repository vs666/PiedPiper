#include <stdio.h>
#include "filehandler.h"
#include "model.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define PRECISION 16
#define MASK(x) (1L << (PRECISION - ((x) + 1)))
#define BUFFER_SIZE 100 * 1024 * 1024L
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int underflow_bits = 0;

void updateLowHigh(ul *low, ul *high, char ch)
{
    ul range, rescale;
    range = (*high) - (*low) + 1;
    rescale = (ul)range * table[ch]->ulimit;
    rescale /= table[255]->ulimit;
    *high = *low + rescale - 1;
    rescale = range * table[ch]->llimit;
    rescale /= table[255]->ulimit;
    *low = *low + rescale;
}

void updateFlow(ul *low, ul *high, file_manager *fm)
{
    while (1)
    {
        if ((MASK(0) & *high) == (MASK(0) & *low)) // OVERFLOW
        {
            insertBit((unsigned char)(*high & MASK(0) != 0), fm);
            while (underflow_bits > 0)
            {
                underflow_bits--;
                insertBit((*high & MASK(0) == 0), fm);
            }
        }
        else if ((*low & MASK(1)) && !(*high & MASK(1))) // underflow
        {
            underflow_bits++;
            *low &= ~(MASK(0) | MASK(1)); // drop first and second bits if 1
            *high |= MASK(1);             // on the second bit if 0
            // understand math behind underflow more accuratley
        }
        else
        {
            break;
        }
        *low <<= 1;
        *high <<= 1;
        *high |= 1;
    }
    // fprintf(stderr, "Debug %lu %lu\n", *low, *high);
}

int assignFrequencies(file_manager *fm)
{
    // read first how many bytes??

    int c;
    ul count;
    int i, nextBit;

    total_size = 0;
    initTable();

    /* read [character, probability] sets */
    for (;;)
    {
        c = readChar(fm);
        printf("%d is fc\n", c);
        count = 0;

        /* read PRECISION - 2 bits one at a time (LSB first) */
        for (i = 0; i < (PRECISION - 2); i++)
        {
            if ((nextBit = readBit(fm)) == EOF)
            {
                /* premature EOF */
                fprintf(stderr, "Error: unexpected EOF\n");
                return 0;
            }
            else if (nextBit == 1)
            {
                /* or in 1 bit */
                count |= (1 << i);
            }
        }

        if (count == 0)
        {
            break;
        }
        printf("Count is %d\n", count);
        table[c]->freq = count;
        total_size += count;
    }

    scaleTable();
    for (int x = 0; x < 256; x++)
    {
        printf("%d\t%lu\t%lu\n", x, table[c]->llimit, table[c]->ulimit);
    }
    return 1;
}

void staticDecompression(char *source_filename, int permissionsSource, char *dest_filename, int permissionsDest)
{
    file_manager *inFile = openFile(source_filename, permissionsSource, READ_TYPE);
    file_manager *outFile = openFile(dest_filename, permissionsDest, WRITE_TYPE);
    if (!assignFrequencies(inFile))
    {
        printf("Error : Return 0 \n");
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Error :: Invalid inputs.");
        return 1;
    }
    staticDecompression(argv[1], O_RDWR | __O_LARGEFILE, argv[2], O_RDWR | O_CREAT | O_TRUNC);

    return 0;
}