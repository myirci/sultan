#pragma once

#include <string>
#include <memory>
#include <stack>
#include "move.hpp"

class Board;
class MoveGenerator;


class Engine
{
    std::unique_ptr<Board> board;
    std::unique_ptr<MoveGenerator> mg;

    std::stack<Move> moves_stack;
    std::stack<int8_t> castling_rights_stack;
    std::stack<int8_t> en_passant_loc_stack;
    std::stack<int8_t> captured_pieces_stack;

public:
    Engine(const std::string& fen_str);
    std::uint64_t perft(int depth);
    std::uint64_t perft_divide(int depth);
};