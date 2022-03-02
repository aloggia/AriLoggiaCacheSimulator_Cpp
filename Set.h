//
// Created by alogg on 2/11/2022.
//

#ifndef ARILOGGIACACHESIMULATOR_CPP_SET_H
#define ARILOGGIACACHESIMULATOR_CPP_SET_H
#include "Block.h"
#include "Memory.h"
#include "GlobalFunctions.h"
#include <optional>

#include <vector>

using namespace std;

class Set {
private:
    vector<int> tagQueue;
    vector<Block> blocks;

public:
    Set();


    bool checkTag(int tagToCheck);

    Block& getBlock(unsigned int addr);

    int getNumBlocksInSet() const;

    int getIndexInSet(unsigned int addr) const;

    vector<int> getTagQueue() const;

    void updateQueue(unsigned int addr);
};


#endif //ARILOGGIACACHESIMULATOR_CPP_SET_H
