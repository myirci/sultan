# project pre-sultal 1.0: chess engine development
# Murat Yirci

cmake_minimum_required(VERSION 2.8)
project(pre_sultan)
add_definitions(-std=c++0x)
set(CMAKE_BUILD_TYPE Debug)
set(HEADERS src/piece_traits.hpp src/color_traits.hpp)
set(SOURCES main.cpp src/x88_board.cpp src/x88_fen.cpp src/pieces.cpp src/utility_functions.cpp
            src/move.cpp)
add_executable(pre_sultan ${SOURCES} ${HEADERS})

