#pragma once

#include <string>
#include <memory>
#include <stack>

#include "move.hpp"
#include "move_generator.hpp"
#include "board.hpp"
#include "definitions.hpp"

class Board;
class MoveGenerator;

class Engine
{
    std::unique_ptr<Board> board;
    std::unique_ptr<MoveGenerator> mg;
    std::stack<Move> moves_stack;
public:

    Engine(const std::string& fen_str);
    uint64_t perft(int depth);
    void perft_with_statistics(int depth, perft::perft_stats& stats);
    uint64_t perft_divide(int depth);
};