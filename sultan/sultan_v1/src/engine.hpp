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
	std::unique_ptr<Game> game;

	bool debug_mode;
	void print_header() const;

public:
	Engine();
	void run();
};