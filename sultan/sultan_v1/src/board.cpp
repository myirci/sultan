#include "board.hpp"
#include "square.hpp"
#include "piece.hpp"
#include "definitions.hpp"

#include <algorithm>

Board::Board()
{
    clear();
}

void Board::clear()
{
    std::fill(board, board + BOARDSIZE, def::none);
    en_passant_loc = def::none;
    castling_rights = 0;
    half_move_counter = -1;
    full_move_counter = -1;
    side_to_move = def::none;
    piece_loc.clear();
}

int8_t Board::get_side_to_move() const { return side_to_move; }

void Board::set_side_to_move(int8_t c) { side_to_move = c; }

int8_t Board::get_castling_rights() const { return castling_rights; }

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
    board[from] = piece::eM;
}

void Board::update_castling_rights(Move m)
{
    /*
    if (castling_rights == 0) return;

    int8_t from{ m.get_from() };
    if (board[from] == piece::wK)
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
    else if (board[from] == piece::bK)
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
    else if (board[from] == piece::wR)
    {
        if (from == square::a1 && query_castling_rights(Castling::white_queen_side))
        {
            stored_castling_rights = castling_rights;
            set_castling_rights(Castling::white_queen_side, false);
        }
        else if (from == square::h1 && query_castling_rights(Castling::white_king_side))
        {
            stored_castling_rights = castling_rights;
            set_castling_rights(Castling::white_king_side, false);
        }
    }
    else if (board[from] == piece::bR)
    {
        if (from == square::a8 && query_castling_rights(Castling::black_queen_side))
        {
            stored_castling_rights = castling_rights;
            set_castling_rights(Castling::black_queen_side, false);
        }
        else if (from == square::h8 && query_castling_rights(Castling::black_king_side))
        {
            stored_castling_rights = castling_rights;
            set_castling_rights(Castling::black_king_side, false);
        }
    }

    if (m.is_capture())
    {
        int8_t to{ m.get_to() };
        if (board[to] == piece::wR && to == square::h1 && query_castling_rights(Castling::white_king_side))
        {
            stored_castling_rights = castling_rights;
            set_castling_rights(Castling::white_king_side, false);
        }
        else if (board[to] == piece::wR && to == square::a1 && query_castling_rights(Castling::white_queen_side))
        {
            stored_castling_rights = castling_rights;
            set_castling_rights(Castling::white_queen_side, false);
        }
        if (board[to] == piece::bR && to == square::h8 && query_castling_rights(Castling::black_king_side))
        {
            stored_castling_rights = castling_rights;
            set_castling_rights(Castling::black_king_side, false);
        }
        else if (board[to] == piece::bR && to == square::a8 && query_castling_rights(Castling::black_queen_side))
        {
            stored_castling_rights = castling_rights;
            set_castling_rights(Castling::black_queen_side, false);
        }
    }
    */
}

void Board::make_move(Move m) 
{
    /*
    int8_t from{m.get_from()}, to{m.get_to()}, stm{get_side_to_move()};
    MoveType flg{m.get_move_type()};

    update_castling_rights(m);

    if (flg == MoveType::Quite)
    {
        make_quite_move(from, to);
    }
    else if (flg == MoveType::Capture) 
    {
        stored_captured_piece = board[to];
        remove_piece(to);
        make_quite_move(from, to);
    }
    else if (flg == MoveType::King_Side_Castle)
    {
        make_quite_move(from, to);
        if (stm == color::white) make_quite_move(square::h1, square::f1);
        else make_quite_move(square::h8, square::f8);
    }
    else if (flg == MoveType::Queen_Side_Castle)
    {
        make_quite_move(from, to);
        if (stm == color::white) make_quite_move(square::a1, square::d1);
        else make_quite_move(square::a8, square::d8);
    }
    else if (flg == MoveType::En_Passant_Capture)
    {
        stored_en_passant_loc = en_passant_loc;
        remove_piece(en_passant_loc + stm * direction::S);
        make_quite_move(from, to);
        board[en_passant_loc + stm * direction::S] = piece::eM;
    }

    if (flg == MoveType::Double_Pawn_Push)
    {
        en_passant_loc = from + stm * direction::N;
        make_quite_move(from, to);
    }
    else 
    {
        en_passant_loc = def::none;
    }

    if (m.is_promotion())
    {
        int8_t p = stm * piece::Bishop;
        if (flg == MoveType::Queen_Promotion) p = stm * piece::Queen;
        else if (flg == MoveType::Knight_Promotion) p = stm * piece::Knight;
        else if (flg == MoveType::Rook_Promotion) p = stm * piece::Rook;

        remove_piece(from);
        if (m.is_capture()) remove_piece(to);
        add_piece(p, to);

        board[from] = piece::eM;
        board[to] = p;
    }

    if (!m.is_capture())
        stored_captured_piece = def::none;

    half_move_counter++;
    if (side_to_move == color::black) full_move_counter++;
    side_to_move = -side_to_move;
    */
}

void Board::unmake_move(Move m)
{
    /*
    castling_rights = stored_castling_rights;
    en_passant_loc = stored_en_passant_loc;

    int8_t from{ m.get_from() }, to{ m.get_to() }, stm{ get_side_to_move() };
    MoveType flg{ m.get_move_type() };

    if (flg == MoveType::Quite || flg == MoveType::Double_Pawn_Push)
    {
        make_quite_move(to, from);
    }
    else if (flg == MoveType::Capture)
    {
        make_quite_move(to, from);
        board[to] = stored_captured_piece;
        add_piece(stored_captured_piece, to);
    }
    else if (flg == MoveType::King_Side_Castle)
    {
        make_quite_move(to, from);
        if (stm == color::black) make_quite_move(square::f1, square::h1);
        else make_quite_move(square::f8, square::h8);
    }
    else if (flg == MoveType::Queen_Side_Castle)
    {
        make_quite_move(from, to);
        if (stm == color::black) make_quite_move(square::d1, square::h1);
        else make_quite_move(square::d8, square::a8);
    }
    else if (flg == MoveType::En_Passant_Capture)
    {
        make_quite_move(to, from);
        int8_t loc = en_passant_loc + stm * direction::N;
        int8_t p = stm * piece::Pawn;
        add_piece(loc, p);
        board[loc] = p;
    }

    if (m.is_promotion())
    {
        int8_t p = -stm * piece::Pawn;
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
    if (side_to_move == color::white) full_move_counter--;
    side_to_move = -side_to_move;
    */
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


