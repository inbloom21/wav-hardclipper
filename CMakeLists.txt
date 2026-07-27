cmake_minimum_required(VERSION 3.10)
project(WavHardClipper)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

set(SOURCES
    source/main.cpp
    source/FileHandler.cpp
    source/AudioProcessor.cpp
)

add_executable(WavHardClipper ${SOURCES})
