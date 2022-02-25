//
// Created by alogg on 2/25/2022.
//
#include <vector>
#include <iostream>

using namespace std;

int main() {

    //unsigned int addr;
    //tuple<unsigned int, unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr);
    int tag = 12;
    vector<int> tagQueue = {5,7,50,1};
    bool containsBlocks = false;
    int numBlocksInSet = 0;
    bool blockIsInSet = false;
    int blockInSetIndex = 0;
    for (int i = 0; i < tagQueue.size(); ++i) {
        if (tagQueue[i] != -1) {
            containsBlocks = true;
            numBlocksInSet += 1;
        }
        if (tagQueue[i] == tag) {
            blockIsInSet = true;
            blockInSetIndex = i;
        }
    }
    if (!containsBlocks) {
        // Edge case of empty set
        tagQueue.pop_back();
        tagQueue.insert(tagQueue.begin(), tag);
    }
    else if (numBlocksInSet == tagQueue.size()) {
        // Edge case of full set
        if (blockIsInSet) {
            // Block is in the set, so erase that block from tagQueue and push it onto tag queue at index 0
            tagQueue.erase(tagQueue.begin() + blockInSetIndex);
            tagQueue.insert(tagQueue.begin(), tag);
        } else {
            // block is not in the set, so erase the least recently used tag and push the new tag
            tagQueue.pop_back();
            tagQueue.insert(tagQueue.begin(), tag);
        }
    } else {
        // Case of some blocks in set
        if (blockIsInSet) {
            // block is in set, but there are empty sets
            // So we just erase the tag from tagQueue and push it into tagQueue at the beggining
            tagQueue.erase(tagQueue.begin() + blockInSetIndex);
            tagQueue.insert(tagQueue.begin(), tag);
        } else {
            // Block is not in set & there are empty sets
            tagQueue.pop_back();
            tagQueue.insert(tagQueue.begin(), tag);
        }
    }
    for (int & i : tagQueue) {
        cout << i;
        cout << ", ";
    }
    return 0;
};
