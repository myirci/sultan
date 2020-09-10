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

struct Fen
{ 
    static constexpr std::string_view startpos{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };

    Fen(std::string_view fen_str = startpos);

    std::string piece_placement;
    std::string active_color;
    std::string castling_availability;
    std::string en_passant;
    int16_t half_move_clock;
    int16_t full_move_number;
};

