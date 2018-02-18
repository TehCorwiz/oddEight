#include <fstream>
#include <iostream>
#include <unistd.h>

#include "Chip8.h"


int main() {
    std::string filename = "test.ch8";

    // Open rom file.
    std::ifstream rom_file(filename, std::ios::binary);

    if (!rom_file.is_open()) {
        std::cout << "Failed to open " << filename << "\r\n";
        return 1;
    }

    std::cout << "Opened rom file: " << filename << "\r\n";

    // Init system
    auto *chip8 = new Chip8;
    chip8->loadRom(rom_file);

    // Main loop
    while (chip8->isRunning) {
        chip8->runStep();
    }

    chip8->reset();
    return false;
}