#include "output_mode_helpers.h"
#include <getopt.h>
#include "PageTable.h"
#include "tracereader.h"

#define DEFAULTN -1 //Process all addresses
#define DEFAULTC 0 //no TLB caching
#define DEFAULTO "summary" //Show summary stats

int main(int argc, char **argv) {
    if (argc < 3) { //make sure correct args amount
        cout << "Please input correct number of inputs" << endl;
        exit(EXIT_FAILURE);
    }
    char* traceFile;
    int temp, mandatoryargs = 0;
    vector<int> levels;
    int optionalints[2] = {DEFAULTN, DEFAULTC};
    char* optionaloutput = (char*)DEFAULTO;
    while ((temp = getopt (argc, argv, "-:n:c:o:")) != -1) {
        switch (temp) {
            case 'n':
                if (optarg)
                    if (strtol(optarg, nullptr, 10) >= 0)
                        optionalints[0] = strtol(optarg, nullptr, 10);
                break;
            case 'c':
                if (optarg && strtol(optarg, nullptr, 10) >= 0) //ensures cache capacity is at least 0
                    optionalints[1] = strtol(optarg, nullptr, 10);
                else {
                    cerr << "Cache capacity must be a number, greater than or equal to 0";
                    exit(EXIT_FAILURE);
                }
                break;
            case 'o':
                optionaloutput = optarg;
                break;
            case 1: //mandatory args
                if (optarg) {
                    if (mandatoryargs++ == 0) { //saves first mandatory arg as file name
                        traceFile = optarg;
                    } else {
                        if (strtol(optarg, nullptr, 10) > 1)
                            levels.push_back(strtol(optarg, nullptr, 10));
                        else {
                            cerr << "Level " << levels[0] << " page table must be at least 1 bit";
                            exit(EXIT_FAILURE);
                        }
                    }
                }
            default:
                break;
        }
    }
    temp = 0;
    for (int level : levels)
        temp += level;
    if (temp > 28) {
        cerr << "Too many bits used in page tables";
        exit(EXIT_FAILURE);
    }
    auto* pt = new PageTable(optionalints[0], optionalints[1], optionaloutput, traceFile, levels);
}
