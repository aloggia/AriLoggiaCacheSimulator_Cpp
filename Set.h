//
// Created by alogg on 2/11/2022.
//

#ifndef ARILOGGIACACHESIMULATOR_CPP_SET_H
#define ARILOGGIACACHESIMULATOR_CPP_SET_H
#include "Block.h"

#include <vector>

using namespace std;

class Set {
private:
    int associativity;
    vector<int> tagQueue;
    vector<Block> blocks;

public:
    Set(int associativity, int blockSize);

    void moveIn(Block moveBlock);

    bool checkTag(int tagToCheck);

    Block& getBlock(int blockNumber);
};


#endif //ARILOGGIACACHESIMULATOR_CPP_SET_H
