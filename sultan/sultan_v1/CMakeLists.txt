cmake_minimum_required(VERSION 3.5)

project(sultan_v1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
    fen.hpp fen.cpp
    move.hpp move.cpp
    board_representation/board.hpp board_representation/board.cpp
    board_representation/square.hpp board_representation/square.cpp
    board_representation/move_generator.hpp board_representation/move_generator.cpp
    utility.hpp utility.cpp
    uci.hpp uci.cpp
    engine.hpp engine.cpp
    test.hpp test.cpp
    piece.hpp
    enums.hpp
    main.cpp)
