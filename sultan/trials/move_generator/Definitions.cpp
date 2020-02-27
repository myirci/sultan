#include "Definitions.hpp"
#include <iostream>

Square square_visit_order[64] = {
	a8, b8, c8, d8, e8, f8, g8, h8,
	a7, b7, c7, d7, e7, f7, g7, h7,
	a6, b6, c6, d6, e6, f6, g6, h6,
	a5, b5, c5, d5, e5, f5, g5, h5,
	a4, b4, c4, d4, e4, f4, g4, h4,
	a3, b3, c3, d3, e3, f3, g3, h3,
	a2, b2, c2, d2, e2, f2, g2, h2,
	a1, b1, c1, d1, e1, f1, g1, h1
};

std::string square_to_string[65] = {
	"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
	"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
	"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
	"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
	"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
	"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
	"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
	"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
	"-"
};

std::string color_to_string[2] = {
	"Black",
	"White"
};

std::string move_flag_to_string[16] = {
	"Quite",
	"Double_Pawn_Push",
	"King_Side_Castle",
	"Queen_Side_Castle",
	"Capture",
	"En_Passant_Capture",
	"-",
	"-",
	"Knight_Promotion",
	"Bishop_Promotion",
	"Rook_Promotion",
	"Queen_Promotion",
	"Knight_Promotion_Capture",
	"Bishop_Promotion_Capture",
	"Rook_Promotion_Capture",
	"Queen_Promotion_Capture"
};


std::uint8_t black_attacked_squares[64] = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0
};

std::uint8_t white_attacked_squares[64] = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0
};

std::uint8_t rank(std::uint8_t s) {
	return s / 8;
}

std::uint8_t file(std::uint8_t s) {
	return s % 8;
}

Square sq(std::uint8_t rank_no, std::uint8_t file_no) {
	return static_cast<Square>(8 * rank_no + file_no);
}

Piece char_to_piece(char c) {

	switch (c) {
	case 'P':
		return WPawn;
		break;
	case 'p':
		return BPawn;
		break;
	case 'K':
		return WKing;
		break;
	case 'k':
		return BKing;
		break;
	case 'R':
		return WRook;
		break;
	case 'r':
		return BRook;
		break;
	case 'N':
		return WKnight;
		break;
	case 'n':
		return BKnight;
		break;
	case 'B':
		return WBishop;
		break;
	case 'b':
		return BBishop;
		break;
	case 'Q':
		return WQueen;
		break;
	case 'q':
		return BQueen;
		break;
	default:
		return Empty;
		break;
	}
}

char piece_to_char(Piece p) {

	switch (p) {
	case Empty:
		return '-';
		break;
	case WPawn:
		return 'P';
		break;
	case BPawn:
		return 'p';
		break;
	case WKing:
		return 'K';
		break;
	case BKing:
		return 'k';
		break;
	case WRook:
		return 'R';
		break;
	case BRook:
		return 'r';
		break;
	case WKnight:
		return 'N';
		break;
	case BKnight:
		return 'n';
		break;
	case WBishop:
		return 'B';
		break;
	case BBishop:
		return 'b';
		break;
	case WQueen:
		return 'Q';
		break;
	case BQueen:
		return 'q';
		break;
	default:
		return 'X';
		break;
	}
}

void print_attacked_squares(Color c) {

	if (c == White) {
		for (int i = 0; i < 64; ++i) {
			if (i > 0 && i % 8 == 0)
				std::cout << std::endl;
			std::cout << static_cast<int>(white_attacked_squares[square_visit_order[i]]) << " ";
		}
		std::cout << "\n" << std::endl;
		std::cout << "------------------------------------" << std::endl;
	}
	else {
		for (int i = 0; i < 64; ++i) {
			if (i > 0 && i % 8 == 0)
				std::cout << std::endl;
			std::cout << static_cast<int>(black_attacked_squares[square_visit_order[i]]) << " ";
		}
		std::cout << "\n" << std::endl;
		std::cout << "------------------------------------" << std::endl;
	}
}