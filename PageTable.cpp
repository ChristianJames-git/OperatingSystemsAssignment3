#include "PageTable.h"

#include <utility>

PageTable::PageTable(int n, int c, char *o, char* file, vector<int> levels) {
    memoryaccesses = n;
    cachecap = c;
    outputmode = o;
    inputfile = file;
    this->levels = move(levels);
}

Map* PageTable::pageLookup(unsigned int virtualAddress) {
    return nullptr;
}
