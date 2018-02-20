cmake_minimum_required(VERSION 3.9)
project(eighteight)

set(eighteight_VERSION_MAJOR 0)
set(eighteight_VERSION_MINOR 1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Werror -m64 -O3 -static-libgcc -static-libstdc++ -static")

file(GLOB SOURCES "src/*.cpp")
add_executable(eighteight ${SOURCES})
