//
// Created by Corwin on 2018-02-17.
//

#include <iostream>

#include "IO.h"

bool IO::readKey(uint8_t key) {
    return this->_keypad[key];
}

bool IO::keyDown(uint8_t key) {
    this->_keypad[key] = true;
}

bool IO::keyUp(uint8_t key) {
    this->_keypad[key] = false;
}
