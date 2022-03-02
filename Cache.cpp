//
// Created by alogg on 2/11/2022.
//

#include "Cache.h"

Cache::Cache() {
    int numBlocks = 0;
    numBlocks = CACHE_SIZE / BLOCK_SIZE;
    numSets = numBlocks / ASSOCIATIVITY;
    this->blockSize = BLOCK_SIZE;
    this->numSets = numSets;
    isWriteBack = WRITE_BACK;
    memory = Memory();
    for (int i = 0; i < numSets; i++) {
        sets.emplace_back(Set());
    }
}

unsigned int Cache::readWordFromCache(unsigned int addr, bool isHit) {
    // metadata for the block
    tuple<unsigned int, unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr);
    // output formatting
    if (isHit) {
        // word we are reading is in the cache
        // read the word from cache
        return getSet(getBlockNumber(addr) % numSets).getBlock(addr).readWord(addr);
    } else {
        // word we are reading is not in the cache, so move that block into the cache
        moveIn(addr);
        // read the word from cache
        return getSet(getBlockNumber(addr) % numSets).getBlock(addr).readWord(addr);
    }

}

void Cache::writeWordToCache(unsigned int addr, unsigned int word) {
    // 3 tuple holding addr metadata
    tuple<unsigned int, unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr);
    tuple<unsigned int, unsigned int> blockRange = getBlockRange(addr);
    // output
    cout << " word=" << word << " (" <<
         get<0>(blockRange) << "-" << get<1>(blockRange) << ")]" << endl;
    // write the word to cache
    getSet(getBlockNumber(addr) % numSets).getBlock(addr).writeWord(addr, word);
}

Set &Cache::getSet(int setNum) {
    //return a refrence to a set in the cache
    return sets[setNum];
}

void Cache::readWord(unsigned int addr) {
    // 3 tuple to hold address metadata
    tuple<unsigned int, unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr);
    tuple<unsigned int, unsigned int> blockRange = getBlockRange(addr);
    // TODO: This will always hold the wrong tagQueue
    vector<int> tagQueue = sets[getBlockNumber(addr) % numSets].getTagQueue();
    bool isHit;
    int wordRead = 0;
    cout << "read ";
        if ((sets[getBlockNumber(addr) % numSets].getBlock(addr).getTag() == get<0>(addrComponents)) &&
            sets[getBlockNumber(addr) % numSets].getBlock(addr).getValid()) {
            // Cache hit
            // Read word
            sets[getBlockNumber(addr) % numSets].updateQueue(addr);
            isHit = true;
            wordRead = readWordFromCache(addr, isHit);
            cout << "hit";
            cout << "[addr=" << addr << " index=" << get<1>(addrComponents);
            //TODO: Wrong block index
            cout << " block_index=" << sets[getBlockNumber(addr) % numSets].getIndexInSet(addr);
            cout << " tag=" << get<0>(addrComponents);
            cout << ": word=" << wordRead;
            cout << " (" << get<0>(blockRange) << " - " << get<1>(blockRange) << ")";
            cout << "]" << endl;
            for (int & i : tagQueue) {
                cout << to_string(i) << " ";
            }
            cout << "]" << endl;

        } else {
            // Cache miss
            //write new block into cache, then read from it
            isHit = false;
            sets[getBlockNumber(addr) % numSets].updateQueue(addr);
            wordRead = readWordFromCache(addr, isHit);
            cout << "miss ";
            cout << "[addr=" << addr << " index=" << get<1>(addrComponents);
            cout << " block_index=" << sets[getBlockNumber(addr) % numSets].getIndexInSet(addr);
            cout << " tag=" << get<0>(addrComponents);
            cout << ": word=" << wordRead;
            cout << " (" << get<0>(blockRange) << " - " << get<1>(blockRange) << ")";
            cout << "]" << endl;
            cout << "[ ";
            for (int & i : tagQueue) {
                cout << to_string(i) << " ";
            }
            cout << "]" << endl;
        }
}

void Cache::writeWord(unsigned int addr, unsigned int word) {
    // Create tuple to hold address metadata
    tuple<unsigned int, unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr);
    tuple<unsigned int, unsigned int> blockRange = getBlockRange(addr);
    vector<int> tagQueue = sets[getBlockNumber(addr) % numSets].getTagQueue();
    cout << "write";
    if ((sets[getBlockNumber(addr) % numSets].getBlock(addr).getTag() == get<0>(addrComponents)) &&
        (sets[getBlockNumber(addr) % numSets].getBlock(addr).getValid())) {
        sets[getBlockNumber(addr) % numSets].updateQueue(addr);
        // Cache hit
        if (isWriteBack) {
            // Write back cache, so write word to cache and set the dirty flag as true
            cout << " hit";
            cout << "[addr=" << addr << " index=" << get<1>(addrComponents);
            cout << " block_index=" << sets[getBlockNumber(addr) % numSets].getIndexInSet(addr);
            cout << " tag=" << get<0>(addrComponents);
            cout << ": word=" << word;
            cout << " (" << get<0>(blockRange) << " - " << get<1>(blockRange) << ")";
            cout << "]" << endl;
            writeWordToCache(addr, word);
            cout << "[ ";
            for (int & i : tagQueue) {
                cout << to_string(i) << " ";
            }
            cout << "]" << endl;
            sets[getBlockNumber(addr) % numSets].getBlock(addr).setDirty(true);
        } else {
            // Write through cache, write to block and memory
            cout << " hit";
            cout << "[addr=" << addr << " index=" << get<1>(addrComponents);
            cout << " block_index=" << sets[getBlockNumber(addr) % numSets].getIndexInSet(addr);
            cout << ": word=" << word;
            cout << " (" << get<0>(blockRange) << " - " << get<1>(blockRange) << ")";
            cout << "]" << endl;
            writeWordToCache(addr, word);
            cout << "[ ";
            for (int & i : tagQueue) {
                cout << to_string(i) << " ";
            }
            cout << "]" << endl;
            memory.writeWord(addr, word);
        }
    } else {
        // cache miss
        // move the needed block into cache
        sets[getBlockNumber(addr) % numSets].updateQueue(addr);
        moveIn(addr);
        if (isWriteBack) {
            // if writeback cache, write to cache and set as dirty
            cout << " miss";
            cout << "[addr=" << addr << " index=" << get<1>(addrComponents);
            cout << " block_index=" << sets[getBlockNumber(addr) % numSets].getIndexInSet(addr);
            cout << ": word=" << word;
            cout << " (" << get<0>(blockRange) << " - " << get<1>(blockRange) << ")";
            cout << "]" << endl;
            writeWordToCache(addr, word);
            cout << "[ ";
            for (int & i : tagQueue) {
                cout << to_string(i) << " ";
            }
            cout << "]" << endl;
            sets[getBlockNumber(addr) % numSets].getBlock(addr).setDirty(true);
        } else {
            // write through cache, write to both cache and memory
            cout << " miss";
            cout << "[addr=" << addr << " index=" << get<1>(addrComponents);
            cout << " block_index=" << sets[getBlockNumber(addr) % numSets].getIndexInSet(addr);
            cout << ": word=" << word;
            cout << " (" << get<0>(blockRange) << " - " << get<1>(blockRange) << ")";
            cout << "]" << endl;
            cout << "[ ";
            for (int & i : tagQueue) {
                cout << to_string(i) << " ";
            }
            cout << "]" << endl;
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

tuple<unsigned int, unsigned int> Cache::getBlockRange(unsigned int addr) const  {
    // calculate the first and last mem addresses of a block
    // first mem address of block = address - block offset
    tuple<unsigned int, unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr);
    unsigned int startOfBlock = addr - get<2>(addrComponents);
    // last mem address = first mem address + (blockSize - 1)
    unsigned int endOfBlock = startOfBlock + (blockSize - 1);
    return make_tuple(startOfBlock, endOfBlock);
}

void Cache::moveIn(unsigned int addr) {
    // Will need to update to deal with asociative caching
    // Get which block the needed addr is in
    // Set all values of blocks[0] to the values of the addresses block
    // TODO: Update to deal with writeback caching
    //cout << " replace";
    tuple<unsigned int, unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr);
    tuple<unsigned int, unsigned int> blockMemRange = Cache::getBlockRange(addr);
    /*
     * TODO: Lets optimize this for set associative caches
     * The tag is only used to uniquely identify an address
     * We will need to modify our address as tuple function to return a tag, which identifies the block uniquely
     * The index: which uniquly identifies the set a block can be put in
     * Offset: The unique distance that each address has from the top of a block
     */
    // These couple lines are to ensure write-back caches get written back
    if (sets[getBlockNumber(addr) % numSets].getBlock(addr).getDirty()) {
        for (int i = addr; i < get<1>(blockMemRange); i++) {
            memory.writeByte(i, sets[getBlockNumber(addr) % numSets].getBlock(addr).readByte(i));
        }
    }
    sets[getBlockNumber(addr) % numSets].getBlock(addr).setTag(get<0>(addrComponents));
    sets[getBlockNumber(addr) % numSets].getBlock(addr).setDirty(false);
    sets[getBlockNumber(addr) % numSets].getBlock(addr).setValid(true);
    for (int i = addr; i < get<1>(blockMemRange); i++) {
        sets[getBlockNumber(addr) % numSets].getBlock(addr).writeByte(i, memory.readByte(i));
    }
}

int Cache::getBlockNumber(unsigned int addr) const {
    // divide size of memory by the size of each block -> total num blocks
    // mod with the number of blocks -> which block in cache should the block be put into
    return (addr / blockSize) % numSets;
}

Memory &Cache::getMemory() {
    // return a refrence to the memory vector
    return memory;
}

