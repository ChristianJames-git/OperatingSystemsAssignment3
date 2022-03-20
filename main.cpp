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
    int temp = 0, mandatoryargs = 0;
    int levels[argc];
    while ((temp = getopt (argc, argv, "-:n:c:o:")) != -1) {
        switch (temp) {
            case 'n':
                if (optarg) cout << "n:" << optarg << endl;
                break;
            case 'c':
                if (optarg && strtol(optarg, nullptr, 10) >= 0) //ensures cache capacity is at least 0
                    cout << "c:" << optarg << endl;
                else {
                    cerr << "Cache capacity must be a number, greater than or equal to 0";
                    exit(EXIT_FAILURE);
                }
                break;
            case 'o':
                if (optarg) cout << "o:" << optarg << endl;
                break;
            case 1: //mandatory args
                if (optarg) {
                    if (mandatoryargs++ == 0) { //saves first mandatory arg as file name
                        traceFile = optarg;
                        levels[0] = 0;
                    } else {
                        levels[0];
                        if (strtol(optarg, nullptr, 10) > 1)
                            levels[++levels[0]] = strtol(optarg, nullptr, 10);
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
    for (int i = 1 ; i <= levels[0] ; i++)
        temp += levels[i];
    if (temp > 28) {
        cerr << "Too many bits used in page table";
        exit(EXIT_FAILURE);
    }
    cout << "File: " << traceFile << endl;
    for (int i = 1 ; i <= levels[0] ; i++) {
        cout << "Level " << i << ": " << levels[i] << endl;
    }
}
