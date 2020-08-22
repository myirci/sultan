#include "move.hpp"
#include "square.hpp"

Move::Move(std::int8_t loc_from, std::int8_t loc_to, MoveType move_type, std::int8_t captured_piece): from{ loc_from }, to{ loc_to }, mtype{ move_type }, captured{ captured_piece } { }

std::int8_t Move::get_from() const { return from; }

std::int8_t Move::get_to() const { return to; }

MoveType Move::get_move_type() const { return mtype; }

std::int8_t Move::get_captured_piece() const { return captured; }

bool Move::is_promotion() const { return (static_cast<std::int8_t>(mtype) & 0x08) > 0; }

bool Move::is_capture() const { return (static_cast<std::int8_t>(mtype) & 0x04) > 0; }