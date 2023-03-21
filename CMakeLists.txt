cmake_minimum_required(VERSION 3.24)
project(SER334_assignment3 C)

set(CMAKE_C_STANDARD 99)

add_executable(SER334_assignment3 src/main.c headers/BMPHandler.h headers/Image.h src/BMPHandler.c src/Image.c)
