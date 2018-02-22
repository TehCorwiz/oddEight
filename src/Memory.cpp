//
// Created by Corwin on 2018-02-17.
//

#include <iostream>
#include <fstream>

#include "Memory.h"

Memory::Memory() {
    reset();
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

void Memory::writeWord(uint16_t value, uint16_t address) {
    if ((address + 1) > Memory::memorySize)
        throw std::range_error("Attempted to write beyond memory bounds.");

    auto most_significant = (uint8_t) (value >> 8);
    auto least_significant = (uint8_t) ((value & 0x00FF));

    this->_map[address] = most_significant;
    this->_map[address + 1] = least_significant;
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
    for (uint8_t &cell: this->_map) {
        cell = 0;
    }

    for (uint8_t &cell: this->_stack) {
        cell = 0;
    }

    std::cout << "Memory cleared." << std::endl;

    // Load fontset at address range 0x00 => 0x50
    std::cout << "Memory: Loading fontset..." << std::endl;
    writeBytes(this->_fontSet, 80, 0x00);
}
