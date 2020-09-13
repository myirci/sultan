#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <memory>

#include "uci_game.hpp"

class UciEngine 
{
	static constexpr int VERSION = 1;
	static constexpr int SUBVERSION = 0;
	static constexpr std::string_view ENGINE_NAME = "Sultan";
	static constexpr std::string_view AUTHOR_NAME = "Murat Yirci";

	std::vector<std::pair<int8_t, std::string>> communication_history;

	std::unique_ptr<Board> board;
	std::unique_ptr<UciGame> game;
	std::string last_position_cmd;
	bool debug_mode;

	void print_header() const;

	bool check_board() const;
	bool check_tokens_size(std::vector<std::string> const& tokens, size_t expected_size) const;
	bool check_min_tokens_size(std::vector<std::string> const& tokens, size_t expected_size) const;

	void send_engine_to_gui_command(std::string const& cmd);
	inline void store_gui_to_engine_command(std::string const& cmd);

	bool execute_command(std::string const& engine_command);
	void execute_go_command(std::vector<std::string> const& tokens);
	void execute_perft_command(std::vector<std::string> const& tokens);
	void execute_show_command(std::vector<std::string> const& tokens);
	void reset(std::vector<std::string> const& tokens);
	
	
	
	
public:
	UciEngine();
	void run();
};