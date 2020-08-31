#pragma once

#include <string>
#include <memory>
#include <stack>

#include "move.hpp"
#include "move_generator.hpp"
#include "board.hpp"
#include "definitions.hpp"

class Game
{
    std::unique_ptr<Board> board; 
    std::unique_ptr<MoveGenerator> mg;

    std::stack<Move> move_stack;
    std::stack<state::BoardState> state_stack;

public:

    Game();
    Game(const std::string& fen_str);

    void make_move(std::string_view mv_str);
    void make_move(Move const& mv);
    void unmake_move();
};