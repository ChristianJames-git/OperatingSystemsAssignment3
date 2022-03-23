#ifndef OPERATINGSYSTEMSASSIGNMENT3_LEVEL_H
#define OPERATINGSYSTEMSASSIGNMENT3_LEVEL_H

#include "PageTable.h"
#include <array>

class Level {
public:
    Level(int depth, PageTable* pagetable);
    unsigned int addLevelPtr(unsigned int);
    unsigned int addFrameMap(unsigned int);
    Level* getLevelPtr(unsigned int);
    Map* getFrameMap(unsigned int);
    PageTable* pt;
private:
    int depth;
    Level** nextLevelPtr;
    Map** frameMap;
};


#endif //OPERATINGSYSTEMSASSIGNMENT3_LEVEL_H
