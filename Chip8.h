//
// Created by Corwin on 2018-02-17.
//

#ifndef EIGHTEIGHT_SYSTEM_H
#define EIGHTEIGHT_SYSTEM_H


#include <fstream>

#include "Memory.h"
#include "Cpu.h"
#include "IO.h"
#include "Display.h"

class Chip8 {
public:
    Chip8();

    void reset();

    void runStep();

    bool loadRom(const std::string filename);

    bool loadRom(std::ifstream &rom_file);

    bool run();

    double steps;

    bool isRunning = true;

private:
    Memory *_memory;
    Cpu *_cpu;
    IO *_io;
    Display *_display;

    static std::streampos _fileSize(std::ifstream &file_stream);
};


#endif //EIGHTEIGHT_SYSTEM_H
