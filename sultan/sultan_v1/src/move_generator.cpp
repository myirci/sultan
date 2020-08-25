#include <stdexcept>

#include "move_generator.hpp"
#include "square.hpp"
#include "board.hpp"

#include "piece.hpp"
#include "definitions.hpp"

MoveGenerator::MoveGenerator(Board const& b) : board_obj{ b }, board{ b.get_board() }, piece_loc{b.get_piece_locations()}
{
    compute_checks_and_pins(color::white);
    compute_checks_and_pins(color::black);
}

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
    std::vector<attack::AttackInfo>& attacks = (attacking_side == color::white) ? wattacks : battacks;
    int8_t opponent_king_pos = find_king_pos(-attacking_side);

    // compute attacks to the opponent_king_position from the ranks and files
    for (int i{ 0 }; i < 4; i++)
    {
        int8_t next{ opponent_king_pos }, target_sq{ opponent_king_pos };
        while (!((next = next + direction::flat_dirs[i]) & square::inside))
        {
            if (board[next] == piece::eM) continue;

            int8_t p = attacking_side * board[next];
            if (piece::is_same_sign(attacking_side, board[next]) && (p == piece::Rook || p == piece::Queen))
            {
                attacks.push_back(attack::AttackInfo{ next, direction::flat_dirs[i], target_sq }); 
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

            int8_t p = attacking_side * board[next];
            if (p == piece::Bishop || p == piece::Queen ||
               (p == piece::Pawn && (next - target_sq) == direction::diagonal_dirs[i] && direction::diagonal_dirs[i] * attacking_side < 0))
            {
                attacks.push_back(attack::AttackInfo{ next, direction::diagonal_dirs[i], target_sq });
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
            attacks.push_back(attack::AttackInfo{ next, direction::ND, opponent_king_pos });
    }
}

// behaves as if defender king does not exist in the board when defending_side_king_pos is given different than def::none
bool MoveGenerator::is_under_attack(int8_t attacking_side, int8_t target_sq, int8_t defender_king_pos) const
{
    // check attacks from the ranks and files
    for (int i{ 0 }; i < 4; i++)
    {
        int8_t next{ target_sq };
        while (!((next = next + direction::flat_dirs[i]) & square::inside))
        {
            if (board[next] == piece::eM || next == defender_king_pos) continue;
            
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
            if (board[next] == piece::eM || next == defender_king_pos) continue;

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

void MoveGenerator::generate_king_moves(int8_t clr, bool under_check, std::vector<Move>& moves) const
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

    if (under_check) return;

    if (clr == color::white)
    {
        if (board_obj.query_castling_rights(Castling::white_king_side) && 
            board[square::f1] == piece::eM && board[square::g1] == piece::eM && 
            !is_under_attack(color::black, square::f1) && !is_under_attack(color::black, square::g1))
            moves.emplace_back(Move(king_pos, square::g1, MoveType::King_Side_Castle, def::none));

        if (board_obj.query_castling_rights(Castling::white_queen_side) && 
            board[square::d1] == piece::eM && board[square::c1] == piece::eM && 
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
            board[square::d8] == piece::eM && board[square::c8] == piece::eM && 
            !is_under_attack(color::white, square::d8) && !is_under_attack(color::white, square::c8))
            moves.emplace_back(Move(king_pos, square::c8, MoveType::Queen_Side_Castle, def::none));
    }
}

int8_t MoveGenerator::get_pin_direction(int8_t clr, int8_t sq) const
{
    std::vector<attack::AttackInfo> const& attacks = clr == color::white ? battacks : wattacks;
    for (auto it = attacks.begin(); it != attacks.end(); it++)
        if (it->target_loc == sq)
            return it->attack_dir;
    return direction::ND;
}

// generates non-king check eliminating moves, precondition: single check
void MoveGenerator::generate_check_eliminating_moves(int8_t clr, int8_t king_pos, std::vector<Move>& moves) const 
{
    std::vector<attack::AttackInfo> const& attacks = clr == color::white ? battacks : wattacks;
    int8_t attacker_pos{ -1 }, attack_dir{ 0 };

    for (auto it = attacks.begin(); it != attacks.end(); it++) 
    {
        if (it->target_loc == king_pos) 
        {
            attacker_pos = it->attacker_loc;
            attack_dir = it->attack_dir;
        }
    }

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
                if ((p == piece::Rook || p == piece::Queen) && get_pin_direction(clr, next) == direction::ND)
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
                if ((p == piece::Bishop || p == piece::Queen) && get_pin_direction(clr, next) == direction::ND)
                    moves.emplace_back(next, sq, mtype, captured);
            }
            break;
        }
    }

    for (int i{ 0 }; i < 8; i++)
    {
        int8_t next = sq + direction::knight_jumps[i];
        if (!(next & square::inside) && clr * board[next] == piece::Knight && get_pin_direction(clr, next) == direction::ND)
            moves.emplace_back(next, sq, mtype, captured);
    }

    if (mtype == MoveType::Quite)
    {
        int8_t next = sq + clr * direction::S;
        if (clr * board[next] == piece::Pawn && get_pin_direction(clr, next) == direction::ND)
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
        if (clr * board[pos] == piece::Pawn && board[next] == piece::eM && get_pin_direction(clr, pos) == direction::ND &&
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
            if (!(pos[i] & square::inside) && clr * board[pos[i]] == piece::Pawn && get_pin_direction(clr, pos[i]) == direction::ND)
            {
                int8_t rank = square::rank(sq);
                if ((clr == color::white && rank == 7) || ( clr == color::black && rank == 0))
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
    }
}

/*

void MoveGenerator::generate_non_king_to_square_moves(int8_t sq, int8_t stm, const std::vector<Pin>& pins, std::vector<Move> &moves) const
{
    int8_t cap = board[sq] == piece::eM ? def::none : board[sq];
    MoveType mtype = board[sq] == piece::eM ? MoveType::Quite : MoveType::Capture;

    int8_t dir{0};
    for(auto i{0}; i < 4; i++)
    {
        int8_t next{sq};
        dir = direction::flat_dirs[i];
        while(!((next = next + dir) & square::inside))
        {
            if(board[next] == piece::eM) continue;

            if(piece::is_same_sign(stm, board[next]))
            {
                int8_t p = stm * board[next];
                if ((p == piece::Rook || p == piece::Queen) && get_pin_direction(next, pins) == direction::ND) 
                    moves.emplace_back(Move(next, sq, mtype, cap));
            }
            break;
        }
    }

    for(auto i{0}; i < 4; i++)
    {
        int8_t next{sq};
        dir = direction::diagonal_dirs[i];
        while(!((next = next + dir) & square::inside))
        {
            if(board[next] == piece::eM) continue;

            if(piece::is_same_sign(stm, board[next]))
            {
                int8_t p = stm * board[next];
                if ((p == piece::Bishop || p == piece::Queen) && get_pin_direction(next, pins) == direction::ND) 
                    moves.emplace_back(next, sq, mtype, cap);
            }
            break;
        }
    }

    for(auto i{0}; i < 8; i++)
    {
        int8_t next = sq + direction::knight_jumps[i];
        if (!(next & square::inside) && stm * board[next] == piece::Knight && get_pin_direction(next, pins) == direction::ND) 
            moves.emplace_back(next, sq, mtype, cap);    
    }

    if(mtype == MoveType::Quite)
    {
        int8_t pos1 = sq + stm * direction::S;
        if(stm * board[pos1] == piece::Pawn && get_pin_direction(pos1, pins) == direction::ND)
        {
            int8_t rank = square::rank(sq);
            if((stm > 0 && rank == 7) || (stm < 0 && rank == 0))
            {
                moves.emplace_back(pos1, sq, MoveType::Queen_Promotion, cap);
                moves.emplace_back(pos1, sq, MoveType::Knight_Promotion, cap);
                moves.emplace_back(pos1, sq, MoveType::Bishop_Promotion, cap);
                moves.emplace_back(pos1, sq, MoveType::Rook_Promotion, cap);
            }
            else
            {
                moves.emplace_back(pos1, sq, mtype, cap);
            }
        }

        int8_t pos2 = pos1 + stm * direction::S;
        if(stm * board[pos2] == piece::Pawn && board[pos1] == piece::eM && get_pin_direction(pos2, pins) == direction::ND &&
                ((stm > 0 && square::rank(pos2) == 1) || (stm < 0 && square::rank(pos2) == 6)))
            moves.emplace_back(pos2, sq, MoveType::Double_Pawn_Push, cap);
    }
    else if(mtype == MoveType::Capture)
    {
        int8_t pos[2] = {
            static_cast<int8_t>(sq + stm * direction::SE),
            static_cast<int8_t>(sq + stm * direction::SW) };

        for(auto i = 0; i < 2; i++)
        {
            if(!(pos[i] & square::inside) && stm * board[pos[i]] == piece::Pawn && get_pin_direction(pos[i], pins) == direction::ND)
            {
                int8_t rank = square::rank(sq);
                if((stm > 0 && rank == 7) || (stm < 0 && rank == 0))
                {
                    moves.emplace_back(pos[i], sq, MoveType::Queen_Promotion_Capture, cap);
                    moves.emplace_back(pos[i], sq, MoveType::Rook_Promotion_Capture, cap);
                    moves.emplace_back(pos[i], sq, MoveType::Bishop_Promotion_Capture, cap);
                    moves.emplace_back(pos[i], sq, MoveType::Knight_Promotion_Capture, cap);
                }
                else
                {
                    moves.emplace_back(pos[i], sq, mtype, cap);
                }
            }
        }
    }
}

void MoveGenerator::generate_sliding_piece_moves(int8_t sq, int8_t ptype, int8_t stm, const std::vector<Pin> &pins, std::vector<Move> &moves) const
{
    int8_t const * dir_arr = direction::all_dirs;
    int size {8};
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

    int8_t pin_dir = get_pin_direction(sq, pins);
    for(int i{0}; i < size; i++)
    {
        if(pin_dir != direction::ND && (dir_arr[i] != pin_dir && dir_arr[i] != -pin_dir))
            continue;

        int8_t next{sq};
        while(!((next = next + dir_arr[i]) & square::inside))
        {
            if(piece::is_same_sign(stm, board[next]))
                break;

            if(board[next] == piece::eM)
            {
                moves.emplace_back(sq, next, MoveType::Quite, def::none);
            }
            else
            {
                moves.emplace_back(sq, next, MoveType::Capture, board[next]);
                break;
            }
        }
    }
}

void MoveGenerator::generate_knight_moves(int8_t sq, int8_t stm, const std::vector<Pin> &pins, std::vector<Move>& moves) const
{
    if(get_pin_direction(sq, pins) != direction::ND) 
        return;

    for(int i = 0; i < 8; i++)
    {
        int8_t next = sq + direction::knight_jumps[i];
        if(!(next & square::inside))
        {
            if(piece::is_same_sign(stm, board[next])) 
                continue;
            
            if(board[next] == piece::eM) 
                moves.emplace_back(sq, next, MoveType::Quite, def::none);
            else 
                moves.emplace_back(sq, next, MoveType::Capture, board[next]);
        }
    }
}

void MoveGenerator::generate_pawn_moves(int8_t sq, int8_t stm, const std::vector<Pin>& pins, std::vector<Move>& moves) const
{
    int8_t pin_dir = get_pin_direction(sq, pins);

    // single push, double push and single push-promotion moves
    int8_t next = sq + stm * direction::N;
    if(board[next] == piece::eM && (pin_dir == direction::ND || pin_dir == stm * direction::N))
    {
        int8_t rank = square::rank(sq);
        if((stm > 0 && rank == 6) || (stm < 0 && rank == 1))
        {
            moves.emplace_back(Move(sq, next, MoveType::Queen_Promotion, def::none));
            moves.emplace_back(Move(sq, next, MoveType::Knight_Promotion, def::none));
            moves.emplace_back(Move(sq, next, MoveType::Bishop_Promotion, def::none));
            moves.emplace_back(Move(sq, next, MoveType::Rook_Promotion, def::none));
        }
        else
        {
            moves.emplace_back(Move(sq, next, MoveType::Quite, def::none));
        }

        if((stm > 0 && rank == 1) || (stm < 0 && rank == 6))
        {
            int8_t next2 = next + stm * direction::N;
            if(board[next2] == piece::eM)
                moves.emplace_back(Move(sq, next2, MoveType::Double_Pawn_Push, def::none));
        }
    }

    // captures, capture-promotions
    next = sq + stm * direction::NE;
    if (!(next & square::inside) && piece::is_different_sign(board[next], stm) && 
        (pin_dir == direction::ND || pin_dir == stm * direction::NE))
    {
        int8_t rank = square::rank(sq);
        if ((stm > 0 && rank == 6) || (stm < 0 && rank == 1))
        {
            moves.emplace_back(Move(sq, next, MoveType::Queen_Promotion_Capture, board[next]));
            moves.emplace_back(Move(sq, next, MoveType::Knight_Promotion_Capture, board[next]));
            moves.emplace_back(Move(sq, next, MoveType::Bishop_Promotion_Capture, board[next]));
            moves.emplace_back(Move(sq, next, MoveType::Rook_Promotion_Capture, board[next]));
        }
        else
        {
            moves.emplace_back(Move(sq, next, MoveType::Capture, board[next]));
        }
    }

    next = sq + stm * direction::NW;
    if (!(next & square::inside) && piece::is_different_sign(board[next], stm) && 
        (pin_dir == direction::ND || pin_dir == stm * direction::NW))
    {
        int8_t rank = square::rank(sq);
        if ((stm > 0 && rank == 6) || (stm < 0 && rank == 1))
        {
            moves.emplace_back(Move(sq, next, MoveType::Queen_Promotion_Capture, board[next]));
            moves.emplace_back(Move(sq, next, MoveType::Knight_Promotion_Capture, board[next]));
            moves.emplace_back(Move(sq, next, MoveType::Bishop_Promotion_Capture, board[next]));
            moves.emplace_back(Move(sq, next, MoveType::Rook_Promotion_Capture, board[next]));
        }
        else
        {
            moves.emplace_back(Move(sq, next, MoveType::Capture, board[next]));
        }
    }

    // en-passant captures
    int8_t ep = board_obj.get_en_passant_loc();
    if(ep != def::none && (sq == (ep + stm * direction::SW) || sq == (ep + stm * direction::SE)) && 
       (pin_dir == direction::ND || pin_dir == (ep - sq)))
        moves.emplace_back(Move(sq, ep, MoveType::En_Passant_Capture, -stm * piece::Pawn));
}

std::vector<Move> MoveGenerator::generate_moves() const
{
   std::vector<Move> moves;
   auto ploc = board_obj.get_piece_locations();

   int8_t stm = static_cast<int8_t>(board_obj.get_side_to_move());
   int8_t king_pos = ploc.find(stm * piece::King)->second;
   auto attpin = compute_attacks_and_pins(king_pos, stm);

   // generate king moves:
   generate_king_moves(king_pos, stm, attpin.first, moves);

   // if double check, then only king can move
   if (attpin.first.size() == 2)
       return moves;

   // single check, look for blocking or capturing the checker with a non-king piece
   if (attpin.first.size() == 1)
   {
       generate_non_king_check_eliminating_moves(king_pos, stm, attpin, moves);
       return moves;
   }

   // generate queen moves
   auto r = ploc.equal_range(stm * piece::Queen);
   for (auto it = r.first; it != r.second; it++) 
       generate_sliding_piece_moves(it->second, piece::Queen, stm, attpin.second, moves);

   r = ploc.equal_range(stm * piece::Rook);
   for (auto it = r.first; it != r.second; it++)
       generate_sliding_piece_moves(it->second, piece::Rook, stm, attpin.second, moves);

   r = ploc.equal_range(stm * piece::Bishop);
   for (auto it = r.first; it != r.second; it++)
       generate_sliding_piece_moves(it->second, piece::Bishop, stm, attpin.second, moves);
   
   r = ploc.equal_range(stm * piece::Knight);
   for (auto it = r.first; it != r.second; it++)
       generate_knight_moves(it->second, stm, attpin.second, moves);

   r = ploc.equal_range(stm * piece::Pawn);
   for (auto it = r.first; it != r.second; it++)
       generate_pawn_moves(it->second, stm, attpin.second, moves);

   return moves;
}

std::vector<Move> MoveGenerator::generate_moves(perft::perft_stats& stats) const 
{
    std::vector<Move> moves;
    auto ploc = board_obj.get_piece_locations();

    int8_t stm = static_cast<int8_t>(board_obj.get_side_to_move());
    int8_t king_pos = ploc.find(stm * piece::King)->second;
    auto attpin = compute_attacks_and_pins(king_pos, stm);

    // generate king moves:
    generate_king_moves(king_pos, stm, attpin.first, moves);

    // if double check, then only king can move
    if (attpin.first.size() == 2) 
    {
        stats.num_double_checks++;
        return moves;
    }
        
    // single check, look for blocking or capturing the checker with a non-king piece
    if (attpin.first.size() == 1)
    {
        generate_non_king_check_eliminating_moves(king_pos, stm, attpin, moves);
        stats.num_checks++;
        return moves;
    }

    // generate queen moves
    auto r = ploc.equal_range(stm * piece::Queen);
    for (auto it = r.first; it != r.second; it++)
        generate_sliding_piece_moves(it->second, piece::Queen, stm, attpin.second, moves);

    r = ploc.equal_range(stm * piece::Rook);
    for (auto it = r.first; it != r.second; it++)
        generate_sliding_piece_moves(it->second, piece::Rook, stm, attpin.second, moves);

    r = ploc.equal_range(stm * piece::Bishop);
    for (auto it = r.first; it != r.second; it++)
        generate_sliding_piece_moves(it->second, piece::Bishop, stm, attpin.second, moves);

    r = ploc.equal_range(stm * piece::Knight);
    for (auto it = r.first; it != r.second; it++)
        generate_knight_moves(it->second, stm, attpin.second, moves);

    r = ploc.equal_range(stm * piece::Pawn);
    for (auto it = r.first; it != r.second; it++)
        generate_pawn_moves(it->second, stm, attpin.second, moves);

    return moves;
}

*/