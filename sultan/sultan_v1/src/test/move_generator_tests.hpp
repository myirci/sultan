#pragma once

#include "test_base.hpp"
#include "../move.hpp"

#include <vector>


namespace test
{
    class MoveGeneratorTests : public TestBase
    {
        MoveGeneratorTests() = delete;

        static bool compare_moves(std::vector<Move> const& moves, std::vector<Move> const& expected_moves);

    public:
        static void run_test();
        static void debug_func();
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
        static bool test22();
        static bool test23();
        static bool test24();
        static bool test25();
        static bool test26();
        static bool test27();
        static bool test28();
        static bool test29();
        static bool test30();

    };
}