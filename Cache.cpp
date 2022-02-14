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
    tuple<unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr, blockSize);
    for (int i = 0; i <= sets.size(); i++) {
        if ((getSet(i).getBlock(0).getTag() == get<0>(addrComponents))&&
            getSet(i).getBlock(0).getValid()) {
            // Cache hit
            // Read word
            return readWordFromCache(addr);
        } else {
            // Cache miss
            //write new block into cache, than read from it

        }
    }
    return 0;
}

void Cache::writeWord(unsigned int addr, unsigned int word) {
    // Create tuple to hold address metadata
    // For loop to loop through all sets
    // If block we want is in cache:
    // On write back: write word to block, set dirty flag to true
    // On read through, write word to cache block & memory
    // else block not in cache
    // write block to cache, than write word
    // Same logic as on cache hit, just we write block to cache first
}

tuple<unsigned int, unsigned int> Cache::getBlockRange(unsigned int addr, const Memory &mem)  {
    // calculate the first and last mem addresses of a block
    // first mem address of block = address - block offset
    tuple<unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr, blockSize);
    unsigned int startOfBlock = addr - get<1>(addrComponents);
    // last mem address = first mem address + (blockSize - 1)
    unsigned int endOfBlock = startOfBlock + (blockSize - 1);
    return make_tuple(startOfBlock, endOfBlock);
}

void Cache::moveIn(unsigned int addr, const Memory &mem) {
    // Need to edit to get to the correct set, then access the block inside of that set
    // get the block number for the address, than which block in cache by blockNum % num sets


    // Will need to update to deal with asociative caching
    // Get which block the needed addr is in
    // Set all values of blocks[0] to the values of the addresses block
    tuple<unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr, blockSize);
    tuple<unsigned int, unsigned int> blockMemRange = Cache::getBlockRange(addr, mem);
    //Cache::getBlockRange(addr, mem);
    blocks[0].setSize(blockSize);
    for (int i = addr; i <= get<1>(blockMemRange); i++) {
        blocks[0].writeByte(i, mem.readByte(i));
    }
    blocks[0].setTag(get<0>(addrComponents));
    blocks[0].setDirty(false);
    blocks[0].setValid(true);
    //blocks[0] = moveBlock;
}

int Cache::getBlockNumber(unsigned int addr) {
    // divide size of memory by the size of each block -> total num blocks
}

