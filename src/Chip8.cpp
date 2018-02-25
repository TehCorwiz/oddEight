//
// Created by Corwin on 2018-02-17.
//

#include <iostream>
#include <thread>

#include "Chip8.h"


/* Public */
Chip8::Chip8() {
    this->_memory = new Memory;
    this->_io = new IO;
    this->_display = new Display;
    this->_cpu = new Cpu(this->_memory, this->_io, this->_display);

    std::cout << "System initialized." << std::endl;
}

void Chip8::reset() {
    std::cout << "Chip8: Resetting system..." << std::endl;

    this->_display->clear();
    this->_memory->reset();
    this->_cpu->reset();

    std::cout << "Chip8 reset." << std::endl;
}

bool Chip8::loadRom(const std::string filename) {
    std::ifstream rom_file(filename, std::ios::binary);

    if (!rom_file.is_open() && rom_file.good()) {
        std::cout << "Failed to open " << filename << std::endl;
        return true;
    }

    std::cout << "Opened rom file: " << filename << std::endl;

    return loadRom(rom_file);
}

bool Chip8::loadRom(std::ifstream &rom_file) {
    const std::streampos rom_size = Chip8::_fileSize(rom_file);
    if (rom_size > Memory::romMaxSize) {
        std::cout << "Rom size: " << rom_size
                  << " exceed maximum size of " << Memory::romMaxSize << " bytes."
                  << std::endl;
        return true;
    }

    char rom_data[Memory::romMaxSize];
    // This integer coercion from std::streamsize (long long) to uint16_t should be safe in this context
    // because we're limiting the file size to less than 4KB.
    const auto size_read = (uint16_t) rom_file.readsome(rom_data, Memory::romMaxSize);

    this->_memory->writeBytes((uint8_t *) rom_data, size_read, Memory::romStartAddress);

    return false;
}

void Chip8::run() {
    this->_isRunning = true;

    // Main loop
    while (this->_isRunning && !this->_cpu->error()) {
        this->_runFrame();
    }
}

const bool Chip8::isRunning() const {
    return this->_isRunning;
}


/* Private */
std::streampos Chip8::_fileSize(std::ifstream &file_stream) {
    std::streampos file_size = 0;

    file_size = file_stream.tellg();
    file_stream.seekg(0, std::ios::end);
    file_size = file_stream.tellg() - file_size;
    file_stream.seekg(0, std::ios::beg);

    return file_size;
}

void Chip8::_runFrame() {
    constexpr auto frame_duration = std::chrono::nanoseconds(1000000000 / Display::framesPerSecond);

    const auto tick_start = std::chrono::high_resolution_clock::now();

    for (uint8_t ticks = 0; ticks < Cpu::instructionsPerFrame; ticks++) {
        this->_cpu->runTick();
        if (this->_cpu->error()) return;
    }

    this->_cpu->decrementTimers();

    const auto tick_end = std::chrono::high_resolution_clock::now();

    // Sleep until time for next frame
    const std::chrono::duration<int64_t, std::nano> elapsed = tick_end - tick_start;
    const std::chrono::duration<int64_t, std::nano> delta = frame_duration - elapsed;

    std::this_thread::sleep_for(delta);
}
