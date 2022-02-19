//
// Created by alogg on 2/11/2022.
//

#include "Cache.h"

Cache::Cache() {
    int numBlocks = 0;
    numBlocks = CACHE_SIZE / BLOCK_SIZE;
    int numSets = 0;
    numSets = numBlocks / ASSOCIATIVITY;
    this->blockSize = BLOCK_SIZE;
    this->numSets = numSets;
    isWriteBack = WRITE_BACK;
    memory = Memory();
    for (int i = 0; i < numSets; i++) {
        sets.emplace_back(Set());
    }
}

unsigned int Cache::readWordFromCache(unsigned int addr) {
    tuple<unsigned int, unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr);
    tuple<unsigned int, unsigned int> blockRange = getBlockRange(addr, memory);
    cout << " word=" << getSet(get<1>(addrComponents)).getBlock(addr).readWord(addr) << " (" <<
         get<0>(blockRange) << "-" << get<1>(blockRange) << ")]" << endl;
    return getSet(getBlockNumber(addr) % numSets).getBlock(addr).readWord(addr);

}

void Cache::writeWordToCache(unsigned int addr, unsigned int word) {
    tuple<unsigned int, unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr);
    tuple<unsigned int, unsigned int> blockRange = getBlockRange(addr, memory);
    cout << " word=" << word << " (" <<
         get<0>(blockRange) << "-" << get<1>(blockRange) << ")]" << endl;
    getSet(getBlockNumber(addr) % numSets).getBlock(addr).writeWord(addr, word);
}

Set &Cache::getSet(int setNum) {
    return sets[setNum];
}

unsigned int Cache::readWord(unsigned int addr) {
    tuple<unsigned int, unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr);
    cout << "read ";
        if ((sets[getBlockNumber(addr) % numSets].getBlock(addr).getTag() == get<0>(addrComponents)) &&
            sets[getBlockNumber(addr) % numSets].getBlock(addr).getValid()) {
            // Cache hit
            // Read word
            cout << "hit";
            cout << "[addr=" << addr << " index=0 tag=" << get<0>(addrComponents);
            return readWordFromCache(addr);
        } else {
            // Cache miss
            //write new block into cache, then read from it
            cout << "miss";
            cout << "[addr=" << addr << " index=0 tag=" << get<0>(addrComponents);
            moveIn(addr, memory);
            return readWordFromCache(addr);
        }
}

void Cache::writeWord(unsigned int addr, unsigned int word) {
    // Create tuple to hold address metadata
    tuple<unsigned int, unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr);
    cout << "write";
    if ((sets[getBlockNumber(addr) % numSets].getBlock(addr).getTag() == get<0>(addrComponents)) &&
        (sets[getBlockNumber(addr) % numSets].getBlock(addr).getValid())) {
        // Cache hit
        if (isWriteBack) {
            // Write back cache, so write word to cache and set the dirty flag as true
            cout << " hit";
            cout << "[addr=" << addr << " index=0 tag=" << get<0>(addrComponents);
            writeWordToCache(addr, word);
            sets[getBlockNumber(addr) % numSets].getBlock(addr).setDirty(true);
        } else {
            // Write through cache, write to block and memory
            cout << " hit";
            cout << "[addr=" << addr << " index=0 tag=" << get<0>(addrComponents);
            writeWordToCache(addr, word);
            memory.writeWord(addr, word);
        }
    } else {
        // cache miss
        moveIn(addr, memory);
        if (isWriteBack) {
            cout << " miss";
            cout << "[addr=" << addr << " index=0 tag=" << get<0>(addrComponents);
            writeWordToCache(addr, word);
            sets[getBlockNumber(addr) % numSets].getBlock(addr).setDirty(true);
        } else {
            cout << " miss";
            cout << "[addr=" << addr << " index=0 tag=" << get<0>(addrComponents);
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
    tuple<unsigned int, unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr);
    unsigned int startOfBlock = addr - get<2>(addrComponents);
    // last mem address = first mem address + (blockSize - 1)
    unsigned int endOfBlock = startOfBlock + (blockSize - 1);
    return make_tuple(startOfBlock, endOfBlock);
}

void Cache::moveIn(unsigned int addr, const Memory &mem) {
    // Will need to update to deal with asociative caching
    // Get which block the needed addr is in
    // Set all values of blocks[0] to the values of the addresses block
    cout << " replace";
    tuple<unsigned int, unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr);
    tuple<unsigned int, unsigned int> blockMemRange = Cache::getBlockRange(addr, mem);
    /*
     * TODO: Lets optimize this for set associative caches
     * The tag is only used to uniquely identify an address
     * We will need to modify our address as tuple function to return a tag, which identifies the block uniquely
     * The index: which uniquly identifies the set a block can be put in
     * Offset: The unique distance that each address has from the top of a block
     */
    sets[getBlockNumber(addr) % numSets].getBlock(addr).setTag(get<0>(addrComponents));
    sets[getBlockNumber(addr) % numSets].getBlock(addr).setDirty(false);
    sets[getBlockNumber(addr) % numSets].getBlock(addr).setValid(true);
    for (int i = addr; i < get<1>(blockMemRange); i++) {
        sets[getBlockNumber(addr) % numSets].getBlock(addr).writeByte(i, mem.readByte(i));
    }
}

int Cache::getBlockNumber(unsigned int addr) const {
    // divide size of memory by the size of each block -> total num blocks
    // mod with the number of blocks -> which block in cache should the block be put into
    return (addr / blockSize) % numSets;
}

Memory &Cache::getMemory() {
    return memory;
}

