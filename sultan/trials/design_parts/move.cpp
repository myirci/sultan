#include "move.hpp"

Move::Move(std::uint8_t from, std::uint8_t to, Move_flag mflag)
{
    set_from(from);
    set_to(to);
    set_move_flag(mflag);
}

// from should be a valid board square: { [0-7], [16-23], [32-39], [48-55], [64-71], [80-87], [96-103], [112-119] }
void Move::set_from(std::uint8_t from)
{
    mv &= 0x03FF;
    mv |= (static_cast<std::uint16_t>(from) << 10);
}

// to should be a valid board square: { [0-7], [16-23], [32-39], [48-55], [64-71], [80-87], [96-103], [112-119] }
void Move::set_to(std::uint8_t to)
{
    mv &= 0xFC0F;
    mv |= (static_cast<std::uint16_t>(to) << 4);
}

void Move::set_move_flag(std::uint8_t mflag)
{
    mv &= 0xFFF0;
    mv |= static_cast<std::uint16_t>(mflag);
}

std::uint8_t Move::get_from() const
{
    return static_cast<std::uint8_t>((mv & 0xFC00) >> 10);
}

std::uint8_t Move::get_to() const
{
    return static_cast<std::uint8_t>((mv & 0x03F0) >> 4);
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
