//
// Created by alogg on 2/11/2022.
//

#include "Block.h"

Block::Block(int size) {
    tag = -1;
    this->size = size;
    mem.reserve(size);
    isDirty = true;
    isValid = false;
}

int Block::getTag() const {
    return tag;
}

void Block::setTag(int new_tag) {
    tag = new_tag;
}

bool Block::getDirty() const {
    return isDirty;
}

void Block::setDirty(bool newFlag) {
    isDirty = newFlag;
}

bool Block::getValid() const {
    return isValid;
}

void Block::setValid(bool newFlag) {
    isValid = newFlag;
}

unsigned int Block::readByte(unsigned int addr) {
    tuple<unsigned int, unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr);
    return mem[get<2>(addrComponents)];
}

void Block::writeByte(unsigned int addr, unsigned int byte) {
    tuple<unsigned int, unsigned int, unsigned int> addrComponents = GlobalFunctions::addressAsTuple(addr);
    mem[get<2>(addrComponents)] = byte;
}

unsigned int Block::readWord(unsigned int addr) {
    return readByte(addr) + 256 * (readByte(addr + 1) + 256 * (readByte(addr + 2) + 256 * (
            readByte(addr + 3))));
}

void Block::writeWord(unsigned int addr, unsigned int word) {
    for (int i = 0; i <= 3; i++) {
        writeByte(addr, GlobalFunctions::extractBits(word, 8, 8*i));
        addr += 1;
    }
}

unsigned int Block::getSize() const {
    return size;
}

void Block::setSize(unsigned int size) {
    this->size = size;
}
