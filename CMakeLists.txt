cmake_minimum_required(VERSION 3.9)
project(oddEight)

set(oddEight_VERSION_MAJOR 0)
set(oddEight_VERSION_MINOR 1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Werror -static-libgcc -static-libstdc++ -static")

configure_file("test_roms/Maze (alt) [David Winter, 199x].ch8" ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

file(GLOB SOURCES "src/*.cpp")
add_executable(oddEight ${SOURCES})
