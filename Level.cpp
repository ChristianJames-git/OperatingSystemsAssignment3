#include "Level.h"

Level::Level(int depth, PageTable* pagetable) {
    this->depth = depth;
    pt = pagetable;
    if (depth < pt->maxDepth-1) {
        nextLevelPtr = new Level *[(int) pow(2, pt->levels[depth])];
        for (int i = 0 ; i < (int) pow(2, pt->levels[depth]) ; i++)
            nextLevelPtr[i] = nullptr;
        pt->totalBytes += (unsigned int)(sizeof(Level*) * pow(2, pt->levels[depth]));
    }
    else {
        frameMap = new Map *[(int) pow(2, pt->levels[depth])];
        for (int i = 0; i < (int) pow(2, pt->levels[depth]); i++)
            frameMap[i] = nullptr;
        pt->totalBytes += (unsigned int)(sizeof(Map*) * pow(2, pt->levels[depth]));
    }
}

unsigned int Level::addLevelPtr(unsigned int address) {
    if (depth < pt->maxDepth - 1) {
        auto* temp = new Level(depth+1, pt);
        nextLevelPtr[PageTable::virtualAddressToPageNum(address, pt->bitmask[depth], pt->bitshift[depth])] = temp;
        return temp->addLevelPtr(address);
    } else
        return addFrameMap(address);
}

unsigned int Level::addFrameMap(unsigned int address) {
    Map* temp = new Map();
    temp->frameNumber = pt->frameindex;
    frameMap[PageTable::virtualAddressToPageNum(address, pt->bitmask[depth], pt->bitshift[depth])] = temp;
    return pt->frameindex++;
}

Level *Level::getLevelPtr(unsigned int address) {
    if (depth == pt->maxDepth-1)
        return nullptr;
    return nextLevelPtr[PageTable::virtualAddressToPageNum(address, pt->bitmask[depth], pt->bitshift[depth])];
}

Map* Level::getFrameMap(unsigned int address) {
    return frameMap[address];
}
