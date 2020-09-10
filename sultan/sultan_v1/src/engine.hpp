#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <memory>

#include "game.hpp"

class Engine 
{
	static constexpr int VERSION = 1;
	static constexpr int SUBVERSION = 0;
	static constexpr std::string_view ENGINE_NAME = "Sultan";
	static constexpr std::string_view AUTHOR_NAME = "Murat Yirci";

	std::vector<std::pair<int8_t, std::string>> communication_history;
	std::string last_position_cmd;
	std::unique_ptr<Board> board;
	std::unique_ptr<Game> game;

	bool debug_mode;
	void print_header() const;
	bool execute_command(std::string const& engine_command);
	void execute_go_command(std::vector<std::string> const& tokens);
	void execute_perft_command(std::vector<std::string> const& tokens);
	void execute_show_command(std::vector<std::string> const& tokens);
	
	bool check_board() const;
	bool check_size(std::vector<std::string> const& tokens, size_t expected_size) const;
	bool check_min_size(std::vector<std::string> const& tokens, size_t expected_size) const;
public:
	Engine();
	void run();
};