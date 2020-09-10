#pragma once

#include <memory>
#include <string_view>

#include "board.hpp"
#include "game.hpp"
#include "perft.hpp"
#include "analyzer.hpp"

class Factory 
{
	Factory() = delete;

public:

	static std::unique_ptr<Board> CreateBoard(std::string_view fen_str);
	static std::unique_ptr<Board> CloneBoard(Board const& board);

	static std::unique_ptr<Game> CreateGame(Board * brd);

	static std::unique_ptr<Perft> CreatePerft(Board* brd);
	
	static std::unique_ptr<Analyzer> CreateAnalyzer(Board* brd);
};