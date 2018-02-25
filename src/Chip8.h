//
// Created by Corwin on 2018-02-17.
//

#ifndef ODDEIGHT_SYSTEM_H
#define ODDEIGHT_SYSTEM_H


#include <fstream>
#include <cstdint>

#include "Memory.h"
#include "Cpu.h"
#include "IO.h"
#include "Display.h"

class Chip8 {
public:
    /* System components */
    Memory *memory;
    Cpu *cpu;
    IO *io;
    Display *display;

    Chip8();

    void reset();

    bool loadRom(std::string filename);

    bool loadRom(std::ifstream &rom_file);

    void run();

    const bool isRunning() const;

private:
    /* Helper */
    static std::streampos _fileSize(std::ifstream &file_stream);

    /* System state */
    bool _isRunning = false;

    void _runFrame();
};


#endif //ODDEIGHT_SYSTEM_H
