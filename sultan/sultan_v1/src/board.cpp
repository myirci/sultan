#include "board.hpp"
#include "square.hpp"
#include "piece.hpp"
#include "definitions.hpp"

#include <algorithm>

Board::Board() : 
    side_to_move{ def::none }, 
    en_passant_loc{ def::none }, 
    castling_rights{ 0 }, 
    half_move_counter{ -1 }, 
    full_move_counter{ -1 }, 
    ploc{ nullptr },
    attacks{ nullptr } { std::fill(board, board + BOARDSIZE, def::none); }

Board::Board(Board const& brd) :
    side_to_move{ brd.side_to_move },
    en_passant_loc{ brd.en_passant_loc },
    castling_rights { brd.castling_rights },
    half_move_counter {brd.half_move_counter },
    full_move_counter{ brd.full_move_counter } 
{
    std::copy(brd.board, brd.board + BOARDSIZE, board);
}

int8_t* Board::get_board() { return  board; }

int8_t const* Board::get_board() const { return board; }

int8_t Board::get_side_to_move() const { return side_to_move; }

void Board::set_side_to_move(int8_t c) { side_to_move = c; }

int8_t Board::get_castling_rights() const { return castling_rights; }

bool Board::query_castling_rights(Castling c) const { return castling_rights & static_cast<int8_t>(c); }

void Board::set_castling_rights(Castling c, bool val) { castling_rights = val ? castling_rights | static_cast<int8_t>(c) : castling_rights & ~static_cast<int8_t>(c); }

void Board::set_castling_rights(int8_t color, bool val) 
{
    if (color == color::white) 
    {
        set_castling_rights(Castling::white_king_side, val);
        set_castling_rights(Castling::white_queen_side, val);
    }
    else 
    {
        set_castling_rights(Castling::black_king_side, val);
        set_castling_rights(Castling::black_queen_side, val);
    }
}

int16_t Board::get_half_move_counter() const { return half_move_counter; }

void Board::set_half_move_counter(int16_t val) { half_move_counter = val; }

int16_t Board::get_full_move_counter() const { return full_move_counter; }

void Board::set_full_move_counter(int16_t val) { full_move_counter = val; }

int8_t Board::get_en_passant_loc() const { return en_passant_loc; }

void Board::set_en_passant_loc(int8_t val) { en_passant_loc = val; }

int8_t Board::find_piece_location(int8_t ptype) const { return ploc->find_piece_location(ptype); }

std::pair<PieceLocation::PLConstIt, PieceLocation::PLConstIt> Board::find_piece_locations(int8_t ptype) const { return ploc->find_piece_locations(ptype); };

int8_t Board::num_checkers(int8_t king_pos) const { return attacks->num_checkers(king_pos); }

std::pair<int8_t, int8_t> Board::get_checker_positions(int8_t king_pos) const { return attacks->get_checker_positions(king_pos); }

bool Board::is_under_attack(int8_t attacking_side, int8_t target_sq, int8_t exclude_pos1, int8_t exclude_pos2) const { return attacks->is_under_attack(attacking_side, target_sq, exclude_pos1, exclude_pos2); }

int8_t Board::get_pin_direction(int8_t sq) const { return attacks->get_pin_direction(sq); }

std::pair<int8_t, int8_t> Board::get_attack_info(int8_t pos) const { return attacks->get_attack_info(pos); }

void Board::make_quite_move(int8_t from, int8_t to) 
{
    ploc->update_piece_loc(board[from], from, to);
    board[to] = board[from];
    board[from] = piece::eM;
}

state::BoardState Board::make_move(Move const& mv)
{
    state::BoardState st{ castling_rights, en_passant_loc, half_move_counter };

    int8_t from{ mv.get_from() }, to{ mv.get_to() };
    MoveType mtype{ mv.get_move_type() };

    // update half move counter
    half_move_counter = (board[from] * side_to_move == piece::Pawn || mv.is_capture()) ? 0 : half_move_counter + 1;

    // remove captured piece from the piece locations
    if (mv.is_capture()) 
    {
        if (mtype == MoveType::En_Passant_Capture) 
        {
            int8_t pawn_loc = en_passant_loc + side_to_move * direction::S;
            ploc->remove_piece(-side_to_move*piece::Pawn, pawn_loc);
            board[pawn_loc] = piece::eM;
        }
        else 
        {
            ploc->remove_piece(board[to], to);

            if (castling_rights != 0)
            {
                int8_t captured{ mv.get_captured_piece() };
                if (captured == piece::wR)
                {
                    if (to == square::h1)       set_castling_rights(Castling::white_king_side, false);
                    else if (to == square::a1)  set_castling_rights(Castling::white_queen_side, false);
                }
                else if (captured == piece::bR)
                {
                    if (to == square::h8)       set_castling_rights(Castling::black_king_side, false);
                    else if (to == square::a8)  set_castling_rights(Castling::black_queen_side, false);
                }
            }
        }
    }

    // update en passant location
    en_passant_loc = (mtype == MoveType::Double_Pawn_Push) ? (from + side_to_move * direction::N) : (def::none);
    
    if (mv.is_promotion())
    {
        int8_t p = side_to_move * piece::Bishop;
        if (mtype == MoveType::Queen_Promotion || mtype == MoveType::Queen_Promotion_Capture)           p = side_to_move * piece::Queen;
        else if (mtype == MoveType::Knight_Promotion || mtype == MoveType::Knight_Promotion_Capture)    p = side_to_move * piece::Knight;
        else if (mtype == MoveType::Rook_Promotion || mtype == MoveType::Rook_Promotion_Capture)        p = side_to_move * piece::Rook;

        ploc->remove_piece(board[from], from);
        ploc->add_piece(p, to);
        board[from] = piece::eM;
        board[to] = p;
    }
    else 
    {
        if (castling_rights != 0)
        {
            if (board[from] == piece::wK)
            {
                set_castling_rights(color::white, false);
            }
            else if (board[from] == piece::bK)
            {
                set_castling_rights(color::black, false);
            }
            else if (board[from] == piece::wR)
            {
                if (from == square::a1)         set_castling_rights(Castling::white_queen_side, false);
                else if (from == square::h1)    set_castling_rights(Castling::white_king_side, false);
            }
            else if (board[from] == piece::bR)
            {
                if (from == square::a8)         set_castling_rights(Castling::black_queen_side, false);
                else if (from == square::h8)    set_castling_rights(Castling::black_king_side, false);
            }
        }

        make_quite_move(from, to);

        if (mtype == MoveType::King_Side_Castle)
        {
            if (side_to_move == color::white)   make_quite_move(square::h1, square::f1);
            else                                make_quite_move(square::h8, square::f8);
        }
        else if (mtype == MoveType::Queen_Side_Castle)
        {
            if (side_to_move == color::white)   make_quite_move(square::a1, square::d1);
            else                                make_quite_move(square::a8, square::d8);
        }
    }

    if (side_to_move == color::black) full_move_counter++;
    
    side_to_move = -side_to_move;

    attacks->compute_checks_and_pins();

    return std::move(st);
}

void Board::unmake_move(Move const& mv, state::BoardState const& st)
{
    // restore side to move 
    side_to_move = -side_to_move;

    // restore the castling rights, en-passant location and half move counters
    castling_rights = st.castling_rights;
    en_passant_loc = st.en_passant_loc;
    half_move_counter = st.half_move_counter;

    // restore the full move counters
    if (side_to_move == color::black) 
        full_move_counter--;

    // restore pieces
    int8_t from{ mv.get_from() }, to{ mv.get_to() };
    MoveType mtype{ mv.get_move_type() };

    if (mv.is_promotion())
    {
        int8_t p1{ side_to_move * piece::Pawn };
        ploc->remove_piece(board[to], to);
        ploc->add_piece(p1, from);
        board[from] = p1;
        board[to] = piece::eM;
    }
    else 
    {
        make_quite_move(to, from);
        
        if (mtype == MoveType::King_Side_Castle)
        {
            if (side_to_move == color::white)   make_quite_move(square::f1, square::h1);
            else                                make_quite_move(square::f8, square::h8);
        }
        else if (mtype == MoveType::Queen_Side_Castle)
        {
            if (side_to_move == color::white)   make_quite_move(square::d1, square::a1);
            else                                make_quite_move(square::d8, square::a8);
        }
    }

    if (mv.is_capture())
    {
        if (mtype == MoveType::En_Passant_Capture)
        {
            int8_t pawn_loc = en_passant_loc + side_to_move * direction::S;
            int8_t p = -side_to_move * piece::Pawn;
            ploc->add_piece(p, pawn_loc);
            board[pawn_loc] = p;
        }
        else 
        {
            int8_t p{ mv.get_captured_piece() };
            board[to] = p;
            ploc->add_piece(p, to);
        }
    }

    attacks->compute_checks_and_pins();
}