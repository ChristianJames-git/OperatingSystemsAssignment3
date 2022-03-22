#include "paging.h"
#include "tracereader.h"

paging::paging(PageTable *pt) {
    this->pt = pt;
    if (strcmp(pt->outputmode, "bitmasks") == 0) outputMode = OBITMASK;
    else if (strcmp(pt->outputmode, "virtual2physical") == 0) outputMode = OV2PADDRESS;
    else if (strcmp(pt->outputmode, "v2p_tlb_pt") == 0) outputMode = OV2PADDRESS_TLB;
    else if (strcmp(pt->outputmode, "vpn2pfn") == 0) outputMode = OV2PNUMBER;
    else if (strcmp(pt->outputmode, "offset") == 0) outputMode = OOFFSET;
    else if (strcmp(pt->outputmode, "summary") == 0) outputMode = OSUMMARY;

    openFile();
}

void paging::openFile() {
    inFile = fopen(pt->inputfile, "r");
    if (!inFile) {
        cerr << "Unable to open <<" << pt->inputfile << ">>" << endl;
        exit(EXIT_FAILURE);
    }
}

void paging::readTrace() {
    p2AddrTr mtrace;
    unsigned int vAddr;
    while(NextAddress(inFile, &mtrace)) {
        vAddr = mtrace.addr;
    }
}
