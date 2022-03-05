//
// Created by alogg on 2/11/2022.
//

#include "Cache.h"

Cache::Cache() {
    int numBlocks = 0;
    numBlocks = CACHE_SIZE / BLOCK_SIZE;
    numSets = numBlocks / ASSOCIATIVITY;
    this->blockSize = BLOCK_SIZE;
    isWriteBack = WRITE_BACK;
    memory = Memory();
    for (int i = 0; i < numSets; i++) {
        sets.emplace_back(Set());
    }
}

unsigned int Cache::readWordFromCache(unsigned int addr) {
    // metadata for the block
    // output formatting
    return getSet(getBlockNumber(addr) % numSets).getBlock(addr).readWord(addr);
}

void Cache::writeWordToCache(unsigned int addr, unsigned int word) {
    // 3 tuple holding addr metadata
    tuple<unsigned int, unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr);
    tuple<unsigned int, unsigned int> blockRange = getBlockRange(addr);
    // write the word to cache
    getSet(getBlockNumber(addr) % numSets).getBlock(addr).writeWord(addr, word);
}

Set &Cache::getSet(int setNum) {
    //return a reference to a set in the cache
    return sets[setNum];
}

void Cache::readWord(unsigned int addr) {
    // 3 tuple to hold address metadata
    tuple<unsigned int, unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr);
    tuple<bool, unsigned int, unsigned int> blockEjectionComponents;
    tuple<unsigned int, unsigned int> blockRange = getBlockRange(addr);
    int wordRead = 0;
    cout << "read ";
        if ((sets[getBlockNumber(addr) % numSets].getBlock(addr).getTag() == get<0>(addrComponents)) &&
            sets[getBlockNumber(addr) % numSets].getBlock(addr).getValid()) {
            // Cache hit
            // Read word
            sets[getBlockNumber(addr) % numSets].updateQueue(addr);
            // store read word for outputting
            wordRead = readWordFromCache(addr);
            cout << "hit";
            cout << "[addr=" << addr << " index=" << get<1>(addrComponents);
            cout << " block_index=" << sets[getBlockNumber(addr) % numSets].getIndexInSet(get<0>(addrComponents));
            cout << " tag=" << get<0>(addrComponents);
            cout << ": word=" << wordRead;
            cout << " (" << get<0>(blockRange) << " - " << get<1>(blockRange) << ")";
            cout << "]" << endl;
            // print new tage queue
            cout << "[";
            vector<int> tagQueue = sets[getBlockNumber(addr) % numSets].getTagQueue();
            for (int & i : tagQueue) {
                cout << to_string(i) << " ";
            }
            cout << "]\n" << endl;

        } else {
            // Cache miss
            //write new block into cache, then read from it
            //TODO: change blockIndexBeingEjected to return the correct block
            // RN it is taking in the wrong address to look for
            vector<int> oldTagQueue = sets[getBlockNumber(addr) % numSets].getTagQueue();
            int tagBeingEjected = oldTagQueue[oldTagQueue.size() - 1];
            int blockIndexBeingEjected = sets[getBlockNumber(addr) % numSets].getIndexInSet(tagBeingEjected);
            // Update the block queue -> we made an access to the set
            sets[getBlockNumber(addr) % numSets].updateQueue(addr);
            // Move a new block into the cache
            blockEjectionComponents = moveIn(addr);
            // store the word at address so we can output it
            wordRead = readWordFromCache(addr);
            cout << "miss ";
            if (get<0>(blockEjectionComponents)) {
                cout << "+ replace ";
            }
            cout << "[addr=" << addr << " index=" << get<1>(addrComponents);
            cout << " block_index=" << sets[getBlockNumber(addr) % numSets].getIndexInSet(get<0>(addrComponents));
            cout << " tag=" << get<0>(addrComponents);
            cout << ": word=" << wordRead;
            cout << " (" << get<0>(blockRange) << " - " << get<1>(blockRange) << ")";
            cout << "]" << endl;
            if (get<0>(blockEjectionComponents)) {
                // Block got ejected, so we want to print out info about the ejected block
                cout << "evict tag " << tagBeingEjected << " in block_index " << blockIndexBeingEjected << endl;
                cout << "read in (" << get<1>(blockEjectionComponents) << " - " << get<2>(blockEjectionComponents) << ")" << endl;
            }
            // Output the new tagQueue for the block we are in
            cout << "[ ";
            vector<int> tagQueue = sets[getBlockNumber(addr) % numSets].getTagQueue();
            for (int & i : tagQueue) {
                cout << to_string(i) << " ";
            }
            cout << "]\n" << endl;
        }
}

void Cache::writeWord(unsigned int addr, unsigned int word) {
    // Create tuple to hold address metadata
    tuple<unsigned int, unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr);
    tuple<bool, unsigned int, unsigned int> blockEjectionComponents;
    tuple<unsigned int, unsigned int> blockRange = getBlockRange(addr);
    cout << "write";
    if ((sets[getBlockNumber(addr) % numSets].getBlock(addr).getTag() == get<0>(addrComponents)) &&
        (sets[getBlockNumber(addr) % numSets].getBlock(addr).getValid())) {
        sets[getBlockNumber(addr) % numSets].updateQueue(addr);
        // Cache hit
        if (isWriteBack) {
            // Write back cache, so write word to cache and set the dirty flag as true
            cout << " hit";
            cout << "[addr=" << addr << " index=" << get<1>(addrComponents);
            cout << " block_index=" << sets[getBlockNumber(addr) % numSets].getIndexInSet(get<0>(addrComponents));
            cout << " tag=" << get<0>(addrComponents);
            cout << ": word=" << word;
            cout << " (" << get<0>(blockRange) << " - " << get<1>(blockRange) << ")";
            cout << "]" << endl;
            // Write our given word to the cache
            writeWordToCache(addr, word);
            // tag queue output
            cout << "[ ";
            vector<int> tagQueue = sets[getBlockNumber(addr) % numSets].getTagQueue();
            for (int & i : tagQueue) {
                cout << to_string(i) << " ";
            }
            cout << "]\n" << endl;
            // Write back cache, so set dirty flag
            sets[getBlockNumber(addr) % numSets].getBlock(addr).setDirty(true);
        } else {
            // Write through cache, write to block and memory
            // address output
            cout << " hit";
            cout << "[addr=" << addr << " index=" << get<1>(addrComponents);
            cout << " block_index=" << sets[getBlockNumber(addr) % numSets].getIndexInSet(get<0>(addrComponents));
            cout << ": word=" << word;
            cout << " (" << get<0>(blockRange) << " - " << get<1>(blockRange) << ")";
            cout << "]" << endl;
            // write the given word to the cache
            writeWordToCache(addr, word);
            //Tag queue output
            cout << "[ ";
            vector<int> tagQueue = sets[getBlockNumber(addr) % numSets].getTagQueue();
            for (int & i : tagQueue) {
                cout << to_string(i) << " ";
            }
            cout << "]\n" << endl;
            //write back cache, so also write word to memeory
            memory.writeWord(addr, word);
        }
    } else {
        // cache miss
        // move the needed block into cache
        vector<int> oldTagQueue = sets[getBlockNumber(addr) % numSets].getTagQueue();
        int tagBeingEjected = oldTagQueue[oldTagQueue.size() - 1];
        int blockIndexBeingEjected = sets[getBlockNumber(addr) % numSets].getIndexInSet(tagBeingEjected);
        sets[getBlockNumber(addr) % numSets].updateQueue(addr);
        // Move a new block into the cache
        blockEjectionComponents = moveIn(addr);
        if (isWriteBack) {
            // if writeback cache, write to cache and set as dirty
            cout << " miss ";
            if (get<0>(blockEjectionComponents)) {
                cout << "+ replace ";
            }
            cout << "[addr=" << addr << " index=" << get<1>(addrComponents);
            cout << " block_index=" << sets[getBlockNumber(addr) % numSets].getIndexInSet(addr);
            cout << ": word=" << word;
            cout << " (" << get<0>(blockRange) << " - " << get<1>(blockRange) << ")";
            cout << "]" << endl;
            //write our word to cache
            writeWordToCache(addr, word);
            //TODO: block_index doesn't return the correct value
            if (get<0>(blockEjectionComponents)) {
                // Block got ejected, so we want to print out info about the ejected block
                cout << "evict tag " << tagBeingEjected << " in block_index " << blockIndexBeingEjected << endl;
                cout << "read in (" << get<1>(blockEjectionComponents) << " - " << get<2>(blockEjectionComponents) << ")" << endl;
            }
            // tag queue output
            cout << "[ ";
            vector<int> tagQueue = sets[getBlockNumber(addr) % numSets].getTagQueue();
            for (int & i : tagQueue) {
                cout << to_string(i) << " ";
            }
            cout << "]\n" << endl;
            // set the dirty flag because this is write back
            sets[getBlockNumber(addr) % numSets].getBlock(addr).setDirty(true);
        } else {
            // write through cache, write to both cache and memory
            cout << " miss";
            if (get<0>(blockEjectionComponents)) {
                cout << "+ replace ";
            }
            // Output formatting
            cout << "[addr=" << addr << " index=" << get<1>(addrComponents);
            cout << " block_index=" << sets[getBlockNumber(addr) % numSets].getIndexInSet(addr);
            cout << ": word=" << word;
            cout << " (" << get<0>(blockRange) << " - " << get<1>(blockRange) << ")";
            cout << "]" << endl;
            cout << "[ ";
            if (get<0>(blockEjectionComponents)) {
                // Block got ejected, so we want to print out info about the ejected block
                cout << "evict tag " << tagBeingEjected << " in block_index " << blockIndexBeingEjected << endl;
                cout << "read in (" << get<1>(blockEjectionComponents) << " - " << get<2>(blockEjectionComponents) << ")" << endl;
            }
            vector<int> tagQueue = sets[getBlockNumber(addr) % numSets].getTagQueue();
            for (int & i : tagQueue) {
                cout << to_string(i) << " ";
            }
            cout << "]\n" << endl;
            //write word to memory and to the cache
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

tuple<bool, unsigned int, unsigned int> Cache::moveIn(unsigned int addr) {
    // Return Tuple is of the form (isEvicting, newRangeStart, newRangeEnd)
    tuple<unsigned int, unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr);
    tuple<unsigned int, unsigned int> blockMemRange = Cache::getBlockRange(addr);
    /*
     * The tag is only used to uniquely identify an address
     * We will need to modify our address as tuple function to return a tag, which identifies the block uniquely
     * The index: which uniquly identifies the set a block can be put in
     * Offset: The unique distance that each address has from the top of a block
     */
    // These couple lines are to ensure write-back caches get written back
    bool isEvicting;
    if (sets[getBlockNumber(addr) % numSets].getBlock(addr).getTag() == -1) {
        // The block we are replacing is empty, so no eviction
        isEvicting = false;
    } else {
        // we are evicting a block
        isEvicting = true;
    }
    // If th block is dirty, copy it into memory
    if (sets[getBlockNumber(addr) % numSets].getBlock(addr).getDirty()) {
        for (int i = addr; i < get<1>(blockMemRange); i++) {
            memory.writeByte(i, sets[getBlockNumber(addr) % numSets].getBlock(addr).readByte(i));
        }
    }
    // Overwrite/move in the metadata about the new block
    sets[getBlockNumber(addr) % numSets].getBlock(addr).setTag(get<0>(addrComponents));
    sets[getBlockNumber(addr) % numSets].getBlock(addr).setDirty(false);
    sets[getBlockNumber(addr) % numSets].getBlock(addr).setValid(true);
    // Copy memory data into the cache
    for (int i = addr; i < get<1>(blockMemRange); i++) {
        sets[getBlockNumber(addr) % numSets].getBlock(addr).writeByte(i, memory.readByte(i));
    }
    // Return tuple for output
    return make_tuple(isEvicting, get<0>(blockMemRange), get<1>(blockMemRange));
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

