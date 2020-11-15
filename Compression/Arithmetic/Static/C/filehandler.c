#include "filehandler.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
long long cun = 0;
file_manager *openFile(const char *filename, int flags, int type)
{
    file_manager *fp = (file_manager *)malloc(sizeof(file_manager));
    if (fp == NULL)
    {
        printf("Error in memory allocation\n");
        errno = ENOMEM;
        return NULL;
    }
    if (type == WRITE_TYPE)
        fp->fileptr = open(filename, O_RDWR | O_CREAT | __O_LARGEFILE, 0666);
    else if (type == READ_TYPE)
        fp->fileptr = open(filename, O_RDWR | __O_LARGEFILE);

    fp->file_type = type;
    // allocation done
    if (fp->fileptr < 0)
    {
        perror("Error");
        printf("File Opening Error %s \n", filename);
        errno = ENFILE;
        free(fp);
        return NULL;
    }

    return fp;
}

int closeFile(file_manager *fp)
{
    if (fp->file_type == WRITE_TYPE && fp->waitlength != 0)
    {
        flush(fp);
    }
    close(fp->fileptr);
    free(fp); // error handling here ??
    return 0;
}

void insertBit(unsigned char ch, file_manager *fm)
{
    if (ch != 1 && ch != 0)
    {
        errno = EINVAL;
        return;
    }
    fm->buffer <<= 1;
    fm->buffer |= ch;
    fm->waitlength++;
    if (fm->waitlength == 8)
    {
        flush(fm);
    }
}

// check this while writing decompression code once, then delete this comment
unsigned char readBit(file_manager *fm)
{
    if (fm->waitlength == 0)
    {
        read(fm->fileptr, &fm->buffer, 1);
        fm->waitlength = 8;
    }
    fm->waitlength--;

    unsigned char rval = fm->buffer & (1 << fm->waitlength);
    fm->buffer &= ~(1 << fm->waitlength);
    return (unsigned char)(rval != 0);
}

unsigned char readChar(file_manager *fm)
{
    if (fm->waitlength == 0)
    {
        char ch[2];
        int n = read(fm->fileptr, ch, 1);
        printf("Read Character is %d n is %d\n", ch[0], n);
        return ch[0];
    }
    else if (fm->waitlength == 8)
    {
        fm->waitlength = 0;
        unsigned char ret = fm->buffer;
        fm->buffer = (unsigned char)0;
        return ret;
    }
    else
    {
        // make something
        int ret = fm->buffer;
        ret <<= 8;
        char ch;
        read(fm->fileptr, &ch, 1);
        ret |= ch;
        fm->buffer = 0;
        fm->buffer |= (unsigned char)(ret & ((1 << (fm->waitlength)) - 1));
        ret >>= (fm->waitlength - 1);
        return (unsigned char)(ret & ((1 << 8) - 1));
    }
}

void flush(file_manager *fm)
{
    // printf("%d ", fm->buffer);
    write(fm->fileptr, &fm->buffer, 1);
    fm->waitlength = 0;
    fm->buffer = (unsigned char)0;
}
