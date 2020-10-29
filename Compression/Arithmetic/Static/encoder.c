#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long freq[256];    /* freq of each symbol */

void encode(int argc, char *argv[])
{
    char path_of_file[100005];
    strcpy(path_of_file, argv[1]);          // argv[1] = <path_of_file>

    FILE *fp = fopen(path_of_file, "r");
    if (fp !=NULL)
    {
        char line[1024];
        while (fgets(line, sizeof(line), fp) != NULL)
        {
                   
        }
        fclose(fp); 
    }
    else
        fprintf(stderr,"Incorrect path to file");    
}