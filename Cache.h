//
// Created by alogg on 2/11/2022.
//

#ifndef ARILOGGIACACHESIMULATOR_CPP_CACHE_H
#define ARILOGGIACACHESIMULATOR_CPP_CACHE_H

#include "Memory.h"
#include "Block.h"
#include "Set.h"

#include <vector>

using namespace std;

class Cache {
private:
    static int blockSize;
    int numSets;
    vector<Set> sets;
    bool isWriteBack;
    Memory memory;

public:
    Cache(int cacheSize, unsigned int blockSize, int associativity, bool writeBack);

    unsigned int readWordFromCache(unsigned int addr);

    void rightWordToCache(unsigned int addr, unsigned int word);

    Set& getSet(int setNum);

    unsigned int readWord(unsigned int addr);

    void writeWord(unsigned int addr, unsigned int word);

    static tuple<unsigned int, unsigned int> getBlockRange(unsigned int addr, const Memory& mem);

    void moveIn(unsigned int addr, const Memory& mem);

    int getBlockNumber(unsigned int addr);
};


#endif //ARILOGGIACACHESIMULATOR_CPP_CACHE_H
