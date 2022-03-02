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
    int numBlocksInSet = getNumBlocksInSet();

    // TODO: Return the correct block
    // No blocks in the set, so return the first block
    if (numBlocksInSet == 0) {
        return blocks[0];
    }
    // If the block is in the set, return that block
    int blockToReturnIndex = 0;
    for (auto & block : blocks) {
        if (block.getTag() == get<0>(addrComponents)) {
            return block;
        }
    }
    // If there are blocks in the set, but the one we want is not one of them, return the first empty block
    for (auto & block : blocks) {
        if (block.getTag() == -1) {
            return block;
        }
    }
    // If the set is full, return the oldest block
    return blocks[tagQueue.size()];
}

int Set::getNumBlocksInSet() const {
    int numOfEmptyBlocks = 0;
    for (auto & i : blocks) {
        if (i.getTag() == -1) {
            numOfEmptyBlocks += 1;
        }
    }
    return blocks.size() - numOfEmptyBlocks;
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

void Set::updateQueue(unsigned int addr) {
    tuple<unsigned int, unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr);
    int numBlocksInSet = getNumBlocksInSet();
    bool containsBlocks = false;
    bool blockIsInSet = false;
    int blockInQueueIndex = 0;

    if (numBlocksInSet != 0) {
        containsBlocks = true;
    }
    for (int i = 0; i < tagQueue.size(); ++i) {
        if (tagQueue[i] == get<0>(addrComponents)) {
            blockIsInSet = true;
            blockInQueueIndex = i;
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
            tagQueue.erase(tagQueue.begin() + blockInQueueIndex);
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
            tagQueue.erase(tagQueue.begin() + blockInQueueIndex);
            tagQueue.insert(tagQueue.begin(), get<0>(addrComponents));
        } else {
            // Block is not in set & there are empty sets
            tagQueue.pop_back();
            tagQueue.insert(tagQueue.begin(), get<0>(addrComponents));
        }
    }
}
