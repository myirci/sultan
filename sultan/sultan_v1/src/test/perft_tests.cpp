#include "perft_tests.hpp"
#include "../perft.hpp"
#include "../utility.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

namespace test 
{
    void PerftTests::run_test()
    {
        std::vector<bool (*)()> tests{ initial_pos_test };
        TestBase::run_test("Perft", tests);
    }

    bool PerftTests::perft(std::string const& fen, std::vector<uint64_t> const& perft_results) 
    {
        std::cout << "Position: " << fen << std::endl;
        for (size_t i = 0; i < perft_results.size(); i++)
        {
            Perft p(fen);
            auto res = p.perft(i + 1);
            if (res == perft_results[i])
            {
                std::cout << "\tPeft-" << i + 1 << " computed correctly: " << res << std::endl;
            }
            else
            {
                std::cout << "\tPerft error! expected: " << perft_results[i] << ", got: " << res << std::endl;
                return false;
            }
        }
        return true;
    }

    void PerftTests::debug_func() 
    {
        // Perft p("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
        //p.perft_divide(2);

        // Perft p("rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR b KQkq d3 0 1");
        // p.perft_divide(6);

        // Perft p("r1bqkbnr/pppppppp/2n5/8/3P4/8/PPP1PPPP/RNBQKBNR w KQkq - 0 1");
        // p.perft_divide(5);

        // Perft p("r1bqkbnr/pppppppp/2n5/8/3P4/3Q4/PPP1PPPP/RNB1KBNR b KQkq - 0 1");
        // p.perft_divide(4);

        // Perft p("1rbqkbnr/pppppppp/2n5/8/3P4/3Q4/PPP1PPPP/RNB1KBNR w KQk - 0 1");
        // p.perft_divide(3);

        // Perft p("1rbqkbnr/pppppppp/2n5/8/3P4/3Q4/PPPBPPPP/RN2KBNR b KQk - 0 1");
        // p.perft_divide(2);

        Utility::generate_and_print_moves("8/2p5/3p4/KP5r/1R3pPk/8/4P3/8 b - g3 0 1");

    }

    bool PerftTests::initial_pos_test()
    {
        std::vector<uint64_t> perft_results{ 20, 400, 8902, 197281, 4865609, 119060324, 3195901860 };
        return perft("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", perft_results);
    }

    bool PerftTests::batch_test(std::string const& fen_positions_file_path)
    {
        auto file = std::ifstream(fen_positions_file_path, std::ios::out);
        if (!file.is_open()) 
        {
            throw std::exception("File could not be opened!");
        }

        std::string line, token, fen;
        while (std::getline(file, line)) 
        {
            std::vector<uint64_t> perft_results;
            std::stringstream ss{ line };
            int i{ 0 };
            while (std::getline(ss, token,';')) 
            {
                if (i++ == 0) 
                {
                    fen = token;
                    continue;
                }
                perft_results.push_back(std::stoull(token));
            }

            if (!perft(fen, perft_results))
                return false;
        }
        return true;
    }
}
