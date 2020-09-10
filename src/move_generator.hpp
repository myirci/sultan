#pragma once

#include <vector>
#include <utility>
#include <memory>
#include <unordered_map>

#include "board.hpp"
#include "piece_location.hpp"
#include "move.hpp"
#include "definitions.hpp"
#include "attack_detector.hpp"
#include "test/move_generator_tests.hpp"

class MoveGenerator
{
    MoveGenerator() = delete;

    // Move generators
    static void generate_king_moves(Board const& board, std::vector<Move>& moves);
    static void generate_to_square_moves(Board const& board, int8_t sq, std::vector<Move>& moves);
    static void generate_check_eliminating_moves(Board const& board, int8_t king_pos, std::vector<Move>& moves);
    static void generate_knight_moves(Board const& board, std::vector<Move>& moves);
    static void generate_sliding_piece_moves(Board const& board, int8_t ptype, std::vector<Move>& moves);
    static void generate_pawn_moves(Board const& board, std::vector<Move>& moves);

    friend class test::MoveGeneratorTests;

public:
    
    static std::vector<Move> generate_moves(Board const& board);
};
