cmake_minimum_required(VERSION 3.5)

project(sultan LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
	${PROJECT_NAME}
    src/fen.hpp src/fen.cpp
    src/move.hpp src/move.cpp
    src/board.hpp src/board.cpp
	src/piece_location.hpp src/piece_location.cpp
    src/square.hpp src/square.cpp
    src/move_generator.hpp src/move_generator.cpp
	src/attack_detector.hpp src/attack_detector.cpp
	src/factory.hpp src/factory.cpp
    src/utility.hpp src/utility.cpp
    src/move_selector.hpp src/move_selector.cpp
	src/analyzer.hpp src/analyzer.cpp
	src/uci_engine.hpp src/uci_engine.cpp
    src/uci_game.hpp src/uci_game.cpp
	src/perft.hpp src/perft.cpp
    src/piece.hpp
    src/enums.hpp
    src/definitions.hpp
    
	src/test/board_tests.hpp src/test/board_tests.cpp
	src/test/fen_tests.hpp src/test/fen_tests.cpp
	src/test/move_generator_tests.hpp src/test/move_generator_tests.cpp
	src/test/move_tests.hpp src/test/move_tests.cpp 
	src/test/perft_tests.hpp src/test/perft_tests.cpp 
	src/test/piece_tests.hpp src/test/piece_tests.cpp
	src/test/test_base.hpp src/test/test_base.cpp
	
	src/main.cpp)
