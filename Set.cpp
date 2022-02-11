//
// Created by alogg on 2/11/2022.
//

#include "Set.h"

Set::Set(int associativity, int blockSize) {
    vector<Block> blockSet;
    for (int i = 0; i <= associativity; i++) {
        blockSet.emplace_back(blockSize);
        tagQueue.emplace_back(-1);
    }
    this->associativity = associativity;
}

void Set::moveIn(Block moveBlock) {
    blocks[0] = moveBlock;
}

bool Set::checkTag(int tagToCheck) {
    return tagQueue[0] == tagToCheck;
}

Block& Set::getBlock(int blockNumber) {
    return blocks[blockNumber];
}
