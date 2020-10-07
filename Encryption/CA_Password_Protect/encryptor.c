/**
 * This code contains the implementation of one of the parts of our encryption algorithm, 
 * which takes a file as input, encrypts/decrypts it using standard Wolfram CA under the hood, 
 * and produces an output file (same extension).
 * 
 * Created By: Ashwin Mittal
 * Reviewed by: Ashwin Mittal
 * 
*/

#include "prg.h"
#include "encryptor.h"
#include <stdlib.h>
#include <stdio.h>

/* Returns 1 in case of errors, otherwise returns 0 */
int main(int argc, char *argv[])
{
    /* 
    argv[1] is filename
    argv[2] is password
    argv[3] is length of columns or 
    number of generations in Wolfram CA (optional)
    */
    struct stat stats;

    if (argc < 3 || argc > 4)
    {
        char error[] = "Usage: ./encrypt <filename> [PASSWORD] (x-width of stream)\n";

        write(2, error, strlen(error));
        return 1;
    }

    if (stat(argv[1], &stats) == 0 && S_ISDIR(stats.st_mode))
    {
        char error[] = "ERROR: please provide file\n";
        write(2, error, strlen(error));
        return 1;
    }

    // SEGMENTATION FAULT ON THIS LINE

    argc <= 3 ? initMain(argv[2], strlen(argv[2]))
              : initMain(argv[2], atoi(argv[3]));

    char s[1005], e[1005] = "_encrypted";

    long long i, j, n = strlen(argv[1]);

    for (i = n - 1; i >= 0; i--)
    {
        if (argv[1][i] == '.')
        {
            break;
        }
    }

    if (i >= strlen(e) && strncmp(&argv[1][i - strlen(e)], e, strlen(e)) == 0)
    {
        strcpy(s, argv[1]);
        s[i - strlen(e) + 1] = 'd';
        s[i - strlen(e) + 2] = 'e';
    }
    else
    {
        strncpy(s, argv[1], i);

        strcpy(&e[strlen(e)], &argv[1][i]);

        strcpy(&s[i], e);
    }

    int fd_in = open(argv[1], O_RDONLY);

    if (fd_in < 0)
    {
        perror(argv[1]);
        return 1;
    }

    int fd_out = open(s, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd_out < 0)
    {
        close(fd_in);
        perror(s);
        return 1;
    }
    long long SIZE = lseek(fd_in, 0, SEEK_END);

    long long BUFFER_SIZE = 1000000,
              num = SIZE / BUFFER_SIZE;

    char bytes[1000005];

    lseek(fd_in, 0, SEEK_SET);

    for (i = 0; i < num; i++)
    {
        read(fd_in, bytes, BUFFER_SIZE);
        for (j = 0; j < BUFFER_SIZE; j++)
        {
            bytes[j] ^= getNum();
        }
        write(fd_out, bytes, BUFFER_SIZE);
    }

    if (SIZE % BUFFER_SIZE > 0)
    {
        read(fd_in, bytes, SIZE % BUFFER_SIZE);
        for (j = 0; j < SIZE % BUFFER_SIZE; j++)
        {
            bytes[j] ^= getNum();
        }
        write(fd_out, bytes, SIZE % BUFFER_SIZE);
    }

    if (close(fd_in) < 0)
    {
        perror(argv[1]);
        return 1;
    }

    if (close(fd_out) < 0)
    {
        perror(s);
        return 1;
    }

    return 0;
}
