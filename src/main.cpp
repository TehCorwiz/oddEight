//
// Created by Corwin on 2018-02-17.
//

#include <iostream>

#include "Chip8.h"

int main() {
    const std::string filename = "Pong (1 player).ch8";

    try {
        auto *chip8 = new Chip8;

        chip8->loadRom(filename);

        chip8->run();
    }
    catch (const std::exception &e) {
        std::cout << e.what() << '\n';
        return true;
    }

    return false;
}
