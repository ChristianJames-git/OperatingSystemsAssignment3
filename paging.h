#ifndef OPERATINGSYSTEMSASSIGNMENT3_PAGING_H
#define OPERATINGSYSTEMSASSIGNMENT3_PAGING_H

#define OBITMASK 0
#define OV2PADDRESS 1
#define OV2PADDRESS_TLB 2
#define OV2PNUMBER 3
#define OOFFSET 4
#define OSUMMARY 5

#include "PageTable.h"
#include <cstring>
#include <fstream>

class paging {
public:
    explicit paging(PageTable* pt);
    void readTrace();
private:
    void openFile();
    FILE* inFile;
    PageTable* pt;
    int outputMode;
};


#endif //OPERATINGSYSTEMSASSIGNMENT3_PAGING_H
