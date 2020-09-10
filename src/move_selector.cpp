#include "move_selector.hpp"
#include "board.hpp"
#include "move_generator.hpp"
#include "utility.hpp"

Move MoveSelector::select_random_move(Board const& board) 
{
	auto moves = MoveGenerator::generate_moves(board);
	return moves[Utility::get_random(0, moves.size()-1)];
}