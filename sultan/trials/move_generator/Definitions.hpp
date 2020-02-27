#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#include <stdint.h>
#include <string>

enum Square : std::uint8_t {
	a1, b1, c1, d1, e1, f1, g1, h1,
	a2, b2, c2, d2, e2, f2, g2, h2,
	a3, b3, c3, d3, e3, f3, g3, h3,
	a4, b4, c4, d4, e4, f4, g4, h4,
	a5, b5, c5, d5, e5, f5, g5, h5,
	a6, b6, c6, d6, e6, f6, g6, h6,
	a7, b7, c7, d7, e7, f7, g7, h7,
	a8, b8, c8, d8, e8, f8, g8, h8,
	Invalid_square
};

enum Move_flag : std::uint8_t {
	Quite = 0,                  // 0        0000
	Double_Pawn_Push,           // 1        0001
	King_Side_Castle,           // 2        0010
	Queen_Side_Castle,          // 3        0011
	Capture,                    // 4        0100
	En_Passant_Capture,         // 5        0101
	Knight_Promotion = 8,       //          1000
	Bishop_Promotion,           // 9        1001
	Rook_Promotion,             // 10       1010
	Queen_Promotion,            // 11       1011
	Knight_Promotion_Capture,   // 12       1100
	Bishop_Promotion_Capture,   // 13       1101
	Rook_Promotion_Capture,     // 14       1110
	Queen_Promotion_Capture     // 15       1111
};

enum Piece : std::int8_t {
	Empty = 0,
	WPawn = 1,
	WRook = 2,
	WKnight = 3,
	WBishop = 4,
	WQueen = 5,
	WKing = 6,
	BPawn = -WPawn,
	BRook = -WRook,
	BKnight = -WKnight,
	BBishop = -WBishop,
	BQueen = -WQueen,
	BKing = -WKing
};

enum Color : std::uint8_t {
	Black,
	White
};

enum Castling : std::uint8_t {
	White_king_side = 1,
	White_queen_side = 2,
	Black_king_side = 4,
	Black_queen_side = 8
};

enum Check : std::uint8_t {
	No_check,
	Single_check,
	Double_check
};

Piece char_to_piece(char c);

char piece_to_char(Piece p);

std::uint8_t rank(std::uint8_t s);

std::uint8_t file(std::uint8_t s);

Square sq(std::uint8_t rank_no, std::uint8_t file_no);

void print_attacked_squares(Color c);

#endif