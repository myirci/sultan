#include "test_base.hpp"

#include <iostream>

namespace test 
{
    void TestBase::run_test(std::string_view desc, const std::vector<bool (*)()>& tests)
    {
        auto str{ "" };
        int i{ 0 };
        std::cout << desc << " Tests : " << std::endl;
        for (auto tf : tests)
        {
            str = tf() ? " PASS" : " FAIL";
            std::cout << "\tTest-" << ++i << " " << str << std::endl;
        }
        std::cout << std::endl;
    }
}
