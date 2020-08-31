#pragma once

/*
FEN: ForsythEdwards Notation
Field-1: Piece Placement
Field-2: Active Color
Field-3: Castling Availability
Field-4: En Passant
Field-5: Half Move Clock
Field-6: Full Move Nummber
*/

#include <string>
#include <ostream>

class Fen
{
    std::string fen_str;
    std::string piece_placement;
    std::string active_color;
    std::string castling_availability;
    std::string en_passant;
    int half_move_clock;
    int full_move_number;
    void parse();

    friend std::ostream& operator<<(std::ostream& os, const Fen& f);
    friend class Utility;

public:
    static constexpr std::string_view startpos{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
    Fen();
    Fen(std::string&&);
    Fen(const std::string&);
    Fen(Fen&& f);

    void set_fen(const std::string& str);
    const std::string& get_fen() const;
};

