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

uint8_t Display::drawSprite(const uint8_t sprite[], uint8_t height, uint8_t x, uint8_t y) {
    uint8_t collision = 0;

    for (uint8_t row = 0; row < height; row++) { // Row (y coordinate)
        for (uint8_t column = 0; column < 8; column++) { // Column (x coordinate)
            if ((sprite[row] & (0x80 >> column)) != 0) { // Pixel is on?
                auto current_pixel = (x + column + ((y + row) * Display::displayWidth)); // Current pixel
                if (this->_bitmap[current_pixel] == 1) collision = 1; // Collision check
                this->_bitmap[current_pixel] ^= 1; // XOR the pixel
            }
        }
    }

    return collision;
}
