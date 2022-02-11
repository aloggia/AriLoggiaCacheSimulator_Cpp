//
// Created by alogg on 2/11/2022.
//

#include "Cache.h"

Cache::Cache(int cacheSize, unsigned int blockSize, int associativity, bool writeBack) {
    int numBlocks = cacheSize / blockSize;
    int numSets = numBlocks / associativity;
    for (int i = 0; i <= numSets; i++) {
        sets.emplace_back(Set(associativity, blockSize));
    }
    this->blockSize = blockSize;
    this->numSets = numSets;
    isWriteBack = writeBack;
    memory = Memory();
}

unsigned int Cache::readWordFromCache(unsigned int addr) {
    unsigned int blockNumber = (addr - (addr % blockSize)) / blockSize;
    int setNum = blockNumber % sets.size();
    return getSet(setNum).getBlock(0).readWord(addr);

}

void Cache::rightWordToCache(unsigned int addr, unsigned int word) {
    unsigned int blockNumber = (addr - (addr % blockSize)) / blockSize;
    int setNum = blockNumber % sets.size();
    getSet(setNum).getBlock(0).writeWord(addr, word);
}

Set &Cache::getSet(int setNum) {
    return sets[setNum];
}

unsigned int Cache::readWord(unsigned int addr) {
    return 0;
}

void Cache::writeWord(unsigned int addr, unsigned int word) {

}
