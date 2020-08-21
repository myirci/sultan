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
}

namespace direction
{
    inline constexpr int8_t N{ 16 }, S{ -16 }, E{ 1 }, W{ -1 }, NE{ 17 }, SW{ -17 }, NW{ 15 }, SE{ -15 }, ND{ 0 };
    inline constexpr int8_t all_dirs[]{ N, S, E, W, NE, SW, NW, SE };
    inline constexpr int8_t flat_dirs[]{ N, E, S, W };
    inline constexpr int8_t diagonal_dirs[]{ NE, SE, SW, NW };
    inline constexpr int8_t knight_jumps[]{ 33, 18, -14, -31, -33, -18, 14, 31 };

    inline const std::unordered_map<int8_t, char const*> direction_to_chararray =
    {
        { N, "North" }, 
        { S, "South" }, 
        { E, "East" }, 
        { W, "West" },
        { NE, "North-East" }, 
        { SW, "South-West" }, 
        { NW, "North-West" }, 
        { SE, "South-East" },
        { ND, "No Direction" }
    };
}