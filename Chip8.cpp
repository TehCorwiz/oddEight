//
// Created by Corwin on 2018-02-17.
//

#include <iostream>

#include "Chip8.h"

Chip8::Chip8() {

    this->_memory = new Memory;
    this->_io = new IO;
    this->_display = new Display;
    this->_cpu = new Cpu(_memory, _io, _display);

    this->steps = 0;

    std::cout << "System initialized." << std::endl;
}

void Chip8::loadRom(std::ifstream &rom) {
    char rom_data[Memory::romMaxSize];
    rom.readsome(rom_data, Memory::romMaxSize);

    this->_memory->writeBytes(rom_data, Memory::romMaxSize, Memory::romStartAddress);
}

void Chip8::runStep() {
    // TODO

    this->steps++;
    if (this->steps >= 1000) this->isRunning = false;
}

void Chip8::reset() {
    this->_cpu->reset();
    this->_memory->clear();
    this->_display->clear();
}
