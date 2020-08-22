#pragma once

#include "test_base.hpp"

namespace test 
{
	class PieceTests : public TestBase 
	{
		PieceTests() = delete;
	
	public:
		static void run_test();
		static bool test1();
	};
}