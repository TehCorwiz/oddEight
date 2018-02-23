//
// Created by Corwin on 2018-02-17.
//

#include <iostream>

#include "Memory.h"

Memory::Memory() {
    this->_loadFontset();

    std::cout << "Memory initialized." << std::endl;
}

uint8_t Memory::readByte(const uint16_t address) {
    if (address > Memory::memorySize)
        throw std::range_error("Attempted to read beyond memory bounds.");

    return this->_map[address];
}

uint16_t Memory::readWord(const uint16_t address) {
    if ((address + 1) > Memory::memorySize)
        throw std::range_error("Attempted to read beyond memory bounds.");

    return (unsigned short) (this->_map[address]) << 8 | (unsigned short) (this->_map[address + 1]);
}

void Memory::writeByte(const uint8_t value, const uint16_t address) {
    if (address > Memory::memorySize)
        throw std::range_error("Attempted to write beyond memory bounds.");

    this->_map[address] = value;
}

void Memory::writeBytes(const uint8_t data[], const uint16_t data_size, const uint16_t start_address) {
    if ((start_address + data_size) > Memory::memorySize)
        throw std::range_error("Attempted to write beyond memory bounds.");

    for (int i = 0; i < data_size; i++) {
        this->_map[start_address + i] = data[i];
    }

    std::cout << data_size << " bytes written to memory at address " << start_address << std::endl;
}

void Memory::reset() {
    // Clear memory
    for (uint8_t &cell: this->_map) { cell = 0; }

    std::cout << "Memory cleared." << std::endl;
}

void Memory::_loadFontset() {
    // Load fontset at address range 0x00 => 0x50
    writeBytes(this->_fontset, 80, Memory::fontsetStartaddress);
    std::cout << "Fontset loaded at address 0x" << std::hex << std::uppercase << Memory::fontsetStartaddress
              << std::endl;
}
