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
    Memory *_memory;
    IO *_io;
    Display *_display;

    /* Registers*/
    unsigned short _I;   // Index (16-bit)
    unsigned short _PC;  // Program Counter (16-bit)

    std::byte _V[16]; // V0 - VF (VF doubles as carry flag for some operations)

    /* Timers */
    std::byte _delay_timer;
    std::byte _sound_timer;
};


#endif //EIGHTEIGHT_CPU_H
