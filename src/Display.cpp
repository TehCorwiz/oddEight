//
// Created by Corwin on 2018-02-17.
//

#include <iostream>

#include "Display.h"

Display::Display() {
    std::cout << "Display initialized." << std::endl;
}

void Display::clear() {
    for (uint8_t &pixel_block: this->_bitmap) {
        pixel_block = 0;
    }

    std::cout << "Display cleared." << std::endl;
}