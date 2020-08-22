#pragma once

#include <cstdint>
#include <unordered_map>
#include "definitions.hpp"

namespace piece
{
    inline constexpr int8_t eM { 0 };     // empty
    inline constexpr int8_t wP { 1 };     // white pawn
    inline constexpr int8_t bP { -wP };   // black pawn
    inline constexpr int8_t wR { 2 };     // white rook
    inline constexpr int8_t bR { -wR };   // black rook
    inline constexpr int8_t wN { 3 };     // white knight
    inline constexpr int8_t bN { -wN };   // black knight
    inline constexpr int8_t wB { 4 };     // white bishop
    inline constexpr int8_t bB { -wB };   // black bishop
    inline constexpr int8_t wQ { 5 };     // white queen
    inline constexpr int8_t bQ { -wQ };   // black queen
    inline constexpr int8_t wK { 6 };     // white king
    inline constexpr int8_t bK { -wK };   // black king

    inline constexpr int8_t Pawn { wP };
    inline constexpr int8_t Rook { wR };
    inline constexpr int8_t Knight { wN };
    inline constexpr int8_t Bishop { wB };
    inline constexpr int8_t Queen { wQ };
    inline constexpr int8_t King { wK };

    inline const std::unordered_map<char, int8_t> char_to_piece =
    {
        {'P', piece::wP }, {'p', piece::bP },
        {'R', piece::wR }, {'r', piece::bR },
        {'N', piece::wN }, {'n', piece::bN },
        {'B', piece::wB }, {'b', piece::bB },
        {'Q', piece::wQ }, {'q', piece::bQ },
        {'K', piece::wK }, {'k', piece::bK }
    };

    inline const std::unordered_map<int8_t, char> piece_to_char =
    {
        { piece::wP, 'P' }, { piece::bP, 'p' },
        { piece::wR, 'R' }, { piece::bR, 'r' },
        { piece::wN, 'N' }, { piece::bN, 'n' },
        { piece::wB, 'B' }, { piece::bB, 'b' },
        { piece::wQ, 'Q' }, { piece::bQ, 'q' },
        { piece::wK, 'K' }, { piece::bK, 'k' }, 
        { piece::eM, ' ' }, { def::none, 'x' }
    };

    inline constexpr bool is_same_sign(int8_t a, int8_t b)
    {
        return (a > 0 && b > 0) || (a < 0 && b < 0);
    }

    inline constexpr bool is_different_sign(int8_t a, int8_t b)
    {
        return (a > 0 && b < 0) || (a < 0 && b > 0);
    }

}

