#include "perft.hpp"
#include "board.hpp"

#include "fen.hpp"
#include "utility.hpp"
#include "square.hpp"
#include "piece.hpp"
#include "move_generator.hpp"

#include <iostream>

Perft::Perft(Board* brd) : board{ brd } { }

uint64_t Perft::perft(int depth)
{
	if (depth == 1)
		return MoveGenerator::generate_moves(*board).size();

	uint64_t sum{ 0 };
	auto moves = MoveGenerator::generate_moves(*board);
	for (auto const & mv : moves)
	{
		auto st = board->make_move(mv);
		sum += perft(depth - 1);
		board->unmake_move(mv, st);
	}
	return sum;
}

uint64_t Perft::perft_divide(int depth)
{
	if (depth == 1)
		return MoveGenerator::generate_moves(*board).size();

	uint64_t total{ 0 }, sum{ 0 };
	auto moves = MoveGenerator::generate_moves(*board);
	for (auto const& mv : moves)
	{
		auto st = board->make_move(mv);
		sum = perft(depth - 1);
		std::cout << mv << " : " << sum << std::endl;
		total += sum;
		board->unmake_move(mv, st);
	}

	std::cout << "Moves: " << moves.size() << std::endl;
	std::cout << "Nodes: " << total << std::endl;
	return total;
}

void Perft::perft_with_statistics(int depth) 
{
	auto stats = std::make_unique<PerftStats>();
	perft_with_statistics_implementation(depth, stats);
	print_stats(stats);
}

void Perft::perft_with_statistics_implementation(int depth, std::unique_ptr<PerftStats>& stats)
{
	auto moves = MoveGenerator::generate_moves(*board);

	if (depth == 1) 
	{
		stats->num_nodes += moves.size();
		for (auto const& mv : moves)
		{
			if (mv.is_capture()) stats->num_captures++;
			if (mv.is_promotion()) stats->num_promotions++;
			if (mv.is_castle()) stats->num_castles++;
			if (mv.is_en_passant_capture()) stats->num_en_passant_captures++;

			auto st = board->make_move(mv);

			int8_t stm = board->get_side_to_move();
			auto king_pos = board->find_piece_location(stm * piece::King);
			auto checkers = board->get_checker_positions(king_pos);
			auto num_checkers {0};
			if (checkers.first != def::none) num_checkers++;
			if (checkers.second != def::none) num_checkers++;


			auto num_children = MoveGenerator::generate_moves(*board).size();
			if (num_checkers > 0) 
			{
				stats->num_checks++;
				if (num_children == 0) stats->num_check_mates++;
			    if (num_checkers == 1 && mv.get_to() != checkers.first) stats->num_discovery_checks++;
				else stats->num_double_checks++;
			}
			else 
			{
				if (num_children == 0)  stats->num_stale_mates++;
			}
			board->unmake_move(mv, st);
		}
		return;
	}
	else 
	{
		for (auto const& mv : moves)
		{
			auto st = board->make_move(mv);
			perft_with_statistics_implementation(depth - 1, stats);
			board->unmake_move(mv, st);
		}
	}
}

void Perft::print_stats(std::unique_ptr<PerftStats> const& stats) 
{
	std::cout << "Number of nodes                   : " << stats->num_nodes << std::endl;
	std::cout << "Number of captures                : " << stats->num_captures << std::endl;
	std::cout << "Number of en-passsant captures    : " << stats->num_en_passant_captures << std::endl;
	std::cout << "Number of promotions              : " << stats->num_promotions << std::endl;
	std::cout << "Number of castles                 : " << stats->num_castles << std::endl;
	std::cout << "Number of checks                  : " << stats->num_checks << std::endl;
	std::cout << "Number of discovery checks        : " << stats->num_discovery_checks << std::endl;
	std::cout << "Number of double checks           : " << stats->num_double_checks << std::endl;
	std::cout << "Number of check mates             : " << stats->num_check_mates << std::endl;
	std::cout << "Number of stale mates             : " << stats->num_stale_mates << std::endl;
}