#include <iostream>
#include "fen.hpp"
#include "board.hpp"
#include "utility.hpp"

#include "square.hpp"

#include "test/fen_tests.hpp"
#include "test/move_tests.hpp"
#include "test/piece_tests.hpp"
#include "test/board_tests.hpp"
#include "test/utility_tests.hpp"
#include "test/move_generator_tests.hpp"
#include "test/perft_tests.hpp"

using namespace std;

int main()
{

    /*
    test::FenTests::run_test();
    test::MoveTests::run_test();
    test::PieceTests::run_test();
    test::BoardTests::run_test();
    test::UtilityTests::run_test();
    test::MoveGeneratorTests::run_test();
    */
    
    // test::BoardTests::run_test();

    test::MoveGeneratorTests::run_test();
    // test::PerftTests::batch_test(R"(C:\Users\myirci\Documents\dev\bitbucket\chess\sultan\sultan_v1\src\test\data\perft_positions1.txt)");
    // test::PerftTests::batch_test(R"(C:\Users\myirci\Documents\dev\bitbucket\chess\sultan\sultan_v1\src\test\data\perft_positions2.txt)");
    // test::MoveGeneratorTests::debug_func();
    // test::PerftTests::debug_func();

    // std::unique_ptr<Board> b = std::make_unique<Board>();
    // Fen f("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    // Utility::fen_to_board(f, *b);
    // Utility::print_board(*b, true);

    return 0;
}
