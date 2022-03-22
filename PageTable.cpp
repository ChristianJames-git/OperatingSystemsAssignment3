#include "PageTable.h"
#include "Level.h"

PageTable::PageTable(int n, int c, char *o, char* file, vector<int> levelsBits) {
    //store values
    memoryaccesses = n;
    cachecap = c;
    outputmode = o;
    inputfile = file;
    levels = move(levelsBits);
    maxDepth = levels.size();
    for (int level : levels)
        offsetsize -= level;
    //create root
    Level0 = new Level(0, this);
    //calculate bitshift and bitmask
    bitshift.push_back(ADDRESSSIZE - levels[0]);
    for (int i = 1 ; i < maxDepth ; i++)
        bitshift.push_back(bitshift[i-1] - levels[i]);
    for (int i = 0 ; i < maxDepth ; i++)
        bitmask.push_back((unsigned int)(pow(2, levels[i])-1) << bitshift[i]);
    offsetbitmask = (unsigned int)(pow(2, offsetsize)-1);
}

unsigned int PageTable::pageLookup(unsigned int virtualAddress) {
    Level* tempPtr = Level0;
    for (int i = 0 ; i < maxDepth-1 ; i++) {
        tempPtr = tempPtr->getLevelPtr(virtualAddress);
        if (!tempPtr) { //if ptr is null, mark as a miss, add the missing page, and return
            pagetablemisses++;
            pageInsert(virtualAddress);
            return UINTMAX;
        }
    }
    unsigned int temp = tempPtr->getFrameMap(virtualAddressToPageNum(virtualAddress, bitmask[maxDepth-1], bitshift[maxDepth-1]));
    if (temp == UINTMAX) { //if frame mapping is missing, mark as a miss, add the missing frame mapping, and return
        pagetablemisses++;
        pageInsert(virtualAddress);
        return UINTMAX;
    }
    pagetablehits++;
    temp = temp
    return temp; //return found frame number
}

unsigned int PageTable::virtualAddressToPageNum(unsigned int virtualAddress, unsigned int mask, unsigned int shift) {
    unsigned int temp = virtualAddress & mask;
    temp = temp>>shift;
    return temp;
}

void PageTable::pageInsert(unsigned int virtualAddress) const {
    Level* tempPtr = Level0->getLevelPtr(virtualAddress);
    Level* prevPtr = Level0;
    while (tempPtr) {
        prevPtr = tempPtr;
        tempPtr = tempPtr->getLevelPtr(virtualAddress);
    }
    prevPtr->addLevelPtr(virtualAddress);
}
