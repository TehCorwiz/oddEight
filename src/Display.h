//
// Created by Corwin on 2018-02-17.
//

#ifndef ODDEIGHT_DISPLAY_H
#define ODDEIGHT_DISPLAY_H


#include <cstdint>

class Display {
public:
    constexpr static uint8_t displayWidth = 64;
    constexpr static uint8_t displayHeight = 32;
    constexpr static uint8_t framesPerSecond = 60;

    Display();

    void clear();

    uint8_t drawSprite(const uint8_t sprite[], uint8_t height, uint8_t x, uint8_t y);

private:
    uint8_t _bitmap[displayWidth * displayHeight] = {0};
    uint32_t _frameCount = 0;
};


#endif //ODDEIGHT_DISPLAY_H
