//
// Created by alogg on 2/11/2022.
//

#ifndef ARILOGGIACACHESIMULATOR_CPP_BLOCK_H
#define ARILOGGIACACHESIMULATOR_CPP_BLOCK_H
#include "GlobalFunctions.h"
#include <vector>
#include <tuple>
using namespace std;

class Block {
private:
    int tag;
    unsigned int size;
    vector<unsigned int> mem;
    bool isDirty;
    bool isValid;
public:
    Block(unsigned int size);

    int getTag() const;
    void setTag(int new_tag);

    bool getDirty() const;
    void setDirty(bool newFlag);

    bool getValid() const;
    void setValid(bool newFlag);

    unsigned int readByte(unsigned int addr);
    void writeByte(unsigned int addr, unsigned int byte);

    unsigned int readWord(unsigned int addr);
    void writeWord(unsigned int addr, unsigned int word);
};


#endif //ARILOGGIACACHESIMULATOR_CPP_BLOCK_H
