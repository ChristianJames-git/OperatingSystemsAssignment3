#include <iostream>
#include "output_mode_helpers.h"
#include <getopt.h>

#define DEFAULTC = 0

int main(int argc, char **argv) {
    if (argc < 3) { //make sure correct args amount
        cout << "Please input correct number of inputs" << endl;
        exit(EXIT_FAILURE);
    }
    char* traceFile;
    int c = 0, mandatoryargs = 0;
    int levels[argc];
    while ((c = getopt (argc, argv, "-:n:c:o:")) != -1) {
        switch (c) {
            case 'n':
                if (optarg) cout << "n:" << optarg << endl;
                break;
            case 'c':
                if (optarg && strtol(optarg, nullptr, 10) >= 0)
                    cout << "c:" << optarg << endl;
                else {
                    cerr << "Cache capacity must be a number, greater than or equal to 0";
                    exit(EXIT_FAILURE);
                }
                break;
            case 'o':
                if (optarg) cout << "o:" << optarg << endl;
                break;
            case 1:
                if (optarg) {
                    if (mandatoryargs++ == 0) {
                        traceFile = optarg;
                        levels[0] = 0;
                    } else {
                        levels[0]++;
                        levels[levels[0]] = strtol(optarg, nullptr, 10);
                    }
                }
            default:
                break;
        }
    }
    cout << "File: " << traceFile << endl;
    for (int i = 1 ; i <= levels[0] ; i++) {
        cout << "Level " << i << ": " << levels[i] << endl;
    }
}
