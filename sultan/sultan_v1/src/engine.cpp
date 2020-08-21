#include "engine.hpp"
#include "fen.hpp"
#include "utility.hpp"
#include "move.hpp"
#include "move_generator.hpp"
#include "board.hpp"

#include <iostream>

Engine::Engine(const std::string& fen_str) : board{ std::make_unique<Board>() }, mg{ std::make_unique<MoveGenerator>(*board)}
{
	Fen f{fen_str};
	Utility::fen_to_board(f, *board);
}

std::uint64_t Engine::perft(int depth) 
{
	if (depth == 0) return 1;

	int64_t sum{0};
	auto moves = mg->generate_moves();
	for (auto it = moves.begin(); it != moves.end(); it++) 
	{
		board->make_move(*it);
		sum += perft(depth - 1);
		board->unmake_move(*it);
	}
	return sum;
}

std::uint64_t Engine::perft_divide(int depth)
{
	if (depth == 0) return 1;
	std::uint64_t total{ 0 }, sum{ 0 };
	auto moves = mg->generate_moves();
	for (auto it = moves.begin(); it != moves.end(); it++)
	{
		Utility::print_move(*it);
		board->make_move(*it);
		sum = perft(depth - 1);
		std::cout << "Number of moves: " << sum << std::endl;
		total += sum;
		board->unmake_move(*it);
	}
	return total;
}