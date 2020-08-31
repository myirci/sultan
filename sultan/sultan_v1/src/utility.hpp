#pragma once

#include <string>
#include <unordered_map>

#include "move.hpp"
#include "move_generator.hpp"

class Fen;
class Board;

class Utility
{
    Utility() = delete;

public:

    static const std::unordered_map<MoveType, char const*> move_type_to_chararr;

    static std::string board_to_fen_string(Board const & b);

    static void fen_to_board(Fen const & f, Board& b);

    static void print_board(Board const & b, bool full = false);

    static void print_moves(std::vector<Move> const& moves);

    static void generate_and_print_moves(Board const& board);

    static void generate_and_print_moves(std::string const& fen_str);

    static void print_move(Move const& m);

    static void print_attacks(std::unordered_map<int8_t, std::pair<int8_t, int8_t>> const& attack_info);

    static std::string to_string(Move const& mv);

    static bool is_equal(Move m1, Move m2);

    static std::vector<std::string> tokenize(std::string const& str);

    // trim from start (in place)
    static inline void ltrim(std::string& s) 
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !std::isspace(ch); }));
    }

    // trim from end (in place)
    static inline void rtrim(std::string& s) 
    { 
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !std::isspace(ch);}).base(), s.end());
    }

    // trim from both ends (in place)
    static inline void trim(std::string& s) 
    {
        ltrim(s);
        rtrim(s);
    }

};
