cmake_minimum_required(VERSION 3.9)
project(eighteight)

set(CMAKE_CXX_STANDARD 17)

add_executable(eighteight main.cpp Cpu.cpp Cpu.h Memory.cpp Memory.h Display.cpp Display.h IO.cpp IO.h Chip8.cpp Chip8.h)