#include "PageTable.h"
#include "Level.h"

PageTable::PageTable(unsigned int n, unsigned int c, char *o, char* file, vector<int> levelsBits) {
    //store values
    memoryaccesses = n; //store values
    cachecap = c; //store values
    outputmode = o; //store values
    inputfile = file; //store values
    levels = move(levelsBits); //store values
    maxDepth = levels.size(); //store values
    for (int level : levels) //calculate offset size
        offsetsize -= level;
    //create root
    Level0 = new Level(0, this);
    //calculate bitshifts and bitmasks
    bitshift.push_back(ADDRESSSIZE - levels[0]);
    for (int i = 1 ; i < maxDepth ; i++)
        bitshift.push_back(bitshift[i-1] - levels[i]);
    for (int i = 0 ; i < maxDepth ; i++)
        bitmask.push_back((unsigned int)(pow(2, levels[i])-1) << bitshift[i]);
    offsetbitmask = (unsigned int)(pow(2, offsetsize)-1);
    vpnshift = offsetsize;
    vpnmask = (unsigned int)(pow(2, ADDRESSSIZE - offsetsize)-1) << vpnshift;

    totalBytes += sizeof(memoryaccesses) + sizeof(cachecap) + sizeof(outputmode) + sizeof(inputfile) + sizeof(levels) + sizeof(maxDepth) + sizeof(offsetsize)
            + sizeof(Level0) + sizeof(bitshift) + sizeof(bitmask) + sizeof(offsetbitmask) + sizeof(cachehits) + sizeof(pagetablehits) + sizeof(pagetablemisses)
            + sizeof(frameindex); //estimate size of PageTable
}

unsigned int PageTable::pageLookup(unsigned int virtualAddress) {
    Level* tempPtr = Level0;
    for (int i = 0 ; i < maxDepth-1 ; i++) { //traverse to leaf node
        tempPtr = tempPtr->getLevelPtr(virtualAddress);
        if (!tempPtr) { //if ptr is null, mark as a miss, add the missing page, and return
            pagetablehit = false;
            return pageInsert(virtualAddress);
        }
    }
    Map* temp = tempPtr->getFrameMap(virtualAddressToPageNum(virtualAddress, bitmask[maxDepth-1], bitshift[maxDepth-1])); //get Map*
    if (!temp) { //if frame mapping is missing, mark as a miss, add the missing frame mapping, and return
        pagetablehit = false;
        return pageInsert(virtualAddress);
    }
    pagetablehit = true;
    return temp->frameNumber; //return found frame number
}

unsigned int PageTable::virtualAddressToPageNum(unsigned int virtualAddress, unsigned int mask, unsigned int shift) { //helper method to isolate addresses using masking
    unsigned int temp = virtualAddress & mask;
    temp = temp>>shift;
    return temp;
}

unsigned int PageTable::pageInsert(unsigned int virtualAddress) const {
    Level* tempPtr = Level0->getLevelPtr(virtualAddress);
    Level* prevPtr = Level0;
    while (tempPtr) { //traverse until missing Level found then add
        prevPtr = tempPtr;
        tempPtr = tempPtr->getLevelPtr(virtualAddress);
    }
    return prevPtr->addLevelPtr(virtualAddress);
}
