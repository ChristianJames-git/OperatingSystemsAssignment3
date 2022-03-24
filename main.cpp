#include <getopt.h>
#include "paging.h"

#define DEFAULTN UINTMAX //Process all addresses
#define DEFAULTC 0 //no TLB caching
#define DEFAULTO "summary" //Show summary stats

int main(int argc, char **argv) {
    if (argc < 2) { //make sure correct args amount
        cout << "Please input correct number of inputs" << endl;
        exit(EXIT_FAILURE);
    }
    char* traceFile;
    int temp, mandatoryargs = 0;
    vector<int> levels;
    unsigned int optionalints[2] = {DEFAULTN, DEFAULTC}; //set to default values
    char* optionaloutput = (char*)DEFAULTO; //set to default
    while ((temp = getopt (argc, argv, "-:n:c:o:")) != -1) { //handle arguments. '-' causes it to not skip mandatory ones, they have temp=1
        switch (temp) {
            case 'n':
                if (optarg)
                    if (strtol(optarg, nullptr, 10) >= 0)
                        optionalints[0] = strtol(optarg, nullptr, 10); //store n
                break;
            case 'c':
                if (optarg && strtol(optarg, nullptr, 10) >= 0) //ensures cache capacity is at least 0
                    optionalints[1] = strtol(optarg, nullptr, 10);
                else { //throw error
                    cerr << "Cache capacity must be a number, greater than or equal to 0";
                    exit(EXIT_FAILURE);
                }
                break;
            case 'o':
                optionaloutput = optarg; //store outputmode
                break;
            case 1: //mandatory args
                if (optarg) {
                    if (mandatoryargs++ == 0) { //saves first mandatory arg as file name
                        traceFile = optarg;
                    } else {
                        if (strtol(optarg, nullptr, 10) > 1)
                            levels.push_back(strtol(optarg, nullptr, 10)); //level bit counts
                        else { //throw error
                            cerr << "Level " << levels.size() << " page table must be at least 1 bit";
                            exit(EXIT_FAILURE);
                        }
                    }
                }
            default:
                break;
        }
    }
    temp = 0;
    for (int level : levels) //add up total bits used for levels
        temp += level;
    if (temp > 28) { //check if too many bits were used for levels
        cerr << "Too many bits used in page tables";
        exit(EXIT_FAILURE);
    }
    auto* pt = new PageTable(optionalints[0], optionalints[1], optionaloutput, traceFile, levels); //create PageTable controller
    auto* pagingPtr = new paging(pt); //create paging object
    pagingPtr->readTrace(); //call main process
}
