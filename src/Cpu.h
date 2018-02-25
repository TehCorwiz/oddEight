//
// Created by Corwin on 2018-02-17.
//

#ifndef ODDEIGHT_CPU_H
#define ODDEIGHT_CPU_H


#include <cstdint>
#include <chrono>

#include "Memory.h"
#include "IO.h"
#include "Display.h"

class Cpu {
public:
    constexpr static uint16_t executionStartAddress = 0x200;

    // I couldn't find any documentation on this, so I ended up looking at other implementation UIs. This appears to be
    // variable. So I'll probably pull this out at some point.
    constexpr static uint16_t instructionsPerFrame = 30; // ticks per frame.

    constexpr static uint16_t instructionsPerSecond = Display::framesPerSecond * Cpu::instructionsPerFrame;

    constexpr static int64_t nanosecondsPerInstruction = 1000000000 / instructionsPerSecond;

    Cpu(Memory *memory, IO *io, Display *display);

    void reset();

    void runTick();

    void decrementTimers(); // Happens once per frame.

    // Getters for private values.
    const bool error() const;
    const uint32_t tickCount() const;
    const uint16_t I() const;
    const uint16_t PC() const;
    const uint16_t SP() const;
    const uint8_t* V() const;
    const uint16_t* stack() const;
    const uint8_t delayTimer() const;
    const uint8_t soundTimer() const;

private:
    /* Referecnces */
    Memory *_memory;
    IO *_io;
    Display *_display;

    /* Registers*/
    uint16_t _I = 0;   // Index (16-bit)
    uint16_t _PC = Cpu::executionStartAddress;  // Program Counter (16-bit). Execution begins at address 0x200
    uint16_t _SP = 0;  // Stack Pointer (16-bit)

    uint8_t _V[16] = {0}; // V0 - VF (VF doubles as carry flag for some operations)

    // OK, stack is here instead of memory because it "feels" better to code with it here, and it's not used in the
    // Memory class directly. It's possible that there is some ROM out there that relies on the stack being addressable
    // but I would have had to bother with that eventually anyway.
    uint16_t _stack[Memory::stackSize] = {0};

    /* Timers */
    uint8_t _delayTimer = 0;
    uint8_t _soundTimer = 0;

    /* Stats */
    uint32_t _tickCount = 0;

    std::string _disassembly[Memory::memorySize];

    bool _error = false;

    /* actions */
    void _executeOpcode(uint16_t opcode);

    void _pushStack(uint16_t value);

    uint16_t _popStack();

    /* Helpers */
    std::string _toHexString(uint16_t value);

    void _dumpDisassembly();
};


#endif //ODDEIGHT_CPU_H
