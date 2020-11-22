#ifndef __FILEHANDLER___
#define __FILEHANDLER___

#define READ_TYPE 0
#define WRITE_TYPE 1

typedef struct file_manager
{
    int fileptr;          // file pointer for syscalls
    unsigned char buffer; // waiting buffer content
    int waitlength;       // waiting buffer size
    int FLAGS;            // flags for the file
    int file_type;        // READ or WRITE
} file_manager;

file_manager *openFile(const char *filename, int flags, int type);
int closeFile(file_manager *fp);
void insertBit(unsigned char bit, file_manager *fm); // inserts 1 bit to the to_be_inserted buffer
unsigned char readBit(file_manager *fm);             //  gets 1 bit from the file opened
void flush(file_manager *fm);
unsigned char readChar(file_manager *fm);
#endif /* __FILEHANDLER___ */