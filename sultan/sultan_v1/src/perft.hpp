#pragma once

#include <cstdint>
#include <string>
#include <memory>

#include "move_generator.hpp"
#include "board.hpp"

class Perft
{
public:
    
    struct PerftStats
    {
        PerftStats() :
            num_nodes{ 0 }, num_captures{ 0 }, num_promotions{ 0 },
            num_en_passant_captures{ 0 }, num_castles{ 0 }, num_checks{ 0 },
            num_double_checks{ 0 }, num_discovery_checks{ 0 }, num_check_mates{ 0 } { }

        uint64_t num_nodes;
        uint64_t num_captures;
        uint64_t num_promotions;
        uint64_t num_en_passant_captures;
        uint64_t num_castles;
        uint64_t num_checks;
        uint64_t num_double_checks;
        uint64_t num_discovery_checks;
        uint64_t num_check_mates;
    };

    Perft(std::string const& fen_string);
    uint64_t perft(int depth);
    uint64_t perft_divide(int depth);
    void perft_with_statistics(int depth);
    void perft_debug(int depth);
    void print_stats();

private:

    std::unique_ptr<Board> board;
    std::unique_ptr<MoveGenerator> mg;
    std::unique_ptr<PerftStats> stats;
};

