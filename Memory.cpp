//
// Created by alogg on 2/11/2022.
//

#include "Memory.h"

Memory::Memory() {
    // Set the size of memory equal to the largest possible size for a given address width
    size = pow(2, ADDRESS_SIZE);
    mem.reserve(size);
}

unsigned int Memory::readByte(unsigned int addr) const{
    // retun the byte at address location
    return mem[addr];
}

void Memory::writeByte(unsigned int addr, unsigned int byte) {
    // write to the specified byte
    mem[addr] = byte;
}

unsigned int Memory::readWord(unsigned int addr) const{
    // align the given address for error correcting reasons
    addr = GlobalFunctions::alignAddress(addr);
    // Read 4 consecutive bytes and add them all together to get a word
    return readByte(addr) + 256 * (readByte(addr + 1) + 256 * (readByte(addr + 2) + 256 * (
            readByte(addr + 3))));
}

void Memory::writeWord(unsigned int addr, unsigned int word) {
    // align the address to prevent errors
    addr = GlobalFunctions::alignAddress(addr);
    // loop 4 times, writing to 4 consecutive bytes
    for (int i = 0; i <= 3; i++) {
        writeByte(addr, GlobalFunctions::extractBits(word, 8, 8*i));
        addr += 1;
    }
}

int Memory::getSize() const{
    return size;
}





