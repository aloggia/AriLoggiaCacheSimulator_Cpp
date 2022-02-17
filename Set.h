//
// Created by alogg on 2/11/2022.
//

#ifndef ARILOGGIACACHESIMULATOR_CPP_SET_H
#define ARILOGGIACACHESIMULATOR_CPP_SET_H
#include "Block.h"
#include "Memory.h"
#include "GlobalFunctions.h"

#include <vector>

using namespace std;

class Set {
private:
    int associativity;
    vector<int> tagQueue;
    vector<Block> blocks;

public:
    Set(int associativity, int blockSize);


    bool checkTag(int tagToCheck);

    Block& getBlock(unsigned int addr);
};


#endif //ARILOGGIACACHESIMULATOR_CPP_SET_H
