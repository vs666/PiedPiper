#ifndef __PSEUDO_BASIC___
#define __PSEUDO_BASIC___

void initMain(char *, int);
char conversion(char **base, int x, int y);
char **removeBias(char *ar);
char **initPrg(char *pswd);
char **initPrg(char *pswd, int row_size);
int getNum();

#endif /* __PSEUDO_BASIC___ */