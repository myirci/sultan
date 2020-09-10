#pragma once

#include <cstdint>

enum class Castling : int8_t
{
    white_king_side = 1,
    white_queen_side = 2,
    black_king_side = 4,
    black_queen_side = 8
};


