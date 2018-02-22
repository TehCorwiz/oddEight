//
// Created by Corwin on 2018-02-17.
//

#include <iostream>
#include "Cpu.h"

Cpu::Cpu(Memory *memory, IO *io, Display *display) {
    this->_memory = memory;
    this->_io = io;
    this->_display = display;

    std::cout << "CPU initialized." << std::endl;
}

void Cpu::reset() {
    this->_I = 0;
    this->_PC = Cpu::ExecutionStartAddress;
    this->_SP = 0;

    for (uint8_t &v_register: this->_V) { v_register = 0; }

    this->_delayTimer = 0;
    this->_soundTimer = 0;

    this->_cycleCount = 0;
    this->_error = false;

    std::cout << "CPU reset." << std::endl;
}

void Cpu::_executeOpcode(uint16_t opcode) {
    switch (opcode & 0xF000) {
        // TODO: Everything
        case (0x6000):
            //6xkk: Set Vx = kk.
            this->_V[(uint16_t) ((opcode & 0x0F00) >> 8)] = (uint8_t) (opcode & 0xFF00);
            break;
        case (0xA000):
            //Annn: Set I = nnn.
            this->_I = (uint16_t) (opcode & 0x0FFF);
            break;
        case (0xD000):
            //Dxyn: Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
            auto x = _V[(opcode & 0x0F00) >> 8];
            auto y = _V[(opcode & 0x00F0) >> 4];
            auto height = opcode & 0x000F;
            uint8_t pixel;

            this->_V[0xF] = 0;

            // TODO:
            break;
        default:
            std::cout << "ERROR on cycle " << _cycleCount << " Unknown opcode: 0x" << std::hex << std::uppercase
                      << opcode << std::endl;
            this->_error = true;
    }

    this->_PC += 2;
}

const uint32_t Cpu::cycleCount() const {
    return this->_cycleCount;
}

void Cpu::runCycle() {
    // Read next opcode
    uint16_t opcode = this->_memory->readWord(_PC);

    std::cout << "Opcode 0x" << std::hex << std::uppercase << opcode << " on cycle " << this->_cycleCount << std::endl;

    // Execute opcode
    _executeOpcode(opcode);

    // Update timers
    if (this->_delayTimer > 0) --this->_delayTimer;
    if (this->_soundTimer > 0) --this->_soundTimer;

    // Update stats
    this->_cycleCount++;
}

const bool Cpu::error() const {
    return this->_error;
}
