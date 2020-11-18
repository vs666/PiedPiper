# SHA-512 

This segment contains hash algorithm SHA-512 and its better (faster implementation using Cellular Automaton). We have used the findings of J. Suganya and K. J. Jegadish Kumar, implementing Rule(N) instead of the overflow addition which is computationally expensive and cryptographically less useful.

## Usage 

Using this for test purpose :

1. The SHA-512 implementation : 

```bash
$ ./hashF [string]
```
for example:    
```bash
$ ./hashF abc
edd852768c45c6c560b7a3bfe3e7de2ce1bee8ad5db22f1c4a9fea74be3a6de2ed4ca45cbd3fe66a56d0f503d6612c80e33a5f2a6486af5c932c87a0ad39c031
```

2. Cell-Automaton based implementation : 

yet to implement


## Written by 

Varul Srivastava