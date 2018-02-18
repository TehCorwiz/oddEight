//
// Created by Corwin on 2018-02-17.
//

#include <fstream>
#include <iostream>

#include "Chip8.h"

std::streampos fileSize(std::ifstream &file_stream) {

    std::streampos file_size = 0;

    file_size = file_stream.tellg();
    file_stream.seekg(0, std::ios::end);
    file_size = file_stream.tellg() - file_size;
    file_stream.seekg(0, std::ios::beg);

    return file_size;
}

int main() {
    std::string filename = "test.ch8";

    // Open rom file.
    std::ifstream rom_file(filename, std::ios::binary);

    if (!rom_file.is_open()) {
        std::cout << "Failed to open " << filename << std::endl;
        return true;
    }

    std::cout << "Opened rom file: " << filename << std::endl;

    auto rom_size = fileSize(rom_file);
    if (rom_size > Memory::romMaxSize) {
        std::cout << "Rom size: " << rom_size
                  << " exceed maximum size of " << Memory::romMaxSize << " bytes."
                  << std::endl;
        return true;
    }

    // Init system
    auto *chip8 = new Chip8;
    chip8->memory->loadRom(rom_file);

    // Main loop
    while (chip8->isRunning) {
        chip8->runStep();
    }

    chip8->reset();

    return false;
}
