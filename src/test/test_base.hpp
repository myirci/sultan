#pragma once

#include <string_view>
#include <vector>

namespace test
{
    class TestBase
    {
        TestBase() = delete;

    protected:
        static void run_test(std::string_view, const std::vector<bool (*)()>&);
    };
}