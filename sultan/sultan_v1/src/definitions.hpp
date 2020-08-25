#pragma once

#include <cstdint>
#include <unordered_map>

namespace def 
{
	inline constexpr int8_t none{ -128 }; // none, invalid
}

namespace color 
{
	inline constexpr int8_t white{ 1 };
	inline constexpr int8_t black{ -1 };
    inline constexpr char const* color_to_string(int8_t clr) { return clr == white ? "white" : "black"; }
}

namespace direction
{
    inline constexpr int8_t N{ 16 }, S{ -16 }, E{ 1 }, W{ -1 }, NE{ 17 }, SW{ -17 }, NW{ 15 }, SE{ -15 }, ND{ 0 };
    inline constexpr int8_t all_dirs[]{ N, E, S, W, NE, SE, SW, NW };
    inline constexpr int8_t flat_dirs[]{ N, E, S, W };
    inline constexpr int8_t diagonal_dirs[]{ NE, SE, SW, NW };
    inline constexpr int8_t knight_jumps[]{ 33, 18, -14, -31, -33, -18, 14, 31 };

    inline const std::unordered_map<int8_t, char const*> direction_to_chararray =
    {
        { N, "N" }, 
        { S, "S" }, 
        { E, "E" }, 
        { W, "W" },
        { NE, "NE" }, 
        { SW, "SW" }, 
        { NW, "NW" }, 
        { SE, "SE" },
        { ND, "ND" }
    };
}

namespace perft
{
    struct perft_stats
    {
        perft_stats() :
            num_nodes{ 0 }, num_captures{ 0 }, num_promotions{ 0 },
            num_en_passant_captures{ 0 }, num_castles{ 0 }, num_checks{ 0 },
            num_double_checks{ 0 }, num_discovery_checks{ 0 }, num_check_mates{ 0 } {}

        uint64_t num_nodes;                 
        uint64_t num_captures;              
        uint64_t num_promotions;            
        uint64_t num_en_passant_captures;   
        uint64_t num_castles;               
        uint64_t num_checks;                
        uint64_t num_double_checks;         
        uint64_t num_discovery_checks;      // needs the information of last moved piece    
        uint64_t num_check_mates;           
    };
}

namespace attack 
{
    struct AttackInfo 
    {
        int8_t attacker_loc;
        int8_t attack_dir;
        int8_t target_loc;
    };
}
