//
// Created by alogg on 2/11/2022.
//

#include "Set.h"


Set::Set() {
    for (int i = 1; i <= ASSOCIATIVITY; i++) {
        blocks.emplace_back(Block());
        tagQueue.emplace_back(-1);
    }
}


bool Set::checkTag(int tagToCheck) {
    // Will need to update to deal with associative caching
    // lmao this is v wrong
    return tagQueue[0] == tagToCheck;
}

Block& Set::getBlock(unsigned int addr) {
    tuple<unsigned int, unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr);
    for (int i = 0; i < tagQueue.size(); i++) {
        if (tagQueue[i] == -1) {
            for (int j = 0; j < i; j++) {
                tagQueue[i] = tagQueue[i-1];
            }
            tagQueue[0] = get<0>(addrComponents);
            return blocks[i];
        }
    }
    return blocks[0];
}
