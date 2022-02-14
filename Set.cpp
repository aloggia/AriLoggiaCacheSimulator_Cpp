//
// Created by alogg on 2/11/2022.
//

#include "Set.h"

#include <utility>

Set::Set(int associativity, int blockSize) {
    vector<Block> blockSet;
    for (int i = 0; i <= associativity; i++) {
        blockSet.emplace_back(blockSize);
        tagQueue.emplace_back(-1);
    }
    this->associativity = associativity;
}

void Set::moveIn(unsigned int addr, const Memory& mem, int blockSize) {
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

bool Set::checkTag(int tagToCheck) {
    // Will need to update to deal with associative caching
    return tagQueue[0] == tagToCheck;
}

Block& Set::getBlock(int blockNumber) {
    // WIll need to update
    return blocks[blockNumber];
}
