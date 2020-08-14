#include "square.hpp"

namespace Square
{

int8_t sq(int8_t rank, int8_t file)
{
    return  rank * 16 + file;
}

std::pair<int8_t, int8_t> rank_and_file(int8_t idx)
{
    return std::make_pair(idx >> 4, idx & 0x07);
}

int8_t rank(int8_t idx)
{
    return idx >> 4;
}

int8_t file(int8_t idx)
{
    return idx & 0x07;
}

std::string square_to_string(int8_t sq)
{
    auto [rank, file] = rank_and_file(sq);
    std::string str{"xx"};
    str[0] = static_cast<char>(file + 'a');
    str[1] = static_cast<char>(rank + '1');
    return str;
}

}
