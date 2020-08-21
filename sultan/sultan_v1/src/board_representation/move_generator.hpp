#ifndef MOVE_GENERATOR_HPP
#define MOVE_GENERATOR_HPP

#include <vector>
#include <utility>

#include "move.hpp"

class Board;

class MoveGenerator
{
    Board const & board_obj;
    int8_t const * board;

    struct Attack { int8_t attacker_loc; int8_t attack_dir; };
    struct Pin { int8_t pinner_loc; int8_t pinned_loc; int8_t pin_dir; };

    std::pair<std::vector<Attack>, std::vector<Pin>> compute_attacks_and_pins(int8_t target, int8_t stm) const;
    bool is_under_attack(int8_t sq, int8_t stm) const;
    int8_t get_pin_direction(int8_t sq, std::vector<Pin> const & pins) const;

    // Move generators
    void generate_king_moves(int8_t king_pos, int8_t stm, bool under_check, std::vector<Move>& moves) const;
    void generate_non_king_check_eliminating_moves(int8_t king_pos, int8_t stm, std::pair<std::vector<Attack>, std::vector<Pin>> const & attpin, std::vector<Move>& moves) const;
    void generate_non_king_to_square_moves(int8_t sq, int8_t stm, std::vector<Pin> const & pins, Move_flag flag, std::vector<Move>& moves) const;
    void generate_sliding_piece_moves(int8_t sq, int8_t ptype, int8_t stm, std::vector<Pin> const & pins, std::vector<Move>& moves) const;
    void generate_knight_moves(int8_t sq, int8_t stm, std::vector<Pin> const & pins, std::vector<Move>& moves) const;
    void generate_pawn_moves(int8_t sq, int8_t stm, std::vector<Pin> const & pins, std::vector<Move>& moves) const;

    friend class Test;

public:
    MoveGenerator(Board const & b);
    std::vector<Move> generate_moves() const;
};

#endif // MOVE_GENERATOR_HPP
