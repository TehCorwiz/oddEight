#include <fstream>
#include <iostream>

#include "Chip8.h"


int main() {
    std::string filename = "test.ch8";

    // Open rom file.
    std::ifstream rom_file(filename, std::ios::binary);

    if (!rom_file.is_open()) {
        std::cout << "failed to open " << filename << "\r\n";
        return 1;
    }

    std::cout << "Opened rom file: " << filename << "\r\n";

    // Init system
    auto chip8 = new Chip8;
    chip8->loadRom(rom_file);

    // Main loop
    bool is_running = true;
    while (is_running) {
        chip8->runStep();

        if (chip8->isStopped()) is_running = false;
    }
    chip8->reset();
    return false;
}