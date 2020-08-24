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
    */
    // test::MoveGeneratorTests::run_test();
    // test::PerftTests::run_test();


    // test::MoveGeneratorTests::debug_func();
    test::PerftTests::debug_func();
    
    return 0;
}
