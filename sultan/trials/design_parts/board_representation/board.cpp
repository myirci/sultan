#include "board.hpp"
#include "square.hpp"
#include "../piece.hpp"

#include <algorithm>
#include <array>

Board::Board()
{
    clear();
}

void Board::clear()
{
    std::fill(board, board + BOARDSIZE, Piece::nN);
    white_pieces.clear();
    black_pieces.clear();
    en_passant_loc = Piece::nN;
    flags = 0;
    half_move_counter = -1;
    full_move_counter = -1;
}

int8_t Board::get_side_to_move() const { return flags & 1 ? Piece::white : Piece::black; }

void Board::set_side_to_move(int8_t c) { flags = (c == Piece::white ? flags | 1 : flags & 0xFE); }

bool Board::query_castling_availability(Castling c) const { return flags & static_cast<int8_t>(c); }

void Board::set_castling_availability(Castling c, bool val) { flags = val ? flags | static_cast<int8_t>(c) : flags & ~static_cast<int8_t>(c); }

int8_t* Board::get_board() { return  board; }

int8_t const * Board::get_board() const { return board; }

int16_t Board::get_half_move_counter() const { return half_move_counter; }

void Board::set_half_move_counter(int16_t val) { half_move_counter = val; }

int16_t Board::get_full_move_counter() const { return full_move_counter; }

void Board::set_full_move_counter(int16_t val) { full_move_counter = val; }

int8_t Board::get_en_passant_loc() const { return en_passant_loc; }

void Board::set_en_passant_loc(int8_t val) { en_passant_loc = val; }

std::vector<int8_t>& Board::get_white_piece_locations() { return white_pieces; }
const std::vector<int8_t>& Board::get_white_piece_locations() const { return white_pieces; }

std::vector<int8_t>& Board::get_black_piece_locations() { return black_pieces; }
const std::vector<int8_t>& Board::get_black_piece_locations() const { return black_pieces; }

void Board::set_piece_locations(const std::unordered_multimap<int8_t, int8_t>& plocs)
{
    white_pieces.clear();
    black_pieces.clear();

    std::array<int8_t, 6> piece_order { Piece::wK, Piece::wQ, Piece::wR, Piece::wB, Piece::wN, Piece::wP };
    for(auto it = piece_order.begin(); it != piece_order.end(); it++)
    {
        auto r = plocs.equal_range(*it);
        std::for_each(r.first, r.second, [&](auto p) { white_pieces.push_back(p.second); });
    }

    std::for_each(piece_order.begin(), piece_order.end(), [](int8_t& i) { i*= -1; });
    for(auto it = piece_order.begin(); it != piece_order.end(); it++)
    {
        auto r = plocs.equal_range(*it);
        std::for_each(r.first, r.second, [&](auto p) { black_pieces.push_back(p.second); });
    }
}
