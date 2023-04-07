cmake_minimum_required(VERSION 3.24)
project(SER334_assignment3 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")



#add_executable(SER334_assignment3 src/HakobyanImageProcessor.c headers/BMPHandler.h headers/Image.h src/BMPHandler.c src/Image.c src/BaseFilters.c headers/BmpProcessor.h headers/PixelProcessor.h src/HeaderUsageSample.c)
add_executable(HakobyanFilters HakobyanFilters.c BmpProcessor.c BmpProcessor.h PixelProcessor.c PixelProcessor.h)

target_link_libraries(HakobyanFilters m)