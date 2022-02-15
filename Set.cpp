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
    return tagQueue[0] == tagToCheck;
}

Block& Set::getBlock(int blockNumber) {
    // WIll need to update
    return blocks[blockNumber];
}
