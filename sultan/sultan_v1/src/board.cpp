#include "board.hpp"
#include "square.hpp"
#include "piece.hpp"
#include "definitions.hpp"

#include <algorithm>
#include <array>
#include <stdexcept>

Board::Board()
{
    clear();
}

void Board::clear()
{
    std::fill(board, board + BOARDSIZE, def::none);
    en_passant_loc = def::none;
    stored_en_passant_loc = def::none;
    castling_rights = 0;
    stored_castling_rights = 0;
    half_move_counter = -1;
    full_move_counter = -1;
    side_to_move = def::none;
    stored_captured_piece = def::none;
    piece_loc.clear();
}

int8_t Board::get_side_to_move() const { return side_to_move; }

void Board::set_side_to_move(int8_t c) { side_to_move = c; }

bool Board::query_castling_rights(Castling c) const { return castling_rights & static_cast<int8_t>(c); }

void Board::set_castling_rights(Castling c, bool val) { castling_rights = val ? castling_rights | static_cast<int8_t>(c) : castling_rights & ~static_cast<int8_t>(c); }

int8_t* Board::get_board() { return  board; }

int8_t const * Board::get_board() const { return board; }

int16_t Board::get_half_move_counter() const { return half_move_counter; }

void Board::set_half_move_counter(int16_t val) { half_move_counter = val; }

int16_t Board::get_full_move_counter() const { return full_move_counter; }

void Board::set_full_move_counter(int16_t val) { full_move_counter = val; }

int8_t Board::get_en_passant_loc() const { return en_passant_loc; }

void Board::set_en_passant_loc(int8_t val) { en_passant_loc = val; }

void Board::set_piece_locations(std::unordered_multimap<int8_t, int8_t>&& plocs) { piece_loc = std::move(plocs);  }

std::unordered_multimap<int8_t, int8_t>& Board::get_piece_locations() { return piece_loc;  };

std::unordered_multimap<int8_t, int8_t> const& Board::get_piece_locations() const { return piece_loc;  };

void Board::make_quite_move(int8_t from, int8_t to) 
{
    update_piece_loc(from, to);
    board[to] = board[from];
    board[from] = Piece::eM;
}

void Board::update_castling_rights(Move m)
{
    if (castling_rights == 0) return;

    int8_t from{ m.get_from() };
    if (board[from] == Piece::wK)
    {
        if (query_castling_rights(Castling::white_king_side))
        {
            stored_castling_rights = castling_rights;
            set_castling_rights(Castling::white_king_side, false);
        }
        if (query_castling_rights(Castling::white_queen_side))
        {
            stored_castling_rights = castling_rights;
            set_castling_rights(Castling::white_queen_side, false);
        }
    }
    else if (board[from] == Piece::bK)
    {
        if (query_castling_rights(Castling::black_king_side))
        {
            stored_castling_rights = castling_rights;
            set_castling_rights(Castling::black_king_side, false);
        }
        if (query_castling_rights(Castling::black_queen_side))
        {
            stored_castling_rights = castling_rights;
            set_castling_rights(Castling::black_queen_side, false);
        }
    }
    else if (board[from] == Piece::wR)
    {
        if (from == Square::a1 && query_castling_rights(Castling::white_queen_side))
        {
            stored_castling_rights = castling_rights;
            set_castling_rights(Castling::white_queen_side, false);
        }
        else if (from == Square::h1 && query_castling_rights(Castling::white_king_side))
        {
            stored_castling_rights = castling_rights;
            set_castling_rights(Castling::white_king_side, false);
        }
    }
    else if (board[from] == Piece::bR)
    {
        if (from == Square::a8 && query_castling_rights(Castling::black_queen_side))
        {
            stored_castling_rights = castling_rights;
            set_castling_rights(Castling::black_queen_side, false);
        }
        else if (from == Square::h8 && query_castling_rights(Castling::black_king_side))
        {
            stored_castling_rights = castling_rights;
            set_castling_rights(Castling::black_king_side, false);
        }
    }

    if (m.is_capture())
    {
        int8_t to{ m.get_to() };
        if (board[to] == Piece::wR && to == Square::h1 && query_castling_rights(Castling::white_king_side))
        {
            stored_castling_rights = castling_rights;
            set_castling_rights(Castling::white_king_side, false);
        }
        else if (board[to] == Piece::wR && to == Square::a1 && query_castling_rights(Castling::white_queen_side))
        {
            stored_castling_rights = castling_rights;
            set_castling_rights(Castling::white_queen_side, false);
        }
        if (board[to] == Piece::bR && to == Square::h8 && query_castling_rights(Castling::black_king_side))
        {
            stored_castling_rights = castling_rights;
            set_castling_rights(Castling::black_king_side, false);
        }
        else if (board[to] == Piece::bR && to == Square::a8 && query_castling_rights(Castling::black_queen_side))
        {
            stored_castling_rights = castling_rights;
            set_castling_rights(Castling::black_queen_side, false);
        }
    }
}

void Board::make_move(Move m) 
{
    int8_t from{m.get_from()}, to{m.get_to()}, stm{get_side_to_move()};
    Move_flag flg{m.get_move_flag()};

    update_castling_rights(m);

    if (flg == Move_flag::Quite)
    {
        make_quite_move(from, to);
    }
    else if (flg == Move_flag::Capture) 
    {
        stored_captured_piece = board[to];
        remove_piece(to);
        make_quite_move(from, to);
    }
    else if (flg == Move_flag::King_Side_Castle)
    {
        make_quite_move(from, to);
        if (stm == Piece::white) make_quite_move(Square::h1, Square::f1);
        else make_quite_move(Square::h8, Square::f8);
    }
    else if (flg == Move_flag::Queen_Side_Castle)
    {
        make_quite_move(from, to);
        if (stm == Piece::white) make_quite_move(Square::a1, Square::d1);
        else make_quite_move(Square::a8, Square::d8);
    }
    else if (flg == Move_flag::En_Passant_Capture)
    {
        stored_en_passant_loc = en_passant_loc;
        remove_piece(en_passant_loc + stm * Square::Directions::S);
        make_quite_move(from, to);
        board[en_passant_loc + stm * Square::Directions::S] = Piece::eM;
    }

    if (flg == Move_flag::Double_Pawn_Push)
    {
        en_passant_loc = from + stm * Square::Directions::N;
        make_quite_move(from, to);
    }
    else 
    {
        en_passant_loc = def::none;
    }

    if (m.is_promotion())
    {
        int8_t p = stm * Piece::Bishop;
        if (flg == Move_flag::Queen_Promotion) p = stm * Piece::Queen;
        else if (flg == Move_flag::Knight_Promotion) p = stm * Piece::Knight;
        else if (flg == Move_flag::Rook_Promotion) p = stm * Piece::Rook;

        remove_piece(from);
        if (m.is_capture()) remove_piece(to);
        add_piece(p, to);

        board[from] = Piece::eM;
        board[to] = p;
    }

    if (!m.is_capture())
        stored_captured_piece = def::none;

    half_move_counter++;
    if (side_to_move == Piece::black) full_move_counter++;
    side_to_move = -side_to_move;
}

void Board::unmake_move(Move m)
{
    castling_rights = stored_castling_rights;
    en_passant_loc = stored_en_passant_loc;

    int8_t from{ m.get_from() }, to{ m.get_to() }, stm{ get_side_to_move() };
    Move_flag flg{ m.get_move_flag() };

    if (flg == Move_flag::Quite || flg == Move_flag::Double_Pawn_Push)
    {
        make_quite_move(to, from);
    }
    else if (flg == Move_flag::Capture)
    {
        make_quite_move(to, from);
        board[to] = stored_captured_piece;
        add_piece(stored_captured_piece, to);
    }
    else if (flg == Move_flag::King_Side_Castle)
    {
        make_quite_move(to, from);
        if (stm == Piece::black) make_quite_move(Square::f1, Square::h1);
        else make_quite_move(Square::f8, Square::h8);
    }
    else if (flg == Move_flag::Queen_Side_Castle)
    {
        make_quite_move(from, to);
        if (stm == Piece::black) make_quite_move(Square::d1, Square::h1);
        else make_quite_move(Square::d8, Square::a8);
    }
    else if (flg == Move_flag::En_Passant_Capture)
    {
        make_quite_move(to, from);
        int8_t loc = en_passant_loc + stm * Square::Directions::N;
        int8_t p = stm * Piece::Pawn;
        add_piece(loc, p);
        board[loc] = p;
    }

    if (m.is_promotion())
    {
        int8_t p = -stm * Piece::Pawn;
        remove_piece(to);
        add_piece(p, from);
        if (m.is_capture()) 
        {
            board[to] = stored_captured_piece;
            add_piece(stored_captured_piece, to);
        }

        board[from] = p;
        board[to] = stored_captured_piece;
    }

    half_move_counter--;
    if (side_to_move == Piece::white) full_move_counter--;
    side_to_move = -side_to_move;
}

void Board::update_piece_loc(int8_t old_loc, int8_t new_loc)
{
    auto r = piece_loc.equal_range(board[old_loc]);
    for (auto it = r.first; it != r.second; it++) 
        if (it->second == old_loc)
            it->second = new_loc;
}

void Board::remove_piece(int8_t loc)
{
    auto r = piece_loc.equal_range(board[loc]);
    for (auto it = r.first; it != r.second; it++) 
    {
        if (it->second == loc) 
        {
            piece_loc.erase(it);
            return;
        }
    }
}

void Board::add_piece(int8_t p, int8_t loc) { piece_loc.insert(std::make_pair(p, loc)); }


