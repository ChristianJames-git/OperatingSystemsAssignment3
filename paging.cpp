#include "paging.h"

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

bool paging::readTrace() {
    p2AddrTr mtrace;
    unsigned int vAddr;
    unsigned int temp;
    if (outputMode == OBITMASK) {
        report_bitmasks((int)pt->maxDepth, &pt->bitmask[0]);
        return true;
    }
    while((pt->pagetablehits + pt->pagetablemisses) < pt->memoryaccesses && NextAddress(inFile, &mtrace)) {
        vAddr = mtrace.addr;
        switch (outputMode) {
            case OV2PADDRESS:
                temp = pt->pageLookup(vAddr);
                report_virtual2physical(vAddr, (temp<<pt->offsetsize) + pt->offsetbitmask & vAddr);
                break;
            case OV2PADDRESS_TLB:
                cout << "not done yet" << endl;
                break;
            case OV2PNUMBER:
                break;
            case OOFFSET:
                hexnum(pt->offsetbitmask & vAddr);
                break;
            case OSUMMARY:
                break;
        }
    }
}
