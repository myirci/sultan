#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <unordered_map>

namespace square
{
    inline constexpr uint8_t inside{ 0x88 }; 

    inline constexpr int8_t a1{0}, a2{16}, a3{32}, a4{48}, a5{64}, a6{80}, a7{96},  a8{112};
    inline constexpr int8_t b1{1}, b2{17}, b3{33}, b4{49}, b5{65}, b6{81}, b7{97},  b8{113};
    inline constexpr int8_t c1{2}, c2{18}, c3{34}, c4{50}, c5{66}, c6{82}, c7{98},  c8{114};
    inline constexpr int8_t d1{3}, d2{19}, d3{35}, d4{51}, d5{67}, d6{83}, d7{99},  d8{115};
    inline constexpr int8_t e1{4}, e2{20}, e3{36}, e4{52}, e5{68}, e6{84}, e7{100}, e8{116};
    inline constexpr int8_t f1{5}, f2{21}, f3{37}, f4{53}, f5{69}, f6{85}, f7{101}, f8{117};
    inline constexpr int8_t g1{6}, g2{22}, g3{38}, g4{54}, g5{70}, g6{86}, g7{102}, g8{118};
    inline constexpr int8_t h1{7}, h2{23}, h3{39}, h4{55}, h5{71}, h6{87}, h7{103}, h8{119};
    
    inline constexpr int8_t order[64]
    {
        a8, b8, c8, d8, e8, f8, g8, h8,
        a7, b7, c7, d7, e7, f7, g7, h7,
        a6, b6, c6, d6, e6, f6, g6, h6,
        a5, b5, c5, d5, e5, f5, g5, h5,
        a4, b4, c4, d4, e4, f4, g4, h4,
        a3, b3, c3, d3, e3, f3, g3, h3,
        a2, b2, c2, d2, e2, f2, g2, h2,
        a1, b1, c1, d1, e1, f1, g1, h1
    };
    
    constexpr int8_t twin(int8_t sq) { return sq + 8; }
    
    inline constexpr int8_t full_order[128]
    {
        a8, b8, c8, d8, e8, f8, g8, h8, twin(a8), twin(b8), twin(c8), twin(d8), twin(e8), twin(f8), twin(g8), twin(h8),
        a7, b7, c7, d7, e7, f7, g7, h7, twin(a7), twin(b7), twin(c7), twin(d7), twin(e7), twin(f7), twin(g7), twin(h7),
        a6, b6, c6, d6, e6, f6, g6, h6, twin(a6), twin(b6), twin(c6), twin(d6), twin(e6), twin(f6), twin(g6), twin(h6),
        a5, b5, c5, d5, e5, f5, g5, h5, twin(a5), twin(b5), twin(c5), twin(d5), twin(e5), twin(f5), twin(g5), twin(h5),
        a4, b4, c4, d4, e4, f4, g4, h4, twin(a4), twin(b4), twin(c4), twin(d4), twin(e4), twin(f4), twin(g4), twin(h4),
        a3, b3, c3, d3, e3, f3, g3, h3, twin(a3), twin(b3), twin(c3), twin(d3), twin(e3), twin(f3), twin(g3), twin(h3),
        a2, b2, c2, d2, e2, f2, g2, h2, twin(a2), twin(b2), twin(c2), twin(d2), twin(e2), twin(f2), twin(g2), twin(h2),
        a1, b1, c1, d1, e1, f1, g1, h1, twin(a1), twin(b1), twin(c1), twin(d1), twin(e1), twin(f1), twin(g1), twin(h1)
    };

    constexpr int8_t sq(int8_t rank, int8_t file) { return  rank * 16 + file;  }

    constexpr std::pair<int8_t, int8_t> rank_and_file(int8_t idx) { return std::make_pair(idx >> 4, idx & 0x07); }

    constexpr int8_t rank(int8_t idx) { return idx >> 4; }

    constexpr int8_t file(int8_t idx) { return idx & 0x07; }

    std::string square_to_string(int8_t sq);
}

