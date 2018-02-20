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

    for (std::byte &v_register: this->_V) {
        v_register = (std::byte) 0;
    }

    this->_delay_timer = (std::byte) 0;
    this->_sound_timer = (std::byte) 0;

    std::cout << "CPU reset." << std::endl;
}
