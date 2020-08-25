#include "engine.hpp"
#include "fen.hpp"
#include "utility.hpp"
#include <iostream>

Engine::Engine(const std::string& fen_str) : board{ std::make_unique<Board>() }, mg{ std::make_unique<MoveGenerator>(*board)}
{
	Fen f{fen_str};
	Utility::fen_to_board(f, *board);
}

uint64_t Engine::perft(int depth) 
{
	/*
	if (depth == 1)
		return mg->generate_moves().size();

	uint64_t sum{ 0 };
	auto moves = mg->generate_moves();
	for (auto it = moves.begin(); it != moves.end(); it++) 
	{
		Board::KeepBeforeMakeMove keep{ board->get_castling_rights(), board->get_en_passant_loc(), board->get_half_move_counter() };
		board->make_move(*it);
		sum += perft(depth - 1);
		board->unmake_move(*it, keep);
	}
	return sum;
	*/
	return 0;
}

uint64_t Engine::perft_divide(int depth)
{
	/*
	if (depth == 1)
		return mg->generate_moves().size();

	uint64_t total{ 0 }, sum{ 0 };
	auto moves = mg->generate_moves();
	for (auto it = moves.begin(); it != moves.end(); it++)
	{
		Board::KeepBeforeMakeMove keep{ board->get_castling_rights(), board->get_en_passant_loc(), board->get_half_move_counter() };
		board->make_move(*it);
		sum = perft(depth - 1);
		std::cout << *it << " : " << sum << std::endl;
		total += sum;
		board->unmake_move(*it, keep);
	}
	std::cout << "Moves: " << moves.size() << std::endl;
	std::cout << "Nodes: " << total << std::endl;
	return total;
	*/
	return 0;
}

void Engine::perft_with_statistics(int depth, perft::perft_stats& stats)
{
	/*
	auto moves = mg->generate_moves(stats);
	if (moves.empty()) stats.num_check_mates++;

	for (auto it = moves.begin(); it != moves.end(); it++) 
	{
		if (it->is_capture())  stats.num_captures++;
		if (it->is_promotion()) stats.num_promotions++;
		if (it->is_castle()) stats.num_castles++;
		if (it->is_en_passant_capture()) stats.num_en_passant_captures++;
	}

	if (depth == 1) 
	{
		stats.num_nodes += moves.size();
		return;
	}
		
	for (auto it = moves.begin(); it != moves.end(); it++)
	{
		Board::KeepBeforeMakeMove keep{ board->get_castling_rights(), board->get_en_passant_loc(), board->get_half_move_counter() };
		board->make_move(*it);
		perft_with_statistics(depth - 1, stats);
		board->unmake_move(*it, keep);
	}
	*/
}