//
// Created by Corwin on 2018-02-17.
//

#ifndef EIGHTEIGHT_CPU_H
#define EIGHTEIGHT_CPU_H


#include <cstddef>
#include "Memory.h"
#include "IO.h"
#include "Display.h"

class Cpu {
public:
    Cpu(Memory *memory, IO *io, Display *display);

    void reset();

private:
    /* Referecnces */
    Memory *memory;
    IO *io;
    Display *display;

    /* Registers*/
    unsigned short I;   // Index (16-bit)
    unsigned short PC;  // Program Counter (16-bit)

    std::byte V[16]; // V0 - VF (VF doubles as carry flag for some operations)

    /* Timers */
    std::byte delay_timer;
    std::byte sound_timer;
};


#endif //EIGHTEIGHT_CPU_H
