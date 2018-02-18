//
// Created by Corwin on 2018-02-17.
//

#ifndef EIGHTEIGHT_DISPLAY_H
#define EIGHTEIGHT_DISPLAY_H


#include <cstddef>

class Display {
public:
    Display();

    std::byte bitmap[64 * 32] = {(std::byte) 0};
    double frames;
};


#endif //EIGHTEIGHT_DISPLAY_H
