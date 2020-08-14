cmake_minimum_required(VERSION 3.5)

project(sultan_v1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
    src/fen.hpp src/fen.cpp
    src/move.hpp src/move.cpp
    src/board_representation/board.hpp 
	src/board_representation/board.cpp
    src/board_representation/square.hpp 
	src/board_representation/square.cpp
    src/board_representation/move_generator.hpp 
	src/board_representation/move_generator.cpp
    src/utility.hpp 
	src/utility.cpp
    src/uci.hpp 
	src/uci.cpp
    src/engine.hpp 
	src/engine.cpp
    src/test.hpp 
	src/test.cpp
    src/piece.hpp
    src/enums.hpp
    src/main.cpp)
