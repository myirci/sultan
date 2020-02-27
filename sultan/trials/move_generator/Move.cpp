#include "Move.hpp"
#include <iostream>

extern std::string square_to_string[65];
extern std::string move_flag_to_string[16];

Move::Move(std::uint8_t from, std::uint8_t to, Move_flag mflag) {
	set_from(from);
	set_to(to);
	set_move_flag(mflag);
}

Move::Move(const Move& m) {
	mv = m.mv;
}

std::uint8_t Move::get_from() const {
	return static_cast<std::uint8_t>((mv & 0xFC00) >> 10);
}

std::uint8_t Move::get_to() const {
	return static_cast<std::uint8_t>((mv & 0x03F0) >> 4);
}

Move_flag Move::get_flag() const {
	return static_cast<Move_flag>(mv & 0x000F);
}

bool Move::is_promotion() const {
	return (mv & 0x0008) > 0;
}

bool Move::is_capture() const {
	return (mv & 0x0004) > 0;
}

void Move::set_from(std::uint8_t from) {
	mv &= 0x03FF;
	mv |= (static_cast<std::uint16_t>(from) << 10);
}

void Move::set_to(std::uint8_t to) {
	mv &= 0xFC0F;
	mv |= (static_cast<std::uint16_t>(to) << 4);
}

void Move::set_move_flag(std::uint8_t mflag) {
	mv &= 0xFFF0;
	mv |= static_cast<std::uint16_t>(mflag);
}

void Move::print() const {
	std::cout << "From : " << square_to_string[get_from()] << std::endl;
	std::cout << "To   : " << square_to_string[get_to()] << std::endl;
	std::cout << "Flag : " << move_flag_to_string[get_flag()] << std::endl;
}