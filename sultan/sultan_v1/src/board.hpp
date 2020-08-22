#pragma once

/* Implements 0x88 board representation */

#include <cstdint>
#include <unordered_map>
#include <vector>
#include <memory>

#include "move.hpp"
#include "enums.hpp"

class Board
{
    int8_t board[128];

    int16_t half_move_counter;
    int16_t full_move_counter;
    int8_t en_passant_loc;
    int8_t side_to_move;
    int8_t castling_rights;

    std::unordered_multimap<int8_t, int8_t> piece_loc; // keys: pieces, values: locations

public:

    constexpr static int BOARDSIZE = 128;

    Board();

    int8_t* get_board();
    int8_t const * get_board() const;

    int8_t get_side_to_move() const;
    void set_side_to_move(int8_t);

    bool query_castling_rights(Castling c) const;
    void set_castling_rights(Castling c, bool val);
    std::int8_t get_castling_rights() const;

    int16_t get_half_move_counter() const;
    void set_half_move_counter(int16_t val);

    int16_t get_full_move_counter() const;
    void set_full_move_counter(int16_t val);

    int8_t get_en_passant_loc() const;
    void set_en_passant_loc(int8_t val);

    void set_piece_locations(std::unordered_multimap<int8_t, int8_t>&& plocs);
    std::unordered_multimap<int8_t, int8_t>& get_piece_locations();
    std::unordered_multimap<int8_t, int8_t> const& get_piece_locations() const;

    void clear();

    void update_castling_rights(Move m);
    void make_move(Move m);
    inline void make_quite_move(int8_t from, int8_t to);
    void unmake_move(Move m);
     
    void update_piece_loc(int8_t old_loc, int8_t new_loc);
    void remove_piece(int8_t loc);
    inline void add_piece(int8_t p, int8_t loc);
};
