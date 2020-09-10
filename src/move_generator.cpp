#include "move_generator.hpp"
#include "square.hpp"
#include "piece.hpp"
#include "definitions.hpp"

#include <iostream>
#include <stdexcept>

void MoveGenerator::generate_king_moves(Board const& board, std::vector<Move>& moves)
{
    auto brd = board.get_board();
    int8_t stm{ board.get_side_to_move() };

    int8_t next_pos{ 0 }, king_pos{ board.find_piece_location(stm * piece::King) };
    for (int i{ 0 }; i < 8; i++)
    {
        next_pos = king_pos + direction::all_dirs[i];
        if (next_pos & square::inside || piece::is_same_sign(brd[next_pos], stm) || board.is_under_attack(-stm, next_pos, king_pos))
            continue;

        if (brd[next_pos] == piece::eM)   moves.emplace_back(Move(king_pos, next_pos, MoveType::Quite, def::none));
        else                              moves.emplace_back(Move(king_pos, next_pos, MoveType::Capture, brd[next_pos]));
    }

    if (board.num_checkers(king_pos) > 0) return;

    if (stm == color::white)
    {
        if (board.query_castling_rights(Castling::white_king_side) && 
            brd[square::f1] == piece::eM && brd[square::g1] == piece::eM && 
            !board.is_under_attack(color::black, square::f1) && !board.is_under_attack(color::black, square::g1))
            moves.emplace_back(Move(king_pos, square::g1, MoveType::King_Side_Castle, def::none));

        if (board.query_castling_rights(Castling::white_queen_side) && 
            brd[square::b1] == piece::eM && brd[square::c1] == piece::eM && brd[square::d1] == piece::eM &&
            !board.is_under_attack(color::black, square::d1) && !board.is_under_attack(color::black, square::c1))
            moves.emplace_back(Move(king_pos, square::c1, MoveType::Queen_Side_Castle, def::none));
    }
    else
    {
        if (board.query_castling_rights(Castling::black_king_side) && 
            brd[square::f8] == piece::eM && brd[square::g8] == piece::eM && 
            !board.is_under_attack(color::white, square::f8) && !board.is_under_attack(color::white, square::g8))
            moves.emplace_back(Move(king_pos, square::g8, MoveType::King_Side_Castle, def::none));

        if (board.query_castling_rights(Castling::black_queen_side) && 
            brd[square::b8] == piece::eM && brd[square::c8] == piece::eM && brd[square::d8] == piece::eM &&
            !board.is_under_attack(color::white, square::d8) && !board.is_under_attack(color::white, square::c8))
            moves.emplace_back(Move(king_pos, square::c8, MoveType::Queen_Side_Castle, def::none));
    }
}

// generates non-king check eliminating moves, precondition: single check
void MoveGenerator::generate_check_eliminating_moves(Board const& board, int8_t king_pos, std::vector<Move>& moves)
{
    auto [attacker_pos, attack_dir] = board.get_attack_info(king_pos);
    if (attack_dir != direction::ND)
    {
        int8_t next{ king_pos };
        while ((next = next + attack_dir) != attacker_pos)
            generate_to_square_moves(board, next, moves);
    }
    
    generate_to_square_moves(board, attacker_pos, moves);
}

// generate non-king to square moves, precondition: single check
void MoveGenerator::generate_to_square_moves(Board const& board, int8_t sq, std::vector<Move>& moves)
{
    auto brd = board.get_board();
    int8_t stm{ board.get_side_to_move() };

    MoveType mtype{ MoveType::Quite };
    int8_t captured{ def::none };
    if (brd[sq] != piece::eM) 
    {
        captured = brd[sq];
        mtype = MoveType::Capture;
    }

    for (int i{ 0 }; i < 4; i++)
    {
        int8_t next{ sq };
        while (!((next = next + direction::flat_dirs[i]) & square::inside))
        {
            if (brd[next] == piece::eM) continue;

            if (piece::is_same_sign(stm, brd[next]))
            {
                int8_t p = stm * brd[next];
                if ((p == piece::Rook || p == piece::Queen) && board.get_pin_direction(next) == direction::ND)
                    moves.emplace_back(Move(next, sq, mtype, captured));
            }
            break;
        }
    }
    
    for (int i{ 0 }; i < 4; i++)
    {
        int8_t next{ sq };
        while (!((next = next + direction::diagonal_dirs[i]) & square::inside))
        {
            if (brd[next] == piece::eM) continue;

            if (piece::is_same_sign(stm, brd[next]))
            {
                int8_t p = stm * brd[next];
                if ((p == piece::Bishop || p == piece::Queen) && board.get_pin_direction(next) == direction::ND)
                    moves.emplace_back(next, sq, mtype, captured);
            }
            break;
        }
    }

    for (int i{ 0 }; i < 8; i++)
    {
        int8_t next = sq + direction::knight_jumps[i];
        if (!(next & square::inside) && stm * brd[next] == piece::Knight && board.get_pin_direction(next) == direction::ND)
            moves.emplace_back(next, sq, mtype, captured);  
    }

    if((stm == color::white && sq >= square::a3 && sq <= square::h8) || (stm == color::black && sq >= square::a1 && sq <= square::h6))
    {
        if (mtype == MoveType::Quite)
        {
            int8_t next = sq + stm * direction::S;
            if (stm * brd[next] == piece::Pawn && board.get_pin_direction(next) == direction::ND)
            {
                int8_t rank = square::rank(sq);
                if ((stm == color::white && rank == 7) || (stm == color::black && rank == 0))
                {
                    moves.emplace_back(next, sq, MoveType::Queen_Promotion, def::none);
                    moves.emplace_back(next, sq, MoveType::Rook_Promotion, def::none);
                    moves.emplace_back(next, sq, MoveType::Bishop_Promotion, def::none);
                    moves.emplace_back(next, sq, MoveType::Knight_Promotion, def::none);
                }
                else
                {
                    moves.emplace_back(next, sq, MoveType::Quite, def::none);
                }
            }

            int8_t pos = next + stm * direction::S;

            if (stm * brd[pos] == piece::Pawn && brd[next] == piece::eM && board.get_pin_direction(pos) == direction::ND &&
                ((stm == color::white && square::rank(pos) == 1) || (stm == color::black && square::rank(pos) == 6)))
                moves.emplace_back(pos, sq, MoveType::Double_Pawn_Push, def::none);
        }
        else if (mtype == MoveType::Capture)
        {
            int8_t pos[2] = {
                static_cast<int8_t>(sq + stm * direction::SE),
                static_cast<int8_t>(sq + stm * direction::SW) };

            for (auto i = 0; i < 2; i++)
            {
                if (!(pos[i] & square::inside) && stm * brd[pos[i]] == piece::Pawn && board.get_pin_direction(pos[i]) == direction::ND)
                {
                    int8_t rank = square::rank(sq);
                    if ((stm == color::white && rank == 7) || (stm == color::black && rank == 0))
                    {
                        moves.emplace_back(pos[i], sq, MoveType::Queen_Promotion_Capture, captured);
                        moves.emplace_back(pos[i], sq, MoveType::Rook_Promotion_Capture, captured);
                        moves.emplace_back(pos[i], sq, MoveType::Bishop_Promotion_Capture, captured);
                        moves.emplace_back(pos[i], sq, MoveType::Knight_Promotion_Capture, captured);
                    }
                    else
                    {
                        moves.emplace_back(pos[i], sq, MoveType::Capture, captured);
                    }
                }
            }

            int8_t ep = board.get_en_passant_loc();
            if (ep != def::none && sq == ep + stm * direction::S)
            {
                int8_t pawn_pos[2] =
                {
                    static_cast<int8_t>(ep + stm * direction::SW),
                    static_cast<int8_t>(ep + stm * direction::SE)
                };

                for (int i = 0; i < 2; i++)
                {
                    if (brd[pawn_pos[i]] == stm * piece::Pawn)
                    {
                        int8_t pin_dir = board.get_pin_direction(pawn_pos[i]);
                        if (pin_dir == (ep - pawn_pos[i]) || (pin_dir == direction::ND && !board.is_under_attack(-stm, board.find_piece_location(stm * piece::King), pawn_pos[i], sq)))
                            moves.emplace_back(Move(pawn_pos[i], ep, MoveType::En_Passant_Capture, -stm * piece::Pawn));
                    }
                }
            }
        }
    }
}

void MoveGenerator::generate_knight_moves(Board const& board, std::vector<Move>& moves)
{
    auto brd = board.get_board();
    int8_t stm{ board.get_side_to_move() };

    auto r = board.find_piece_locations(stm * piece::Knight);
    for (auto it = r.first; it != r.second; it++) 
    {
        if (board.get_pin_direction(it->second) != direction::ND) continue;

        for (int i = 0; i < 8; i++)
        {
            int8_t next = it->second + direction::knight_jumps[i];
            if (!(next & square::inside))
            {
                if (piece::is_same_sign(stm, brd[next])) continue;
                if (brd[next] == piece::eM)   moves.emplace_back(it->second, next, MoveType::Quite, def::none);
                else                          moves.emplace_back(it->second, next, MoveType::Capture, brd[next]);
            }
        }
    }
}

void MoveGenerator::generate_sliding_piece_moves(Board const& board, int8_t ptype, std::vector<Move>& moves)
{
    auto brd = board.get_board();
    int8_t stm{ board.get_side_to_move() };

    int8_t const* dir_arr = direction::all_dirs;
    int size{ 8 };
    switch (ptype)
    {
    case piece::Rook:
        dir_arr = direction::flat_dirs;
        size = 4;
        break;
    case piece::Bishop:
        dir_arr = direction::diagonal_dirs;
        size = 4;
        break;
    case piece::Queen:
        break;
    default:
        throw std::logic_error("Not a sliding piece!");
    }

    auto r = board.find_piece_locations(stm * ptype);
    for (auto it = r.first; it != r.second; it++) 
    {
        int8_t pin_dir = board.get_pin_direction(it->second);
        for (int i{ 0 }; i < size; i++)
        {
            if (pin_dir != direction::ND && (dir_arr[i] != pin_dir && dir_arr[i] != -pin_dir)) continue;

            int8_t next{ it->second };
            while (!((next = next + dir_arr[i]) & square::inside))
            {
                if (piece::is_same_sign(stm, brd[next])) break;

                if (brd[next] == piece::eM)
                {
                    moves.emplace_back(it->second, next, MoveType::Quite, def::none);
                }
                else
                {
                    moves.emplace_back(it->second, next, MoveType::Capture, brd[next]);
                    break;
                }
            }
        }
    }
}

void MoveGenerator::generate_pawn_moves(Board const& board, std::vector<Move>& moves)
{
    auto brd = board.get_board();
    int8_t stm { board.get_side_to_move() };

    auto r = board.find_piece_locations(stm * piece::Pawn);
    for (auto it = r.first; it != r.second; it++) 
    {
        int8_t pin_dir = board.get_pin_direction(it->second);

        // single push, double push and single push-promotion moves
        int8_t next = it->second + stm * direction::N;
        if (brd[next] == piece::eM && (pin_dir == direction::ND || pin_dir == direction::N || pin_dir == direction::S))
        {
            int8_t rank = square::rank(it->second);
            if ((stm == color::white && rank == 6) || (stm == color::black && rank == 1))
            {
                moves.emplace_back(Move(it->second, next, MoveType::Queen_Promotion, def::none));
                moves.emplace_back(Move(it->second, next, MoveType::Knight_Promotion, def::none));
                moves.emplace_back(Move(it->second, next, MoveType::Bishop_Promotion, def::none));
                moves.emplace_back(Move(it->second, next, MoveType::Rook_Promotion, def::none));
            }
            else
            {
                moves.emplace_back(Move(it->second, next, MoveType::Quite, def::none));
            }

            if ((stm == color::white && rank == 1) || (stm == color::black && rank == 6))
            {
                int8_t next2 = next + stm * direction::N;
                if (brd[next2] == piece::eM)
                    moves.emplace_back(Move(it->second, next2, MoveType::Double_Pawn_Push, def::none));
            }
        }

        // captures, capture-promotions
        next = it->second + stm * direction::NE;
        if (!(next & square::inside) && piece::is_different_sign(brd[next], stm) &&
            (pin_dir == direction::ND || pin_dir == stm * direction::NE))
        {
            int8_t rank = square::rank(it->second);
            if ((stm == color::white && rank == 6) || (stm == color::black && rank == 1))
            {
                moves.emplace_back(Move(it->second, next, MoveType::Queen_Promotion_Capture, brd[next]));
                moves.emplace_back(Move(it->second, next, MoveType::Knight_Promotion_Capture, brd[next]));
                moves.emplace_back(Move(it->second, next, MoveType::Bishop_Promotion_Capture, brd[next]));
                moves.emplace_back(Move(it->second, next, MoveType::Rook_Promotion_Capture, brd[next]));
            }
            else
            {
                moves.emplace_back(Move(it->second, next, MoveType::Capture, brd[next]));
            }
        }

        next = it->second + stm * direction::NW;
        if (!(next & square::inside) && piece::is_different_sign(brd[next], stm) && (pin_dir == direction::ND || pin_dir == stm * direction::NW))
        {
            int8_t rank = square::rank(it->second);
            if ((stm == color::white && rank == 6) || (stm == color::black && rank == 1))
            {
                moves.emplace_back(Move(it->second, next, MoveType::Queen_Promotion_Capture, brd[next]));
                moves.emplace_back(Move(it->second, next, MoveType::Knight_Promotion_Capture, brd[next]));
                moves.emplace_back(Move(it->second, next, MoveType::Bishop_Promotion_Capture, brd[next]));
                moves.emplace_back(Move(it->second, next, MoveType::Rook_Promotion_Capture, brd[next]));
            }
            else
            {
                moves.emplace_back(Move(it->second, next, MoveType::Capture, brd[next]));
            }
        }

        // en-passant captures
        int8_t ep = board.get_en_passant_loc();
        if (
                ep != def::none && 
                (it->second == (ep + stm * direction::SW) || it->second == (ep + stm * direction::SE)) &&
                (
                    pin_dir == (ep - it->second) || 
                    (
                        pin_dir == direction::ND && 
                        (
                            !board.is_under_attack(-stm, board.find_piece_location(stm * piece::King), it->second, ep + stm * direction::S) ||
                            square::rank(board.find_piece_location(stm * piece::King)) != square::rank(it->second)
                        )
                    )
                )
            )

            moves.emplace_back(Move(it->second, ep, MoveType::En_Passant_Capture, -stm * piece::Pawn));
    }
}

std::vector<Move> MoveGenerator::generate_moves(Board const& board)
{
   std::vector<Move> moves;
   
   int8_t stm{ board.get_side_to_move() }, king_pos{ board.find_piece_location(stm * piece::King) }, num_checkers{ board.num_checkers(king_pos) };
   
   generate_king_moves(board, moves);

   if (num_checkers == 1) 
   {
       generate_check_eliminating_moves(board, king_pos, moves);
   }
   else if(num_checkers == 0)
   {
       generate_sliding_piece_moves(board, piece::Queen, moves);
       generate_sliding_piece_moves(board, piece::Rook, moves);
       generate_sliding_piece_moves(board, piece::Bishop, moves);
       generate_knight_moves(board, moves);
       generate_pawn_moves(board, moves);
   }

   return std::move(moves);
}