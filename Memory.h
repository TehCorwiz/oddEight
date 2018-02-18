//
// Created by Corwin on 2018-02-17.
//

#ifndef EIGHTEIGHT_MEMORY_H
#define EIGHTEIGHT_MEMORY_H


#include <cstddef>

class Memory {
public:
    Memory();

    std::byte readByte(int address);

    void writeByte(int address, std::byte byte);

    bool loadRom(std::ifstream &rom);

    void clear();

    const static int romStartAddress = 0x0200;
    const static int romMaxSize = 4096 - 512 - 256 - 96;
private:
    std::byte memory[4096] = {(std::byte)0};
};


#endif //EIGHTEIGHT_MEMORY_H
