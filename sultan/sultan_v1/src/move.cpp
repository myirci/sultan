#include "move.hpp"
#include "square.hpp"

Move::Move(std::int8_t from, std::int8_t to, Move_flag mflag)
{
    set_from(from);
    set_to(to);
    set_move_flag(mflag);
}

// from should be a valid board square: { [0-7], [16-23], [32-39], [48-55], [64-71], [80-87], [96-103], [112-119] }
void Move::set_from(std::int8_t from)
{
    // clear the previous value
    mv &= 0x03FF; 
    
    // map to the range 0-63 and update the value 
    auto [r, f] = square::rank_and_file(from);

    mv |= (static_cast<std::int16_t>(r * 8 + f) << 10);
}

// to should be a valid board square: { [0-7], [16-23], [32-39], [48-55], [64-71], [80-87], [96-103], [112-119] }
void Move::set_to(std::int8_t to)
{
    // clear the previous value
    mv &= 0xFC0F;

    // map to the range 0-63 and update the value 
    auto [r, f] = square::rank_and_file(to);
    mv |= (static_cast<std::int16_t>(r * 8 + f) << 4);
}

void Move::set_move_flag(Move_flag mflag)
{
    mv &= 0xFFF0;
    mv |= static_cast<std::int16_t>(mflag);
}

std::int8_t Move::get_from() const
{
    int8_t idx = static_cast<std::int8_t>((mv & 0xFC00) >> 10);
    return static_cast<std::int8_t>( (idx/8) * 16 + (idx % 8));
}

std::int8_t Move::get_to() const
{
    int8_t idx = static_cast<std::int8_t>((mv & 0x03F0) >> 4);
    return static_cast<std::int8_t>( (idx/8) * 16 + (idx % 8));
}

Move_flag Move::get_move_flag() const
{
    return static_cast<Move_flag>(mv & 0x000F);
}

bool Move::is_promotion() const
{
	return (mv & 0x0008) > 0;
}

bool Move::is_capture() const
{
	return (mv & 0x0004) > 0;
}
