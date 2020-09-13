#include "move_selector.hpp"
#include "board.hpp"
#include "move_generator.hpp"
#include "utility.hpp"

std::unique_ptr<Move> MoveSelector::select_random_move(Board const& board) 
{
	auto moves = MoveGenerator::generate_moves(board);
	return moves.empty() 
		? nullptr 
		: std::move(std::make_unique<Move>(moves[Utility::get_random(0, static_cast<int>(moves.size()-1))]));
}