#include <stdexcept>

#include "move_generator.hpp"
#include "square.hpp"
#include "board.hpp"

#include "piece.hpp"
#include "definitions.hpp"
#include <iostream>

MoveGenerator::MoveGenerator(Board const& b) : board_obj{ b }, board{ b.get_board() }, piece_loc{b.get_piece_locations()} { }

void MoveGenerator::compute_attacks() 
{
    attack_info.clear();
    compute_checks_and_pins(color::white);
    compute_checks_and_pins(color::black);
}

std::unordered_multimap<int8_t, std::pair<int8_t, int8_t>> const& MoveGenerator::get_attack_info() const { return attack_info; }

int8_t MoveGenerator::find_king_pos(int8_t clr) const 
{
    auto it = piece_loc.find(clr * piece::King);
    int8_t opponent_king_pos{ -1 };
    if (it == piece_loc.end()) 
    {
        std::string error_msg("King position could not be found for the given color: ");
        error_msg += color::color_to_string(clr);
        throw std::logic_error(error_msg);
    }
        
    return it->second;
}

void MoveGenerator::compute_checks_and_pins(int8_t attacking_side)
{
    int8_t opponent_king_pos = find_king_pos(-attacking_side);

    // compute attacks to the opponent_king_position from the ranks and files
    for (int i{ 0 }; i < 4; i++)
    {
        int8_t next{ opponent_king_pos }, target_sq{ opponent_king_pos };
        while (!((next = next + direction::flat_dirs[i]) & square::inside))
        {
            if (board[next] == piece::eM) continue;
            
            if (piece::is_same_sign(attacking_side, board[next]))
            {
                int8_t p = attacking_side * board[next];
                if (p == piece::Rook || p == piece::Queen)
                    attack_info.emplace(std::make_pair(target_sq, std::make_pair(next, direction::flat_dirs[i])));
                break;
            }
            else
            {
                if (target_sq == opponent_king_pos) target_sq = next; 
                else break;
            }
        }
    }

    // compute attacks to the opponent_king_position from the diagonals
    for (int i{ 0 }; i < 4; i++)
    {
        int8_t next{ opponent_king_pos }, target_sq{ opponent_king_pos };
        while (!((next = next + direction::diagonal_dirs[i]) & square::inside))
        {
            if (board[next] == piece::eM) continue;

            if (piece::is_same_sign(attacking_side, board[next])) 
            {
                int8_t p = attacking_side * board[next];
                if (p == piece::Bishop || p == piece::Queen ||
                   (p == piece::Pawn && (next - target_sq) == direction::diagonal_dirs[i] && direction::diagonal_dirs[i] * attacking_side < 0) &&  target_sq == opponent_king_pos)
                    attack_info.emplace(std::make_pair(target_sq, std::make_pair(next, direction::diagonal_dirs[i])));
                break;
            }
            else
            {
                if (target_sq == opponent_king_pos) target_sq = next;
                else break;
            }
        }
    }

    // compute attacks to the opponent king position by the knights
    for (int i{ 0 }; i < 8; i++)
    {
        int8_t next = opponent_king_pos + direction::knight_jumps[i];
        if (!(next & square::inside) && attacking_side * board[next] == piece::Knight) 
            attack_info.emplace(std::make_pair(opponent_king_pos, std::make_pair(next, direction::ND)));
    }
}

// behaves as if board[exclude_pos1] and board[exclude_pos2] are empty if they are different than def::none
bool MoveGenerator::is_under_attack(int8_t attacking_side, int8_t target_sq, int8_t exclude_pos1, int8_t exclude_pos2) const
{
    // check attacks from the ranks and files
    for (int i{ 0 }; i < 4; i++)
    {
        int8_t next{ target_sq };
        while (!((next = next + direction::flat_dirs[i]) & square::inside))
        {
            if (board[next] == piece::eM || next == exclude_pos1 || next == exclude_pos2) continue;
            
            if (piece::is_same_sign(attacking_side, board[next])) 
            {
                int8_t p = attacking_side * board[next];
                if (p == piece::Rook || p == piece::Queen || (p == piece::King && (next - target_sq) == direction::flat_dirs[i]))
                    return true;
            }
            break;
        }
    }

    // check attacks from the diagonals
    for (int i{ 0 }; i < 4; i++)
    {
        int8_t next{ target_sq };
        while (!((next = next + direction::diagonal_dirs[i]) & square::inside))
        {
            if (board[next] == piece::eM || next == exclude_pos1 || next == exclude_pos2) continue;

            if (piece::is_same_sign(attacking_side, board[next])) 
            {
                int8_t p = attacking_side * board[next];
                if (p == piece::Bishop || p == piece::Queen ||
                   (p == piece::Pawn && (next - target_sq) == direction::diagonal_dirs[i] && direction::diagonal_dirs[i] * attacking_side < 0) ||
                   (p == piece::King && (next - target_sq) == direction::diagonal_dirs[i]))
                    return true;
            }
            break;
        }
    }

    for (int i{ 0 }; i < 8; i++)
    {
        int8_t next = target_sq + direction::knight_jumps[i];
        if (!(next & square::inside) && attacking_side * board[next] == piece::Knight)
            return true;
    }

    return false;
}

void MoveGenerator::generate_king_moves(int8_t clr, std::vector<Move>& moves) const
{
    int8_t next_pos{ 0 }, king_pos{ find_king_pos(clr) };
    for (int i{ 0 }; i < 8; i++)
    {
        next_pos = king_pos + direction::all_dirs[i];
        if (next_pos & square::inside || piece::is_same_sign(board[next_pos], clr) || is_under_attack(-clr, next_pos, king_pos))
            continue;

        if (board[next_pos] == piece::eM)   moves.emplace_back(Move(king_pos, next_pos, MoveType::Quite, def::none));
        else                                moves.emplace_back(Move(king_pos, next_pos, MoveType::Capture, board[next_pos]));
    }

    auto it = attack_info.find(king_pos);
    if (it != attack_info.end()) return;

    if (clr == color::white)
    {
        if (board_obj.query_castling_rights(Castling::white_king_side) && 
            board[square::f1] == piece::eM && board[square::g1] == piece::eM && 
            !is_under_attack(color::black, square::f1) && !is_under_attack(color::black, square::g1))
            moves.emplace_back(Move(king_pos, square::g1, MoveType::King_Side_Castle, def::none));

        if (board_obj.query_castling_rights(Castling::white_queen_side) && 
            board[square::b1] == piece::eM && board[square::c1] == piece::eM && board[square::d1] == piece::eM &&
            !is_under_attack(color::black, square::d1) && !is_under_attack(color::black, square::c1))
            moves.emplace_back(Move(king_pos, square::c1, MoveType::Queen_Side_Castle, def::none));
    }
    else
    {
        if (board_obj.query_castling_rights(Castling::black_king_side) && 
            board[square::f8] == piece::eM && board[square::g8] == piece::eM && 
            !is_under_attack(color::white, square::f8) && !is_under_attack(color::white, square::g8))
            moves.emplace_back(Move(king_pos, square::g8, MoveType::King_Side_Castle, def::none));

        if (board_obj.query_castling_rights(Castling::black_queen_side) && 
            board[square::b8] == piece::eM && board[square::c8] == piece::eM && board[square::d8] == piece::eM &&
            !is_under_attack(color::white, square::d8) && !is_under_attack(color::white, square::c8))
            moves.emplace_back(Move(king_pos, square::c8, MoveType::Queen_Side_Castle, def::none));
    }
}

int8_t MoveGenerator::get_pin_direction(int8_t sq) const
{
    auto it = attack_info.find(sq);
    return it == attack_info.end() ? direction::ND : it->second.second;
}

// generates non-king check eliminating moves, precondition: single check
void MoveGenerator::generate_check_eliminating_moves(int8_t clr, int8_t king_pos, std::vector<Move>& moves) const 
{
    auto it = attack_info.find(king_pos);
    if (it == attack_info.end())
        throw std::logic_error("Checking piece could not be found!");

    int8_t attacker_pos{ it->second.first }, attack_dir{ it->second.second };
    if (attack_dir != direction::ND)
    {
        int8_t next{ king_pos };
        while ((next = next + attack_dir) != attacker_pos)
            generate_to_square_moves(clr, next, moves);
    }
    
    generate_to_square_moves(clr, attacker_pos, moves);
}

// generate non-king to square moves, precondition: single check
void MoveGenerator::generate_to_square_moves(int8_t clr, int8_t sq, std::vector<Move>& moves) const
{
    MoveType mtype{ MoveType::Quite };
    int8_t captured{ def::none };
    if (board[sq] != piece::eM) 
    {
        captured = board[sq];
        mtype = MoveType::Capture;
    }

    for (int i{ 0 }; i < 4; i++)
    {
        int8_t next{ sq };
        while (!((next = next + direction::flat_dirs[i]) & square::inside))
        {
            if (board[next] == piece::eM) continue;

            if (piece::is_same_sign(clr, board[next]))
            {
                int8_t p = clr * board[next];
                if ((p == piece::Rook || p == piece::Queen) && get_pin_direction(next) == direction::ND) 
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
            if (board[next] == piece::eM) continue;

            if (piece::is_same_sign(clr, board[next]))
            {
                int8_t p = clr * board[next];
                if ((p == piece::Bishop || p == piece::Queen) && get_pin_direction(next) == direction::ND) 
                    moves.emplace_back(next, sq, mtype, captured);
            }
            break;
        }
    }

    for (int i{ 0 }; i < 8; i++)
    {
        int8_t next = sq + direction::knight_jumps[i];
        if (!(next & square::inside) && clr * board[next] == piece::Knight && get_pin_direction(next) == direction::ND) 
            moves.emplace_back(next, sq, mtype, captured);  
    }

    if((clr == color::white && sq >= square::a3 && sq <= square::h8) || (clr == color::black && sq >= square::a1 && sq <= square::h6))
    {
        if (mtype == MoveType::Quite)
        {
            int8_t next = sq + clr * direction::S;
            if (clr * board[next] == piece::Pawn && get_pin_direction(next) == direction::ND)
            {
                int8_t rank = square::rank(sq);
                if ((clr == color::white && rank == 7) || (clr == color::black && rank == 0))
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

            int8_t pos = next + clr * direction::S;

            if (clr * board[pos] == piece::Pawn && board[next] == piece::eM && get_pin_direction(pos) == direction::ND &&
                ((clr == color::white && square::rank(pos) == 1) || (clr == color::black && square::rank(pos) == 6)))
                moves.emplace_back(pos, sq, MoveType::Double_Pawn_Push, def::none);
        }
        else if (mtype == MoveType::Capture)
        {
            int8_t pos[2] = {
                static_cast<int8_t>(sq + clr * direction::SE),
                static_cast<int8_t>(sq + clr * direction::SW) };

            for (auto i = 0; i < 2; i++)
            {
                if (!(pos[i] & square::inside) && clr * board[pos[i]] == piece::Pawn && get_pin_direction(pos[i]) == direction::ND)
                {
                    int8_t rank = square::rank(sq);
                    if ((clr == color::white && rank == 7) || (clr == color::black && rank == 0))
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

            int8_t ep = board_obj.get_en_passant_loc();
            if (ep != def::none && sq == ep + clr * direction::S)
            {
                int8_t pawn_pos[2] =
                {
                    static_cast<int8_t>(ep + clr * direction::SW),
                    static_cast<int8_t>(ep + clr * direction::SE)
                };

                for (int i = 0; i < 2; i++)
                {
                    if (board[pawn_pos[i]] == clr * piece::Pawn)
                    {
                        int8_t pin_dir = get_pin_direction(pawn_pos[i]);
                        if (pin_dir == (ep - pawn_pos[i]) || (pin_dir == direction::ND && !is_under_attack(-clr, find_king_pos(clr), pawn_pos[i], sq)))
                            moves.emplace_back(Move(pawn_pos[i], ep, MoveType::En_Passant_Capture, -clr * piece::Pawn));
                    }
                }
            }
        }
    }
}

void MoveGenerator::generate_knight_moves(int8_t clr, std::vector<Move>& moves) const
{
    auto r = piece_loc.equal_range(clr * piece::Knight);
    for (auto it = r.first; it != r.second; it++) 
    {
        if (get_pin_direction(it->second) != direction::ND) continue;

        for (int i = 0; i < 8; i++)
        {
            int8_t next = it->second + direction::knight_jumps[i];
            if (!(next & square::inside))
            {
                if (piece::is_same_sign(clr, board[next])) continue;
                if (board[next] == piece::eM)   moves.emplace_back(it->second, next, MoveType::Quite, def::none);
                else                            moves.emplace_back(it->second, next, MoveType::Capture, board[next]);
            }
        }
    }
}

void MoveGenerator::generate_sliding_piece_moves(int8_t clr, int8_t ptype, std::vector<Move>& moves) const
{
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

    auto r = piece_loc.equal_range(clr * ptype);
    for (auto it = r.first; it != r.second; it++) 
    {
        int8_t pin_dir = get_pin_direction(it->second);
        for (int i{ 0 }; i < size; i++)
        {
            if (pin_dir != direction::ND && (dir_arr[i] != pin_dir && dir_arr[i] != -pin_dir)) continue;

            int8_t next{ it->second };
            while (!((next = next + dir_arr[i]) & square::inside))
            {
                if (piece::is_same_sign(clr, board[next])) break;

                if (board[next] == piece::eM)
                {
                    moves.emplace_back(it->second, next, MoveType::Quite, def::none);
                }
                else
                {
                    moves.emplace_back(it->second, next, MoveType::Capture, board[next]);
                    break;
                }
            }
        }
    }
}

void MoveGenerator::generate_pawn_moves(int8_t clr, std::vector<Move>& moves) const
{
    auto r = piece_loc.equal_range(clr * piece::Pawn);
    for (auto it = r.first; it != r.second; it++) 
    {
        int8_t pin_dir = get_pin_direction(it->second);

        // single push, double push and single push-promotion moves
        int8_t next = it->second + clr * direction::N;
        if (board[next] == piece::eM && (pin_dir == direction::ND || pin_dir == direction::N || pin_dir == direction::S))
        {
            int8_t rank = square::rank(it->second);
            if ((clr == color::white && rank == 6) || (clr == color::black && rank == 1))
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

            if ((clr == color::white && rank == 1) || (clr == color::black && rank == 6))
            {
                int8_t next2 = next + clr * direction::N;
                if (board[next2] == piece::eM)
                    moves.emplace_back(Move(it->second, next2, MoveType::Double_Pawn_Push, def::none));
            }
        }

        // captures, capture-promotions
        next = it->second + clr * direction::NE;
        if (!(next & square::inside) && piece::is_different_sign(board[next], clr) &&
            (pin_dir == direction::ND || pin_dir == clr * direction::NE))
        {
            int8_t rank = square::rank(it->second);
            if ((clr == color::white && rank == 6) || (clr == color::black && rank == 1))
            {
                moves.emplace_back(Move(it->second, next, MoveType::Queen_Promotion_Capture, board[next]));
                moves.emplace_back(Move(it->second, next, MoveType::Knight_Promotion_Capture, board[next]));
                moves.emplace_back(Move(it->second, next, MoveType::Bishop_Promotion_Capture, board[next]));
                moves.emplace_back(Move(it->second, next, MoveType::Rook_Promotion_Capture, board[next]));
            }
            else
            {
                moves.emplace_back(Move(it->second, next, MoveType::Capture, board[next]));
            }
        }

        next = it->second + clr * direction::NW;
        if (!(next & square::inside) && piece::is_different_sign(board[next], clr) && (pin_dir == direction::ND || pin_dir == clr * direction::NW))
        {
            int8_t rank = square::rank(it->second);
            if ((clr == color::white && rank == 6) || (clr == color::black && rank == 1))
            {
                moves.emplace_back(Move(it->second, next, MoveType::Queen_Promotion_Capture, board[next]));
                moves.emplace_back(Move(it->second, next, MoveType::Knight_Promotion_Capture, board[next]));
                moves.emplace_back(Move(it->second, next, MoveType::Bishop_Promotion_Capture, board[next]));
                moves.emplace_back(Move(it->second, next, MoveType::Rook_Promotion_Capture, board[next]));
            }
            else
            {
                moves.emplace_back(Move(it->second, next, MoveType::Capture, board[next]));
            }
        }

        // en-passant captures
        int8_t ep = board_obj.get_en_passant_loc();
        if (
                ep != def::none && 
                (it->second == (ep + clr * direction::SW) || it->second == (ep + clr * direction::SE)) &&
                (
                    pin_dir == (ep - it->second) || 
                    (
                        pin_dir == direction::ND && 
                        (
                            !is_under_attack(-clr, find_king_pos(clr), it->second, ep + clr * direction::S) ||
                            square::rank(find_king_pos(clr)) != square::rank(it->second)
                        )
                    )
                )
            )

            moves.emplace_back(Move(it->second, ep, MoveType::En_Passant_Capture, -clr * piece::Pawn));
    }
}

std::vector<Move> MoveGenerator::generate_moves() const
{
   std::vector<Move> moves;
   
   int8_t stm = static_cast<int8_t>(board_obj.get_side_to_move());
   int8_t king_pos = piece_loc.find(stm * piece::King)->second;

   generate_king_moves(stm, moves);

   auto r = attack_info.equal_range(king_pos);
   auto num_checkers = std::distance(r.first, r.second);

   if (num_checkers == 1) 
   {
       generate_check_eliminating_moves(stm, king_pos, moves);
   }
   else if(num_checkers == 0)
   {
       generate_sliding_piece_moves(stm, piece::Queen, moves);
       generate_sliding_piece_moves(stm, piece::Rook, moves);
       generate_sliding_piece_moves(stm, piece::Bishop, moves);
       generate_knight_moves(stm, moves);
       generate_pawn_moves(stm, moves);
   }

   return std::move(moves);
}