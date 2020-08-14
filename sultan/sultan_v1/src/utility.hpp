#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <string>

class Fen;
class Board;

class Utility
{
    Utility() = delete;

public:

    static std::string board_to_fen_string(const Board& b);

    static void fen_to_board(const Fen& f, Board& b);

    static void print_board(const Board& b, bool full = false);

};

#endif // UTILITY_HPP
