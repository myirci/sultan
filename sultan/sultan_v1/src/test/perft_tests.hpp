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
		static bool test1();
		static bool test2();
		// static bool test3();
		// static bool test4();
		// static bool test5();
		// static bool test6();
	};
}