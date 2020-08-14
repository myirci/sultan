#ifndef MOVE_HPP
#define MOVE_HPP

/*
A move is represented by a 16-bit unsigned interger.

15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0

(15 - 10)  -> from
(9  -  4)  -> to
(3  -  0)  -> move flags
*/

#include <cstdint>

enum class Move_flag : std::uint8_t
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

    std::uint16_t mv;

    void set_from(std::uint8_t from);
    void set_to(std::uint8_t to);
    void set_move_flag(Move_flag mflag);

public:

    Move(std::uint8_t from, std::uint8_t to, Move_flag mflag);

    std::uint8_t get_from() const;
    std::uint8_t get_to() const;
    Move_flag get_move_flag() const;

    bool is_promotion() const;
    bool is_capture() const;
};

#endif // MOVE_HPP
