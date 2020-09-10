#include <sstream>
#include "fen.hpp"

Fen::Fen(std::string_view fen_str) 
{
    std::stringstream ss;
    ss << fen_str;
    ss >> piece_placement
        >> active_color
        >> castling_availability
        >> en_passant
        >> half_move_clock
        >> full_move_number;
}

std::ostream& operator<<(std::ostream& os, const Fen& f)
{
    os << "Piece Placement : " << f.piece_placement << "\n"
       << "Active Color    : " << f.active_color << "\n"
       << "Castling Avail  : " << f.castling_availability << "\n"
       << "En Passant      : " << f.en_passant << "\n"
       << "Half Move Clock : " << f.half_move_clock << "\n"
       << "Full Move Number: " << f.full_move_number << "\n";
    return  os;
}
