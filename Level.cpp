#include "Level.h"

Level::Level(int depth, PageTable* pagetable) {
    this->depth = depth;
    pt = pagetable;
}

void Level::addLevelPtr(unsigned int address) {
    if (depth < pt->maxDepth - 1) {
        auto* temp = new Level(depth+1, pt);
        nextLevelPtr.insert(pair<unsigned int, Level*>(PageTable::virtualAddressToPageNum(address, pt->bitmask[depth], pt->bitshift[depth]), temp));
        temp->addLevelPtr(address);
    } else
        addFrameMap(address);
}

void Level::addFrameMap(unsigned int address) {
    frameMap.insert(pair<unsigned int, unsigned int>(PageTable::virtualAddressToPageNum(address, pt->bitmask[depth], pt->bitshift[depth]), pt->frameindex++));
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
