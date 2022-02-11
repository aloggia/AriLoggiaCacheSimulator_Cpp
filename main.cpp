#include <iostream>
#include <tuple>
#include "Memory.h"
using namespace std;


int main() {

    Memory memory;
    memory.writeWord(65476, 117640237);
    unsigned int readWord = memory.readWord(65476);
    cout << readWord << endl;
    //unsigned int read_word = 117640237;
    //cout << Memory::extractBits(read_word, 8, 0) << endl;

    return 0;
}
