#ifndef OPERATINGSYSTEMSASSIGNMENT3_PAGETABLE_H
#define OPERATINGSYSTEMSASSIGNMENT3_PAGETABLE_H

#include <vector>
#include "Level.h"

using namespace std;

class PageTable {
public:
    PageTable(int n, int c, char* o, char* file, vector<int> levels);
    Map* pageLookup(unsigned int virtualAddress);
    int memoryaccesses, cachecap;
    char* outputmode;
    vector<int> levels;
    char* inputfile;
    int cachehits{}, pagetablehits{}, pagetablemisses{};
    int frameindex = 0;
private:
};

struct Map {
    unsigned int VPN, PFN;
};


#endif //OPERATINGSYSTEMSASSIGNMENT3_PAGETABLE_H
