#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int freq[257];
void decoder(FILE *fp, FILE *fp2)
{
  if (fp != NULL)
  {
    char line[100000];
    int i;
    for (i = 0; i < 256; i++)
    {
      freq[i] = getw(fp);
    }
    /*while(fgets(line, sizeof(line), fp) != NULL)
        {
        }*/
    unsigned int low = 0U;
    unsigned int high = 0xFFFFFFFFU;
    unsigned int range;
    unsigned int a, b;
    while (1)
    {
      range = high - low;
      for (i = 0; i < 256; i++)
      {
      }
    }

    fclose(fp);
  }
  else
    perror("");

  return;
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    printf("INVALID USAGE OF COMMAND\n");
    return 1;
  }

  FILE *fp = fopen(argv[1], "r");
  FILE *fp2 = fopen(argv[2], "w+");

  decoder(fp, fp2);

  return 0;
}