# Coding Guidelines

To maintain synchronized and efficient development, it is better if people follow a common coding standard.


## Code Modularity

1. Code should be written in modular functions to maximize reusability
2. All C and C++ codes that implement an algorithm should have a corresponding `.h` or `.hpp` file
3. Constants and Functions used in mltiple files should be contained in seperate `.h` file if they can be modified. 


## Coding Style - Commands

1. Maintaining the legacy of PiedPiper of HBO, `tab` spaced codes are expected with `4` or `8` as the spacing size.
2. All functions and variables should have meaningful names and written in proper way : 

Functions : 
```c 
int getfactorsarray(){} // not recommended
```
```c
int getFactorsArray(){} // recommended
```

Variables : 
```c
int numberoffactors = 0;    // not recommended
```
```c
int number_of_factors = 0; // recommended
```

Constants : 
```c
const int maxarraysize = 1024;  // not recommended
```
```c
const int MAX_ARRAY_SIZE = 1024; // recommended
// or alternatively
#define MAX_ARRAY_SIZE 1024;
```

Comments : 

Clarify the code as much as possible using comments for other developers to understand it.

```c
// one line comments (acceptable)
```
```c
// multiple
// line
// comments
// (unacceptable)
```
```c
/**
*  Multiple 
*   lines
*   Comments (acceptable)
*/ 
```

## File System

The file-system / directory structure should be followed for making import statements easily

1. All algorithms should be in seperate folers
2. File-System tree : 
    
    - \ 
        - Compression
            - Data/Text Compression
            - Multimedia Compression
                - Video Compression
                - Audio Compression
                - Image Compression

        - EncryptionDecryption
            - Encryption
            - Decryption
