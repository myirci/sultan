#include <iostream>
#include "fen.hpp"
#include "test.hpp"
#include "board_representation/board.hpp"
#include "utility.hpp"

using namespace std;

int main()
{
    Test::run_fen_tests();
    Test::run_move_tests();
    Test::run_piece_tests();
    Test::run_board_tests();
    Test::run_utility_tests();
    Test::run_move_generator_tests();
    return 0;
}
