#include "perft.hpp"
#include "fen.hpp"
#include "utility.hpp"
#include "square.hpp"
#include "piece.hpp"

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
	for (auto const & mv : moves)
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
	if (depth == 0)
	{
		stats->num_nodes++;
		return;
	};

	auto moves = mg->generate_moves();
	
	for (auto const & mv : moves)
	{
		auto st = board->make_move(mv);
		mg->compute_attacks();
		perft_with_statistics(depth - 1);
		board->unmake_move(mv, st);
		mg->compute_attacks();
	}

	/*
	if (depth == 0) 
	{
		stats->num_nodes++;
		if (mg->generate_moves().size() == 0)
			stats->num_check_mates++;
		return;
	}
	
	auto moves = mg->generate_moves();
	
	for (auto const& mv : moves)
	{
		auto st = board->make_move(mv);
		mg->compute_attacks();
		if (depth == 1)
		{
			if (mv.is_capture())  stats->num_captures++;
			if (mv.is_promotion()) stats->num_promotions++;
			if (mv.is_castle()) stats->num_castles++;
			if (mv.is_en_passant_capture()) stats->num_en_passant_captures++;
			auto attack_info = mg->get_attack_info();
			int8_t stm = board->get_side_to_move();
			auto king_pos = mg->find_king_pos(stm);
			auto r = attack_info.equal_range(king_pos);
			auto num_checkers = std::distance(r.first, r.second);
			if (num_checkers > 0) stats->num_checks++;
			if (num_checkers == 2) stats->num_double_checks++;
		}
		perft_with_statistics(depth - 1);
		board->unmake_move(mv, st);
		mg->compute_attacks();
	}
	*/
}

void Perft::perft_debug(int depth)
{
	static bool flag{false};

	if (depth == 0)
	{
		stats->num_nodes++;
		return;
	};

	auto moves = mg->generate_moves();

	for (auto const&  mv : moves)
	{
		if (depth == 3)
		{
			flag = Utility::is_equal(mv, Move(square::d3, square::f3, MoveType::Quite, def::none));
		}

		if (flag && depth == 2 && Utility::is_equal(mv, Move(square::f1, square::b5, MoveType::Quite, def::none)))
		{
			std::cout << "HERE" << std::endl;
			Utility::print_board(*board);
		}

		auto st = board->make_move(mv);

		if (depth == 2 && flag && Utility::is_equal(mv, Move(square::f1, square::b5, MoveType::Quite, def::none)))
		{
			std::cout << "HERE" << std::endl;
			Utility::print_board(*board);
		}

		mg->compute_attacks();
		perft_debug(depth - 1);
		board->unmake_move(mv, st);
		mg->compute_attacks();
	}
}

void Perft::print_stats() 
{
	std::cout << "Number of nodes                   : " << stats->num_nodes << std::endl;
	std::cout << "Number of captures                : " << stats->num_captures << std::endl;
	std::cout << "Number of en-passsant captures    : " << stats->num_en_passant_captures << std::endl;
	std::cout << "Number of promotions              : " << stats->num_promotions << std::endl;
	std::cout << "Number of castles                 : " << stats->num_castles << std::endl;
	std::cout << "Number of checks                  : " << stats->num_checks << std::endl;
	std::cout << "Number of double checks           : " << stats->num_double_checks << std::endl;
	std::cout << "Number of checkmates              : " << stats->num_check_mates << std::endl;
	std::cout << "Number of dicovery checks         : " << stats->num_discovery_checks << std::endl;
}