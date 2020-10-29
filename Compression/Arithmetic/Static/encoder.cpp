#include <iostream>
#include <fstream>
#include <bits/stdc++.h>

#define END_OF_FILE (unsigned char)255
using namespace std;
typedef unsigned int ll;

typedef struct ProbabilityRange
{
    char ch;
    ll high;
    ll low;
    ll denominator;
} PR;

PR *makePR(char cha)
{
    PR *ob = (PR *)malloc(sizeof(PR));
    ob->ch = cha;
    ob->high = 0;
    ob->low = 0;
    ob->denominator = 1;
    return ob;
}

class ProbabilityDensity
{
public:
    ll gen_denom = 0;
    vector<pair<PR *, ll>> ar;
    ProbabilityDensity()
    {
        for (int x = 0; x < 265; x++)
        {
            this->ar.push_back({makePR((char)x), 0});
        }
        this->gen_denom = 0;
        update_probability(END_OF_FILE);
    }
    void update_probability(char ch)
    {
        this->gen_denom++;
        this->ar[(int)ch].second++;
    }
    void initPDF()
    {
        makePDF();
    }
    PR *getProbability(char ch)
    {
        return this->ar[ch].first;
    }
    void makePDF()
    {
        ll low = 0;
        ll high = 0;
        for (int x = 0; x < 256; x++)
        {
            low = high;
            high += this->ar[x].second;
            this->ar[x].first->low = low;
            this->ar[x].first->high = high;
            this->ar[x].first->denominator = gen_denom;
        }
    }
};

// read a file and init pdf model
// then make an encoder function that also reads the file and then encodes the bytes
// encoding to be done in infinite precision
// END_OF_FILE constant to be taken as EOF FLAG.

// Assume pre_initialized model to work with

int genCount = 0;
unsigned char bSt = '\0';
ifstream inf;
ofstream ouf;

void output_bit(ll n)
{
    bSt <<= 1;
    if (n == 1)
    {
        bSt |= 1;
    }
    genCount++;
    if (genCount == 8)
    {
        ouf << (unsigned char)bSt;
        bSt = '\0';
        genCount = 0;
    }
    // cout << n;
}

void encode(ProbabilityDensity *model)
{
    unsigned int high = 0xFFFFFFFFU;
    unsigned int low = 0U;
    char c;
    while (inf >> c)
    {
        unsigned int range = high - low;
        ProbabilityRange *p = model->getProbability(c);
        high = low + (unsigned int)(range * p->high);
        low = low + (unsigned int)(range * p->low);

        for (;;)
        {

            if (high < 0x80000000U) //  1000 0000 0000 0000 0000 0000 0000 0000 in binary 
            {
                output_bit(0);
            }
            else if (low >= 0x80000000U)    //  1000 0000 0000 0000 0000 0000 0000 0000 in binary 
            {
                output_bit(1);
            }
            else
                break;
            fflush(stdout);
            low <<= 1;
            high <<= 1;
            high |= 1;
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Invalid command." << endl;
        cout << "Usage: " << argv[0] << " <original_filename>"
             << " <compressed_filename>" << endl;
        exit(0);
    }

    cout << "Compression Started...." << endl;

    ifstream ink;
    ink.open(argv[1], ios::in);
    char ch;
    ProbabilityDensity *obj = new ProbabilityDensity();

    while (!ink.eof())
    {
        ink.get(ch);
        obj->update_probability(ch);
    }
    ouf.open(argv[2], ios::in);
    inf.open(argv[1], ios::out);
    obj->initPDF();
    encode(obj);

    cout << "----------------------------------------------" << endl;
    cout << "Compression complete" << endl;
}