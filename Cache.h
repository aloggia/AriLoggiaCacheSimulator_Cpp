//
// Created by alogg on 2/11/2022.
//

#ifndef ARILOGGIACACHESIMULATOR_CPP_CACHE_H
#define ARILOGGIACACHESIMULATOR_CPP_CACHE_H

#include "Memory.h"
#include "Block.h"
#include "Set.h"
#include "common.h"

#include <vector>
#include <ostream>
#include <iostream>

using namespace std;

class Cache {
private:
    int blockSize;
    int numSets;
    vector<Set> sets;
    bool isWriteBack;
    Memory memory;

public:
    Cache();

    unsigned int readWordFromCache(unsigned int addr);

    void writeWordToCache(unsigned int addr, unsigned int word);

    Set& getSet(int setNum);

    void readWord(unsigned int addr);

    void writeWord(unsigned int addr, unsigned int word);

    [[nodiscard]] tuple<unsigned int, unsigned int> getBlockRange(unsigned int addr) const;

    tuple<bool, unsigned int, unsigned int> moveIn(unsigned int addr);

    [[nodiscard]] int getBlockNumber(unsigned int addr) const;

    Memory& getMemory();
};


#endif //ARILOGGIACACHESIMULATOR_CPP_CACHE_H
