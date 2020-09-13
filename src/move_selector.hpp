#pragma once

#include "move.hpp"
#include <memory>

class Board;

class MoveSelector
{
	MoveSelector() = delete;

public:

	static std::unique_ptr<Move> select_random_move(Board const& board);
};


