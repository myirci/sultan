#pragma once

#include <unordered_map>

class PieceLocation 
{
	PieceLocation();
	PieceLocation(std::unordered_multimap<int8_t, int8_t> ploc);
	
	// key: piece type, value: piece locations
	std::unordered_multimap<int8_t, int8_t> piece_loc; 

	friend class Factory;
	friend class Utility;

public:
	using PLConstIt = std::unordered_multimap<int8_t, int8_t>::const_iterator;
	void update_piece_loc(int8_t ptype, int8_t old_loc, int8_t new_loc);
	void remove_piece(int8_t ptype, int8_t loc);
	void add_piece(int8_t ptype, int8_t loc);
	int8_t find_piece_location(int8_t ptype) const;
	std::pair<PLConstIt, PLConstIt> find_piece_locations(int8_t ptype) const;
	int8_t get_number_of_pieces() const;
};