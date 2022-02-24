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
    // Returns a refrence to a specific block
    // Updates the tag queue when a block is accessesed
    // Calling this function is equivilent to a block access, so we'll need to update the tag queue
    // due to principle of locality
    // The tag queue updating is a bit broken right now though
    tuple<unsigned int, unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr);
    /*if (numBlocksInSet() == 0) {
        tagQueue[0] = get<0>(addrComponents);
    } else {
        tagQueue.pop_back();
        tagQueue.insert(tagQueue.begin(), get<0>(addrComponents));
    }
     */
    for (Block& i : blocks) {
        if(i.getTag() == get<0>(addrComponents)) {
            return i;
        }
    }
    // If the requested block is not in cache, return the oldest block
    // Do this because on a miss a block will need to be brought into the cache, so by returning the oldest block
    // We tell the program: "Overwrite this block
    /*
     * for tag in tagQueue
     * if a tag == -1:
     * Find that block and return it
     * if all blocks filled:
     *
     */
}

int Set::numBlocksInSet() const {
    int numBlocks = 0;
    for (auto & i : tagQueue) {
        if (i != -1) {
            numBlocks += 1;
        }
    }
    return numBlocks;
}