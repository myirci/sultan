#include <iostream>
#include "fen.hpp"
#include "test.hpp"
#include "board_representation/board.hpp"
#include "utility.hpp"

#include "board_representation/square.hpp"

using namespace std;

int main()
{
    Test::run_fen_tests();
    Test::run_move_tests();
    Test::run_piece_tests();
    Test::run_board_tests();
    Test::run_utility_tests();
    Test::run_move_generator_tests();

    /*
    std::vector<Move> moves
    {
        Move(Square::a2, Square::a4, Move_flag::Double_Pawn_Push),
        Move(Square::a7, Square::a5, Move_flag::Double_Pawn_Push)
    };

    std::cout << "a7: " << static_cast<int>(Square::a7) << std::endl;
    std::cout << "a5: " << static_cast<int>(Square::a5) << std::endl;

    Utility::print_moves(moves);
    */

    return 0;
}
