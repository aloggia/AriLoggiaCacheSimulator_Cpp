#include <iostream>
#include <tuple>
#include "Memory.h"
#include "Cache.h"
#include "Set.h"
#include "Block.h"
#include "common.h"

using namespace std;

int CACHE_SIZE;
int BLOCK_SIZE;
int ASSOCIATIVITY;
bool WRITE_BACK;

int getBlockNumber(unsigned int addr);

int main() {
    CACHE_SIZE = 1024;
    BLOCK_SIZE = 64;
    ASSOCIATIVITY = 1;
    WRITE_BACK = false;
    Cache cache(CACHE_SIZE, BLOCK_SIZE, ASSOCIATIVITY, WRITE_BACK);

    cache.getMemory().writeWord(128, 175237091);
    cache.getMemory().writeWord(1200, 15002);
    cache.getMemory().writeWord(12160, 2011854023);
    cache.writeWord(128, 1090854666);

    cout << cache.getMemory().readWord(1000) << endl;
    cache.readWord(1000);
    //cache.writeWord(68, 2011854023);
    //cout << cache.readWord(68) << endl;



    return 0;
}
int getBlockNumber(unsigned int addr) {
    return (addr / 64) % 16;
}
