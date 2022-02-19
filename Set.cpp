//
// Created by alogg on 2/11/2022.
//

#include "Set.h"


Set::Set() {
    // loop a total of ASSOIATIVITY times, adding a new block to the set and a new value to the tag queue
    for (int i = 1; i <= ASSOCIATIVITY; i++) {
        blocks.emplace_back(Block());
        tagQueue.emplace_back(-1);
    }
}


bool Set::checkTag(int tagToCheck) {
    // Will need to update to deal with associative caching
    // this is v wrong
    return tagQueue[0] == tagToCheck;
}

Block& Set::getBlock(unsigned int addr) {
    // Returns a refrence to a speific block
    // Updates the tag queue when a block is accessesed
    // Calling this function is equivilent to a block access, so we'll need to update the tag queue
    // due to principle of locality
    // The tag queue updating is a bit broken right now though
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
