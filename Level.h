#ifndef OPERATINGSYSTEMSASSIGNMENT3_LEVEL_H
#define OPERATINGSYSTEMSASSIGNMENT3_LEVEL_H

#include "PageTable.h"
#include <map>

class Level {
public:
    Level(int depth, PageTable* pagetable);
    void addLevelPtr(unsigned int);
    void addFrameMap(unsigned int);
    Level* getLevelPtr(unsigned int);
    unsigned int getFrameMap(unsigned int);
    PageTable* pt;
private:
    int depth;
    map<unsigned int, Level*> nextLevelPtr;
    map<unsigned int, unsigned int> frameMap;
};


#endif //OPERATINGSYSTEMSASSIGNMENT3_LEVEL_H
