//
// Created by alogg on 2/11/2022.
//

#ifndef ARILOGGIACACHESIMULATOR_CPP_GLOBALFUNCTIONS_H
#define ARILOGGIACACHESIMULATOR_CPP_GLOBALFUNCTIONS_H
#include <tuple>
using namespace std;

class GlobalFunctions {
public:
    tuple<unsigned int, unsigned int> addressAsTuple(unsigned int addr, unsigned int blockckSize);
    static unsigned int extractBits(unsigned int word, int numBits, int startPos);
    unsigned int alignAddress(unsigned int addr);
};

#endif //ARILOGGIACACHESIMULATOR_CPP_GLOBALFUNCTIONS_H
