/**
 * Arithmetic encoding
 * 
 * 
 * Created By : Zishan Kazi
 * Reviewed by :
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "help.c"

/* return 0 = success | return 1 = error */
int main(int argc, char *argv[])
{
    /* argv[1] = file_path */
    if (argc != 2)
    {
        fprintf(stderr,"Usage: ./compress <path_of_file>\n");
        return 1;
    }
    if (strcmp(argv[1],"--help") == 0 || strcmp(argv[1],"-h") == 0)
        return help();
    
    encode(argc, argv);





    return 0;
}
