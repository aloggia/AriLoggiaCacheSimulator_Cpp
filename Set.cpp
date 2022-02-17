//
// Created by alogg on 2/11/2022.
//

#include "Set.h"


Set::Set(int associativity, int blockSize) {
    this->associativity = associativity;
    for (int i = 1; i <= associativity; i++) {
        blocks.emplace_back(Block(blockSize));
        tagQueue.emplace_back(-1);
    }
}


bool Set::checkTag(int tagToCheck) {
    // Will need to update to deal with associative caching
    // lmao this is v wrong
    return tagQueue[0] == tagToCheck;
}

Block& Set::getBlock(unsigned int addr) {
    // WIll need to update
    tuple<unsigned int, unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr);
    for(auto & i : tagQueue) {
        if (i == -1) {
            return blocks[i];
        }
    }
    return blocks[0];
}
