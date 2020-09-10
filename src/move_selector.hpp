#pragma once

#include "move.hpp"

class Board;

class MoveSelector
{
	MoveSelector() = delete;

public:

	static Move select_random_move(Board const& board);
};


