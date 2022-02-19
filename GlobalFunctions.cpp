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
        return make_tuple(extractBits(addr, maxNumBits(BLOCK_SIZE), maxNumBits(BLOCK_SIZE)),
                          0,
                          extractBits(addr, maxNumBits(BLOCK_SIZE), 0));
    } else {
        return make_tuple(extractBits(addr, maxNumBits(BLOCK_SIZE) + maxNumBits((CACHE_SIZE / BLOCK_SIZE) / ASSOCIATIVITY), maxNumBits(BLOCK_SIZE)),
                          extractBits(addr, maxNumBits((CACHE_SIZE / BLOCK_SIZE) / ASSOCIATIVITY), maxNumBits(BLOCK_SIZE)-1),
                          extractBits(addr, maxNumBits(BLOCK_SIZE), 0));
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

