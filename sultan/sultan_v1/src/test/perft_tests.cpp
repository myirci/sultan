#include "perft_tests.hpp"
#include "../perft.hpp"
#include "../utility.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>

namespace test 
{
    void PerftTests::run_test()
    {
        std::vector<std::string> epd_files
        {
            // R"(C:\Users\myirci\Documents\dev\bitbucket\chess\sultan\sultan_v1\src\test\data\perft_positions1.epd)",
            // R"(C:\Users\myirci\Documents\dev\bitbucket\chess\sultan\sultan_v1\src\test\data\perft_positions2.epd)",
            // R"(C:\Users\myirci\Documents\dev\bitbucket\chess\sultan\sultan_v1\src\test\data\perft_positions3.epd)",
            // R"(C:\Users\myirci\Documents\dev\bitbucket\chess\sultan\sultan_v1\src\test\data\perft_positions4.epd)",
            // R"(C:\Users\myirci\Documents\dev\bitbucket\chess\sultan\sultan_v1\src\test\data\perft_positions5.epd)",
            R"(C:\Users\myirci\Documents\dev\bitbucket\chess\sultan\sultan_v1\src\test\data\perft_positions6.epd)",
            // R"(C:\Users\myirci\Documents\dev\bitbucket\chess\sultan\sultan_v1\src\test\data\perft_positions9.epd)",
            // R"(C:\Users\myirci\Documents\dev\bitbucket\chess\sultan\sultan_v1\src\test\data\perft_positions10.epd)",
            // R"(C:\Users\myirci\Documents\dev\bitbucket\chess\sultan\sultan_v1\src\test\data\perft_positions11.epd)",
            // R"(C:\Users\myirci\Documents\dev\bitbucket\chess\sultan\sultan_v1\src\test\data\perft_positions12.epd)",
        };

        for (auto it = epd_files.begin(); it != epd_files.end(); it++) 
        {
            int id{ 1 };
            auto file = std::ifstream(*it, std::ios::out);
            if (!file.is_open())
            {
                std::string err_msg = "File: " + *it + " could not be opened!";
                throw std::runtime_error(err_msg);
            }

            std::string line, token, fen;
            while (std::getline(file, line))
            {
                std::vector<std::pair<int, uint64_t>> perft_results;
                std::stringstream ss{ line };
                int i{ 0 };
                while (std::getline(ss, token, ';'))
                {
                    Utility::trim(token);
                    if (i++ == 0)
                    {
                        fen = token;
                        continue;
                    }
                    
                    auto pos = token.find_first_of(' ');
                    auto depth = std::stoi((token.substr(0, pos)).substr(1));
                    auto perft_val = std::stoull(token.substr(pos));
                    perft_results.push_back(std::make_pair(depth, perft_val));
                }

                if (!perft(id++, fen, perft_results))
                    return;
            }
            file.close();
        }
    }

    bool PerftTests::perft(int id, std::string const& fen, std::vector<std::pair<int, uint64_t>> const& perft_results)
    {
        std::cout << "Position - " << id << " : " << fen << std::endl;
        for (auto it = perft_results.begin(); it != perft_results.end(); it++) 
        {
            Perft p(fen);
            auto res = p.perft(it->first);
            if (res == it->second)
            {
                std::cout << "\tPeft-" << it->first << " computed correctly: " << res << std::endl;
            }
            else
            {
                std::cout << "\tPerft error! expected: " << it->second << ", got: " << res << std::endl;
                return false;
            }
        }
        return true;
    }

    bool PerftTests::initial_pos_test()
    {
        std::vector<std::pair<int,uint64_t>> perft_results
        { 
            std::make_pair(1,20), 
            std::make_pair(2,400),
            std::make_pair(3,8902),
            std::make_pair(4,197281),
            std::make_pair(5,4865609),
            std::make_pair(6,119060324),
            std::make_pair(7,3195901860)
        };
        return perft(1, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", perft_results);
    }

    void PerftTests::debug_func()
    {
        Perft p("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
        p.perft_with_statistics(5);
        p.print_stats();

        // Perft p("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
        // p.perft_divide(6);

        // Perft p("8/2p5/3p4/KP5r/5p1k/8/4P1P1/1R6 b - - 0 1");
        // p.perft_divide(5);

        // Perft p("r1bqk2r/pp1p1ppp/5n2/n1pP4/Q7/2PB1N2/P4PPP/R1B1K2R w KQkq c6 0 1");
        // std::cout << p.perft(4) << std::endl;

        // Perft p("8/2p5/3p4/KP5r/5pk1/8/4P1P1/6R1 b - - 0 1");
        // p.perft_divide(3);

        // Perft p("8/2p5/3p4/KP4kr/5p2/8/4P1P1/6R1 w - - 0 1");
        // p.perft_divide(2);

        // Utility::generate_and_print_moves("6k1/r2n3p/2p1p1p1/p1PpPp2/3P4/P2B2P1/5P1P/1R4K1 w - f6 0 1");

    }
}
