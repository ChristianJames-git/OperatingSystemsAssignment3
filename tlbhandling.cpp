#include "tlbhandling.h"

tlbhandling::tlbhandling(PageTable* pageTable) {
    cacheCapacity = pageTable->cachecap; //set max cache size
    if (cacheCapacity < 10) //set max lru size, 10 or less if cache is smaller than 10
        lruCapacity = cacheCapacity;
}

tlbIndex *tlbhandling::tlbSearch(unsigned int vpn) { //search tlb
    tlbIndex* toReturn;
    try { //try catch to check if value is in map
        toReturn = tlbCache.at(vpn); //in map?
        toReturn->time = timeAdded++; //only runs if vpn is found in map
        addToLRU(vpn, toReturn); //add or update LRU
        return toReturn;
    } catch (const out_of_range& oor) { //vpn is not in the cache (added to cache later in paging.cpp)
        return nullptr;
    }
}

void tlbhandling::addToTLB(unsigned int vpn, unsigned int frame) { //adds vpn,frame to tlb
    auto* temp = new tlbIndex();
    temp->frame = frame;
    temp->time = timeAdded++;
    if (tlbCache.size() < cacheCapacity) { //if there is space in the cache
        tlbCache.insert(pair<unsigned int, tlbIndex*>(vpn, temp)); //add
        addToLRU(vpn, temp); //add to LRU
    } else { //if cache is full
        map<unsigned int, tlbIndex*>::iterator it;
        unsigned int min = UINTMAX;
        unsigned int minVPN;
        for (it = tlbLRU.begin() ; it != tlbLRU.end() ; it++) { //iterate through LRU to find least recently updated
            if (it->second->time < min) {
                min = it->second->time;
                minVPN = it->first;
            }
        }
        it = tlbLRU.find(minVPN); //get interator to least recently added
        tlbLRU.erase(it); //delete it
        it = tlbCache.find(minVPN); //find and delete in cache as well
        tlbCache.erase(it);
        tlbCache.insert(pair<unsigned int, tlbIndex*>(vpn, temp)); //add new value to cache
        tlbLRU.insert(pair<unsigned int, tlbIndex*>(vpn, temp)); //add new value to LRU as well as we just made a spot
    }
}

bool tlbhandling::addToLRU(unsigned int vpn, tlbIndex *temp) { //add or update LRU
    try { tlbLRU.at(vpn); } catch (const out_of_range& oor) { //if its in LRU, skip
        if (tlbLRU.size() < lruCapacity) //if LRU is not full
            tlbLRU.insert(pair<unsigned int, tlbIndex*>(vpn, temp)); //add to LRU
        else { //if LRU is full
            map<unsigned int, tlbIndex*>::iterator it;
            unsigned int min = UINTMAX;
            unsigned int minVPN;
            for (it = tlbLRU.begin() ; it != tlbLRU.end() ; it++) { //iterate through LRU to find the least recently updated
                if (it->second->time < min) {
                    min = it->second->time;
                    minVPN = it->first;
                }
            }
            it = tlbLRU.find(minVPN); //find and erase least recently updated
            tlbLRU.erase(it);
            tlbLRU.insert(pair<unsigned int, tlbIndex*>(vpn, temp)); //add LRU for provided Cache entry
        }
    }
}
