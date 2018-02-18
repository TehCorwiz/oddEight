//
// Created by Corwin on 2018-02-17.
//

#ifndef EIGHTEIGHT_DISPLAY_H
#define EIGHTEIGHT_DISPLAY_H


#include <cstddef>

class Display {
public:
    Display();

    void clear();

    double frames;

private:
    std::byte _bitmap[64 * 32] = {(std::byte) 0};
};


#endif //EIGHTEIGHT_DISPLAY_H
