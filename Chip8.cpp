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

std::streampos Chip8::_fileSize(std::ifstream &file_stream) {

    std::streampos file_size = 0;

    file_size = file_stream.tellg();
    file_stream.seekg(0, std::ios::end);
    file_size = file_stream.tellg() - file_size;
    file_stream.seekg(0, std::ios::beg);

    return file_size;
}

bool Chip8::loadRom(std::ifstream &rom) {
    char rom_data[Memory::romMaxSize];
    rom.readsome(rom_data, Memory::romMaxSize);

    this->_memory->writeBytes(rom_data, Memory::romMaxSize, Memory::romStartAddress);

    return false;
}

bool Chip8::loadRom(const std::string filename) {
    std::ifstream rom_file(filename, std::ios::binary);

    if (!rom_file.is_open() && rom_file.good()) {
        std::cout << "Failed to open " << filename << std::endl;
        return true;
    }

    std::cout << "Opened rom file: " << filename << std::endl;

    const auto rom_size = Chip8::_fileSize(rom_file);
    if (rom_size > Memory::romMaxSize) {
        std::cout << "Rom size: " << rom_size
                  << " exceed maximum size of " << Memory::romMaxSize << " bytes."
                  << std::endl;
        return true;
    }

    return this->loadRom(rom_file);
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

bool Chip8::run() {
    // Main loop
    while (this->isRunning) {
        this->runStep();
    }

    this->reset();

    return false;
}
