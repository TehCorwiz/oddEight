//
// Created by Corwin on 2018-02-17.
//

#ifndef ODDEIGHT_CPU_H
#define ODDEIGHT_CPU_H


#include <cstdint>

#include "Memory.h"
#include "IO.h"
#include "Display.h"

class Cpu {
public:
    constexpr static uint16_t executionStartAddress = 0x200;
    constexpr static uint16_t executionSpeed = 30; // In cycles per frame

    Cpu(Memory *memory, IO *io, Display *display);

    void reset();

    void runCycle();

    const uint32_t cycleCount() const;

    const bool error() const;
private:
    /* Referecnces */
    Memory *_memory;
    IO *_io;
    Display *_display;

    /* Registers*/
    uint16_t _I = 0;   // Index (16-bit)
    uint16_t _PC = executionStartAddress;  // Program Counter (16-bit). Execution begins at address 0x200
    uint16_t _SP = 0;  // Stack Pointer (16-bit)

    uint8_t _V[16] = {0}; // V0 - VF (VF doubles as carry flag for some operations)

    // OK, stack is here instead of memory because it "feels" better to code with it here, and it's not used in the
    // Memory class directly. It's possible that there is some ROM out there that relies on the stack being addressable
    // but I would have had to bother with that eventually anyway.
    uint16_t _stack[Memory::stackSize] = {0};

    /* Timers */
    uint8_t _delayTimer = 0;
    uint8_t _soundTimer = 0;

    /* actions */
    void _executeOpcode(uint16_t opcode);

    void _pushStack(uint16_t value);

    uint16_t _popStack();

    /* Helpers */
    std::string _toHexString(uint16_t value);

    void _dumpDisassembly();

    /* Stats */
    uint32_t _cycleCount = 0;

    std::string _disassembly[Memory::memorySize];

    bool _error = false;
};


#endif //ODDEIGHT_CPU_H
