//
// Created by alogg on 2/25/2022.
//
#include <vector>
#include <iostream>

using namespace std;

int main() {
    // TODO: Create a function that correctlly returns a block from a set

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


    vector<int> blocks = {3,7,12,1};
    vector<int> tagQueue = {12,3,7,1};
    bool setFilled = false, blockIsInSet = false, setEmpty = true;
    int numBlocksInSet = 0;
    int neededTag = 5;
    for (auto & i : blocks) {
        if (i != -1) {
            setEmpty = false;
            numBlocksInSet += 1;
        }
        if (i == neededTag) {
            blockIsInSet = true;
        }
    }
    if (numBlocksInSet == blocks.size()) {
        setFilled = true;
    }

    if (!setFilled && !blockIsInSet && setEmpty) {
        //empty set -> no blocks in set
        cout << "return block[0]" << endl;
    } else if (!setFilled && !blockIsInSet && !setEmpty) {
        // Some blocks in set, ours not included
        cout << "Return first empty block" << endl;
        for (int i = 0; i < blocks.size(); ++i) {
            if (blocks[i] == -1) {
                cout << "First empty block: " << i << endl;
            }
        }
    } else if (!setFilled && blockIsInSet && !setEmpty) {
        // Some blocks in set, ours IS included
        cout << "Return block we are looking for" << endl;
        for (int i = 0; i < blocks.size(); ++i) {
            if (blocks[i] == neededTag) {
                cout << "Block we want found at: " << i << endl;
            }
        }
    } else if (setFilled && !blockIsInSet && !setEmpty) {
        // All blocks filled, ours NOT included
        cout << "Return least recently used block" << endl;
        int neededBlockTag = 0;
        neededBlockTag = tagQueue[tagQueue.size() - 1];
        for (int i = 0; i < blocks.size(); ++i) {
            if (blocks[i] == neededBlockTag) {
                cout << "Block we want found at: " << i << endl;
            }
        }
    } else {
        // All blocks filled, ours IN set
        cout << "Return the block we are looking for" << endl;
        for (int i = 0; i < blocks.size(); ++i) {
            if (blocks[i] == neededTag) {
                cout << "Block we want found at: " << i << endl;
            }
        }
    }



    return 0;
};
