#include "game.hpp"
#include "fen.hpp"
#include "utility.hpp"

Game::Game(const std::string& fen_str) : board{ std::make_unique<Board>() }, mg{ std::make_unique<MoveGenerator>(*board)}
{
	Fen f{fen_str};
	Utility::fen_to_board(f, *board);
}

void Game::make_move(Move const& mv) 
{
	move_stack.push(mv); 
	state_stack.push(board->make_move(mv));
	mg->compute_attacks();
}

void Game::unmake_move() 
{
	board->unmake_move(move_stack.top(), state_stack.top());
	mg->compute_attacks();
	move_stack.pop();
	state_stack.pop();
}