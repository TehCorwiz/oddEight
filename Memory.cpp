//
// Created by Corwin on 2018-02-17.
//

#include <iostream>
#include <cstddef>

#include "Memory.h"

Memory::Memory() {
    std::cout << "Memory initialized.\r\n";

    this->dump();
}

std::byte Memory::readByte() {
    // TODO
}

void Memory::writeByte(std::byte byte) {
    // TODO
}

void Memory::clear() {
    for (std::byte &cell: this->memory) {
        cell = (std::byte) 0;
    }

    std::cout << "Memory cleared.\r\n";

    this->dump();
}

void Memory::dump() {
    std::string output;

    output.append("0:" + std::to_string((char) this->memory[0]) + "\r\n");

    std::cout << output;
}