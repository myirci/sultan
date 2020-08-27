#pragma once

#include "test_base.hpp"

namespace test 
{
	class BoardTests : public TestBase 
	{
		BoardTests() = delete;

	public:
		static void run_test();
		static bool compare_fen(std::string const& fen, std::string const & expected_fen, int id);
		static bool test1();
		static bool test2();
		static bool test3();
		static bool test4();
		static bool test5();
		static bool test6();
		static bool test7();
		static bool test8();
		static bool test9();
		static bool test10();
		static bool test11();
		static bool test12();
		static bool test13();
		static bool test14();
		static bool test15();
		static bool test16();
		static bool test17();
		static bool test18();
		static bool test19();
		static bool test20();
		static bool test21();
	};
}