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

/*
Here are some positions in FEN notation for testing purposes:
Pos-1: 6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b KQkq c6 1 2
Pos-2: rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
Pos-3: r1bq1rk1/4bppp/p3pn2/1pn3B1/8/P1NBPN2/1P3PPP/R2Q1RK1
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
    Fen();
    Fen(std::string&&);
    Fen(const std::string&);
    Fen(Fen&& f);

    void set_fen(const std::string& str);
    const std::string& get_fen() const;
};

