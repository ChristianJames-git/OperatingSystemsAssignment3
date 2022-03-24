#include "paging.h"

paging::paging(PageTable *pt) {
    this->pt = pt;
    if (strcmp(pt->outputmode, "bitmasks") == 0) outputMode = OBITMASK; //convert string output mode to an int for a switch
    else if (strcmp(pt->outputmode, "virtual2physical") == 0) outputMode = OV2PADDRESS;
    else if (strcmp(pt->outputmode, "v2p_tlb_pt") == 0) outputMode = OV2PADDRESS_TLB;
    else if (strcmp(pt->outputmode, "vpn2pfn") == 0) outputMode = OV2PNUMBER;
    else if (strcmp(pt->outputmode, "offset") == 0) outputMode = OOFFSET;
    else if (strcmp(pt->outputmode, "summary") == 0) outputMode = OSUMMARY;

    openFile(); //open file
}

void paging::openFile() {
    inFile = fopen(pt->inputfile, "r"); //open file read mode
    if (!inFile) { //throw error
        cerr << "Unable to open <<" << pt->inputfile << ">>" << endl;
        exit(EXIT_FAILURE);
    }
}

void paging::readTrace() { //process and output manager
    p2AddrTr mtrace;
    unsigned int vAddr;
    unsigned int temp;
    tlbhandling* tlb;
    tlbIndex* tempTLB = nullptr;
    uint32_t levelAddresses[pt->maxDepth];
    if (outputMode == OBITMASK) { //handle bitmasks and return
        report_bitmasks((int)pt->maxDepth, &pt->bitmask[0]);
        return;
    }
    if (pt->cachecap != 0) //if there is a cache, create a tlb handling object
        tlb = new tlbhandling(pt);
    while((pt->pagetablehits + pt->pagetablemisses) < pt->memoryaccesses && NextAddress(inFile, &mtrace)) { //loop through address inputs
        vAddr = mtrace.addr;
        switch (outputMode) { //run different based on output mode
            case OV2PADDRESS: //"virtual2physical"
                if (pt->cachecap != 0) //if tlb
                    tempTLB = tlb->tlbSearch(PageTable::virtualAddressToPageNum(vAddr, pt->vpnmask, pt->vpnshift)); //search tlb
                if (!tempTLB) { //if not in tlb
                    temp = pt->pageLookup(vAddr); //search page tree
                    if (pt->cachecap != 0) tlb->addToTLB(PageTable::virtualAddressToPageNum(vAddr, pt->vpnmask, pt->vpnshift), temp); //add to tlb if it exists
                }
                else
                    temp = tempTLB->frame; //if in tlb
                report_virtual2physical(vAddr, (temp<<pt->offsetsize) + (pt->offsetbitmask & vAddr)); //print output
                break;
            case OV2PADDRESS_TLB: //v2p_tlb_pt
                if (pt->cachecap != 0) { //if tlb
                    tempTLB = tlb->tlbSearch(PageTable::virtualAddressToPageNum(vAddr, pt->vpnmask, pt->vpnshift)); //search tlb
                    if (!tempTLB) { //if not in tlb
                        pt->cachehit = false;
                        temp = pt->pageLookup(vAddr); //search page tree
                        if (pt->pagetablehit) //increment page hit or miss
                            pt->pagetablehits++;
                        else
                            pt->pagetablemisses++;
                        tlb->addToTLB(PageTable::virtualAddressToPageNum(vAddr, pt->vpnmask, pt->vpnshift), temp); //add tlb
                    } else { //if in tlb
                        temp = tempTLB->frame;
                        pt->cachehit = true;
                        pt->cachehits++; //increment cache hit
                    }
                } else { //if no tlb
                    temp = pt->pageLookup(vAddr);
                    if (pt->pagetablehit)
                        pt->pagetablehits++;
                    else
                        pt->pagetablemisses++;
                }
                report_v2pUsingTLB_PTwalk(vAddr, (temp<<pt->offsetsize) + (pt->offsetbitmask & vAddr), pt->cachehit, pt->pagetablehit); //print output
                break;
            case OV2PNUMBER:
                if (pt->cachecap != 0) //if tlb
                    tempTLB = tlb->tlbSearch(PageTable::virtualAddressToPageNum(vAddr, pt->vpnmask, pt->vpnshift)); //search tlb
                if (!tempTLB) { //if not in tlb
                    temp = pt->pageLookup(vAddr); //search page table
                    if (pt->cachecap != 0) tlb->addToTLB(PageTable::virtualAddressToPageNum(vAddr, pt->vpnmask, pt->vpnshift), temp); //if tlb, add to tlb
                } else //if in tlb
                    temp = tempTLB->frame;
                for (int i = 0 ; i < pt->maxDepth ; i++) //determine level vpns
                    levelAddresses[i] = PageTable::virtualAddressToPageNum(vAddr, pt->bitmask[i], pt->bitshift[i]);
                report_pagemap((int)pt->maxDepth, levelAddresses, temp); //print output
                break;
            case OOFFSET:
                hexnum(pt->offsetbitmask & vAddr); //print offset of inputs
                break;
            case OSUMMARY:
                if (pt->cachecap != 0) { //if tlb
                    tempTLB = tlb->tlbSearch(PageTable::virtualAddressToPageNum(vAddr, pt->vpnmask, pt->vpnshift)); //search tlb
                    if (!tempTLB) { //if not in tlb
                        temp = pt->pageLookup(vAddr); //search page table
                        if (pt->pagetablehit) //increment page hit or miss
                            pt->pagetablehits++;
                        else
                            pt->pagetablemisses++;
                        tlb->addToTLB(PageTable::virtualAddressToPageNum(vAddr, pt->vpnmask, pt->vpnshift), temp); //add to tlb
                    } else { //if in tlb
                        temp = tempTLB->frame;
                        pt->cachehits++; //increment cache hit
                    }
                } else { //if no tlb
                    temp = pt->pageLookup(vAddr); //search page tree to fill tree and determine hits or misses
                    if (pt->pagetablehit) //increment page hit or miss
                        pt->pagetablehits++;
                    else
                        pt->pagetablemisses++;
                }
                break;
        }
    }
    if (outputMode == OSUMMARY) //output summary after all inputs are handled
        report_summary((unsigned int)pow(2, pt->offsetsize), pt->cachehits, pt->pagetablehits, pt->cachehits + pt->pagetablehits + pt->pagetablemisses, pt->frameindex, pt->totalBytes);
}
