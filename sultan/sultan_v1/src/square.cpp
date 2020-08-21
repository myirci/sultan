#include "square.hpp"

namespace Square
{


std::string square_to_string(int8_t sq)
{
    auto [rank, file] = rank_and_file(sq);
    std::string str{"xx"};
    str[0] = static_cast<char>(file + 'a');
    str[1] = static_cast<char>(rank + '1');
    return str;
}

}
