#include "perft_tests.hpp"
#include "../perft.hpp"

#include <iostream>

namespace test 
{
    void PerftTests::run_test()
    {
        std::vector<bool (*)()> tests{ test1 };
        TestBase::run_test("Perft", tests);
    }

    void PerftTests::debug_func() 
    {
        Perft p("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        // std::cout << p.perft(1) << std::endl;

        /*
        perft::perft_stats stats;

        Engine e("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        e.perft_with_statistics(5, stats);

        std::cout << "Number of nodes                   : " << stats.num_nodes << std::endl;
        std::cout << "Number of captures                : " << stats.num_captures << std::endl;
        std::cout << "Number of en-passsant captures    : " << stats.num_en_passant_captures << std::endl;
        std::cout << "Number of promotions              : " << stats.num_promotions << std::endl;
        std::cout << "Number of castles                 : " << stats.num_castles << std::endl;
        std::cout << "Number of checks                  : " << stats.num_checks << std::endl;
        std::cout << "Number of double checks           : " << stats.num_double_checks << std::endl;
        std::cout << "Number of checkmates              : " << stats.num_check_mates << std::endl;

        // Engine e("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        // auto res = e.perft_divide(6);

        // Engine e("rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR b KQkq a3 0 1");   // a2-a4
        // auto res = e.perft_divide(5);

        // Engine e("r1bqkbnr/pppppppp/2n5/8/P7/8/1PPPPPPP/RNBQKBNR w KQkq - 0 1"); // 
        // auto res = e.perft_divide(4);

        // Engine e("r1bqkbnr/pppppppp/2n5/8/P7/1P6/2PPPPPP/RNBQKBNR b KQkq - 0 1");
        // auto res = e.perft_divide(3);
        */
    }

    bool PerftTests::test1()
    {
        /*
        std::vector<int64_t> perft_results{ 20, 400, 8902, 197281, 4865609, 119060324 };
        for (size_t i = 0; i < perft_results.size(); i++) 
        {
            Engine e("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
            auto res = e.perft(i+1);
            if (res == perft_results[i])
            {
                std::cout << "\tPeft-" << i+1 << " computed correctly: " << res << std::endl;
            }
            else 
            {
                std::cout << "Perft error! expected: " << perft_results[i] << ", got: " << res << std::endl;
                return false;
            }
        }
        */
        return true;
    }

    bool PerftTests::test2() 
    {
       
        return false;
    }
}
