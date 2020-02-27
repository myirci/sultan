#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <array>
#include "Definitions.hpp"

class Move;

class Board {

public:
	
	Board();
	
	Board(const std::string& fen);

	void set_to_starting_position();

	bool operator==(const Board& rhs) const;

	std::string export_to_FEN() const;

	void import_from_FEN(const std::string& fen);

	void print_board(bool extended = false) const;

	void make_move(const Move& m);

	void make_move(Square from,	Square to, Piece promotion = Empty);

	void make_moves(std::vector<std::string>& tokens);

	void unmake_move(const Move& m);
	
	void compute_attacked_squares(Color c, Square king_pos, std::vector<std::pair<Piece, Square>>& checkers);

    void generate_moves(std::vector<Move>& moves);

	Piece get_captured() const { return captured; }
	Square get_en_passant_square() const { return en_passant_square;  }
	std::uint8_t get_castling_rights() const { return castling_rights; }
	void set_en_passant_square(Square ep) { en_passant_square = ep; }
	void set_castling_rights(std::uint8_t cs) { castling_rights = cs; }
	void set_captured(Piece p) { captured = p; }

	private:

	Piece brd[64];
	std::uint8_t halfmove_counter;
	std::uint16_t move_counter;
	Piece captured;
	Square en_passant_square;
	std::uint8_t castling_rights;
	Color side_to_move;
	
	void identify_pins(Square king_pos, std::vector<std::pair<Square, Square>>& pins);

	void generate_pawn_moves(Color c, Square pos, std::vector<Move>& moves);
	void generate_pinned_pawn_moves(Color c, Square pos, Square pinner, std::vector<Move>& moves);
	
	void generate_bishop_moves(Color c, Square pos,	std::vector<Move>& moves);
	void generate_pinned_bishop_moves(Color c, Square pos, Square pinner, std::vector<Move>& moves);
	
	void generate_rook_moves(Color c, Square pos, std::vector<Move>& moves);
	void generate_pinned_rook_moves(Color c, Square pos, Square pinner, std::vector<Move>& moves);
	
	void generate_knight_moves(Color c,	Square pos,	std::vector<Move>& moves);
	
	void compute_checking_path(Square king_pos, const std::pair<Piece, Square>& checker, std::vector<Square>& path);
	
	Check generate_king_moves(Color c, Square king_pos, std::pair<Piece, Square>& checker, std::vector<Move>& moves);

	void generate_moves_except_for_king(Color c, const std::vector<std::pair<Square, Square>>& pins, std::vector<Move>& moves);

	void attacked_squares_pawn(Square pos, Color c);

	void attacked_squares_bishop(Square pos, Color c);

	void attacked_squares_rook(Square pos, Color c);

	void attacked_squares_knight(Square pos, Color c);

	void attacked_squares_king(Square pos, Color c);

	void parse_FEN(const std::string& fen, std::array<std::string, 6>& fields) const;

	Square get_king_position(Color c);

	std::tuple<Square, Square, Piece> extract_move_from_string(const std::string& str);

	inline void set_castling_rights(Castling cst, bool flag) { flag ? (castling_rights |= cst) : (castling_rights &= (~cst)); }
	inline bool query_castling_rights(Castling cst) const    { return (castling_rights & cst) > 0; }

	inline void copy_castling_rights()                { castling_rights |= ((castling_rights & 0x0F) << 4); }
    inline void restore_castling_rights()             { castling_rights |= ((castling_rights & 0xF0) >> 4); }
    inline void set_white_castling_rights(bool flag)  { flag ? (castling_rights |= 3) : (castling_rights &= 0xFC); }
    inline void set_black_castling_rights(bool flag)  { flag ? (castling_rights |= 12) : (castling_rights &= 0xF3); }
    inline bool can_white_castle() const              { return (castling_rights & 0x03) > 0; }
    inline bool can_black_castle() const              { return (castling_rights & 0x0C) > 0; }
    inline bool can_white_castle_king_side() const    { return (castling_rights & 0x01) > 0; }
    inline bool can_white_castle_queen_side() const   { return (castling_rights & 0x02) > 0; }
    inline bool can_black_castle_king_side() const    { return (castling_rights & 0x04) > 0; }
    inline bool can_black_castle_queen_side() const   { return (castling_rights & 0x08) > 0; }
	inline bool can_castle_king_side(Color c) const   { return ((c == White) ? can_white_castle_king_side() : can_black_castle_king_side()); }
	inline bool can_castle_queen_side(Color c) const  { return ((c == White) ? can_white_castle_queen_side() : can_black_castle_queen_side()); }
};

#endif