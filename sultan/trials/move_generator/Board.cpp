#include "Board.hpp"
#include "Move.hpp"

#include <iostream>
#include <sstream>
#include <cctype>

extern Square square_visit_order[64];
extern std::string square_to_string[65];
extern std::string color_to_string[2];
extern std::uint8_t white_attacked_squares[64];
extern std::uint8_t black_attacked_squares[64];

Board::Board() {

	set_to_starting_position();
}

Board::Board(const std::string& fen) {

	import_from_FEN(fen);
}

void Board::set_to_starting_position() {
	
	side_to_move = White;
	castling_rights = 0x0F;
	captured = Empty;
	en_passant_square = Invalid_square;
	halfmove_counter = 0;
	move_counter = 1;

	brd[a1] = brd[h1] = WRook;
	brd[b1] = brd[g1] = WKnight;
	brd[c1] = brd[f1] = WBishop;
	brd[d1] = WQueen;
	brd[e1] = WKing;
	for (std::uint8_t s = a2; s <= h2; ++s) 
		brd[s] = WPawn;
	for (std::uint8_t s = a3; s <= h6; ++s) 
		brd[s] = Empty;
	for (std::uint8_t s = a7; s <= h7; ++s) 
		brd[s] = BPawn;
	brd[a8] = brd[h8] = BRook;
	brd[b8] = brd[g8] = BKnight;
	brd[c8] = brd[f8] = BBishop;
	brd[d8] = BQueen;
	brd[e8] = BKing;
}

bool Board::operator==(const Board& rhs) const {

	// check the piece locations
	for (int i = 0; i < 64; ++i)
		if (brd[i] != rhs.brd[i])
			return false;

	// check the move counter
	if (move_counter != rhs.move_counter)
		return false;

	// check halfmove counter
	if (halfmove_counter != rhs.halfmove_counter)
		return false;

	// check the castling rights
	if ((castling_rights & 0x0F) != (rhs.castling_rights & 0x0F))
		return false;

	// check en passant sqaure
	if (en_passant_square != rhs.en_passant_square)
		return false;

	// check side to move
	if (side_to_move != rhs.side_to_move)
		return false;

	return true;
}

void Board::make_moves(std::vector<std::string>& tokens) {

	unsigned int start{ 3 };
	if (tokens[1] == "startpos") {
		set_to_starting_position();
	}
	else if (tokens[1] == "fen") {
		std::string fen = tokens[3] + " " + tokens[4] + " " + tokens[5] + " " +
						  tokens[6] + " " + tokens[7] + " " + tokens[8];
		import_from_FEN(fen);
		start = 9;
	}

	std::tuple<Square, Square, Piece> move;
	for (unsigned int i = start; i < tokens.size(); ++i) {
		auto move = extract_move_from_string(tokens[i]);
		make_move(std::get<0>(move), std::get<1>(move), std::get<2>(move));
		// std::cout << "Debug: extracted move " 
		//	      << square_to_string[std::get<0>(move)]
		//	      << square_to_string[std::get<1>(move)]
		//		  << " promotion " << std::get<2>(move) << std::endl;
	}
}

std::tuple<Square, Square, Piece> Board::extract_move_from_string(const std::string& str) {

	Piece pro{ Empty };
	if (str.size() == 5) {
		if (str[4] == 'q')
			pro = WQueen;
		else if (str[4] == 'n')
			pro = WKnight;
		else if (str[4] == 'r')
			pro = WRook;
		else if (str[4] == 'b')
			pro = WBishop;
	}
	return std::make_tuple(sq(str[1] - '1', str[0] - 'a'), sq(str[3] - '1', str[2] - 'a'), pro);
}

void Board::make_move(Square from, Square to, Piece promotion) {

	if (promotion != Empty) {
		if (promotion == WQueen)
			make_move(Move(from, to, Queen_Promotion));
		else if (promotion == WKnight)
			make_move(Move(from, to, Knight_Promotion));
		else if (promotion == WRook)
			make_move(Move(from, to, Rook_Promotion));
		else if (promotion == WBishop)
			make_move(Move(from, to, Bishop_Promotion));
	}
	else {
		if (brd[to] == Empty) {
			if (side_to_move == White) {
				if (brd[from] == WPawn) {
					if (to - from == 8)
						make_move(Move(from, to, Quite));
					else if (to - from == 16)
						make_move(Move(from, to, Double_Pawn_Push));
					else
						make_move(Move(from, to, En_Passant_Capture));
				}
				else if (brd[from] == WKing) {
					if (from != e1)
						make_move(Move(from, to, Quite));
					else if (to == g1)
						make_move(Move(from, to, King_Side_Castle));
					else if (to == c1)
						make_move(Move(from, to, Queen_Side_Castle));
					else
						make_move(Move(from, to, Quite));
				}
				else {
					make_move(Move(from, to, Quite));
				}
			}
			else {
				if (brd[from] == BPawn) {
					if (from - to == 8)
						make_move(Move(from, to, Quite));
					else if (from - to == 16)
						make_move(Move(from, to, Double_Pawn_Push));
					else
						make_move(Move(from, to, En_Passant_Capture));
				}
				else if (brd[from] == BKing) {
					if (from != e8)
						make_move(Move(from, to, Quite));
					else if (to == g8)
						make_move(Move(from, to, King_Side_Castle));
					else if (to == c8)
						make_move(Move(from, to, Queen_Side_Castle));
					else
						make_move(Move(from, to, Quite));
				}
				else {
					make_move(Move(from, to, Quite));
				}
			}
		}
		else {
			make_move(Move(from, to, Capture));
		}
	}
}

void Board::make_move(const Move& m) {
	
	Move_flag mf { m.get_flag() };
	std::uint8_t from { m.get_from() }, to { m.get_to() };

	// update the halfmove counter
	++halfmove_counter;

	// update en passant square
	if (mf == Double_Pawn_Push) 
		side_to_move == White ? en_passant_square = static_cast<Square>(from + 8) : en_passant_square = static_cast<Square>(from - 8);
	else 
		en_passant_square = Invalid_square;

	// capture
	if (m.is_capture()) {
		if (mf == En_Passant_Capture) {
			if (side_to_move == White) {
				brd[en_passant_square - 8] = Empty;
				captured = BPawn;
			}
			else {
				brd[en_passant_square + 8] = Empty;
				captured = WPawn;
			}
		}
		else {
			captured = brd[to];
		}
		halfmove_counter = 0;
	}

	switch (brd[from]) {
	case WPawn:
	case BPawn:
		halfmove_counter = 0;
		break;
	case WRook:
		if (from == a1 && can_white_castle_queen_side()) {
			copy_castling_rights();
			set_castling_rights(White_queen_side, false);
		}
		else if (from == h1 && can_white_castle_king_side()) {
			copy_castling_rights();
			set_castling_rights(White_king_side, false);
		}
		break;
	case BRook:
		if (from == a8 && can_black_castle_queen_side()) {
			copy_castling_rights();
			set_castling_rights(Black_queen_side, false);
		}
		else if (from == h8 && can_black_castle_king_side()) {
			copy_castling_rights();
			set_castling_rights(Black_king_side, false);
		}
		break;
	case WKing:
		if (can_white_castle()) {
			copy_castling_rights();
			set_white_castling_rights(false);
			if (mf == King_Side_Castle) { 
				brd[h1] = Empty; 
				brd[f1] = WRook; 
			}
			else if (mf == Queen_Side_Castle) { 
				brd[a1] = Empty; 
				brd[d1] = WRook; 
			}
		}
		break;
	case BKing:
		if (can_black_castle()) {
			copy_castling_rights();
			set_black_castling_rights(false);
			if (mf == King_Side_Castle) {
				brd[h8] = Empty; 
				brd[f8] = BRook; 
			}
			else if (mf == Queen_Side_Castle) { 
				brd[a8] = Empty; 
				brd[d8] = BRook; 
			}
		}
		break;
	default:
		break;
	}

	if (m.is_promotion()) {
		switch (mf) {
		case Queen_Promotion:
		case Queen_Promotion_Capture:
			side_to_move == White ? brd[to] = WQueen : brd[to] = BQueen;
			break;
		case Knight_Promotion:
		case Knight_Promotion_Capture:
			side_to_move == White ? brd[to] = WKnight : brd[to] = BKnight;
			break;
		case Rook_Promotion:
		case Rook_Promotion_Capture:
			side_to_move == White ? brd[to] = WRook : brd[to] = BRook;
			break;
		case Bishop_Promotion:
		case Bishop_Promotion_Capture:
			side_to_move == White ? brd[to] = WBishop : brd[to] = BBishop;
			break;
		default:
			std::cerr << "Make move - promotion move error" << std::endl;
			break;
		}
	}
	else {
		brd[to] = brd[from];
	}

	// set the position of current piece to Empty
	brd[from] = Empty;

	// side to move and move counter
	if (side_to_move == White) {
		side_to_move = Black;
	}
	else {
		side_to_move = White;
		++move_counter;
	}
}

void Board::unmake_move(const Move& m) {

	Move_flag mf { m.get_flag() };
	std::uint8_t from { m.get_from() }, to { m.get_to() };

	// update halfmove_counter -> find a good way later,
	// question: how to restore after a zeroize
	if (halfmove_counter != 0)
		--halfmove_counter;

	switch (mf)
	{
	case Quite:
	{
		brd[from] = brd[to];
		brd[to] = Empty;
		break;
	}
	case Double_Pawn_Push: 
	{
		en_passant_square = Invalid_square;
		brd[from] = brd[to];
		brd[to] = Empty;
		break;
	}
	case King_Side_Castle: 
	{
		restore_castling_rights();
		brd[from] = brd[to];
		brd[to] = Empty;
		if (side_to_move == White) {
			brd[f1] = Empty;
			brd[h1] = WRook;
		}
		else {
			brd[f8] = Empty;
			brd[h8] = BRook;
		}
		break;
	}
	case Queen_Side_Castle:
	{
		restore_castling_rights();
		brd[from] = brd[to];
		brd[to] = Empty;
		if (side_to_move == White) {
			brd[d1] = Empty;
			brd[a1] = WRook;
		}
		else {
			brd[d8] = Empty;
			brd[a8] = BRook;
		}
		break;
	}
	case Capture:
	{
		brd[from] = brd[to];
		brd[to] = captured;
		captured = Empty;
		break;
	}
	case En_Passant_Capture:
	{
		brd[from] = brd[to];
		brd[to] = Empty;
		en_passant_square = static_cast<Square>(to);
		side_to_move == White ? brd[en_passant_square - 8] = BPawn : brd[en_passant_square + 8] = WPawn;
		break;
	}
	case Knight_Promotion:
	case Bishop_Promotion:
	case Rook_Promotion:
	case Queen_Promotion:
	{
		brd[from] = (side_to_move == White ? WPawn : BPawn);
		brd[to] = Empty;
		break;
	}
	case Knight_Promotion_Capture:
	case Bishop_Promotion_Capture:
	case Queen_Promotion_Capture:
	case Rook_Promotion_Capture:
	{
		brd[from] = (side_to_move == White ? WPawn : BPawn);
		brd[to] = captured;
		break;
	}
	}

	// side to move and move counter
	if (side_to_move == White) {
		side_to_move = Black;
		--move_counter;
	}
	else {
		side_to_move = White;
	}
}

void Board::print_board(bool extended) const {

	for (int i = 0; i < 64; ++i) {
		if (i > 0 && i % 8 == 0)
			std::cout << std::endl;
		std::cout << piece_to_char(brd[square_visit_order[i]]) << " ";
	}
	std::cout << "\n" << std::endl;

	if (extended) {
		std::cout << "side to move     : " << color_to_string[side_to_move] << std::endl;
		std::cout << "move counter     : " << move_counter << std::endl;
		std::cout << "halfmove counter : " << static_cast<int>(halfmove_counter) << std::endl;
		std::cout << "captured piece   : " << piece_to_char(captured) << std::endl;
		std::cout << "en passant square: " << square_to_string[en_passant_square] << std::endl;
		std::cout << "K                : " << query_castling_rights(White_king_side) << std::endl;
		std::cout << "Q                : " << query_castling_rights(White_queen_side) << std::endl;
		std::cout << "k                : " << query_castling_rights(Black_king_side) << std::endl;
		std::cout << "q                : " << query_castling_rights(Black_queen_side) << std::endl;
	}

	std::cout << "---------------------------------------" << std::endl;
}

std::string Board::export_to_FEN() const {

/*
	FEN: Forsyth - Edwards Notation
	Field - 1 : Piece Placement
	Field - 2 : Active Color
	Field - 3 : Castling Availability
	Field - 4 : En Passant
	Field - 5 : Half Move Clock
	Field - 6 : Full Move Number
*/

	std::ostringstream fen { "" };

	// Field-1: Piece placement
	std::uint8_t empty_counter { 0 };
	for (int i = 0; i < 64; ++i) {
		if (i > 0 && i % 8 == 0) {
			if (empty_counter != 0) {
				fen << static_cast<int>(empty_counter);
				empty_counter = 0;
			}
			fen << '/';
		}
		if (brd[square_visit_order[i]] == Empty) {
			++empty_counter;
		}
		else {
			if (empty_counter != 0) {
				fen << static_cast<int>(empty_counter);
				empty_counter = 0;
			}
			fen << piece_to_char(brd[square_visit_order[i]]);
		}
	}
	if (empty_counter != 0)
		fen << static_cast<int>(empty_counter);

	// Field-2: Active color
	side_to_move == White ? fen << " w " : fen << " b ";

	// Field-3: Castling Availability
	bool flag{ true };
	if (query_castling_rights(White_king_side)) {
		flag = false;
		fen << 'K';
	}
	if (query_castling_rights(White_queen_side)) {
		flag = false;
		fen << 'Q';
	}
	if (query_castling_rights(Black_king_side)) {
		flag = false;
		fen << 'k';
	}
	if (query_castling_rights(Black_queen_side)) {
		flag = false;
		fen << 'q';
	}
	if (flag) fen << '-';

	// Field-4: En Passant Square
	fen << ' ' << square_to_string[en_passant_square] << ' ';

	// Field-5: Half Move Clock
	fen << static_cast<int>(halfmove_counter) << ' ';

	// Field-6: Full Move Number
	fen << move_counter;

	return fen.str();
}

void Board::import_from_FEN(const std::string& fen) {

	std::array<std::string, 6> fields;
	parse_FEN(fen, fields);

	// Field-1: Piece placement
	std::istringstream is{ fields[0] };
	std::string str{ "" };
	for (int i = 0; i < 8; ++i) {
		std::getline(is, str, '/');
		int f{ -1 };
		for (unsigned int j = 0; j < str.length(); ++j) {
			if (std::isdigit(str[j]))
				for (int k = 0; k < int(str[j] - '0'); ++k)
					brd[sq(7 - i, ++f)] = Empty;
			else 
				brd[sq(7 - i, ++f)] = char_to_piece(str[j]);
		}
	}

	// Field-2: Active color
	if (fields[1] == "w") 
		side_to_move = White;
	else 
		side_to_move = Black;

	// Field-3: Castling Availability
	castling_rights = 0;
	for (unsigned int i = 0; i < fields[2].length(); ++i) {
		switch (fields[2][i]) {
		case 'K': 
			set_castling_rights(White_king_side, true); 
			break;
		case 'Q': 
			set_castling_rights(White_queen_side, true); 
			break;
		case 'k': 
			set_castling_rights(Black_king_side, true); 
			break;
		case 'q': 
			set_castling_rights(Black_queen_side, true); 
			break;
		case '-': 
			break;
		default: 
			std::cerr << "Castling rightd error" << std::endl; 
			break;
		}
	}

	// Field-4: En Passant Square
	(fields[3] == "-") ? en_passant_square = Invalid_square : en_passant_square = sq(fields[3][1] - '1', fields[3][0] - 'a');

	// Field-5: Half Move Clock
	halfmove_counter = static_cast<std::uint8_t>(std::stoi(fields[4]));

	// Field-6: Full Move Number
	move_counter = static_cast<uint16_t>(std::stoi(fields[5]));
}

void Board::parse_FEN(const std::string& fen, std::array<std::string, 6>& fields) const {

	std::istringstream ss { fen };
	for (int i = 0; i < 6; ++i)
		ss >> fields[i];
}

void Board::generate_pawn_moves(Color c, Square pos, std::vector<Move>& moves) {

	std::uint8_t r{ rank(pos) }, f{ file(pos) };

	if (c == White) {

		if (r == 1 && brd[pos + 8] == Empty && brd[pos + 16] == Empty) {
			moves.push_back(Move(pos, pos + 16, Double_Pawn_Push));
		}
		else if (r == 4 && en_passant_square != Invalid_square) {
			if (en_passant_square - 7 == pos || en_passant_square - 9 == pos)
				moves.push_back(Move(pos, en_passant_square, En_Passant_Capture));
		}
		else if (r == 6) {
			if (brd[pos + 8] == Empty) {
				moves.push_back(Move(pos, pos + 8, Queen_Promotion));
				moves.push_back(Move(pos, pos + 8, Rook_Promotion));
				moves.push_back(Move(pos, pos + 8, Knight_Promotion));
				moves.push_back(Move(pos, pos + 8, Bishop_Promotion));
			}
			if (pos != a7 && brd[pos + 7] < 0) {
				moves.push_back(Move(pos, pos + 7, Queen_Promotion_Capture));
				moves.push_back(Move(pos, pos + 7, Rook_Promotion_Capture));
				moves.push_back(Move(pos, pos + 7, Knight_Promotion_Capture));
				moves.push_back(Move(pos, pos + 7, Bishop_Promotion_Capture));
			}
			if (pos != h7 && brd[pos + 9] < 0) {
				moves.push_back(Move(pos, pos + 9, Queen_Promotion_Capture));
				moves.push_back(Move(pos, pos + 9, Rook_Promotion_Capture));
				moves.push_back(Move(pos, pos + 9, Knight_Promotion_Capture));
				moves.push_back(Move(pos, pos + 9, Bishop_Promotion_Capture));
			}
		}
		if (r != 6) {
			if (brd[pos + 8] == Empty)
				moves.push_back(Move(pos, pos + 8, Quite));
			if (brd[pos + 7] < 0 && f != 0)
				moves.push_back(Move(pos, pos + 7, Capture));
			if (brd[pos + 9] < 0 && f != 7)
				moves.push_back(Move(pos, pos + 9, Capture));
		}
	}
	else {
		
		if (r == 6 && brd[pos - 8] == Empty && brd[pos - 16] == Empty) {
			moves.push_back(Move(pos, pos - 16, Double_Pawn_Push));
		}
		else if (r == 3 && en_passant_square != Invalid_square) {
			if (en_passant_square + 7 == pos || en_passant_square + 9 == pos)
				moves.push_back(Move(pos, en_passant_square, En_Passant_Capture));
		}
		else if (r == 1) {
			if (brd[pos - 8] == Empty) {
				moves.push_back(Move(pos, pos - 8, Queen_Promotion));
				moves.push_back(Move(pos, pos - 8, Rook_Promotion));
				moves.push_back(Move(pos, pos - 8, Knight_Promotion));
				moves.push_back(Move(pos, pos - 8, Bishop_Promotion));
			}
			if (pos != a2 && brd[pos - 7] > 0) {
				moves.push_back(Move(pos, pos - 7, Queen_Promotion_Capture));
				moves.push_back(Move(pos, pos - 7, Rook_Promotion_Capture));
				moves.push_back(Move(pos, pos - 7, Knight_Promotion_Capture));
				moves.push_back(Move(pos, pos - 7, Bishop_Promotion_Capture));
			}
			if (pos != h2 && brd[pos - 9] > 0) {
				moves.push_back(Move(pos, pos - 9, Queen_Promotion_Capture));
				moves.push_back(Move(pos, pos - 9, Rook_Promotion_Capture));
				moves.push_back(Move(pos, pos - 9, Knight_Promotion_Capture));
				moves.push_back(Move(pos, pos - 9, Bishop_Promotion_Capture));
			}
		}
		if (r != 1) {
			if (brd[pos - 8] == Empty)
				moves.push_back(Move(pos, pos - 8, Quite));
			if (brd[pos - 7] > 0 && f != 7)
				moves.push_back(Move(pos, pos - 7, Capture));
			if (brd[pos - 9] > 0 && f != 0)
				moves.push_back(Move(pos, pos - 9, Capture));
		}
	}
}

void Board::generate_pinned_pawn_moves(Color c, Square pos, Square pinner, std::vector<Move>& moves) {

	if (rank(pos) == rank(pinner)) {
		return;
	}
	else if (file(pos) == file(pinner)) {

		if (c == White) {
			if (brd[pos + 8] == Empty) {
				moves.push_back(Move(pos, pos + 8, Quite));
				if (rank(pos) == 1 && brd[pos + 16] == Empty)
					moves.push_back(Move(pos, pos + 16, Double_Pawn_Push));
			}
		}
		else {
			if (brd[pos - 8] == Empty) {
				moves.push_back(Move(pos, pos - 8, Quite));
				if (rank(pos) == 6 && brd[pos - 16] == Empty)
					moves.push_back(Move(pos, pos - 16, Double_Pawn_Push));
			}
		}
	}
	else {
		
		if (c == White) {
			// capture the pinner 
			if (pinner - pos == 7 || pinner - pos == 9) {
				if (rank(pos) == 6) {
					moves.push_back(Move(pos, pinner, Queen_Promotion_Capture));
					moves.push_back(Move(pos, pinner, Rook_Promotion_Capture));
					moves.push_back(Move(pos, pinner, Knight_Promotion_Capture));
					moves.push_back(Move(pos, pinner, Bishop_Promotion_Capture));
				}
				else {
					moves.push_back(Move(pos, pinner, Capture));
				}
			}

			// en-passant capture
			if (rank(pos) == 4 && en_passant_square != Invalid_square) {
				std::uint8_t max{ pinner }, min{ pos };
				if (min > max)
					std::swap(max, min);
				if (en_passant_square - 7 == pos && (max - min) % 7 == 0)
					moves.push_back(Move(pos, en_passant_square, En_Passant_Capture));
				else if (en_passant_square - 9 == pos && (max - min) % 9 == 0)
					moves.push_back(Move(pos, en_passant_square, En_Passant_Capture));
			}
		}
		else {
			// capture the pinner
			if (pos - pinner == 7 || pos - pinner == 9) {
				if (rank(pos) == 1) {
					moves.push_back(Move(pos, pinner, Queen_Promotion_Capture));
					moves.push_back(Move(pos, pinner, Rook_Promotion_Capture));
					moves.push_back(Move(pos, pinner, Knight_Promotion_Capture));
					moves.push_back(Move(pos, pinner, Bishop_Promotion_Capture));
				}
				else {
					moves.push_back(Move(pos, pinner, Capture));
				}
			}

			// en-passant capture
			if (rank(pos) == 3 && en_passant_square != Invalid_square) {
				std::uint8_t max{ pinner }, min{ pos };
				if (min > max)
					std::swap(max, min);
				if (en_passant_square + 7 == pos && (max - min) % 7 == 0)
					moves.push_back(Move(pos, en_passant_square, En_Passant_Capture));
				else if (en_passant_square + 9 == pos && (max - min) % 9 == 0)
					moves.push_back(Move(pos, en_passant_square, En_Passant_Capture));
			}
		}
	}
}

void Board::generate_bishop_moves(Color c, Square pos, std::vector<Move>& moves) {

	std::uint8_t r{ rank(pos) }, f { file(pos) };
	std::int8_t nr{ r + 1 }, nf { f + 1};
	Square ns{ Invalid_square };

	while (nr < 8 && nf < 8) {
		ns = sq(nr, nf);
		if (brd[ns] == Empty) {
			moves.push_back(Move(pos, ns, Quite));
			++nr;
			++nf;
		}
		else if (brd[ns] < 0 && c == White) {
			moves.push_back(Move(pos, ns, Capture));
			break;
		}
		else if (brd[ns] > 0 && c == Black) {
			moves.push_back(Move(pos, ns, Capture));
			break;
		}
		else {
			break;
		}
	}

	nr = r + 1;
	nf = f - 1;
	while (nr < 8 && nf > -1) {
		ns = sq(nr, nf);
		if (brd[ns] == Empty) {
			moves.push_back(Move(pos, ns, Quite));
			++nr;
			--nf;
		}
		else if (brd[ns] < 0 && c == White) {
			moves.push_back(Move(pos, ns, Capture));
			break;
		}
		else if (brd[ns] > 0 && c == Black) {
			moves.push_back(Move(pos, ns, Capture));
			break;
		}
		else {
			break;
		}
	}

	nr = r - 1;
	nf = f + 1;
	while (nr > -1 && nf < 8) {
		ns = sq(nr, nf);
		if (brd[ns] == Empty) {
			moves.push_back(Move(pos, ns, Quite));
			--nr;
			++nf;
		}
		else if (brd[ns] < 0 && c == White) {
			moves.push_back(Move(pos, ns, Capture));
			break;
		}
		else if (brd[ns] > 0 && c == Black) {
			moves.push_back(Move(pos, ns, Capture));
			break;
		}
		else {
			break;
		}
	}

	nr = r - 1;
	nf = f - 1;
	while (nr > -1 && nf > -1) {
		ns = sq(nr, nf);
		if (brd[ns] == Empty) {
			moves.push_back(Move(pos, ns, Quite));
			--nr;
			--nf;
		}
		else if (brd[ns] < 0 && c == White) {
			moves.push_back(Move(pos, ns, Capture));
			break;
		}
		else if (brd[ns] > 0 && c == Black) {
			moves.push_back(Move(pos, ns, Capture));
			break;
		}
		else {
			break;
		}
	}
}

void Board::generate_pinned_bishop_moves(Color c, Square pos, Square pinner, std::vector<Move>& moves) {

	if (rank(pos) == rank(pinner) || file(pos) == file(pinner)) {
		return;
	}
	else {
		std::uint8_t max{ pinner }, min{ pos };
		if (min > max)
			std::swap(max, min);

		if ((max - min) % 7 == 0) {
			std::int8_t b{ pos - 7 };
			while (brd[b] == Empty) {
				moves.push_back(Move(pos, b, Quite));
				b -= 7;
			}
			if (static_cast<Square>(b) == pinner)
				moves.push_back(Move(pos, pinner, Capture));

			b = pos + 7;
			while (brd[b] == Empty) {
				moves.push_back(Move(pos, b, Quite));
				b += 7;
			}
			if (static_cast<Square>(b) == pinner)
				moves.push_back(Move(pos, pinner, Capture));
		}
		else if ((max - min) % 9 == 0) {
			std::int8_t b{ pos - 9 };
			while (brd[b] == Empty) {
				moves.push_back(Move(pos, b, Quite));
				b -= 9;
			}
			if (static_cast<Square>(b) == pinner)
				moves.push_back(Move(pos, pinner, Capture));

			b = pos + 9;
			while (brd[b] == Empty) {
				moves.push_back(Move(pos, b, Quite));
				b += 9;
			}
			if (static_cast<Square>(b) == pinner)
				moves.push_back(Move(pos, pinner, Capture));
		}
	}
}

void Board::generate_rook_moves(Color c, Square pos, std::vector<Move>& moves) {

	std::uint8_t r{ rank(pos) }, f{ file(pos) };
	std::int8_t j = pos + 1;

	while (rank(j) == r && j < 64) {
		if (brd[j] == Empty) {
			moves.push_back(Move(pos, j, Quite));
			++j;
		}
		else if (brd[j] < 0 && c == White) {
			moves.push_back(Move(pos, j, Capture));
			break;
		}
		else if (brd[j] > 0 && c == Black) {
			moves.push_back(Move(pos, j, Capture));
			break;
		}
		else {
			break;
		}
	}

	j = pos - 1;
	while (rank(j) == r && j > -1) {
		if (brd[j] == Empty) {
			moves.push_back(Move(pos, j, Quite));
			--j;
		}
		else if (brd[j] < 0 && c == White) {
			moves.push_back(Move(pos, j, Capture));
			break;
		}
		else if (brd[j] > 0 && c == Black) {
			moves.push_back(Move(pos, j, Capture));
			break;
		}
		else {
			break;
		}
	}
	
	j = pos + 8;
	while (file(j) == f && j < 64) {
		if (brd[j] == Empty) {
			moves.push_back(Move(pos, j, Quite));
			j += 8;
		}
		else if (brd[j] < 0 && c == White) {
			moves.push_back(Move(pos, j, Capture));
			break;
		}
		else if (brd[j] > 0 && c == Black) {
			moves.push_back(Move(pos, j, Capture));
			break;
		}
		else {
			break;
		}
	}

	j = pos - 8;
	while (file(j) == f && j >= 0) {
		if (brd[j] == Empty) {
			moves.push_back(Move(pos, j, Quite));
			j -= 8;
		}
		else if (brd[j] < 0 && c == White) {
			moves.push_back(Move(pos, j, Capture));
			break;
		}
		else if (brd[j] > 0 && c == Black) {
			moves.push_back(Move(pos, j, Capture));
			break;
		}
		else {
			break;
		}
	}
}

void Board::generate_pinned_rook_moves(Color c, Square pos, Square pinner, std::vector<Move>& moves) {
	
	if (rank(pos) == rank(pinner)) {

		std::int8_t R{ pos + 1 };
		while (brd[R] == Empty) {
			moves.push_back(Move(pos, R, Quite));
			++R;
		}
		if (static_cast<Square>(R) == pinner)
			moves.push_back(Move(pos, pinner, Capture));

		R = pos - 1;
		while (brd[R] == Empty) {
			moves.push_back(Move(pos, R, Quite));
			--R;
		}
		if (static_cast<Square>(R) == pinner)
			moves.push_back(Move(pos, pinner, Capture));
	}
	else if (file(pos) == file(pinner)) {

		std::int8_t R{ pos + 8 };
		while (brd[R] == Empty) {
			moves.push_back(Move(pos, R, Quite));
			R += 8;
		}
		if (static_cast<Square>(R) == pinner)
			moves.push_back(Move(pos, pinner, Capture));

		R = pos - 8;
		while (brd[R] == Empty) {
			moves.push_back(Move(pos, R, Quite));
			R -= 8;
		}
		if (static_cast<Square>(R) == pinner)
			moves.push_back(Move(pos, pinner, Capture));
	}
}

void Board::generate_knight_moves(Color c, Square pos, std::vector<Move>& moves) {

	std::uint8_t r{ rank(pos) }, f{ file(pos) };
	std::int8_t nr{ r + 1 }, nf{ f + 2};
	Square ns{ Invalid_square };

	if (nr < 8) {
		if (nf < 8) {
			ns = sq(nr, nf);
			if (brd[ns] == Empty)
				moves.push_back(Move(pos, ns, Quite));
			else if (brd[ns] < 0 && c == White)
				moves.push_back(Move(pos, ns, Capture));
			else if (brd[ns] > 0 && c == Black)
				moves.push_back(Move(pos, ns, Capture));
		}

		nf = f - 2;
		if (nf > -1) {
			ns = sq(nr, nf);
			if (brd[ns] == Empty)
				moves.push_back(Move(pos, ns, Quite));
			else if (brd[ns] < 0 && c == White)
				moves.push_back(Move(pos, ns, Capture));
			else if (brd[ns] > 0 && c == Black)
				moves.push_back(Move(pos, ns, Capture));
		}
	}

	nr = r + 2;
	if (nr <= 7) {
		nf = f + 1;
		if (nf <= 7) {
			ns = sq(nr, nf);
			if (brd[ns] == Empty)
				moves.push_back(Move(pos, ns, Quite));
			else if (brd[ns] < 0 && c == White)
				moves.push_back(Move(pos, ns, Capture));
			else if (brd[ns] > 0 && c == Black)
				moves.push_back(Move(pos, ns, Capture));
		}

		nf = f - 1;
		if (nf >= 0) {
			ns = sq(nr, nf);
			if (brd[ns] == Empty)
				moves.push_back(Move(pos, ns, Quite));
			else if (brd[ns] < 0 && c == White)
				moves.push_back(Move(pos, ns, Capture));
			else if (brd[ns] > 0 && c == Black)
				moves.push_back(Move(pos, ns, Capture));
		}
	}

	nr = r - 1;
	if (nr >= 0) {
		nf = f + 2;
		if (nf <= 7) {
			ns = sq(nr, nf);
			if (brd[ns] == Empty)
				moves.push_back(Move(pos, ns, Quite));
			else if (brd[ns] < 0 && c == White)
				moves.push_back(Move(pos, ns, Capture));
			else if (brd[ns] > 0 && c == Black)
				moves.push_back(Move(pos, ns, Capture));
		}

		nf = f - 2;
		if (nf >= 0) {
			ns = sq(nr, nf);
			if (brd[ns] == Empty)
				moves.push_back(Move(pos, ns, Quite));
			else if (brd[ns] < 0 && c == White)
				moves.push_back(Move(pos, ns, Capture));
			else if (brd[ns] > 0 && c == Black)
				moves.push_back(Move(pos, ns, Capture));
		}
	}

	nr = r - 2;
	if (nr >= 0) {
		nf = f + 1;
		if (nf <= 7) {
			ns = sq(nr, nf);
			if (brd[ns] == Empty)
				moves.push_back(Move(pos, ns, Quite));
			else if (brd[ns] < 0 && c == White)
				moves.push_back(Move(pos, ns, Capture));
			else if (brd[ns] > 0 && c == Black)
				moves.push_back(Move(pos, ns, Capture));
		}

		nf = f - 1;
		if (nf >= 0) {
			ns = sq(nr, nf);
			if (brd[ns] == Empty)
				moves.push_back(Move(pos, ns, Quite));
			else if (brd[ns] < 0 && c == White)
				moves.push_back(Move(pos, ns, Capture));
			else if (brd[ns] > 0 && c == Black)
				moves.push_back(Move(pos, ns, Capture));
		}
	}
}

Check Board::generate_king_moves(Color c, Square king_pos, std::pair<Piece, Square>& checker, std::vector<Move>& moves) {

	std::vector<std::pair<Piece, Square>> checkers;
	Check status{ No_check };

	if (c == White) {

		compute_attacked_squares(Black, king_pos, checkers);

		if (file(king_pos) != 7) {
			if (brd[king_pos + 1] == Empty && black_attacked_squares[king_pos + 1] == 0)
				moves.push_back(Move(king_pos, king_pos + 1, Quite));
			if (brd[king_pos + 1] < 0 && black_attacked_squares[king_pos + 1] == 0)
				moves.push_back(Move(king_pos, king_pos + 1, Capture));
		}
					
		if (king_pos % 8 != 0) {
			if (brd[king_pos - 1] == Empty && black_attacked_squares[king_pos - 1] == 0)
				moves.push_back(Move(king_pos, king_pos - 1, Quite));
			if (brd[king_pos - 1] < 0 && black_attacked_squares[king_pos - 1] == 0)
				moves.push_back(Move(king_pos, king_pos - 1, Capture));
		}

		if (king_pos + 8 < 64) {
			if (brd[king_pos + 8] == Empty && black_attacked_squares[king_pos + 8] == 0)
				moves.push_back(Move(king_pos, king_pos + 8, Quite));
			if (brd[king_pos + 8] < 0 && black_attacked_squares[king_pos + 8] == 0)
				moves.push_back(Move(king_pos, king_pos + 8, Capture));
		}
			
		if (static_cast<std::int8_t>(king_pos - 8) > 0) {
			if (brd[king_pos - 8] == Empty && black_attacked_squares[king_pos - 8] == 0)
				moves.push_back(Move(king_pos, king_pos - 8, Quite));
			if (brd[king_pos - 8] < 0 && black_attacked_squares[king_pos - 8] == 0)
				moves.push_back(Move(king_pos, king_pos - 8, Capture));
		}
			
		if (file(king_pos) != 7 && king_pos + 9 < 64) {
			if (brd[king_pos + 9] == Empty && black_attacked_squares[king_pos + 9] == 0)
				moves.push_back(Move(king_pos, king_pos + 9, Quite));
			if (brd[king_pos + 9] < 0 && black_attacked_squares[king_pos + 9] == 0)
				moves.push_back(Move(king_pos, king_pos + 9, Capture));
		}		

		if (king_pos % 8 != 0 && king_pos + 7 < 64) {
			if (brd[king_pos + 7] == Empty && black_attacked_squares[king_pos + 7] == 0)
				moves.push_back(Move(king_pos, king_pos + 7, Quite));
			if (brd[king_pos + 7] < 0 && black_attacked_squares[king_pos + 7] == 0)
				moves.push_back(Move(king_pos, king_pos + 7, Capture));
		}
				
		if (king_pos % 8 != 0 && static_cast<std::int8_t>(king_pos - 9) > 0) {
			if (brd[king_pos - 9] == Empty && black_attacked_squares[king_pos - 9] == 0)
				moves.push_back(Move(king_pos, king_pos - 9, Quite));
			if (brd[king_pos - 9] < 0 && black_attacked_squares[king_pos - 9] == 0)
				moves.push_back(Move(king_pos, king_pos - 9, Capture));
		}

		if (file(king_pos) != 7 && static_cast<std::int8_t>(king_pos - 7) > 0) {
			if (brd[king_pos - 7] == Empty && black_attacked_squares[king_pos - 7] == 0)
				moves.push_back(Move(king_pos, king_pos - 7, Quite));
			else if (brd[king_pos - 7] < 0 && black_attacked_squares[king_pos - 7] == 0)
				moves.push_back(Move(king_pos, king_pos - 7, Capture));
		}		

		if (black_attacked_squares[king_pos] != 0) { // check 
			if (black_attacked_squares[king_pos] == 1) { // single check
				status = Single_check;
				checker = checkers[0];
			}
			else {
				status = Double_check;
			}
		}
		else {
			if (can_white_castle_king_side())
				if (brd[king_pos + 1] == Empty && black_attacked_squares[king_pos + 1] == 0 && 
					brd[king_pos + 2] == Empty && black_attacked_squares[king_pos + 2] == 0)
					moves.push_back(Move(king_pos, king_pos + 2, King_Side_Castle));
			if (can_white_castle_queen_side())
				if (brd[king_pos - 1] == Empty && black_attacked_squares[king_pos - 1] == 0 &&
					brd[king_pos - 2] == Empty && black_attacked_squares[king_pos - 2] == 0)
					moves.push_back(Move(king_pos, king_pos - 2, Queen_Side_Castle));
		}
		memset(black_attacked_squares, 0, sizeof(black_attacked_squares));
	}
	else {

		compute_attacked_squares(White, king_pos, checkers);

		if (file(king_pos) != 7) {
			if (brd[king_pos + 1] == Empty && white_attacked_squares[king_pos + 1] == 0)
				moves.push_back(Move(king_pos, king_pos + 1, Quite));
			if (brd[king_pos + 1] > 0 && white_attacked_squares[king_pos + 1] == 0)
				moves.push_back(Move(king_pos, king_pos + 1, Capture));
		}

		if (king_pos % 8 != 0) {
			if (brd[king_pos - 1] == Empty && white_attacked_squares[king_pos - 1] == 0)
				moves.push_back(Move(king_pos, king_pos - 1, Quite));
			if (brd[king_pos - 1] > 0 && white_attacked_squares[king_pos - 1] == 0)
				moves.push_back(Move(king_pos, king_pos - 1, Capture));
		}

		if (king_pos + 8 < 64) {
			if (brd[king_pos + 8] == Empty && white_attacked_squares[king_pos + 8] == 0)
				moves.push_back(Move(king_pos, king_pos + 8, Quite));
			if (brd[king_pos + 8] > 0 && white_attacked_squares[king_pos + 8] == 0)
				moves.push_back(Move(king_pos, king_pos + 8, Capture));
		}

		if (static_cast<std::int8_t>(king_pos - 8) > 0) {
			if (brd[king_pos - 8] == Empty && white_attacked_squares[king_pos - 8] == 0)
				moves.push_back(Move(king_pos, king_pos - 8, Quite));
			if (brd[king_pos - 8] > 0 && white_attacked_squares[king_pos - 8] == 0)
				moves.push_back(Move(king_pos, king_pos - 8, Capture));
		}

		if (file(king_pos) != 7 && king_pos + 9 < 64) {
			if (brd[king_pos + 9] == Empty && white_attacked_squares[king_pos + 9] == 0)
				moves.push_back(Move(king_pos, king_pos + 9, Quite));
			if (brd[king_pos + 9] > 0 && white_attacked_squares[king_pos + 9] == 0)
				moves.push_back(Move(king_pos, king_pos + 9, Capture));
		}

		if (king_pos % 8 != 0 && king_pos + 7 < 64) {
			if (brd[king_pos + 7] == Empty && white_attacked_squares[king_pos + 7] == 0)
				moves.push_back(Move(king_pos, king_pos + 7, Quite));
			if (brd[king_pos + 7] > 0 && white_attacked_squares[king_pos + 7] == 0)
				moves.push_back(Move(king_pos, king_pos + 7, Capture));
		}
			

		if (king_pos % 8 != 0 && static_cast<std::int8_t>(king_pos - 9) > 0) {
			if (brd[king_pos - 9] == Empty && white_attacked_squares[king_pos - 9] == 0)
				moves.push_back(Move(king_pos, king_pos - 9, Quite));
			if (brd[king_pos - 9] > 0 && white_attacked_squares[king_pos - 9] == 0)
				moves.push_back(Move(king_pos, king_pos - 9, Capture));
		}

		if (file(king_pos) != 7 && static_cast<std::int8_t>(king_pos - 7) > 0) {
			if (brd[king_pos - 7] == Empty && white_attacked_squares[king_pos - 7] == 0)
				moves.push_back(Move(king_pos, king_pos - 7, Quite));
			if (brd[king_pos - 7] > 0 && white_attacked_squares[king_pos - 7] == 0)
				moves.push_back(Move(king_pos, king_pos - 7, Capture));
		}
			
		if (white_attacked_squares[king_pos] != 0) { // check 
			if (white_attacked_squares[king_pos] == 1) { // single check
				status = Single_check;
				checker = checkers[0];
			}
			else {
				status = Double_check;
			}
		}
		else {
			if (can_black_castle_king_side())
				if (brd[king_pos + 1] == Empty && white_attacked_squares[king_pos + 1] == 0 && 
					brd[king_pos + 2] == Empty && white_attacked_squares[king_pos + 2] == 0)
					moves.push_back(Move(king_pos, king_pos + 2, King_Side_Castle));
			if (can_black_castle_queen_side())
				if (brd[king_pos - 1] == Empty && white_attacked_squares[king_pos - 1] == 0 && 
					brd[king_pos - 2] == Empty && white_attacked_squares[king_pos - 2] == 0)
					moves.push_back(Move(king_pos, king_pos - 2, Queen_Side_Castle));
		}
		memset(white_attacked_squares, 0, sizeof(white_attacked_squares));
	}

	return status;
}

void Board::generate_moves_except_for_king(Color c, const std::vector<std::pair<Square, Square>>& pins, std::vector<Move>& moves) {

	Square pinner{ Invalid_square };

	if (c == White) {
		for (int i = 0; i < 64; ++i) {
			if (brd[i] > 0) {

				pinner = Invalid_square;
				for (std::uint8_t j = 0; j < pins.size(); ++j) {
					if (static_cast<Square>(i) == pins[j].first) {
						pinner = pins[j].second;
						break;
					}
				}

				switch (brd[i]) {
				case WPawn:
				{
					(pinner != Invalid_square) ? generate_pinned_pawn_moves(White, static_cast<Square>(i), pinner, moves) :
												 generate_pawn_moves(White, static_cast<Square>(i), moves);
					break;
				}
				case WRook:
				{
					(pinner != Invalid_square) ? generate_pinned_rook_moves(White, static_cast<Square>(i), pinner, moves) :
												 generate_rook_moves(White, static_cast<Square>(i), moves);
					break;
				}
				case WKnight:
				{
					if(pinner == Invalid_square)
						generate_knight_moves(White, static_cast<Square>(i), moves);
					break;
				}
				case WBishop:
				{
					(pinner != Invalid_square) ? generate_pinned_bishop_moves(White, static_cast<Square>(i), pinner, moves) :
												 generate_bishop_moves(White, static_cast<Square>(i), moves);
					break;
				}
				case WQueen:
				{
					if (pinner == Invalid_square) {
						generate_bishop_moves(White, static_cast<Square>(i), moves);
						generate_rook_moves(White, static_cast<Square>(i), moves);
					}
					else {
						generate_pinned_bishop_moves(White, static_cast<Square>(i), pinner, moves);
						generate_pinned_rook_moves(White, static_cast<Square>(i), pinner, moves);
					}
					break;
				}
				}
			}
		}
	}
	else {
		for (int i = 0; i < 64; ++i) {
			if (brd[i] < 0) {

				pinner = Invalid_square;
				for (std::uint8_t j = 0; j < pins.size(); ++j) {
					if (static_cast<Square>(i) == pins[j].first) {
						pinner = pins[j].second;
						break;
					}
				}

				switch (brd[i]) {
				case BPawn:
				{
					(pinner != Invalid_square) ? generate_pinned_pawn_moves(Black, static_cast<Square>(i), pinner, moves) :
											     generate_pawn_moves(Black, static_cast<Square>(i), moves);
					break;
				}
				case BRook:
				{
					(pinner != Invalid_square) ? generate_pinned_rook_moves(Black, static_cast<Square>(i), pinner, moves) :
												 generate_rook_moves(Black, static_cast<Square>(i), moves);
					break;
				}
				case BKnight:
				{
					if(pinner == Invalid_square)
						generate_knight_moves(Black, static_cast<Square>(i), moves);
					break;
				}
				case BBishop:
				{
					(pinner != Invalid_square) ? generate_pinned_bishop_moves(Black, static_cast<Square>(i), pinner, moves) :
												 generate_bishop_moves(Black, static_cast<Square>(i), moves);
					break;
				}
				case BQueen:
				{
					if (pinner == Invalid_square) {
						generate_bishop_moves(Black, static_cast<Square>(i), moves);
						generate_rook_moves(Black, static_cast<Square>(i), moves);
					}
					else {
						generate_pinned_bishop_moves(Black, static_cast<Square>(i), pinner, moves);
						generate_pinned_rook_moves(Black, static_cast<Square>(i), pinner, moves);
					}
					break;
				}
				}
			}
		}
	}
}

Square Board::get_king_position(Color c) {

	if (c == White) {
		for (int i = 0; i < 64; ++i)
			if (brd[i] == WKing)
				return static_cast<Square>(i);
	}
	else {
		for (int i = 0; i < 64; ++i)
			if (brd[i] == BKing)
				return static_cast<Square>(i);
	}
	return Invalid_square;
}

void Board::generate_moves(std::vector<Move>& moves) {

	Square king_pos{ get_king_position(side_to_move) };

	std::vector<std::pair<Square, Square>> pins;
	identify_pins(king_pos, pins);
	
	std::pair<Piece, Square> checker;
	Check status = generate_king_moves(side_to_move, king_pos, checker, moves);
	if (status == No_check) {
		generate_moves_except_for_king(side_to_move, pins, moves);
		// if (moves.empty())
		//	std::cout << "Stalemate" << std::endl;
	}
	else if (status == Single_check) {
			std::vector<Square> path;
			compute_checking_path(king_pos, checker, path);
			std::vector<Move> eliminate_check_moves;
			generate_moves_except_for_king(side_to_move, pins, eliminate_check_moves);
			for (unsigned int i = 0; i < eliminate_check_moves.size(); ++i) {
				for (unsigned int j = 0; j < path.size(); ++j) {
					if (path[j] == eliminate_check_moves[i].get_to()) {
						moves.push_back(eliminate_check_moves[i]);
						// break;
					}
				}
			}
			// if(moves.empty())
				// std::cout << "mate" << std::endl;
	}
}

void Board::identify_pins(Square king_pos, std::vector<std::pair<Square, Square>>& pins) {

	// walk every direction from the king position

	// on the same rank
	std::int8_t i{ static_cast<std::int8_t>(king_pos + 1)   }, 
		        r{ static_cast<std::int8_t>(rank(king_pos)) }, 
		        f{ static_cast<std::int8_t>(file(king_pos)) };
	Square first{ Invalid_square }, second{ Invalid_square };
	std::vector<std::pair<Square, Square>> candidates_diagonal, candidates_flat;
	
	while (rank(i) == r) {
		if (brd[i] != Empty) {
			if (first == Invalid_square) {
				first = static_cast<Square>(i);
			}
			else if (second == Invalid_square) {
				second = static_cast<Square>(i);
				break;
			}
		}
		++i;
	}
	if (first != Invalid_square && second != Invalid_square)
		candidates_flat.push_back(std::make_pair(first, second));

	i = king_pos - 1;
	first = second = Invalid_square;
	while (rank(i) == r && i > -1) {
		if (brd[i] != Empty) {
			if (first == Invalid_square) {
				first = static_cast<Square>(i);
			}
			else if (second == Invalid_square) {
				second = static_cast<Square>(i);
				break;
			}
		}
		--i;
	}
	if (first != Invalid_square && second != Invalid_square)
		candidates_flat.push_back(std::make_pair(first, second));

	i = king_pos + 8 ;
	first = second = Invalid_square;
	while (i < 64) {
		if (brd[i] != Empty) {
			if (first == Invalid_square) {
				first = static_cast<Square>(i);
			}
			else if (second == Invalid_square) {
				second = static_cast<Square>(i);
				break;
			}
		}
		i += 8;
	}
	if(first != Invalid_square && second != Invalid_square)
		candidates_flat.push_back(std::make_pair(first, second));

	i = king_pos - 8;
	first = second = Invalid_square;
	while (i > -1) {
		if (brd[i] != Empty) {
			if (first == Invalid_square) {
				first = static_cast<Square>(i);
			}
			else if (second == Invalid_square) {
				second = static_cast<Square>(i);
				break;
			}
		}
		i -= 8;
	}
	if (first != Invalid_square && second != Invalid_square)
		candidates_flat.push_back(std::make_pair(first, second));

	// on the same diagonal
	if (f < 6 && r < 6) {
		i = king_pos + 9;
		first = second = Invalid_square;
		while (i < 64 && file(i - 9) != 7) {
			if (brd[i] != Empty) {
				if (first == Invalid_square) {
					first = static_cast<Square>(i);
				}
				else if (second == Invalid_square) {
					second = static_cast<Square>(i);
					break;
				}
			}
			if (file(i) == 7 || rank(i) == 7)
				break;
			i += 9;
		}
		if (first != Invalid_square && second != Invalid_square)
			candidates_diagonal.push_back(std::make_pair(first, second));
	}

	if (f > 1 && r > 1) {
		i = king_pos - 9;
		first = second = Invalid_square;
		while (i > -1) {
			if (brd[i] != Empty) {
				if (first == Invalid_square) {
					first = static_cast<Square>(i);
				}
				else if (second == Invalid_square) {
					second = static_cast<Square>(i);
					break;
				}
			}
			if (file(i) == 0 || rank(i) == 0)
				break;
			i -= 9;
		}
		if (first != Invalid_square && second != Invalid_square)
			candidates_diagonal.push_back(std::make_pair(first, second));
	}

	if (f > 1 && r < 6) {
		i = king_pos + 7;
		first = second = Invalid_square;
		while (i < 64) {
			if (brd[i] != Empty) {
				if (first == Invalid_square) {
					first = static_cast<Square>(i);
				}
				else if (second == Invalid_square) {
					second = static_cast<Square>(i);
					break;
				}
			}
			if (file(i) == 0 || rank(i) == 7)
				break;
			i += 7;
		}
		if (first != Invalid_square && second != Invalid_square)
			candidates_diagonal.push_back(std::make_pair(first, second));
	}

	if (f < 6 && r > 1) {
		i = king_pos - 7;
		first = second = Invalid_square;
		while (i > -1) {
			if (brd[i] != Empty) {
				if (first == Invalid_square) {
					first = static_cast<Square>(i);
				}
				else if (second == Invalid_square) {
					second = static_cast<Square>(i);
					break;
				}
			}
			if (rank(i) == 0 || file(i) == 7)
				break;
			i -= 7;
		}
		if (first != Invalid_square && second != Invalid_square)
			candidates_diagonal.push_back(std::make_pair(first, second));
	}

	if (side_to_move == White) {
		
		for (std::uint8_t j = 0; j < candidates_flat.size(); ++j)
			if (brd[candidates_flat[j].first] > 0 && (brd[candidates_flat[j].second] == BQueen || brd[candidates_flat[j].second] == BRook))
				pins.push_back(candidates_flat[j]);

		for (std::uint8_t j = 0; j < candidates_diagonal.size(); ++j)
			if (brd[candidates_diagonal[j].first] > 0  && (brd[candidates_diagonal[j].second] == BQueen || brd[candidates_diagonal[j].second] == BBishop))
				pins.push_back(candidates_diagonal[j]);
	}
	else {

		for (std::uint8_t j = 0; j < candidates_flat.size(); ++j)
			if (brd[candidates_flat[j].first] < 0 && (brd[candidates_flat[j].second] == WQueen || brd[candidates_flat[j].second] == WRook))
				pins.push_back(candidates_flat[j]);

		for (std::uint8_t j = 0; j < candidates_diagonal.size(); ++j)
			if (brd[candidates_diagonal[j].first] < 0 && (brd[candidates_diagonal[j].second] == WQueen || brd[candidates_diagonal[j].second] == WBishop))
				pins.push_back(candidates_diagonal[j]);
	}
}

void Board::attacked_squares_pawn(Square pos, Color c) {

	auto f = file(pos);
	if (c == White) {
		if(f != 0)
			++white_attacked_squares[pos + 7];
		if(f != 7)
			++white_attacked_squares[pos + 9];
	}
	else {
		if (f != 7)
			++black_attacked_squares[pos - 7];
		if (f != 0)
			++black_attacked_squares[pos - 9];
	}
}

void Board::attacked_squares_bishop(Square pos, Color c) {

	std::uint8_t r{ rank(pos) }, f{ file(pos) };
	std::int8_t nr{ r + 1 }, nf{ f + 1 };
	Square ns{ Invalid_square };

	if (c == White) {
		
		while (nr < 8 && nf < 8) {
			ns = sq(nr, nf);
			++white_attacked_squares[ns];
			if (brd[ns] == Empty || brd[ns] == BKing) {
				++nr;
				++nf;
			}
			else {
				break;
			}
		}

		nr = r + 1;
		nf = f - 1;
		while (nr < 8 && nf > -1) {
			ns = sq(nr, nf);
			++white_attacked_squares[ns];
			if (brd[ns] == Empty || brd[ns] == BKing) {
				++nr;
				--nf;
			}
			else {
				break;
			}
		}

		nr = r - 1;
		nf = f + 1;
		while (nr > -1 && nf < 8) {
			ns = sq(nr, nf);
			++white_attacked_squares[ns];
			if (brd[ns] == Empty || brd[ns] == BKing) {
				--nr;
				++nf;
			}
			else {
				break;
			}
		}

		nr = r - 1;
		nf = f - 1;
		while (nr > -1 && nf > -1) {
			ns = sq(nr, nf);
			++white_attacked_squares[ns];
			if (brd[ns] == Empty || brd[ns] == BKing) {
				--nr;
				--nf;
			}
			else {
				break;
			}
		}
	}
	else {

		while (nr < 8 && nf < 8) {
			ns = sq(nr, nf);
			++black_attacked_squares[ns];
			if (brd[ns] == Empty || brd[ns] == WKing) {
				++nr;
				++nf;
			}
			else {
				break;
			}
		}

		nr = r + 1;
		nf = f - 1;
		while (nr < 8 && nf > -1) {
			ns = sq(nr, nf);
			++black_attacked_squares[ns];
			if (brd[ns] == Empty || brd[ns] == WKing) {
				++nr;
				--nf;
			}
			else {
				break;
			}
		}

		nr = r - 1;
		nf = f + 1;
		while (nr > -1 && nf < 8) {
			ns = sq(nr, nf);
			++black_attacked_squares[ns];
			if (brd[ns] == Empty || brd[ns] == WKing) {
				--nr;
				++nf;
			}
			else {
				break;
			}
		}

		nr = r - 1;
		nf = f - 1;
		while (nr > -1 && nf > -1) {
			ns = sq(nr, nf);
			++black_attacked_squares[ns];
			if (brd[ns] == Empty || brd[ns] == WKing) {
				--nr;
				--nf;
			}
			else {
				break;
			}
		}
	}
}

void Board::attacked_squares_knight(Square pos, Color c) {

	std::uint8_t r{ rank(pos) }, f{ file(pos) };
	std::int8_t nr{ r + 1 }, nf{ f + 2 };
	Square ns{ Invalid_square };
	if (nr < 8) {
		if (nf < 8) {
			ns = sq(nr, nf);
			c == White ? ++white_attacked_squares[ns] : ++black_attacked_squares[ns];
		}

		nf = f - 2;
		if (nf > -1) {
			ns = sq(nr, nf);
			c == White ? ++white_attacked_squares[ns] : ++black_attacked_squares[ns];
		}
	}

	nr = r + 2;
	if (nr < 8) {
		nf = f + 1;
		if (nf < 8) {
			ns = sq(nr, nf);
			c == White ? ++white_attacked_squares[ns] : ++black_attacked_squares[ns];
		}

		nf = f - 1;
		if (nf > -1) {
			ns = sq(nr, nf);
			c == White ? ++white_attacked_squares[ns] : ++black_attacked_squares[ns];
		}
	}

	nr = r - 1;
	if (nr > -1) {
		nf = f + 2;
		if (nf < 8) {
			ns = sq(nr, nf);
			c == White ? ++white_attacked_squares[ns] : ++black_attacked_squares[ns];
		}

		nf = f - 2;
		if (nf > -1) {
			ns = sq(nr, nf);
			c == White ? ++white_attacked_squares[ns] : ++black_attacked_squares[ns];
		}
	}

	nr = r - 2;
	if (nr > -1) {
		nf = f + 1;
		if (nf < 8) {
			ns = sq(nr, nf);
			c == White ? ++white_attacked_squares[ns] : ++black_attacked_squares[ns];
		}

		nf = f - 1;
		if (nf > -1) {
			ns = sq(nr, nf);
			c == White ? ++white_attacked_squares[ns] : ++black_attacked_squares[ns];
		}
	}
}

void Board::attacked_squares_rook(Square pos, Color c) {

	std::uint8_t r{ rank(pos) }, f{ file(pos) };
	std::int8_t j = pos + 1;
	
	if (c == White) {
		
		while ((rank(j) == r) && (j < 64)) {
			++white_attacked_squares[j];
			if (brd[j] == Empty || brd[j] == BKing)	++j;
			else break;
		}

		j = pos - 1;
		while ((rank(j) == r) && (j > -1)) {
			++white_attacked_squares[j];
			if (brd[j] == Empty || brd[j] == BKing) --j;
			else break;
		}

		j = pos + 8;
		while ((file(j) == f) && (j < 64)) {
			++white_attacked_squares[j];
			if (brd[j] == Empty || brd[j] == BKing) j += 8;
			else break;
		}

		j = pos - 8;
		while ((file(j) == f) && (j > -1)) {
			++white_attacked_squares[j];
			if (brd[j] == Empty || brd[j] == BKing)	j -= 8;
			else break;
		}
	}
	else {

		while ((rank(j) == r) && (j < 64)) {
			++black_attacked_squares[j];
			if (brd[j] == Empty || brd[j] == WKing)	++j;
			else break;
		}

		j = pos - 1;
		while ((rank(j) == r) && (j > -1)) {
			++black_attacked_squares[j];
			if (brd[j] == Empty || brd[j] == WKing) --j;
			else break;
		}

		j = pos + 8;
		while ((file(j) == f) && (j < 64)) {
			++black_attacked_squares[j];
			if (brd[j] == Empty || brd[j] == WKing) j += 8;
			else break;
		}

		j = pos - 8;
		while ((file(j) == f) && (j > -1)) {
			++black_attacked_squares[j];
			if (brd[j] == Empty || brd[j] == WKing)	j -= 8;
			else break;
		}
	}
}

void Board::attacked_squares_king(Square pos, Color c) {
	
	auto f = file(pos);
	if(f != 7)
		c == White ? ++white_attacked_squares[pos + 1] : ++black_attacked_squares[pos + 1];
	
	if (f != 0)
		c == White ? ++white_attacked_squares[pos - 1] : ++black_attacked_squares[pos - 1];

	if (pos + 8 < 64)
		c == White ? ++white_attacked_squares[pos + 8] : ++black_attacked_squares[pos + 8];

	if (static_cast<std::int8_t>(pos - 8) > 0)
		c == White ? ++white_attacked_squares[pos - 8] : ++black_attacked_squares[pos - 8];

	if (f != 7 && pos + 9 < 64)
		c == White ? ++white_attacked_squares[pos + 9] : ++black_attacked_squares[pos + 9];

	if (f != 0 && pos + 7 < 64)
		c == White ? ++white_attacked_squares[pos + 7] : ++black_attacked_squares[pos + 7];

	if (f != 0 && static_cast<std::int8_t>(pos - 9) > 0)
		c == White ? ++white_attacked_squares[pos - 9] : ++black_attacked_squares[pos - 9];

	if (f != 7 && static_cast<std::int8_t>(pos - 7) > 0)
		c == White ? ++white_attacked_squares[pos - 7] : ++black_attacked_squares[pos - 7];
}

void Board::compute_attacked_squares(Color c, Square king_pos, std::vector<std::pair<Piece, Square>>& checkers) {
	
	std::uint8_t num_checking_pieces { 0 };

	if (c == White) {
		for (int i = 0; i < 64; ++i) {
			if (brd[i] > 0) {
				switch (brd[i])
				{
				case WPawn:
				{
					attacked_squares_pawn(static_cast<Square>(i), White);
					break;
				}
				case WRook:
				{
					attacked_squares_rook(static_cast<Square>(i), White);
					break;
				}
				case WKnight:
				{
					attacked_squares_knight(static_cast<Square>(i), White);
					break;
				}
				case WBishop:
				{
					attacked_squares_bishop(static_cast<Square>(i), White);
					break;
				}
				case WQueen:
				{
					attacked_squares_rook(static_cast<Square>(i), White);
					attacked_squares_bishop(static_cast<Square>(i), White);
					break;
				}
				case WKing:
				{
					attacked_squares_king(static_cast<Square>(i), White);
					break;
				}
				}
				
				if (white_attacked_squares[king_pos] != num_checking_pieces) {
					++num_checking_pieces;
					checkers.push_back(std::make_pair(brd[i], static_cast<Square>(i)));
				}
			}
		}
	}
	else {
		for (int i = 0; i < 64; ++i) {
			if (brd[i] < 0) {
				switch (brd[i])
				{
				case BPawn:
				{
					attacked_squares_pawn(static_cast<Square>(i), Black);
					break;
				}
				case BRook:
				{
					attacked_squares_rook(static_cast<Square>(i), Black);
					break;
				}
				case BKnight:
				{
					attacked_squares_knight(static_cast<Square>(i), Black);
					break;
				}
				case BBishop:
				{
					attacked_squares_bishop(static_cast<Square>(i), Black);
					break;
				}
				case BQueen:
				{
					attacked_squares_rook(static_cast<Square>(i), Black);
					attacked_squares_bishop(static_cast<Square>(i), Black);
					break;
				}
				case BKing:
				{
					attacked_squares_king(static_cast<Square>(i), Black);
					break;
				}
				}
				if (black_attacked_squares[king_pos] != num_checking_pieces) {
					++num_checking_pieces;
					checkers.push_back(std::make_pair(brd[i], static_cast<Square>(i)));
				}
			}
		}
	}
}

void Board::compute_checking_path(Square king_pos, const std::pair<Piece, Square>& checker,	std::vector<Square>& path) {

	path.push_back(checker.second);

	std::uint8_t min{ king_pos }, max{ checker.second }, step{ 7 };
	if (min > max)
		std::swap(min, max);

	if (checker.first == BBishop || checker.first == WBishop) {

		if ((max - min) % 7 != 0)
			step = 9;

		min += step;
		while (min != max) {
			path.push_back(static_cast<Square>(min));
			min += step;
		}
	}
	else if (checker.first == BRook || checker.first == WRook) {

		step = 8;
		if (rank(king_pos) == rank(checker.second))
			step = 1;
		min += step;
		while (min != max) {
			path.push_back(static_cast<Square>(min));
			min += step;
		}
	}
	else if (checker.first == BQueen || checker.first == WQueen) {

		if (rank(checker.second) == rank(king_pos) || file(checker.second) == file(king_pos)) {
			step = 8;
			if (rank(king_pos) == rank(checker.second))
				step = 1;
			min += step;
			while (min != max) {
				path.push_back(static_cast<Square>(min));
				min += step;
			}
		}
		else {
			if ((max - min) % 7 != 0)
				step = 9;
			min += step;
			while (min != max) {
				path.push_back(static_cast<Square>(min));
				min += step;
			}
		}
	}
}
