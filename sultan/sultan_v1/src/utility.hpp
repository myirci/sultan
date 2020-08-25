#pragma once

#include <string>
#include <unordered_map>

#include "move.hpp"
#include "move_generator.hpp"

class Fen;
class Board;

class Utility
{
    Utility() = delete;

public:

    static const std::unordered_map<MoveType, char const*> move_type_to_chararr;

    static std::string board_to_fen_string(Board const & b);

    static void fen_to_board(Fen const & f, Board& b);

    static void print_board(Board const & b, bool full = false);

    static void print_moves(std::vector<Move> const& moves);

    // static void generate_and_print_moves(Board const& board);

    static void print_move(Move m);

    static bool is_equal(Move m1, Move m2);

    static void print_attacks(std::vector<attack::AttackInfo> const& attack_infos);

};
