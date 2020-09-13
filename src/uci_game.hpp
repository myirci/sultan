#pragma once

#include <string_view>
#include <memory>
#include <stack>

#include "move.hpp"
#include "move_selector.hpp"

class Board;

class UciGame
{
    UciGame(Board* brd);
    Board* board;
    
    std::stack<Move> move_stack;
    std::stack<state::BoardState> state_stack;

    friend class Factory;

public:
    void make_move(std::string_view mv_str);
    void make_move(Move const& mv);
    std::string make_move(uci::SearchParameters const& param);
    void unmake_move();
};