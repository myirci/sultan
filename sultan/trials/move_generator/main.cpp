#include <iostream>
#include <sstream>
#include <vector>
#include <array>
#include <algorithm>
#include <random>
#include <stack>

#include "Definitions.hpp"
#include "Board.hpp"
#include "Move.hpp"

void make_move_test();
void move_gen_test();
std::uint64_t perft(std::uint8_t depth, Board& b);
void perft_test();
void auto_perft_test(int d);
void uci();
void tokenize(const std::string& cmd, std::vector<std::string>& tokens);
bool execute_command(std::vector<std::string>& tokens);

extern std::string square_to_string[65];
Board board;

int main(void) {

	make_move_test();
	// move_gen_test();
	// uci();
	// perft_test();
	// auto_perft_test(5);
    return 0;
}

void make_move_test() {

	Board b;
	Move m1(a2, a4, Double_Pawn_Push);
	Move m2(b8, c6, Quite);
	b.make_move(m1);
	b.make_move(m2);
	
	b.unmake_move(m2);
	b.unmake_move(m1);

	b.print_board(true);
}

void auto_perft_test(int d) {

	Board b;
	b.set_to_starting_position();
	std::uint64_t perft_results[7] = {
		1, 20, 400, 8902, 197281, 4865609, 119060324
	};

	int i = d+1;
	if (i > 7) i = 7;
	for (int j = 0; j < i; ++j) {
		auto p = perft(j, b);
		std::cout << "perft-" << j << " " << p << " " << " known val: " << perft_results[j] << std::endl;
	}

}

void move_gen_test() {

	std::vector<Move> moves;
	Board b;
	b.import_from_FEN("8/7p/5kR1/8/8/8/7K/8 b - - 2 3");
	b.print_board(true);
	b.generate_moves(moves);
	std::cout << moves.size() << std::endl;
	
	for (uint16_t i = 0; i < moves.size(); ++i)
		moves[i].print();
}

void perft_test() {

	Board b;
	// b.set_to_starting_position();
	b.import_from_FEN("r1bqkbnr/pppppppp/2n5/8/P7/8/1PPPPPPP/RNBQKBNR w KQkq - 0 1");
	// std::cout << perft(5, b) << std::endl;

	std::vector<Move> moves;
	b.generate_moves(moves);
	std::uint64_t total{ 0 };
	for (auto it = moves.begin(); it != moves.end(); ++it) {
		b.make_move(*it);
		it->print();
		std::uint64_t p = perft(2, b);
		std::cout << "perft-moves: "<< p << std::endl;
		total += p;
		b.unmake_move(*it);
	}
	std::cout << "total: " << total << std::endl;

}

std::uint64_t perft(std::uint8_t depth, Board& b) {

	static std::stack<Square> en_passant_squares;
	static std::stack<std::uint8_t> castling_rights;
	static std::stack<Piece> captured_pieces;
	
	if (depth == 0) {
		return 1;
	}

	std::uint64_t nodes{ 0 };
	std::vector<Move> moves;
	b.generate_moves(moves);
	for (uint16_t i = 0; i < moves.size(); ++i) {
		en_passant_squares.push(b.get_en_passant_square());
		castling_rights.push(b.get_castling_rights());
		captured_pieces.push(b.get_captured());
		b.make_move(moves[i]);
		nodes += perft(depth - 1, b);
		b.unmake_move(moves[i]);
		b.set_en_passant_square(en_passant_squares.top());
		en_passant_squares.pop();
		b.set_captured(captured_pieces.top());
		captured_pieces.pop();
		b.set_castling_rights(castling_rights.top());
		castling_rights.pop();
	}
	return nodes;
}

void tokenize(const std::string& cmd, std::vector<std::string>& tokens) {
	std::stringstream ss{ cmd };
	std::string token{ "" };
	while (ss >> token)
		tokens.push_back(token);
}

void uci() {

	std::cout << "Sultan 1.0, by Murat Yirci" << std::endl;

	// 1) wait until uci command is received
	std::string cmd{ "" };
	std::vector<std::string> tokens;
	do {
		std::getline(std::cin, cmd, '\n');	// note that getline is a blocking function
		tokenize(cmd, tokens);
	} while (none_of(tokens.begin(), tokens.end(), [](std::string& s) {return s == "uci"; }));

	// 2) identify the engine
	std::cout << "id name Sultan 1.0" << std::endl;
	std::cout << "id author Murat Yirci" << std::endl;

	// 3) send the options of the engine
	std::cout << "option name Hash type spin default 16 min 1 max 1024" << std::endl;
	std::cout << "option name Ponder type check default false" << std::endl;
	std::cout << "option name MultiPV type spin default 1 min 1 max 20" << std::endl;
	std::cout << "option name Clear Hash type button" << std::endl;

	// 4) send uciok
	std::cout << "uciok" << std::endl;

	// 5) wait for the next commands
	do {
		std::getline(std::cin, cmd, '\n');
		tokens.clear();
		tokenize(cmd, tokens);
	} while (execute_command(tokens));

	std::cout << "Sultan 1.0 is exiting." << std::endl;
}

bool execute_command(std::vector<std::string>& tokens) {

	static bool newgame{ false };

	if (tokens[0] == "quit") {
		return false;
	}
	else if (tokens[0] == "isready") {
		std::cout << "readyok" << std::endl;
	}
	else if (tokens[0] == "ucinewgame") {
		newgame = true;
	}
	else if (tokens[0] == "stop") {
	}
	else if (tokens[0] == "ponderhint") {
	}
	else if (tokens[0] == "position") {
		board.make_moves(tokens);
	}
	else if (tokens[0] == "go") {
		std::vector<Move> moves;
		board.generate_moves(moves);
		std::cout << "Debug: # of moves " << moves.size() << std::endl;
		std::random_device r;
		std::default_random_engine e1(r());
		std::uniform_int_distribution<int> uniform_dist(0, moves.size() - 1);
		int move_num = uniform_dist(e1);
		
		board.make_move(moves[move_num]);
		board.print_board(true);
		std::string bestmove = square_to_string[moves[move_num].get_from()] + square_to_string[moves[move_num].get_to()];
		std::cout << "bestmove " << bestmove << std::endl;
		
	}
	else if (tokens[0] == "register") {
	}
	else if (tokens[0] == "setoption") {
	}
	else if (tokens[0] == "debug") {
	}
	return true;
}