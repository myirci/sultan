#include <iostream>
#include <cmath>
#include "x88_board.hpp"
#include <algorithm>

const std::int8_t X88_board::state1             = 0x28;
const std::int8_t X88_board::en_passant_loc     = 0x2B;
const std::int8_t X88_board::hmove_num_upper    = 0x2C;
const std::int8_t X88_board::hmove_num_lower    = 0x2D;
const std::int8_t X88_board::move_num_upper     = 0x2E;
const std::int8_t X88_board::move_num_lower     = 0x2F;
const std::int8_t X88_board::white_pieces_start = 0x08;
const std::int8_t X88_board::black_pieces_start = 0x78;
const std::int8_t X88_board::board_start        = 0x00;
const std::int8_t X88_board::dummy              = 0x80;

X88_board::X88_board() {
    initialize_to_starting_position();
}

void X88_board::initialize_to_starting_position() {

    // Initialize the board as empty
    for(int i = 0; i < 128; ++i) {
        board[i] = static_cast<std::int8_t>(Pieces::empty);
    }

    // place the white pieces:
    board[0] = board[7] = static_cast<std::int8_t>(Pieces::wR);
    board[1] = board[6] = static_cast<std::int8_t>(Pieces::wN);
    board[2] = board[5] = static_cast<std::int8_t>(Pieces::wB);
    board[3] = static_cast<std::int8_t>(Pieces::wQ);
    board[4] = static_cast<std::int8_t>(Pieces::wK);
    for(int i = 16; i <= 23; ++i) {
        board[i] = static_cast<std::int8_t>(Pieces::wP);
    }

    // place the black pieces:
    board[112] = board[119] = static_cast<std::int8_t>(Pieces::bR);
    board[113] = board[118] = static_cast<std::int8_t>(Pieces::bN);
    board[114] = board[117] = static_cast<std::int8_t>(Pieces::bB);
    board[115] = static_cast<std::int8_t>(Pieces::bQ);
    board[116] = static_cast<std::int8_t>(Pieces::bK);
    for(int i = 96; i <= 103; ++i) {
        board[i] = static_cast<std::int8_t>(Pieces::bP);
    }

    // keep track the white and black piece locations:
    X88_board::piece_iterator wp_it = white_pieces_begin();
    X88_board::piece_iterator bp_it = black_pieces_begin();

    for(int i = 0; i < 8; ++i, ++wp_it, ++bp_it) {
        *wp_it = i;
        *bp_it = 112 + i;
    }

    for(int i = 8; i < 16; ++i, ++wp_it, ++bp_it) {
        *wp_it  = i + 8;
        *bp_it = i + 88;
    }

    // set active_color: true for white, false for black
    set_state_property(State::active_color, true);

    // set castling rights
    set_state_property(State::wks_castling, true);
    set_state_property(State::wqs_castling, true);
    set_state_property(State::bks_castling, true);
    set_state_property(State::bqs_castling, true);

    // En-passant loc
    board[en_passant_loc] = 0x7F;

    // half_move_num and move_num
    set_half_move_num(0);
    set_state_property(State::fifty_rule_draw, false);
    set_move_num(1);
}

std::int8_t const * const X88_board::get_board() const {
    return board;
}

std::int8_t * const X88_board::get_board() {
    return board;
}

void X88_board::set_square(std::int8_t loc, Pieces p) {
    if((loc & 0x88) == 0) {
        board[loc] = static_cast<std::int8_t>(p);
    }
    else {
        std::cerr << "ERROR: [set_square]: "
                  << "Location is outside the board" << std::endl;
    }
}

bool X88_board::get_state_property(State s) const {
    std::int8_t mask = static_cast<std::int8_t>(s);
    return (mask == (mask & board[state1]));
}

void X88_board::set_state_property(State s, bool b) {
    std::int8_t mask = static_cast<std::int8_t>(s);
    (b) ? (board[state1] |= mask) : (board[state1] &= (~mask));
}

std::int16_t X88_board::get_move_num() const {
    if(board[move_num_upper] == 0) {
        return static_cast<std::int16_t>(board[move_num_lower]);
    }
    else {
        return static_cast<std::int16_t>(board[move_num_upper]*128 + board[move_num_lower]);
    }
}

void X88_board::set_move_num(std::int16_t mn) {
    board[move_num_upper] = mn / 128;
    board[move_num_lower] = mn % 128;
}

void X88_board::increment_move_num() {
    if(board[move_num_lower] == 127) {
       board[move_num_lower] = 0;
       board[move_num_upper]++;
    }
    else {
        board[move_num_lower]++;
    }
}

std::int16_t X88_board::get_half_move_num() const{
    if(board[hmove_num_upper] == 0) {
        return static_cast<std::int16_t>(board[hmove_num_lower]);
    }
    else {
        return static_cast<std::int16_t>(board[hmove_num_upper]*128 + board[hmove_num_lower]);
    }
}

void X88_board::set_half_move_num(std::int16_t hmn) {
    board[hmove_num_upper] = hmn / 128;
    board[hmove_num_lower] = hmn % 128;
}

void X88_board::increment_half_move_num() {
    if(board[hmove_num_lower] == 127) {
       board[hmove_num_lower] = 0;
       board[hmove_num_upper]++;
    }
    else {
        board[hmove_num_lower]++;
    }
}

std::int8_t X88_board::get_en_passant_loc() const {
    return board[en_passant_loc];
}

void X88_board::set_en_passant_loc(std::int8_t ep_loc) {
    if((ep_loc & 0x88) == 0) {
        board[en_passant_loc] = ep_loc;
    }
    else {
        std::cerr << "ERROR: [set_en_passant_loc]: "
                  << "Location is outside the board" << std::endl;
    }
}

void X88_board::generate_moves(std::vector<Move>& moves) {
    if(get_state_property(State::active_color)) {
        generate_moves_new<White>(moves);
    }
    else {
        generate_moves_new<Black>(moves);
    }
}

bool X88_board::check_sliding_pieces_for_attack(std::int8_t piece_loc, std::int8_t sq_loc, std::int8_t val) const{
    std::int8_t small_loc = ((sq_loc > piece_loc) ? piece_loc : sq_loc);
    std::int8_t dist = (static_cast<std::int8_t>(std::abs(sq_loc - piece_loc)) / val) - 1;
    if(dist == 0) { return true; }
    else {
        for(int i = 0; i < dist; ++i) {
            small_loc += val;
            if(board[small_loc] != static_cast<std::int8_t>(Pieces::empty)) { return false; }
        }
    }
    return true;
}

bool X88_board::check_diagonal_sliding_pieces_for_attack(std::int8_t piece_loc, std::int8_t sq_loc) const {
    if((static_cast<std::int8_t>(std::abs(sq_loc - piece_loc)) % 15) == 0) {        // diagonal-1
        if(check_sliding_pieces_for_attack(piece_loc, sq_loc, 15)) { return true; }
    }
    else if((static_cast<std::int8_t>(std::abs(sq_loc - piece_loc)) % 17) == 0) {   // diagonal-2
        if(check_sliding_pieces_for_attack(piece_loc, sq_loc, 17)) { return true; }
    }
    return false;
}

bool X88_board::check_straight_sliding_pieces_for_attack(std::int8_t piece_loc, std::int8_t sq_loc) const {
    if((sq_loc >> 4) == (piece_loc >> 4)) {     // check rank
        if(check_sliding_pieces_for_attack(piece_loc, sq_loc, 1)) { return true; }
    }
    else if((sq_loc & 7) == (piece_loc & 7)) {  // check file
        if(check_sliding_pieces_for_attack(piece_loc, sq_loc, 16)) { return true; }
    }
    return false;
}


