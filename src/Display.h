//
// Created by Corwin on 2018-02-17.
//

#ifndef ODDEIGHT_DISPLAY_H
#define ODDEIGHT_DISPLAY_H


#include <cstddef>

class Display {
public:
    Display();

    void clear();

    double frames;

private:
    std::byte _bitmap[64 * 32] = {(std::byte) 0};
};


#endif //ODDEIGHT_DISPLAY_H
