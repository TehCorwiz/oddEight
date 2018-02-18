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

    void loadRom(std::ifstream &rom_file);

    double steps;

    bool isRunning = true;

private:
    Memory *_memory;
    Cpu *_cpu;
    IO *_io;
    Display *_display;
};


#endif //EIGHTEIGHT_SYSTEM_H
