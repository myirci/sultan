#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <string>
#include <unordered_map>

#include "board_representation/move.hpp"

class Fen;
class Board;

class Utility
{
    Utility() = delete;

public:

    static const std::unordered_map<Move_flag, char const*> move_flag_to_chararr;

    static std::string board_to_fen_string(Board const & b);

    static void fen_to_board(Fen const & f, Board& b);

    static void print_board(Board const & b, bool full = false);

    static void print_moves(std::vector<Move> const& moves);

    static void print_move(Move m);

    static bool is_equal(Move m1, Move m2);

};

#endif // UTILITY_HPP
