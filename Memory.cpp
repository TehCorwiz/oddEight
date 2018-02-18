//
// Created by Corwin on 2018-02-17.
//

#include <iostream>
#include <fstream>

#include "Memory.h"

Memory::Memory() {
    std::cout << "Memory initialized." << std::endl;
}

std::byte Memory::readByte(int address) {
    // TODO: This is certainly more nuanced, but this will suffice while I fiddle with the language.
    return this->_map[address];
}

void Memory::writeByte(std::byte value, short address) {
    this->_map[address] = value;
}

void Memory::writeBytes(const char data[], const short data_size, const short start_address) {
    for (int i = 0; i < data_size; i++) {
        this->_map[start_address + i] = (std::byte) data[i];
    }

    std::cout << "Bytes written: " << data_size << "." << std::endl;
}

void Memory::clear() {
    for (std::byte &cell: this->_map) {
        cell = (std::byte) 0;
    }

    std::cout << "Memory cleared." << std::endl;
}
