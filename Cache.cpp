//
// Created by alogg on 2/11/2022.
//

#include "Cache.h"

Cache::Cache(int cacheSize, unsigned int blockSize, int associativity, bool writeBack) {
    int numBlocks = 0;
    numBlocks = cacheSize / blockSize;
    int numSets = 0;
    numSets = numBlocks / associativity;
    this->blockSize = blockSize;
    this->numSets = numSets;
    isWriteBack = writeBack;
    memory = Memory();
    for (int i = 0; i < numSets; i++) {
        sets.emplace_back(Set(associativity, blockSize));
    }
}

unsigned int Cache::readWordFromCache(unsigned int addr) {
    unsigned int blockNumber = (addr - (addr % blockSize)) / blockSize;
    int setNum = blockNumber % sets.size();
    return getSet(setNum).getBlock(0).readWord(addr);

}

void Cache::writeWordToCache(unsigned int addr, unsigned int word) {
    unsigned int blockNumber = getBlockNumber(addr);
    int setNum = blockNumber % sets.size();
    getSet(setNum).getBlock(0).writeWord(addr, word);
}

Set &Cache::getSet(int setNum) {
    return sets[setNum];
}

unsigned int Cache::readWord(unsigned int addr) {
    tuple<unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr, blockSize);
        if ((sets[getBlockNumber(addr)].getBlock(0).getTag() == get<0>(addrComponents)) &&
            sets[getBlockNumber(addr)].getBlock(0).getValid()) {
            // Cache hit
            // Read word
            return readWordFromCache(addr);
        } else {
            // Cache miss
            //write new block into cache, then read from it
            moveIn(addr, memory);
            return readWordFromCache(addr);
        }
}

void Cache::writeWord(unsigned int addr, unsigned int word) {
    // Create tuple to hold address metadata
    tuple<unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr, blockSize);
    if ((sets[getBlockNumber(addr)].getBlock(0).getTag() == get<0>(addrComponents)) &&
        (sets[getBlockNumber(addr)].getBlock(0).getValid())) {
        // Cache hit
        if (isWriteBack) {
            // Write back cache, so write word to cache and set the dirty flag as true
            writeWordToCache(addr, word);
            sets[getBlockNumber(addr)].getBlock(0).setDirty(true);
        } else {
            // Write through cache, write to block and memory
            writeWordToCache(addr, word);
            memory.writeWord(addr, word);
        }
    } else {
        // cache miss
        moveIn(addr, memory);
        if (isWriteBack) {
            writeWordToCache(addr, word);
            sets[getBlockNumber(addr)].getBlock(0).setDirty(true);
        } else {
            writeWordToCache(addr, word);
            memory.writeWord(addr, word);
        }

    }

    // If block we want is in cache:
    // On write back: write word to block, set dirty flag to true
    // On read through, write word to cache block & memory
    // else block not in cache
    // write block to cache, than write word
    // Same logic as on cache hit, just we write block to cache first
}

tuple<unsigned int, unsigned int> Cache::getBlockRange(unsigned int addr, const Memory &mem) const  {
    // calculate the first and last mem addresses of a block
    // first mem address of block = address - block offset
    tuple<unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr, blockSize);
    unsigned int startOfBlock = addr - get<1>(addrComponents);
    // last mem address = first mem address + (blockSize - 1)
    unsigned int endOfBlock = startOfBlock + (blockSize - 1);
    return make_tuple(startOfBlock, endOfBlock);
}

void Cache::moveIn(unsigned int addr, const Memory &mem) {
    // Will need to update to deal with asociative caching
    // Get which block the needed addr is in
    // Set all values of blocks[0] to the values of the addresses block
    tuple<unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr, blockSize);
    tuple<unsigned int, unsigned int> blockMemRange = Cache::getBlockRange(addr, mem);

    sets[getBlockNumber(addr) % sets.size()].getBlock(0).setSize(blockSize);
    sets[getBlockNumber(addr) % sets.size()].getBlock(0).setTag(get<0>(addrComponents));
    sets[getBlockNumber(addr) % sets.size()].getBlock(0).setDirty(false);
    sets[getBlockNumber(addr) % sets.size()].getBlock(0).setValid(true);
    for (int i = addr; i < get<1>(blockMemRange); i++) {
        sets[getBlockNumber(addr) % sets.size()].getBlock(0).writeByte(i, mem.readByte(i));
    }
}

int Cache::getBlockNumber(unsigned int addr) {
    // divide size of memory by the size of each block -> total num blocks
    //TODO: returns the wrong values sometimes
    return (addr - (addr % blockSize)) / blockSize;
}

Memory &Cache::getMemory() {
    return memory;
}

