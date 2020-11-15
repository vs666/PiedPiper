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

void staticCompression(char *source_filename, int permissionsSource, char *dest_filename, int permissionsDest)
{
    file_manager *inpFile = openFile(dest_filename, permissionsDest, WRITE_TYPE);
    int fptr = open(source_filename, permissionsSource); // consider large file always

    // size of file
    struct stat sta;
    stat(source_filename, &sta);
    ul file_size = sta.st_size;
    ul rem_size = file_size;

    initTable();
    while (rem_size > 0)
    {
        ul read_size = MIN(rem_size, BUFFER_SIZE);
        char *buff = (char *)malloc(read_size);
        read(fptr, buff, read_size);
        for (int x = 0; x < read_size; x++)
        {
            insertChar(buff[x]);
        }
        free(buff);
        rem_size -= read_size;
    }
    scaleTable();
    WriteHeader(inpFile);
    exit(0);
    ul high = (1L << PRECISION) - 1;
    ul low = 0;

    lseek64(fptr, (ul)0, SEEK_SET);
    rem_size = file_size;
    while (rem_size > 0)
    {
        ul read_size = MIN(rem_size, BUFFER_SIZE);
        char *buff = (char *)malloc(read_size);
        read(fptr, buff, read_size);
        for (int x = 0; x < read_size; x++)
        {
            // code for updating high and low
            updateLowHigh(&low, &high, buff[x]);
            // code for checking and writing overflow/underflow..
            updateFlow(&low, &high, inpFile);
        }
        free(buff);
        rem_size -= read_size;
        // fprintf(stderr, "Size Remaining: %ld\n", rem_size);
    }
    // printf("Closing fptr 102\n");
    // fflush(stdout);
    close(fptr);

    // UNABLE TO APPEND EOF CHARACTER TO THE FILE (Error will be seen in decoding) idea! instead append something else
    // fprintf(stderr, "Closed fptr 102\n");
    // fflush(stdout);
    // updateLowHigh(&low, &high, (unsigned char)(255));
    // fprintf(stderr, "Last line has error\n");
    // fflush(stderr);
    // updateFlow(&low, &high, inpFile);
    // fprintf(stderr, "Hello world\n");
    // fflush(stderr);
    
    flush(inpFile);
    closeFile(inpFile);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Error :: Invalid inputs.");
        return 1;
    }
    staticCompression(argv[1], O_RDWR | __O_LARGEFILE, argv[2], O_RDWR | O_CREAT | O_TRUNC);
    // printf("Compression Done...\n");
    return 0;
}