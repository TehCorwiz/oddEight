//
// Created by Corwin on 2018-02-17.
//

#ifndef ODDEIGHT_MEMORY_H
#define ODDEIGHT_MEMORY_H


#include <cstdint>

class Memory {
public:
    constexpr static uint16_t memorySize = 4096;
    constexpr static uint16_t stackSize = 16;

    constexpr static uint16_t romStartAddress = 0x0200;
    constexpr static uint16_t romMaxSize = 4096 - 512 - 256 - 96;


    constexpr static uint16_t fontsetStartaddress = 0x0050;
    constexpr static uint8_t fontHeight = 5;

    Memory();

    uint8_t readByte(uint16_t address);

    uint16_t readWord(uint16_t address);

    void writeByte(uint8_t value, uint16_t address);

    void writeWord(uint16_t value, uint16_t address);

    void writeBytes(const uint8_t *data, uint16_t data_size, uint16_t start_address);

    void reset();

private:
    uint8_t _map[Memory::memorySize] = {0};

    constexpr static uint8_t _fontset[80] =
            {
                    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
                    0x20, 0x60, 0x20, 0x20, 0x70, // 1
                    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
                    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
                    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
                    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
                    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
                    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
                    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
                    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
                    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
                    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
                    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
                    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
                    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
                    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
            };
};


#endif //ODDEIGHT_MEMORY_H
