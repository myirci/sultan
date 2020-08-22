#pragma once

#pragma once

#include "test_base.hpp"
#include <vector>

class Board;

namespace test
{
	class UtilityTests : public TestBase
	{
		UtilityTests() = delete;
		static bool check_piece_locations(Board const& b, int8_t piece, std::vector<int8_t> expected_loc);
	public:
		static void run_test();
		static bool test1();
		static bool test2();
		static bool test3();
		static bool test4();
		static bool test5();
		static bool test6();
	};
}