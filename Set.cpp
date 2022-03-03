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
    /*
     * @Inputs: an address
     * @Return: A block
     * If set associativity == 0 -> return blocks[0]
     * ^ this is the edge case of direct mapped cache
     *
     * Cases:
     *  1) empty set -> no blocks in set
     *     setFilled = false
     *     blockIsInSet = false
     *     setEmpty = true;
     *     return blocks[0]
     *  2) Some blocks in set, ours not included
     *      setFilled = false;
     *      blockIsInSet = false;
     *      setEmpty = false;
     *      return the first empty block
     *  3) Some blocks in set, ours IS included
     *      setFilled = false
     *      blockIsInSet = true;
     *      setEmpty = false;
     *      return the block we are looking for
     *  4) All blocks filled, ours NOT included
     *      setFilled = true;
     *      blockIsInSet = false;
     *      setEmpty = false;
     *      return least recently used block
     *  5) All blocks filled, ours IN set
     *      setFilled = true;
     *      blockIsInSet = true
     *      setEmpty = false;
     *      return block we are looking for
     */
    tuple<unsigned int, unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr);
    bool setFilled = false, blockIsInSet = false, setEmpty = true;
    int numBlocksInSet = 0;
    int neededTag = get<0>(addrComponents);

    //Set all bools to be correct
    for (auto & i : blocks) {
        if (i.getTag() != -1) {
            setEmpty = false;
            numBlocksInSet += 1;
        }
        if (i.getTag() == neededTag) {
            blockIsInSet = true;
        }
    }
    if (numBlocksInSet == blocks.size()) {
        setFilled = true;
    }

    // Choose the correct response to take
    if (!setFilled && !blockIsInSet && setEmpty) {
        //empty set -> no blocks in set
        return blocks[0];
    } else if (!setFilled && !blockIsInSet && !setEmpty) {
        // Some blocks in set, ours not included
        for (auto & block : blocks) {
            if (block.getTag() == -1) {
                return block;
            }
        }
    } else if (!setFilled && blockIsInSet && !setEmpty) {
        // Some blocks in set, ours IS included
        for (auto & block : blocks) {
            if (block.getTag() == neededTag) {
                return block;
            }
        }
    } else if (setFilled && !blockIsInSet && !setEmpty) {
        // All blocks filled, ours NOT included
        int neededBlockTag = 0;
        neededBlockTag = tagQueue[tagQueue.size() - 1];
        for (auto & block : blocks) {
            if (block.getTag() == neededBlockTag) {
                return block;
            }
        }
    } else {
        // All blocks filled, ours IN set
        for (auto & block : blocks) {
            if (block.getTag() == neededTag) {
                return block;
            }
        }
    }
    return blocks[0];
}

int Set::getNumBlocksInSet() const {
    // Loop through the set, if a block has a valid tag, then increase the counter
    int numOfEmptyBlocks = 0;
    for (auto & i : blocks) {
        if (i.getTag() == -1) {
            numOfEmptyBlocks += 1;
        }
    }
    return blocks.size() - numOfEmptyBlocks;
}

vector<int> Set::getTagQueue() const {
    // Return the tag queue vector
    return tagQueue;
}

int Set::getIndexInSet(unsigned int tag) const {
    // Take in a tag, and loop through the blocks until we find our tag
    // Then return the index of the block we found
    for (int i = 0; i < blocks.size(); ++i) {
        if(blocks[i].getTag() == tag) {
            return i;
        }
    }
}

void Set::updateQueue(unsigned int addr) {
    tuple<unsigned int, unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr);
    bool containsBlocks = false;
    bool blockIsInSet = false;
    int blockInQueueIndex = 0;

    // check to see if a set contains any blocks
    if (getNumBlocksInSet() != 0) {
        containsBlocks = true;
    }
    // Check to see if the block we are updating is already in the set
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
    else if (getNumBlocksInSet() == tagQueue.size()) {
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
