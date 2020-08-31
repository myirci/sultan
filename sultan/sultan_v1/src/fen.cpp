#include <sstream>
#include "fen.hpp"

Fen::Fen() : fen_str { Fen::startpos } { parse(); }

Fen::Fen(std::string&& f_str) : fen_str { std::move(f_str) } { parse(); }

Fen::Fen(const std::string& f_str) : fen_str { f_str } { parse(); }

Fen::Fen(Fen&& f) : fen_str { std::move(f.fen_str) } { parse(); }

void Fen::parse()
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

void Fen::set_fen(const std::string& str)
{
    fen_str = str;
    parse();
}

const std::string& Fen::get_fen() const { return fen_str; }

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
