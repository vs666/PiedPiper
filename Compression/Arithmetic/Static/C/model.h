#ifndef _MODEL_
#define _MODEL_

#define PRECISION 16 // TRIAL BASIS FOR COMPRESSION RATES
#include "filehandler.h"
typedef unsigned long ul;

typedef struct prob_elem
{
    ul ulimit;        // upper bound exclusive
    ul llimit;        // lower bound inclusive
    unsigned char ch; // ascii equivalent
    int freq;         // unscaled frequency
    int sfreq;        // scaled frequency
} prob_t;

ul total_size;
prob_t *table[256];

void initTable();
void insertChar(const unsigned char ch); // how to make sure it is static insertion??
void scaleTable();
void WriteHeader(file_manager *fp);
#endif /* _MODEL_ */