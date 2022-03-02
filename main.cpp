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
int TAG_LENGTH;


int main() {
    // Set values of global variables
    CACHE_SIZE = 1024;
    BLOCK_SIZE = 64;
    ASSOCIATIVITY = 4;
    WRITE_BACK = false;
    TAG_LENGTH = 8;

    Cache cache;

    cache.getMemory().writeWord(1152, 1152);
    cache.getMemory().writeWord(2176, 2176);
    cache.getMemory().writeWord(3200, 3200);
    cache.getMemory().writeWord(4224, 4224);
    cache.getMemory().writeWord(5248, 5248);
    cache.getMemory().writeWord(7296,7296);
    cache.getMemory().writeWord(7320,7320);
    cache.getMemory().writeWord(4228,4228);
    cache.getMemory().writeWord(3212,3212);
    cache.getMemory().writeWord(8324, 8324);


    cache.readWord(1152);
    cache.readWord(2176);
    cache.readWord(3200);
    cache.readWord(4224);
    cache.readWord(5248);
    cache.readWord(7296);
    cache.readWord(4224);
    cache.readWord(3200);

    cache.writeWord(7312, 17);

    cache.readWord(7320);

    cache.writeWord(5248, 5);
    cache.readWord(5248);
    cache.writeWord(8320, 7);

    cache.readWord(8324);







    return 0;
}