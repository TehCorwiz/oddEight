//
// Created by Corwin on 2018-02-17.
//

#include <iostream>
#include <fstream>

#include "Memory.h"

Memory::Memory() {
    std::cout << "Memory initialized.\r\n";
}

std::byte Memory::readByte(int address) {
    // TODO: This is certainly more nuanced, but this will suffice while I fiddle with the language.
    return this->memory[address];
}

void Memory::writeByte(int address, std::byte value) {
    this->memory[address] = value;
}

bool Memory::loadRom(std::ifstream &rom) {
    if (!rom.good()) return true;

    char rom_data[romMaxSize];
    rom.readsome(rom_data, romMaxSize);

    int current_address = romStartAddress;
    for (char value:rom_data) {
        this->memory[current_address] = (std::byte) value;
        current_address++;
    }

    return false;
}

void Memory::clear() {
    for (std::byte &cell: this->memory) {
        cell = (std::byte) 0;
    }

    std::cout << "Memory cleared." << std::endl;
}
