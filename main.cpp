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


int main() {
    // Set values of global variables
    CACHE_SIZE = 1024;
    BLOCK_SIZE = 64;
    ASSOCIATIVITY = 2;
    WRITE_BACK = true;

    Cache cache;

    cache.getMemory().writeWord(128, 175237091);
    cache.getMemory().writeWord(1200, 15002);
    cache.getMemory().writeWord(12160, 2011854023);
    cache.writeWord(128, 1090854666);
    cache.writeWord(1016, 3011854023);

    cache.readWord(1000);
    cache.writeWord(1016, 4001854973);
    cache.writeWord(1048, 4001854973);
    cache.writeWord(2096, 3914973);





    return 0;
}