#pragma once

#include "test_base.hpp"

namespace test
{
	class PerftTests : public TestBase
	{
		PerftTests() = delete;

	public:
		static void run_test();
		static void debug_func();
		static bool perft(std::string const& fen, std::vector<uint64_t> const& perft_results);
		static bool initial_pos_test();
		static bool batch_test(std::string const& fen_positions_file_path);
	};
}