//
// Created by Corwin on 2018-02-17.
//

#include <fstream>
#include <iostream>

#include "Chip8.h"

int main() {
    const std::string filename = "test.ch8";

    auto *chip8 = new Chip8;

    chip8->loadRom(filename);

    return chip8->run();
}
