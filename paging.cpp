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

void paging::readTrace() {
    p2AddrTr mtrace;
    unsigned int vAddr;
    unsigned int temp;
    tlbhandling* tlb;
    tlbIndex* tempTLB = nullptr;
    uint32_t levelAddresses[pt->maxDepth];
    if (outputMode == OBITMASK) {
        report_bitmasks((int)pt->maxDepth, &pt->bitmask[0]);
        return;
    }
    if (pt->cachecap != 0)
        tlb = new tlbhandling(pt);
    while((pt->pagetablehits + pt->pagetablemisses) < pt->memoryaccesses && NextAddress(inFile, &mtrace)) {
        vAddr = mtrace.addr;
        switch (outputMode) {
            case OV2PADDRESS:
                if (pt->cachecap != 0)
                    tempTLB = tlb->tlbSearch(PageTable::virtualAddressToPageNum(vAddr, pt->vpnmask, pt->vpnshift));
                if (!tempTLB) {
                    temp = pt->pageLookup(vAddr);
                    if (pt->cachecap != 0) tlb->addToTLB(PageTable::virtualAddressToPageNum(vAddr, pt->vpnmask, pt->vpnshift), temp);
                }
                else
                    temp = tempTLB->frame;
                report_virtual2physical(vAddr, (temp<<pt->offsetsize) + (pt->offsetbitmask & vAddr));
                break;
            case OV2PADDRESS_TLB:
                if (pt->cachecap != 0) {
                    tempTLB = tlb->tlbSearch(PageTable::virtualAddressToPageNum(vAddr, pt->vpnmask, pt->vpnshift));
                    if (!tempTLB) {
                        pt->cachehit = false;
                        temp = pt->pageLookup(vAddr);
                        if (pt->pagetablehit)
                            pt->pagetablehits++;
                        else
                            pt->pagetablemisses++;
                        tlb->addToTLB(PageTable::virtualAddressToPageNum(vAddr, pt->vpnmask, pt->vpnshift), temp);
                    } else {
                        temp = tempTLB->frame;
                        pt->cachehit = true;
                        pt->cachehits++;
                    }
                } else {
                    temp = pt->pageLookup(vAddr);
                    if (pt->pagetablehit)
                        pt->pagetablehits++;
                    else
                        pt->pagetablemisses++;
                }
                report_v2pUsingTLB_PTwalk(vAddr, (temp<<pt->offsetsize) + (pt->offsetbitmask & vAddr), pt->cachehit, pt->pagetablehit);
                break;
            case OV2PNUMBER:
                if (pt->cachecap != 0)
                    tempTLB = tlb->tlbSearch(PageTable::virtualAddressToPageNum(vAddr, pt->vpnmask, pt->vpnshift));
                if (!tempTLB) {
                    temp = pt->pageLookup(vAddr);
                    if (pt->cachecap != 0) tlb->addToTLB(PageTable::virtualAddressToPageNum(vAddr, pt->vpnmask, pt->vpnshift), temp);
                } else
                    temp = tempTLB->frame;
                for (int i = 0 ; i < pt->maxDepth ; i++)
                    levelAddresses[i] = PageTable::virtualAddressToPageNum(vAddr, pt->bitmask[i], pt->bitshift[i]);
                report_pagemap((int)pt->maxDepth, levelAddresses, temp);
                break;
            case OOFFSET:
                hexnum(pt->offsetbitmask & vAddr);
                break;
            case OSUMMARY:
                if (pt->cachecap != 0) {
                    tempTLB = tlb->tlbSearch(PageTable::virtualAddressToPageNum(vAddr, pt->vpnmask, pt->vpnshift));
                    if (!tempTLB) {
                        temp = pt->pageLookup(vAddr);
                        if (pt->pagetablehit)
                            pt->pagetablehits++;
                        else
                            pt->pagetablemisses++;
                        tlb->addToTLB(PageTable::virtualAddressToPageNum(vAddr, pt->vpnmask, pt->vpnshift), temp);
                    } else {
                        temp = tempTLB->frame;
                        pt->cachehits++;
                    }
                } else {
                    temp = pt->pageLookup(vAddr);
                    if (pt->pagetablehit)
                        pt->pagetablehits++;
                    else
                        pt->pagetablemisses++;
                }
                break;
        }
    }
    if (outputMode == OSUMMARY)
        report_summary((unsigned int)pow(2, pt->offsetsize), pt->cachehits, pt->pagetablehits, pt->cachehits + pt->pagetablehits + pt->pagetablemisses, pt->frameindex, pt->totalBytes);
}
