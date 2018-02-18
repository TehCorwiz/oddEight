//
// Created by Corwin on 2018-02-17.
//

#include <iostream>
#include "Cpu.h"

Cpu::Cpu() {
    std::cout << "CPU initialized." << std::endl;
}

void Cpu::reset() {
    this->I = 0;
    this->PC = 0;

    for (std::byte &v_register: this->V) {
        v_register = (std::byte) 0;
    }

    this->delay_timer = (std::byte) 0;
    this->sound_timer = (std::byte) 0;
}
