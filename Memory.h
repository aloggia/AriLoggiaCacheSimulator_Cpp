//
// Created by alogg on 2/11/2022.
//

#ifndef ARILOGGIACACHESIMULATOR_CPP_MEMORY_H
#define ARILOGGIACACHESIMULATOR_CPP_MEMORY_H
#include "GlobalFunctions.h"
#include "Block.h"
#include <vector>
#include <cmath>

using namespace std;
const unsigned int ADDRESS_SIZE = 16;

class Memory {
private:
    int size;
    vector<unsigned int> mem;
public:
    Memory();

    [[nodiscard]] unsigned int readByte(unsigned int addr) const;
    void writeByte(unsigned int addr, unsigned int byte);

    [[nodiscard]] unsigned int readWord(unsigned int addr) const;
    void writeWord(unsigned int addr, unsigned int word);

    int getSize() const;






};


#endif //ARILOGGIACACHESIMULATOR_CPP_MEMORY_H
