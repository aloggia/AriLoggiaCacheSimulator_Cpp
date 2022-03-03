//
// Created by alogg on 2/11/2022.
//

#include "Block.h"

Block::Block() {
    tag = -1;
    mem.reserve(BLOCK_SIZE);
    isDirty = false;
    isValid = false;
}

int Block::getTag() const {
    // Return block tag
    return tag;
}

void Block::setTag(int new_tag) {
    // set block tag
    tag = new_tag;
}

bool Block::getDirty() const {
    // get the dirty bit
    return isDirty;
}

void Block::setDirty(bool newFlag) {
    // set the dirty bit
    isDirty = newFlag;
}

bool Block::getValid() const {
    // get the valid bit
    return isValid;
}

void Block::setValid(bool newFlag) {
    // return the valid bit
    isValid = newFlag;
}

unsigned int Block::readByte(unsigned int addr) {
    tuple<unsigned int, unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr);
    // get the offset the byte has from the start of the block
    // return the byte in that position in the block
    return mem[get<2>(addrComponents)];
}

void Block::writeByte(unsigned int addr, unsigned int byte) {
    tuple<unsigned int, unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr);
    // get the offset the byte has from the start of the block
    // set the byte in that position in the block
    mem[get<2>(addrComponents)] = byte;
}

unsigned int Block::readWord(unsigned int addr) {
    // Use the readByte function 4 times to read the 4 bytes of the word and store it in a single value
    return readByte(addr) + 256 * (readByte(addr + 1) + 256 * (readByte(addr + 2) + 256 * (
            readByte(addr + 3))));
}

void Block::writeWord(unsigned int addr, unsigned int word) {
    // loop 4 times, incrementing the address by 1 each time, and writing the byte to the block
    for (int i = 0; i <= 3; i++) {
        writeByte(addr, GlobalFunctions::extractBits(word, 8, 8*i));
        addr += 1;
    }
}

unsigned int Block::getSize() {
    return BLOCK_SIZE;
}

