#ifndef PIECES_HPP
#define PIECES_HPP

#include <cstdint>
#include <string>
#include <vector>

enum class Pieces : std::int8_t {
    bP = -6,
    bK = -5,
    bQ = -4,
    bB = -3,
    bN = -2,
    bR = -1,
    empty = 0,
    wR = 1,
    wN = 2,
    wB = 3,
    wQ = 4,
    wK = 5,
    wP = 6
};

struct Rook {
    const static std::vector<std::int8_t> rook_moves;
};

struct Bishop {
    const static std::vector<std::int8_t> bishop_moves;
};

struct Knight {
    const static std::vector<std::int8_t> knight_moves;
};

struct King {
    const static std::vector<std::int8_t> king_queen_moves;
};

struct Pawn { };
struct Queen { };

struct White { };
struct Black { };

std::string piece_to_string(const Pieces p);
char piece_to_char(const Pieces p);

#endif
