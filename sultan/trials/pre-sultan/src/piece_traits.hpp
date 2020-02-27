#ifndef PIECE_TRAITS_HPP
#define PIECE_TRAITS_HPP

#include "pieces.hpp"
#include "color_traits.hpp"
#include <vector>
#include <cstdint>

template<typename T1, typename T2 = White>
struct PieceTraits;

template<typename T2>
struct PieceTraits<Rook, T2> {
    static Pieces piece_type() {
        if(ColorTraits<T2>::color == 1) { return Pieces::wR; }
        else { return Pieces::bR; }
    }
    static std::vector<std::int8_t> const & move_rights() { return Rook::rook_moves; }
};

template<typename T2>
struct PieceTraits<Bishop, T2> {
    static Pieces piece_type() {
        if(ColorTraits<T2>::color == 1) { return Pieces::wB; }
        else { return Pieces::bB; }
    }
    static std::vector<std::int8_t> const & move_rights() { return Bishop::bishop_moves; }
};

template<typename T2>
struct PieceTraits<Knight, T2> {
    static Pieces piece_type() {
        if(ColorTraits<T2>::color == 1) { return Pieces::wN; }
        else { return Pieces::bN; }
    }
    static std::vector<std::int8_t> const & move_rights() { return Knight::knight_moves; }
};

template<typename T2>
struct PieceTraits<King, T2> {
    static Pieces piece_type() {
        if(ColorTraits<T2>::color == 1) { return Pieces::wK; }
        else { return Pieces::bK; }
    }
    static std::vector<std::int8_t> const & move_rights() { return King::king_queen_moves; }
};

template<typename T2>
struct PieceTraits<Queen, T2> {
    static Pieces piece_type() {
        if(ColorTraits<T2>::color == 1) { return Pieces::wQ; }
        else { return Pieces::bQ; }
    }
    static std::vector<std::int8_t> const & move_rights() { return King::king_queen_moves; }
};

template<typename T2>
struct PieceTraits<Pawn, T2> {
    static Pieces piece_type() {
        if(ColorTraits<T2>::color == 1) { return Pieces::wP; }
        else { return Pieces::bP; }
    }
};



#endif // PIECE_TRAITS_HPP
