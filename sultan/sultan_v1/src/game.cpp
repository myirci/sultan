#include "game.hpp"
#include "board.hpp"
#include "utility.hpp"
#include "move_selector.hpp"

Game::Game(Board* brd) : board{ brd } { }
 
void Game::make_move(std::string_view mv_str) { make_move(Utility::to_move(mv_str, *board)); }

void Game::make_move(Move const& mv) 
{
	move_stack.push(mv); 
	state_stack.push(board->make_move(mv));
}

void Game::unmake_move() 
{
	board->unmake_move(move_stack.top(), state_stack.top());
	move_stack.pop();
	state_stack.pop();
}

std::string Game::make_move(uci::SearchParameters const& param)
{
	auto mv = MoveSelector::select_random_move(*board);
	make_move(mv);
	return Utility::to_string(mv);
}