#include "pieces.hpp"

const std::vector<std::int8_t> Rook::rook_moves         = { 1, -1, 16, -16 };
const std::vector<std::int8_t> Bishop::bishop_moves     = { 15, -15, 17, -17 };
const std::vector<std::int8_t> Knight::knight_moves     = { 33, 18, -14, -31, -33, -18, 14, 31 };
const std::vector<std::int8_t> King::king_queen_moves   = { 15, -15, 17, -17, 1, -1, 16, -16 };

std::string piece_to_string(const Pieces p) {
    std::string str = "";
    switch(p) {
    case Pieces::bP:    str = "Black Pawn";   break;
    case Pieces::bK:    str = "Black King";   break;
    case Pieces::bQ:    str = "Black Queen";  break;
    case Pieces::bB:    str = "Black Bishop"; break;
    case Pieces::bN:    str = "Black Knight"; break;
    case Pieces::bR:    str = "Black Rook";   break;
    case Pieces::empty: str = "Empty";        break;
    case Pieces::wR:    str = "White Rook";   break;
    case Pieces::wN:    str = "White Knight"; break;
    case Pieces::wB:    str = "White Bishop"; break;
    case Pieces::wQ:    str = "White Queen";  break;
    case Pieces::wK:    str = "White King";   break;
    case Pieces::wP:    str = "White Pawn";   break;
    }
    return str;
}
char piece_to_char(const Pieces p) {
    char c;
    switch(p) {
    case Pieces::bP:    c = 'p'; break;
    case Pieces::bK:    c = 'k'; break;
    case Pieces::bQ:    c = 'q'; break;
    case Pieces::bB:    c = 'b'; break;
    case Pieces::bN:    c = 'n'; break;
    case Pieces::bR:    c = 'r'; break;
    case Pieces::empty: c = '-'; break;
    case Pieces::wR:    c = 'R'; break;
    case Pieces::wN:    c = 'N'; break;
    case Pieces::wB:    c = 'B'; break;
    case Pieces::wQ:    c = 'Q'; break;
    case Pieces::wK:    c = 'K'; break;
    case Pieces::wP:    c = 'P'; break;
    }
    return c;
}
