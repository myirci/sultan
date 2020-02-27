#include <iostream>

#include "src/x88_board.hpp"
#include "src/x88_fen.hpp"
#include "src/utility_functions.hpp"
#include "src/move.hpp"
#include <vector>

using namespace std;
int main(void) {
    X88_board b;
    // X88_fen fen("1r4bn/P1P4P/8/8/8/8/8/6K1 w - - 0 1");
    // X88_fen fen("1r1b1r1n/P1P1P1P1/7R/3Q4/5BB1/P1N5/2N2K2/1R6 w - - 0 1");
    // X88_fen fen("1nnrrbbq/PPPPPPPP/1R6/6K1/Q7/2BNNB2/7R/6k1 w - - 0 1");
    // fen.transfer_to_board(b);
    // std::vector<Move> moves;
    // b.generate_moves(moves);
    // print_moves(moves);
    print_board_bb_numeric(b);
    return 0;
}


