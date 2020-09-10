#pragma once

#include <cstdint>
#include <vector>
#include <memory>

#include "move.hpp"
#include "enums.hpp"
#include "piece_location.hpp"
#include "attack_detector.hpp"
#include "test/board_tests.hpp"

class AttackDetector;

class Board
{
    Board();
    Board(Board const& brd);
    Board& operator=(Board const& brd) = delete;
    Board(Board&& brd) = delete;
    Board& operator=(Board&& brd) = delete;

    int8_t board[128];         
    int16_t half_move_counter;
    int16_t full_move_counter;
    int8_t en_passant_loc;
    int8_t side_to_move;
    int8_t castling_rights;

    std::unique_ptr<PieceLocation> ploc;
    std::unique_ptr<AttackDetector> attacks;

    inline void make_quite_move(int8_t from, int8_t to);

    friend class Factory;
    friend class Utility;
    friend class test::BoardTests;

public:

    constexpr static int BOARDSIZE = 128;

    int8_t* get_board();

    int8_t const * get_board() const;
    
    int8_t get_side_to_move() const;
    void set_side_to_move(int8_t);

    bool query_castling_rights(Castling c) const;
    void set_castling_rights(Castling c, bool val);
    void set_castling_rights(int8_t color, bool val);
    int8_t get_castling_rights() const;

    int16_t get_half_move_counter() const;
    void set_half_move_counter(int16_t val);

    int16_t get_full_move_counter() const;
    void set_full_move_counter(int16_t val);

    int8_t get_en_passant_loc() const;
    void set_en_passant_loc(int8_t val);

    // Piece Location:
    int8_t find_piece_location(int8_t ptype) const;
    std::pair<PieceLocation::PLConstIt, PieceLocation::PLConstIt> find_piece_locations(int8_t ptype) const;

    // Attack Detector:
    int8_t num_checkers(int8_t king_pos) const;
    std::pair<int8_t, int8_t> get_checker_positions(int8_t king_pos) const;
    bool is_under_attack(int8_t attacking_side, int8_t target_sq, int8_t exclude_pos1 = def::none, int8_t exclude_pos2 = def::none) const;
    int8_t get_pin_direction(int8_t sq) const;
    std::pair<int8_t, int8_t> get_attack_info(int8_t pos) const;

    state::BoardState make_move(Move const& mv);
    void unmake_move(Move const& mv, state::BoardState const& st);
};
