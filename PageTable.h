#ifndef OPERATINGSYSTEMSASSIGNMENT3_PAGETABLE_H
#define OPERATINGSYSTEMSASSIGNMENT3_PAGETABLE_H

#include <vector>
#include <iostream>

using namespace std;

class Level;

class PageTable {
public:
    PageTable(int n, int c, char* o, char* file, vector<int> levels);
    unsigned int pageLookup(unsigned int virtualAddress);
    unsigned int virtualAddressToPageNum (unsigned int virtualAddress, unsigned int mask, unsigned int shift);
    Level* Level0;
    int memoryaccesses, cachecap;
    char* outputmode;
    vector<int> levels;
    char* inputfile;
    int cachehits{}, pagetablehits{}, pagetablemisses{};
    int frameindex = 0;
    vector<unsigned int> bitmask;
    vector<int> bitshift;
    unsigned int maxDepth;
private:
};


#endif //OPERATINGSYSTEMSASSIGNMENT3_PAGETABLE_H