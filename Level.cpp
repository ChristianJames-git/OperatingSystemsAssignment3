#include "Level.h"

Level::Level(int depth, PageTable* pagetable) {
    this->depth = depth;
    pt = pagetable;
    pt->totalBytes += sizeof(nextLevelPtr) + sizeof(frameMap); //Add base map bytes
}

unsigned int Level::addLevelPtr(unsigned int address) {
    if (depth < pt->maxDepth - 1) {
        auto* temp = new Level(depth+1, pt);
        pair<unsigned int, Level*> a = pair<unsigned int, Level*>(PageTable::virtualAddressToPageNum(address, pt->bitmask[depth], pt->bitshift[depth]), temp);
        pt->totalBytes += sizeof(a);
        nextLevelPtr.insert(a);
        return temp->addLevelPtr(address);
    } else
        return addFrameMap(address);
}

unsigned int Level::addFrameMap(unsigned int address) {
    pair<unsigned int, unsigned int> a = pair<unsigned int, unsigned int>(PageTable::virtualAddressToPageNum(address, pt->bitmask[depth], pt->bitshift[depth]), pt->frameindex);
    pt->totalBytes += sizeof(a);
    frameMap.insert(a);
    return pt->frameindex++;
}

Level *Level::getLevelPtr(unsigned int address) {
    try {
        return nextLevelPtr.at(PageTable::virtualAddressToPageNum(address, pt->bitmask[depth], pt->bitshift[depth]));
    } catch (const out_of_range& oor) {
        return nullptr;
    }
}

unsigned int Level::getFrameMap(unsigned int address) {
    try {
        return frameMap.at(address);
    } catch (const out_of_range& oor) {
        return UINTMAX;
    }
}
