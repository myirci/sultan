
#include <iostream>
#include <cctype>
#include <sstream>
#include "x88_fen.hpp"

X88_fen::X88_fen() {
	// Fen of starting positon;
	fen_str = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	piece_placement = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
	active_color = "w";
    castling_availability = "KQkq";
    en_passant = "-";
	half_move_clock = 0;
	full_move_number = 1;		
}

X88_fen::X88_fen(std::string f_str) : fen_str(f_str) {
    parse_fen_str();
}

void X88_fen::parse_fen_str() {
    std::stringstream ss;
    ss << fen_str;
    ss >> piece_placement
       >> active_color
       >> castling_availability
       >> en_passant
       >> half_move_clock
       >> full_move_number;
}

void X88_fen::print_fen_str() const {
	std::cout << fen_str << std::endl;
}

void X88_fen::print_fen_fields() const {
    std::cout << "Piece Placement : " << piece_placement << "\n"
              << "Active Color    : " << active_color << "\n"
              << "Castling Avail  : " << castling_availability << "\n"
              << "En Passant      : " << en_passant << "\n"
              << "Half Move Clock : " << half_move_clock << "\n"
              << "Full Move Number: " << full_move_number << "\n";
}

void X88_fen::update_fen_str(std::string str) {
    fen_str = str;
    parse_fen_str();
}

void X88_fen::transfer_to_board(X88_board& b) {

    char c;
    std::int8_t strlen = 0;
    std::int8_t file = 0;
    std::int8_t rank = 0;
    std::int8_t index = 0;
    std::string rank_str[8];
    std::stringstream ss;

	// Field-1: Piece Placement
    ss << piece_placement;

    X88_board::piece_iterator bpit;
    for(bpit = b.black_pieces_begin(); bpit != b.black_pieces_end(); ++bpit) {
        *bpit = X88_board::dummy;
    }

    X88_board::piece_iterator wpit;
    for(wpit = b.white_pieces_begin(); wpit != b.white_pieces_end(); ++wpit) {
        *wpit = X88_board::dummy;
    }

    for(int i = 0; i < 8; ++i) {
        std::getline(ss, rank_str[i], '/');
    }
    std::int8_t * const board = b.get_board();
    wpit = b.white_pieces_begin();
    bpit = b.black_pieces_begin();
    for(rank = 7; rank >= 0; --rank) {     // rank7 = rank_str[0], rank6 = rank_str[1], ... , rank0 = rank_str[7]
        strlen = rank_str[rank].length();  // get the legnth of the string that corresponds to the current rank
        index = (7 - rank) * 16;
        for(int i = 0; i < strlen; ++i) {  // process the rank string character by character, before starting zeroize the file
            c = rank_str[rank].at(i);      // get the ith character from the current rank string
            if(std::isdigit(c)) {          // if the character is digit, place int(char - '0') emtpy squares on the board
                for(int k = 0; k < int(c - '0'); ++k) {
                    board[index] = static_cast<std::int8_t>(Pieces::empty);
                    index++;
                }
            }
            else {
                switch(c) {
                    case 'P':
                        board[index] = static_cast<std::int8_t>(Pieces::wP);
                        break;
                    case 'p':
                        board[index] = static_cast<std::int8_t>(Pieces::bP);
                        break;
                    case 'R':
                        board[index] = static_cast<std::int8_t>(Pieces::wR);
                        break;
                    case 'r':
                        board[index] = static_cast<std::int8_t>(Pieces::bR);
                        break;
                    case 'B':
                        board[index] = static_cast<std::int8_t>(Pieces::wB);
                        break;
                    case 'b':
                        board[index] = static_cast<std::int8_t>(Pieces::bB);
                        break;
                    case 'N':
                        board[index] = static_cast<std::int8_t>(Pieces::wN);
                        break;
                    case 'n':
                        board[index] = static_cast<std::int8_t>(Pieces::bN);
                        break;
                    case 'K':
                        board[index] = static_cast<std::int8_t>(Pieces::wK);
                        break;
                    case 'k':
                        board[index] = static_cast<std::int8_t>(Pieces::bK);
                        break;
                    case 'Q':
                        board[index] = static_cast<std::int8_t>(Pieces::wQ);
                        break;
                    case 'q':
                        board[index] = static_cast<std::int8_t>(Pieces::bQ);
                        break;
                }
                if(std::isupper(c)) {           // white piece
                    *wpit = index;
                    ++wpit;
                    index++;
                }
                else {                          // black piece
                    *bpit = index;
                    ++bpit;
                    index++;
                }
            }
        }
    }
    // Field-2: Active Color
    if(active_color == "w") {
        b.set_state_property(State::active_color, true);
    }
    else if(active_color == "b") {
        b.set_state_property(State::active_color, false);
    }
	// Field-3: Castling Availability
    strlen = castling_availability.length();
    b.set_state_property(State::wks_castling, false);
    b.set_state_property(State::wqs_castling, false);
    b.set_state_property(State::bks_castling, false);
    b.set_state_property(State::bqs_castling, false);
    for(int i = 0; i < strlen; ++i) {
        c = castling_availability.at(i);
        switch(c) {
            case 'K':
                b.set_state_property(State::wks_castling, true);
                break;
            case 'Q':
                b.set_state_property(State::wqs_castling, true);
                break;
            case 'k':
                b.set_state_property(State::bks_castling, true);
                break;
            case 'q':
                b.set_state_property(State::bqs_castling, true);
                break;
            case '-':
                break;
        }
    }
	// Field-4: En Passant
    if(en_passant.length() == 2) {
        file = static_cast<std::uint8_t>(std::tolower(en_passant.at(0)) - 'a');
        rank = static_cast<std::uint8_t>(en_passant.at(1) - '0') - 1;
        b.set_en_passant_loc(rank*16 + file);
    }
    // Field-5: Half Move Clock
    b.set_half_move_num(half_move_clock);
	// Field-6: Full Move Nummber
    b.set_move_num(full_move_number);
}

