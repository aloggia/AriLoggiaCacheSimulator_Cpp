//
// Created by alogg on 2/11/2022.
//

#include "GlobalFunctions.h"
unsigned int GlobalFunctions::extractBits(unsigned int word, int numBits, int startPos) {
    return (((1 << numBits) - 1) & (word >> (startPos)));
}

tuple<unsigned int, unsigned int> GlobalFunctions::addressAsTuple(unsigned int addr, unsigned int blockSize) {
    //(tag, offset)
    if (blockSize == 32) {
        return make_tuple(extractBits(addr, 16-5, 5), extractBits(addr, 5, 0));
    } else if (blockSize == 64) {
        return make_tuple(extractBits(addr, 16-6, 6), extractBits(addr, 6, 0));
    } else if (blockSize == 128) {
        return make_tuple(extractBits(addr, 16-7, 7), extractBits(addr, 7, 0));
    }
    return make_tuple(0,0);
}
unsigned int GlobalFunctions::alignAddress(unsigned int addr) {
    return addr - (addr % 4);
}


