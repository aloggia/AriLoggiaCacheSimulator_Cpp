//
// Created by alogg on 2/11/2022.
//

#ifndef ARILOGGIACACHESIMULATOR_CPP_GLOBALFUNCTIONS_H
#define ARILOGGIACACHESIMULATOR_CPP_GLOBALFUNCTIONS_H
#include <tuple>
#include "common.h"
using namespace std;

class GlobalFunctions {
public:
    // Return a 3-tuple containing (tag, index, offset)
    static tuple<unsigned int, unsigned int, unsigned int> addressAsTuple(unsigned int addr);
    // Helper function that takes in a word, number of bits to be extracted, and starting position
    // The bit index starts at 0
    static unsigned int extractBits(unsigned int word, int numBits, int startPos);
    // A verification function that ensures an address is pointed to the start of a word
    static unsigned int alignAddress(unsigned int addr);
    // Take in an int value and return the maximum num of bits needed to represent it
    // Used for determining tags, indexes and offsets
    static int maxNumBits(int value);
};

#endif //ARILOGGIACACHESIMULATOR_CPP_GLOBALFUNCTIONS_H
