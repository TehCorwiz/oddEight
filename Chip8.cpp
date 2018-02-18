//
// Created by Corwin on 2018-02-17.
//

#include <iostream>

#include "Chip8.h"

Chip8::Chip8() {

    this->memory = new Memory;
    this->io = new IO;
    this->display = new Display;
    this->cpu = new Cpu(memory, io, display);

    this->steps = 0;

    std::cout << "System initialized." << std::endl;
}

void Chip8::runStep() {
    // TODO

    this->steps++;
    if (this->steps >= 1000) this->isRunning = false;
}

void Chip8::reset() {
    this->cpu->reset();
    this->memory->clear();
    this->display->clear();
}
