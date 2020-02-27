#ifndef UTILITY_FUNCTIONS_HPP
#define UTILITY_FUNCTIONS_HPP

#include "move.hpp"
#include <vector>

class X88_board;

void print_moves(std::vector<Move>& moves);
void print_board_bb_numeric(const X88_board& b);
void print_board_wb_numeric(const X88_board& b);
void print_board_wb_alphabetic(const X88_board& b);
void print_board_bb_alphabetic(const X88_board& b);
void print_black_pieces(const X88_board& b);
void print_white_pieces(const X88_board& b);
void print_other_info(const X88_board& b);
void generate_moves_iteratively(X88_board& b, int ply);

#endif // UTILITY_FUNCTIONS_HPP
