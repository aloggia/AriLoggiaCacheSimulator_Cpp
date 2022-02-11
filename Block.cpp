//
// Created by alogg on 2/11/2022.
//

#include "Block.h"

Block::Block(unsigned int size) {

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
    tuple<unsigned int, unsigned int> addrComponents = addressAsTuple(addr, size);
    unsigned int offset = get<1>(addrComponents);
    return mem[offset];
}

void Block::writeByte(unsigned int addr, unsigned int byte) {
    tuple<unsigned int, unsigned int> addrComponents = addressAsTuple(addr, size);
    unsigned int offset = get<1>(addrComponents);
    mem[offset] = byte;
}

unsigned int Block::readWord(unsigned int addr) {
    return readByte(addr) + 256 * (readByte(addr + 1) + 256 * (readByte(addr + 2) + 256 * (
            readByte(addr + 3))));
}

void Block::writeWord(unsigned int addr, unsigned int word) {
    for (int i = 0; i <= 3; i++) {
        writeByte(addr, extractBits(word, 8, 8*i));
        addr += 1;
    }
}
