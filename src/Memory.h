//
// Created by Corwin on 2018-02-17.
//

#ifndef ODDEIGHT_MEMORY_H
#define ODDEIGHT_MEMORY_H


#include <cstddef>

class Memory {
public:
    const static short memorySize = 4096;
    const static short romStartAddress = 0x0200;
    const static short romMaxSize = 4096 - 512 - 256 - 96;

    Memory();

    std::byte readByte(int address);

    void writeByte(std::byte byte, short address);

    void writeBytes(const char data[], short data_size, short start_address);

    void clear();

private:
    std::byte _map[Memory::memorySize] = {(std::byte) 0};
};


#endif //ODDEIGHT_MEMORY_H
