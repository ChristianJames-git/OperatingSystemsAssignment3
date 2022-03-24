#include "tlbhandling.h"

tlbhandling::tlbhandling(PageTable* pageTable) {
    cacheCapacity = pageTable->cachecap;
    if (cacheCapacity < 10)
        lruCapacity = cacheCapacity;
}

tlbIndex *tlbhandling::tlbSearch(unsigned int vpn) {
    tlbIndex* toReturn;
    try {
        toReturn = tlbCache.at(vpn);
        toReturn->time = timeAdded++;
        addToLRU(vpn, toReturn);
        return toReturn;
    } catch (const out_of_range& oor) { //vpn is not in the cache
        return nullptr;
    }
}

void tlbhandling::addToTLB(unsigned int vpn, unsigned int frame) {
    auto* temp = new tlbIndex();
    temp->frame = frame;
    temp->time = timeAdded++;
    if (tlbCache.size() < cacheCapacity) {
        tlbCache.insert(pair<unsigned int, tlbIndex*>(vpn, temp));
        addToLRU(vpn, temp);
    } else {
        map<unsigned int, tlbIndex*>::iterator it;
        unsigned int min = UINTMAX;
        unsigned int minVPN;
        for (it = tlbLRU.begin() ; it != tlbLRU.end() ; it++) {
            if (it->second->time < min) {
                min = it->second->time;
                minVPN = it->first;
            }
        }
        it = tlbLRU.find(minVPN);
        tlbLRU.erase(it);
        it = tlbCache.find(minVPN);
        tlbCache.erase(it);
        tlbCache.insert(pair<unsigned int, tlbIndex*>(vpn, temp));
        tlbLRU.insert(pair<unsigned int, tlbIndex*>(vpn, temp));
    }
}

bool tlbhandling::addToLRU(unsigned int vpn, tlbIndex *temp) {
    try { tlbLRU.at(vpn); } catch (const out_of_range& oor) {
        if (tlbLRU.size() < lruCapacity)
            tlbLRU.insert(pair<unsigned int, tlbIndex*>(vpn, temp));
        else {
            map<unsigned int, tlbIndex*>::iterator it;
            unsigned int min = UINTMAX;
            unsigned int minVPN;
            for (it = tlbLRU.begin() ; it != tlbLRU.end() ; it++) {
                if (it->second->time < min) {
                    min = it->second->time;
                    minVPN = it->first;
                }
            }
            it = tlbLRU.find(minVPN);
            tlbLRU.erase(it);
            tlbLRU.insert(pair<unsigned int, tlbIndex*>(vpn, temp));
        }
    }
}
