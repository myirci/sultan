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
    inline constexpr int8_t knight_jumps[]{ 31, 33, 18, -14, -31, -33, -18, 14};

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

namespace state 
{
    struct BoardState
    {
        int8_t castling_rights;
        int8_t en_passant_loc;
        int16_t half_move_counter;
    };
}
