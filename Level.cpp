#include "Level.h"

Level::Level(int depth, PageTable* pagetable) {
    this->depth = depth;
    pt = pagetable;
}

void Level::addLevelPtr(unsigned int address) {
    if (depth >= pt->maxDepth) {
        cout << "Cannot add level to leaf node" << endl;
        exit(EXIT_FAILURE);
    }
    auto* temp = new Level(depth+1, pt);
    nextLevelPtr.insert(pair<unsigned int, Level*>(address, temp));
}

void Level::addFrameMap(unsigned int address) {
    if (depth < pt->maxDepth) {
        cout << "Cannot add mapping to interior node" << endl;
        exit(EXIT_FAILURE);
    }
    frameMap.insert(pair<unsigned int, unsigned int>(address, pt->frameindex++));
}

Level *Level::getLevelPtr(unsigned int address) {
    try {
        return nextLevelPtr.at(address);
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
