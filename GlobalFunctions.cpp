//
// Created by alogg on 2/11/2022.
//

#include "GlobalFunctions.h"
unsigned int GlobalFunctions::extractBits(unsigned int word, int numBits, int startPos) {
    return (((1 << numBits) - 1) & (word >> (startPos)));
}

tuple<unsigned int, unsigned int, unsigned int> GlobalFunctions::addressAsTuple(unsigned int addr) {
    // tag, index, offset
    // If associativity = 1, then all blocks will always have an index of 0
    if (ASSOCIATIVITY == 1) {
        int offsetBits = (16 - TAG_LENGTH);
        return make_tuple(extractBits(addr, TAG_LENGTH, offsetBits + 1),
                          0,
                          extractBits(addr, maxNumBits(BLOCK_SIZE), 0));
    } else {
        int indexBits = maxNumBits(ASSOCIATIVITY);
        int offsetBits = (16 - TAG_LENGTH) - indexBits;
        return make_tuple(extractBits(addr, TAG_LENGTH, offsetBits + indexBits),
                          extractBits(addr, indexBits, offsetBits),
                          extractBits(addr, offsetBits, 0));
    }
}
unsigned int GlobalFunctions::alignAddress(unsigned int addr) {
    return addr - (addr % 4);
}

int GlobalFunctions::maxNumBits(int value) {
    if (value == 1) {
        // edge case
        return 1;
    } else {
        // Repeatedly dividde the value by 2, counting how many times it can be divided by 2, than return that value
        int neededBits = 0;
        while (value != 1) {
            value = value / 2;
            neededBits += 1;
        }
        return neededBits;
    }
}

