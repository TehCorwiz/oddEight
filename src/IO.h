//
// Created by Corwin on 2018-02-17.
//

#ifndef ODDEIGHT_IO_H
#define ODDEIGHT_IO_H


#include <cstdint>

class IO {
public:
    bool readKey(uint8_t key);

    bool keyDown(uint8_t key);
    bool keyUp(uint8_t key);

private:
    bool _keypad[16] = {false};
};


#endif //ODDEIGHT_IO_H
