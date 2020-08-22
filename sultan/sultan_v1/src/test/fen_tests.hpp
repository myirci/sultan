#pragma once

#include "test_base.hpp"

namespace test 
{
    class FenTests : public TestBase
    {
        FenTests() = delete;

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