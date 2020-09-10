#pragma once

#include <string>
#include <vector>

namespace test
{
	class PerftTests
	{
		PerftTests() = delete;
		static bool perft(int id, std::string const& fen, std::vector<std::pair<int,uint64_t>> const& perft_results);
	public:
		static void run_test();
		static bool initial_pos_test();
	};
}