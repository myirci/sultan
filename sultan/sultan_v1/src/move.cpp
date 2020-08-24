#include "move.hpp"
#include "square.hpp"

Move::Move(int8_t loc_from, int8_t loc_to, MoveType move_type, int8_t captured_piece): from{ loc_from }, to{ loc_to }, mtype{ move_type }, captured{ captured_piece } { }

int8_t Move::get_from() const { return from; }

int8_t Move::get_to() const { return to; }

MoveType Move::get_move_type() const { return mtype; }

int8_t Move::get_captured_piece() const { return captured; }

bool Move::is_promotion() const { return (static_cast<int8_t>(mtype) & 0x08) > 0; }

bool Move::is_capture() const { return (static_cast<int8_t>(mtype) & 0x04) > 0; }

bool Move::is_castle() const { return mtype == MoveType::King_Side_Castle || mtype == MoveType::Queen_Side_Castle; }

bool Move::is_en_passant_capture() const { return mtype == MoveType::En_Passant_Capture; }

std::ostream& operator << (std::ostream& os, Move m) 
{
	os << square::square_to_string(m.get_from()) << square::square_to_string(m.get_to());
	return os;
}