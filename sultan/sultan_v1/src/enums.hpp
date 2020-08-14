#ifndef COLOR_HPP
#define COLOR_HPP

#include <cstdint>

enum class Castling : int8_t
{
    white_king_side = 2,
    white_queen_side = 4,
    black_king_side = 8,
    black_queen_side = 16
};

#endif // COLOR_HPP
