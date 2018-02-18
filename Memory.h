//
// Created by Corwin on 2018-02-17.
//

#ifndef EIGHTEIGHT_MEMORY_H
#define EIGHTEIGHT_MEMORY_H


#include <cstddef>

class Memory {
public:
    Memory();

    std::byte readByte();

    void writeByte(std::byte byte);

    void clear();

private:
    std::byte memory[4096] = {(std::byte)0};
};


#endif //EIGHTEIGHT_MEMORY_H
