#ifndef OPERATINGSYSTEMSASSIGNMENT3_PAGETABLE_H
#define OPERATINGSYSTEMSASSIGNMENT3_PAGETABLE_H

#include <vector>
#include <iostream>
#include <cmath>

#define ADDRESSSIZE 32
#define UINTMAX 4294967295

using namespace std;

class Level;

class PageTable {
public:
    PageTable(unsigned int n, unsigned int c, char* o, char* file, vector<int> levels);
    unsigned int pageLookup(unsigned int virtualAddress);
    unsigned int pageInsert(unsigned int virtualAddress) const;
    static unsigned int virtualAddressToPageNum (unsigned int virtualAddress, unsigned int mask, unsigned int shift);
    Level* Level0;
    unsigned int memoryaccesses, cachecap;
    char* outputmode;
    vector<int> levels;
    int offsetsize = 32;
    char* inputfile;
    int cachehits{}, pagetablehits{}, pagetablemisses{};
    unsigned int frameindex = 0;
    vector<unsigned int> bitmask;
    unsigned int offsetbitmask;
    unsigned int vpnmask;
    vector<int> bitshift;
    unsigned int vpnshift;
    unsigned int maxDepth;
    unsigned int totalBytes = 0;
    bool cachehit = false, pagetablehit = false;
};

struct Map {
    unsigned int frameNumber;
};


#endif //OPERATINGSYSTEMSASSIGNMENT3_PAGETABLE_H
