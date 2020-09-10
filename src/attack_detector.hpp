#pragma once

#include "definitions.hpp"
#include "test/board_tests.hpp"
#include <unordered_map>

class Board;
class PieceLocation;

class AttackDetector 
{
	AttackDetector(Board const& b, PieceLocation const& pl);
	AttackDetector(Board const& b, PieceLocation const& pl, std::unordered_multimap<int8_t, std::pair<int8_t, int8_t>> const& cap);

	// key: target loc, 
	// value: attacker loc and attack direction, double entry is only possible in the case of double check
	//        attack direction is the direction from the attacked square to the attacker location
	std::unordered_multimap<int8_t, std::pair<int8_t, int8_t>> checks_and_pins;
	Board const& board;
	PieceLocation const& ploc;

	void compute_checks_and_pins(int8_t attacking_side);

	friend class Factory;
	friend class Utility;
	friend class test::BoardTests;

public:

	void compute_checks_and_pins();
	
	// check and pin queries 
	int8_t num_checkers(int8_t king_pos) const;
	std::pair<int8_t, int8_t> get_checker_positions(int8_t king_pos) const;
	int8_t get_pin_direction(int8_t sq) const;
	std::pair<int8_t, int8_t> get_attack_info(int8_t pos) const;
	
	// query used to check if king will be safe at the given target square
	bool is_under_attack(int8_t attacking_side, int8_t target_sq, int8_t exclude_pos1 = def::none, int8_t exclude_pos2 = def::none) const;
};