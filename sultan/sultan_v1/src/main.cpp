#include <iostream>

#include "engine.hpp"

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

void unit_tests();
void perft_tests();

int main(int argc, char* argv[])
{
    auto engine = make_unique<Engine>();
    engine->run();
    return 0;
}

void unit_tests()
{
    test::FenTests::run_test();
    test::MoveTests::run_test();
    test::PieceTests::run_test();
    test::BoardTests::run_test();
    test::UtilityTests::run_test();
    test::MoveGeneratorTests::run_test();
}

void perft_tests() 
{
    test::PerftTests::run_test();
}