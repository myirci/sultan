#pragma once

#include <vector>
#include <utility>
#include <memory>
#include <unordered_map>

#include "move.hpp"
#include "definitions.hpp"
#include "test/move_generator_tests.hpp"

class Board;

class MoveGenerator
{
    Board const& board_obj;
    int8_t const* board;
    std::unordered_multimap<int8_t, int8_t> const& piece_loc;

    // key: target loc, value: attacker loc and attack direction, double entry is only possible in the case of double check
    std::unordered_multimap<int8_t, std::pair<int8_t, int8_t>> attack_info;  

    void compute_checks_and_pins(int8_t attacking_side);
    bool is_under_attack(int8_t attacking_side, int8_t target_sq, int8_t exclude_pos1 = def::none, int8_t exclude_pos2 = def::none) const;
    int8_t MoveGenerator::get_pin_direction(int8_t sq) const;
   
    // Move generators
    void generate_king_moves(int8_t clr, std::vector<Move>& moves) const;
    void generate_check_eliminating_moves(int8_t clr, int8_t king_pos, std::vector<Move>& moves) const;
    void generate_to_square_moves(int8_t clr, int8_t sq, std::vector<Move>& moves) const;

    void generate_knight_moves(int8_t clr, std::vector<Move>& moves) const;
    void generate_sliding_piece_moves(int8_t clr, int8_t ptype, std::vector<Move>& moves) const;
    void generate_pawn_moves(int8_t clr, std::vector<Move>& moves) const;

    friend class test::MoveGeneratorTests;

public:
    
    MoveGenerator(Board const& b);
    std::vector<Move> generate_moves() const;
    void compute_attacks();
    int8_t find_king_pos(int8_t clr) const;
    std::unordered_multimap<int8_t, std::pair<int8_t, int8_t>> const & get_attack_info() const;
};
