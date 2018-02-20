//
// Created by Corwin on 2018-02-17.
//

#include <iostream>
#include <fstream>

#include "Memory.h"

Memory::Memory() {
    std::cout << "Memory initialized." << std::endl;
}

std::byte Memory::readByte(const int address) {
    // TODO: This is certainly more nuanced, but this will suffice while I fiddle with the language.
    if (address > Memory::memorySize)
        throw std::range_error("Attempted to read beyond memory bounds.");

    return this->_map[address];
}

void Memory::writeByte(const std::byte value, const short address) {
    if (address > Memory::memorySize)
        throw std::range_error("Attempted to write beyond memory bounds.");

    this->_map[address] = value;
}

void Memory::writeBytes(const char data[], const short data_size, const short start_address) {
    // TODO: Better bounds checking. Make sure that the data doesn't conflict with reserved ranges.
    if ((start_address + data_size) > Memory::memorySize)
        throw std::range_error("Attempted to write beyond memory bounds.");

    for (int i = 0; i < data_size; i++) {
        this->_map[start_address + i] = (std::byte) data[i];
    }

    std::cout << data_size << " bytes written to memory at address " << start_address << std::endl;
}

void Memory::clear() {
    for (std::byte &cell: this->_map) {
        cell = (std::byte) 0;
    }

    std::cout << "Memory cleared." << std::endl;
}
