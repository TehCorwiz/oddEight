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
    this->_PC = 0;

    for (uint8_t &v_register: this->_V) {
        v_register = 0;
    }

    this->_delayTimer = 0;
    this->_soundTimer = 0;

    std::cout << "CPU reset." << std::endl;
}
