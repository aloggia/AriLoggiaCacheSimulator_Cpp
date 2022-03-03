//
// Created by alogg on 2/11/2022.
//

#include "GlobalFunctions.h"
unsigned int GlobalFunctions::extractBits(unsigned int word, int numBits, int startPos) {
    return (((1 << numBits) - 1) & (word >> (startPos)));
}

tuple<unsigned int, unsigned int, unsigned int> GlobalFunctions::addressAsTuple(unsigned int addr) {
    // Returns tuple in form:
    // tag, index, offset
    // If associativity = 1, then all blocks will always have an index of 0
    if (ASSOCIATIVITY == 1) {
        int offsetBits = (16 - TAG_LENGTH);
        return make_tuple(extractBits(addr, TAG_LENGTH, offsetBits + 1),
                          0,
                          extractBits(addr, maxNumBits(BLOCK_SIZE), 0));
    } else {
        // index bits is the num of bits we need to uniquely identfy a set, only used in associative caching
        int indexBits = maxNumBits(ASSOCIATIVITY);
        // Offset size will be the remainder of address len - tag - index
        int offsetBits = (16 - TAG_LENGTH) - indexBits;
        return make_tuple(extractBits(addr, TAG_LENGTH, offsetBits + indexBits),
                          extractBits(addr, indexBits, offsetBits),
                          extractBits(addr, offsetBits, 0));
    }
}
unsigned int GlobalFunctions::alignAddress(unsigned int addr) {
    // takes in an address and returns the start of the word that the address is pointing at
    return addr - (addr % 4);
}

int GlobalFunctions::maxNumBits(int value) {
    // There is probably some c++ library to determine the max number of bits needed to store a value, but I created my
    // own function to do that
    if (value == 1) {
        // edge case
        return 1;
    } else {
        // Repeatedly divide the value by 2, counting how many times it can be divided by 2, than return that value
        int neededBits = 0;
        while (value != 1) {
            value = value / 2;
            neededBits += 1;
        }
        return neededBits;
    }
}

