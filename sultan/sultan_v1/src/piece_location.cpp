#include "piece_location.hpp"
#include <stdexcept>

PieceLocation::PieceLocation() { }

PieceLocation::PieceLocation(std::unordered_multimap<int8_t, int8_t> ploc) : piece_loc { ploc } { }

void PieceLocation::update_piece_loc(int8_t ptype, int8_t old_loc, int8_t new_loc)
{
    auto r = piece_loc.equal_range(ptype);
    for (auto it = r.first; it != r.second; it++)
    {
        if (it->second == old_loc)
        {
            it->second = new_loc;
            return;
        }
    }

    throw std::logic_error("UPDATE_PIECE_LOC FAILURE!");
}

void PieceLocation::remove_piece(int8_t ptype, int8_t loc)
{
    auto r = piece_loc.equal_range(ptype);
    for (auto it = r.first; it != r.second; it++)
    {
        if (it->second == loc)
        {
            piece_loc.erase(it);
            return;
        }
    }
    throw std::logic_error("REMOVE_PIECE FAILURE!"); 
}

void PieceLocation::add_piece(int8_t ptype, int8_t loc)
{
    piece_loc.insert(std::make_pair(ptype, loc));
    if (piece_loc.size() > 32 || piece_loc.size() < 2)
        throw std::logic_error("ADD_PIECE FAILURE!");
}

int8_t PieceLocation::find_piece_location(int8_t ptype) const
{
    auto it = piece_loc.find(ptype);
    if (it != piece_loc.end())
        return it->second;
    throw std::logic_error("FIND_PIECE FAILURE!");
}

std::pair<PieceLocation::PLConstIt, PieceLocation::PLConstIt> PieceLocation::find_piece_locations(int8_t ptype) const
{
    return piece_loc.equal_range(ptype);
}

int8_t PieceLocation::get_number_of_pieces() const  { return piece_loc.size(); }