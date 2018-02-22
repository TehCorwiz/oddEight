//
// Created by Corwin on 2018-02-17.
//

#ifndef ODDEIGHT_CPU_H
#define ODDEIGHT_CPU_H


#include <cstddef>
#include <array>
#include "Memory.h"
#include "IO.h"
#include "Display.h"

class Cpu {
public:
    Cpu(Memory *memory, IO *io, Display *display);

    void reset();

private:
    /* Referecnces */
    Memory *_memory;
    IO *_io;
    Display *_display;

    /* Registers*/
    uint16_t _I = 0;   // Index (16-bit)
    uint16_t _PC = 0x200;  // Program Counter (16-bit). Execution begins at address 0x200
    uint16_t _SP = 0;  // Stack Pointer (16-bit)

    uint8_t _V[16] = {0}; // V0 - VF (VF doubles as carry flag for some operations)

    /* Timers */
    uint8_t _delayTimer = 0;
    uint8_t _soundTimer = 0;
};


#endif //ODDEIGHT_CPU_H
