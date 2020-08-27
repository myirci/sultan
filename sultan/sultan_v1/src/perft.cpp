#include "perft.hpp"
#include "fen.hpp"
#include "utility.hpp"

#include <iostream>

Perft::Perft(const std::string& fen_str) : 
	board{ std::make_unique<Board>() }, 
	mg{ std::make_unique<MoveGenerator>(*board) },
	stats{ std::make_unique<PerftStats>() }
{
	Fen f{ fen_str };
	Utility::fen_to_board(f, *board);
	mg->compute_attacks();
}

uint64_t Perft::perft(int depth)
{
	if (depth == 1) return mg->generate_moves().size();

	uint64_t sum{ 0 };
	auto moves = mg->generate_moves();
	for (auto & const mv : moves)
	{
		auto st = board->make_move(mv);
		mg->compute_attacks();

		sum += perft(depth - 1);
		board->unmake_move(mv, st);
		mg->compute_attacks();
	}
	return sum;
}

uint64_t Perft::perft_divide(int depth)
{
	if (depth == 1) return mg->generate_moves().size();

	uint64_t total{ 0 }, sum{ 0 };
	auto moves = mg->generate_moves();
	for (auto const& mv : moves)
	{
		auto st = board->make_move(mv);
		mg->compute_attacks();
		sum = perft(depth - 1);
		std::cout << mv << " : " << sum << std::endl;
		total += sum;
		board->unmake_move(mv, st);
		mg->compute_attacks();
	}

	std::cout << "Moves: " << moves.size() << std::endl;
	std::cout << "Nodes: " << total << std::endl;
	return total;
}

void Perft::perft_with_statistics(int depth)
{
	auto moves = mg->generate_moves();
	if (moves.empty()) stats->num_check_mates++;

	for (auto const& mv : moves)
	{
		if (mv.is_capture())  stats->num_captures++;
		if (mv.is_promotion()) stats->num_promotions++;
		if (mv.is_castle()) stats->num_castles++;
		if (mv.is_en_passant_capture()) stats->num_en_passant_captures++;
	}

	if (depth == 1)
	{
		stats->num_nodes += moves.size();
		return;
	}

	for (auto const& mv : moves)
	{
		auto st = board->make_move(mv);
		mg->compute_attacks();

		perft_with_statistics(depth - 1);
		board->unmake_move(mv, st);
		mg->compute_attacks();
	}
}