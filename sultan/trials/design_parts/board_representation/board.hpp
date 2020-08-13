#ifndef BOARD_HPP
#define BOARD_HPP

/*

Implements 0x88 board representation

*/

#include <cstdint>
#include <unordered_map>
#include <vector>
#include <memory>

#include "../enums.hpp"

class Move;

class Board
{
    int8_t board[128];

    int16_t half_move_counter;
    int16_t full_move_counter;
    int8_t en_passant_loc;
    int8_t flags;

    std::vector<int8_t> white_pieces;
    std::vector<int8_t> black_pieces;

    friend class Test;

public:

    constexpr static int BOARDSIZE = 128;

    Board();

    int8_t* get_board();
    int8_t const * get_board() const;

    int8_t get_side_to_move() const;
    void set_side_to_move(int8_t);

    bool query_castling_availability(Castling c) const;
    void set_castling_availability(Castling c, bool val);

    int16_t get_half_move_counter() const;
    void set_half_move_counter(int16_t val);

    int16_t get_full_move_counter() const;
    void set_full_move_counter(int16_t val);

    int8_t get_en_passant_loc() const;
    void set_en_passant_loc(int8_t val);

    void set_piece_locations(const std::unordered_multimap<int8_t, int8_t>& plocs);

    std::vector<int8_t>& get_white_piece_locations();
    const std::vector<int8_t>& get_white_piece_locations() const;

    std::vector<int8_t>& get_black_piece_locations();
    const std::vector<int8_t>& get_black_piece_locations() const;

    void clear();
};

#endif // BOARD_HPP
