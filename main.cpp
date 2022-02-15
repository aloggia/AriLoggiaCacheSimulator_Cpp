#include <iostream>
#include <tuple>
#include "Memory.h"
#include "Cache.h"
#include "Set.h"
#include "Block.h"

using namespace std;

int getBlockNumber(unsigned int addr);

int main() {
    int CACHE_SIZE = 1024;
    int BLOCK_SIZE = 64;
    int ASSOCIATIVITY = 1;
    bool WRITE_BACK = true;


/*    Cache cache(CACHE_SIZE, BLOCK_SIZE, ASSOCIATIVITY, WRITE_BACK);
    cache.getMemory().writeWord(1000, 175237091);
    cache.getMemory().writeWord(1200, 15002);
    cache.getMemory().writeWord(12160, 2011854023);
    cache.getMemory().writeWord(68, 1090854666);

    cout << cache.getMemory().readWord(1000) << endl;
    int cacheWord = 118238340;
    cout << cache.readWord(1000) << endl;
    cache.writeWord(68, 2011854023);
    cout << cache.readWord(1200) << endl;*/
    vector<int> whichSet;
    for (int i = 0; i < 16; i++) {
        whichSet.push_back(0);
    }
    for (unsigned int i = 0; i < 65536; i++) {
        whichSet[getBlockNumber(i)] += 1;
    }
    for (int i = 0; i < whichSet.size(); i++) {
        cout << whichSet[i] << endl;
    }


    /*
     * Put a word in memory
     * write the word from mem -> cache
     * read the word from cache
     * if word[memory] == word[cache] then good!
     */

    return 0;
}
int getBlockNumber(unsigned int addr) {
    return (addr % 16) / 16;
}
