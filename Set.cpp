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
    bool containsBlocks = false;
    int numBlocksInSet = 0;
    bool blockIsInSet = false;
    int blockInSetIndex = 0;
    for (int i = 0; i < tagQueue.size(); ++i) {
        if (tagQueue[i] != -1) {
            containsBlocks = true;
            numBlocksInSet += 1;
        }
        if (tagQueue[i] == get<0>(addrComponents)) {
            blockIsInSet = true;
            blockInSetIndex = i;
        }
    }
    if (!containsBlocks) {
        // Edge case of empty set
        tagQueue.pop_back();
        tagQueue.insert(tagQueue.begin(), get<0>(addrComponents));
    }
    else if (numBlocksInSet == tagQueue.size()) {
        // Edge case of full set
        if (blockIsInSet) {
            // Block is in the set, so erase that block from tagQueue and push it onto tag queue at index 0
            tagQueue.erase(tagQueue.begin() + blockInSetIndex);
            tagQueue.insert(tagQueue.begin(), get<0>(addrComponents));
        } else {
            // block is not in the set, so erase the least recently used tag and push the new tag
            tagQueue.pop_back();
            tagQueue.insert(tagQueue.begin(), get<0>(addrComponents));
        }
    } else {
        // Case of some blocks in set
        if (blockIsInSet) {
            // block is in set, but there are empty sets
            // So we just erase the tag from tagQueue and push it into tagQueue at the beggining
            tagQueue.erase(tagQueue.begin() + blockInSetIndex);
            tagQueue.insert(tagQueue.begin(), get<0>(addrComponents));
        } else {
            // Block is not in set & there are empty sets
            tagQueue.pop_back();
            tagQueue.insert(tagQueue.begin(), get<0>(addrComponents));
        }
    }


    // If the block is in the set, return that block
    int blockToReturnIndex = 0;
    for (int i = 0; i < blocks.size(); ++ i) {
        if (blocks[i].getTag() == get<0>(addrComponents)) {
            return blocks[i];
        }
        if (blocks[i].getTag() == -1) {
            blockToReturnIndex = i;
        }
    }
    // If block is not in the set return the first block we find with index -1
    return blocks[blockToReturnIndex];
}

int Set::numBlocksInSet() const {
    return blocks.size();
}

vector<int> Set::getTagQueue() const {
    return tagQueue;
}

int Set::getIndexInSet(unsigned int addr) const {
    tuple<unsigned int, unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr);
    for (int i = 0; i < blocks.size(); ++i) {
        if(i == get<1>(addrComponents)) {
            return i;
        }
    }
}
