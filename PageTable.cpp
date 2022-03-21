#include "PageTable.h"
#include "Level.h"

#define ADDRESSSIZE 32

PageTable::PageTable(int n, int c, char *o, char* file, vector<int> levelsBits) {
    //store values
    memoryaccesses = n;
    cachecap = c;
    outputmode = o;
    inputfile = file;
    levels = move(levelsBits);
    maxDepth = levels.size();
    //create root
    Level0 = new Level(0, this);
    //calculate bitshift and bitmask
    bitshift[0] = ADDRESSSIZE - levels[0];
    for (int i = 1 ; i < maxDepth ; i++)
        bitshift[i] = bitshift[i-1] - levels[i];
    for (int i = 0 ; i < maxDepth ; i++)
        bitmask[i] = levels[i]<<bitshift[i];
}

unsigned int PageTable::pageLookup(unsigned int virtualAddress) {
    Level* tempPtr = Level0;
    for (int i = 0 ; i < maxDepth-1 ; i++)
        tempPtr = tempPtr->getLevelPtr(virtualAddressToPageNum(virtualAddress, bitmask[i], bitshift[i]));
    return tempPtr->getFrameMap(virtualAddressToPageNum(virtualAddress, bitmask[maxDepth], bitshift[maxDepth]));
}

unsigned int PageTable::virtualAddressToPageNum(unsigned int virtualAddress, unsigned int mask, unsigned int shift) {
    unsigned int temp = virtualAddress & mask;
    temp = temp>>shift;
    return temp;
}
