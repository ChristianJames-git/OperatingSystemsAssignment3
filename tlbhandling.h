#ifndef OPERATINGSYSTEMSASSIGNMENT3_TLBHANDLING_H
#define OPERATINGSYSTEMSASSIGNMENT3_TLBHANDLING_H

#include "PageTable.h"
#include <map>

struct tlbIndex {
    unsigned int time;
    unsigned int frame;
};

class tlbhandling {
public:
    explicit tlbhandling(PageTable*);
    tlbIndex* tlbSearch(unsigned int vpn);
    void addToTLB(unsigned int vpn, unsigned int frame);
private:
    bool addToLRU(unsigned int vpn, tlbIndex* temp);
    unsigned int timeAdded = 0;
    map<unsigned int, tlbIndex*> tlbCache;
    map<unsigned int, tlbIndex*> tlbLRU;
    unsigned int cacheCapacity;
    unsigned int lruCapacity = 10;
};

#endif //OPERATINGSYSTEMSASSIGNMENT3_TLBHANDLING_H
