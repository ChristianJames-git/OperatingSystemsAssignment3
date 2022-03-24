#include "Level.h"

Level::Level(int depth, PageTable* pagetable) {
    this->depth = depth;
    pt = pagetable; //save pagetable
    if (depth < pt->maxDepth-1) { //if interior node
        nextLevelPtr = new Level *[(int) pow(2, pt->levels[depth])]; //create nextLevelPtr array
        for (int i = 0 ; i < (int) pow(2, pt->levels[depth]) ; i++) //set all values to nullptr
            nextLevelPtr[i] = nullptr;
        pt->totalBytes += (unsigned int)(sizeof(Level*) * pow(2, pt->levels[depth])); //add bytes of all pointers
    }
    else { //if leaf node
        frameMap = new Map *[(int) pow(2, pt->levels[depth])]; //create frameMap array
        for (int i = 0; i < (int) pow(2, pt->levels[depth]); i++) //set all to nullptr
            frameMap[i] = nullptr;
        pt->totalBytes += (unsigned int)(sizeof(Map*) * pow(2, pt->levels[depth])); //add bytes of all pointers
    }
}

unsigned int Level::addLevelPtr(unsigned int address) {
    if (depth < pt->maxDepth - 1) { //if interior node
        auto* temp = new Level(depth+1, pt); //create Level* of next depth
        nextLevelPtr[PageTable::virtualAddressToPageNum(address, pt->bitmask[depth], pt->bitshift[depth])] = temp; //add to array
        return temp->addLevelPtr(address); //recurse
    } else //if leaf node
        return addFrameMap(address); //create Map* and return frame
}

unsigned int Level::addFrameMap(unsigned int address) {
    Map* temp = new Map(); //create Map*
    temp->frameNumber = pt->frameindex; //set frame number to incrementing value
    frameMap[PageTable::virtualAddressToPageNum(address, pt->bitmask[depth], pt->bitshift[depth])] = temp; //add to array
    return pt->frameindex++; //return frame and increment
}

Level *Level::getLevelPtr(unsigned int address) {
    if (depth == pt->maxDepth-1) //if leaf node return null as no level pointers
        return nullptr;
    return nextLevelPtr[PageTable::virtualAddressToPageNum(address, pt->bitmask[depth], pt->bitshift[depth])]; //return Level*
}

Map* Level::getFrameMap(unsigned int address) {
    return frameMap[address]; //only called on leaf nodes so no checking needed
}
