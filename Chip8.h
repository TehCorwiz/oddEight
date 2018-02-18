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

    void loadRom(std::ifstream& rom);

    bool isStopped();

    Memory *memory;
    Cpu *cpu;
    IO *io;
    Display *display;
    double steps;
};


#endif //EIGHTEIGHT_SYSTEM_H
