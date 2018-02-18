//
// Created by Corwin on 2018-02-17.
//

#include <iostream>
#include "Chip8.h"

Chip8::Chip8() {

    this->display = new Display;
    this->memory = new Memory;
    this->io = new IO;
    this->cpu = new Cpu;

    this->steps = 0;

    std::cout << "System initialized.\r\n";
}

void Chip8::loadRom(std::ifstream &rom) {
    // TODO
}

void Chip8::runStep() {
    // TODO
    this->steps++;
}

void Chip8::reset() {
    // TODO
    this->memory->clear();
}