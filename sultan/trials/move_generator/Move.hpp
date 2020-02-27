#ifndef MOVE_HPP
#define MOVE_HPP

/*

A move is represented by a 16-bit unsigned interger.

15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0

(15 - 10)  -> from
(9  -  4)  -> to
(3  -  0)  -> move flags

*/

#include "Definitions.hpp"

class Move {

public:

	Move(std::uint8_t from, std::uint8_t to, Move_flag mflag);
	Move(const Move& m);

	std::uint8_t get_from() const;
	std::uint8_t get_to() const;
	Move_flag get_flag() const;
	bool is_promotion() const;
	bool is_capture() const;
	void print() const;

private:
	void set_from(std::uint8_t from);
	void set_to(std::uint8_t to);
	void set_move_flag(std::uint8_t mflag);

    std::uint16_t mv;
};


#endif