//
// Created by alogg on 2/11/2022.
//

#include "Memory.h"

Memory::Memory() {
    size = pow(2, ADDRESS_SIZE);
    mem.reserve(size);
}

unsigned int Memory::readByte(unsigned int addr) const{
    return mem[addr];
}

void Memory::writeByte(unsigned int addr, unsigned int byte) {
    mem[addr] = byte;
}

unsigned int Memory::readWord(unsigned int addr) const{
    addr = GlobalFunctions::alignAddress(addr);
    return readByte(addr) + 256 * (readByte(addr + 1) + 256 * (readByte(addr + 2) + 256 * (
            readByte(addr + 3))));
}

void Memory::writeWord(unsigned int addr, unsigned int word) {
    addr = GlobalFunctions::alignAddress(addr);
    for (int i = 0; i <= 3; i++) {
        writeByte(addr, GlobalFunctions::extractBits(word, 8, 8*i));
        addr += 1;
    }
}

int Memory::getSize() const{
    return size;
}





