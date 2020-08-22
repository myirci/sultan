#pragma once

#include <cstdint>

#include "definitions.hpp"

enum class MoveType : std::int8_t
{
    Quite = 0,                  // 0        0000
    Double_Pawn_Push,           // 1        0001
    King_Side_Castle,           // 2        0010
    Queen_Side_Castle,          // 3        0011
    Capture,                    // 4        0100
    En_Passant_Capture,         // 5        0101
    Knight_Promotion = 8,       //          1000
    Bishop_Promotion,           // 9        1001
    Rook_Promotion,             // 10       1010
    Queen_Promotion,            // 11       1011
    Knight_Promotion_Capture,   // 12       1100
    Bishop_Promotion_Capture,   // 13       1101
    Rook_Promotion_Capture,     // 14       1110
    Queen_Promotion_Capture     // 15       1111
};

class Move
{
private:

    std::int8_t from;
    std::int8_t to;
    MoveType mtype;
    std::int8_t captured;

public:

    Move(std::int8_t loc_from, std::int8_t loc_to, MoveType move_type, std::int8_t captured_piece);

    std::int8_t get_from() const;
    std::int8_t get_to() const;
    std::int8_t get_captured_piece() const;
    MoveType get_move_type() const;

    bool is_promotion() const;
    bool is_capture() const;
};

