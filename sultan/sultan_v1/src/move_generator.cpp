#include <stdexcept>

#include "move_generator.hpp"
#include "square.hpp"
#include "board.hpp"

#include "piece.hpp"
#include "definitions.hpp"

MoveGenerator::MoveGenerator(Board const & b) : board_obj{b}, board{b.get_board()} { }

std::pair<std::vector<MoveGenerator::Attack>, std::vector<MoveGenerator::Pin>> MoveGenerator::compute_attacks_and_pins(int8_t target, int8_t stm) const
{
    std::vector<Attack> attacks;
    std::vector<Pin> pins;

    int8_t dir{ 0 }, ntm {-stm};
    for(auto i {0}; i < 4; i++)
    {
        dir = direction::flat_dirs[i];
        int8_t next{target}, pinned {def::none};
        while(!((next = next + dir) & square::inside))
        {
            if(board[next] == piece::eM) continue;

            if(piece::is_same_sign(ntm, board[next]))
            {
                int8_t p = ntm * board[next];
                if(p == piece::Rook || p == piece::Queen)
                {
                    if(pinned == def::none)
                        attacks.push_back(Attack{next, dir});
                    else
                        pins.push_back(Pin{next, pinned, dir});
                }
                break;
            }
            else
            {
                if(pinned == def::none) pinned = next;
                else break;
            }
        }
    }

    for(auto i{0}; i < 4; i++)
    {
        dir = direction::diagonal_dirs[i];
        int8_t next {target}, pinned {def::none};
        while(!((next = next + dir) & square::inside))
        {
            if(board[next] == piece::eM) continue;

            if(piece::is_same_sign(ntm, board[next]))
            {
                int8_t p = ntm * board[next];
                if(p == piece::Bishop || p == piece::Queen || (p == piece::Pawn && (next - target) == dir && dir * ntm < 0))
                {
                    if(pinned == def::none)
                        attacks.push_back(Attack{next, dir});
                    else
                        pins.push_back(Pin{next, pinned, dir});
                }
                break;
            }
            else
            {
                if(pinned == def::none) pinned = next;
                else break;
            }
        }
    }

    for(auto i{0}; i < 8; i++)
    {
        int8_t next = target + direction::knight_jumps[i];
        if(!(next & square::inside) && ntm * board[next] == piece::Knight)
            attacks.push_back(Attack{next, direction::ND});
    }

    return std::make_pair(std::move(attacks), std::move(pins));
}

bool MoveGenerator::is_under_attack(int8_t sq, int8_t stm) const
{
    int8_t dir{ 0 }, ntm { -stm };

    for(auto i {0}; i < 4; i++)
    {
        int8_t next{sq};
        dir = direction::flat_dirs[i];
        while(!((next = next + dir) & square::inside))
        {
            if(board[next] == piece::eM) continue;

            if(piece::is_same_sign(ntm, board[next]))
            {
                int8_t p = ntm * board[next];
                if(p == piece::Rook || p == piece::Queen || (p == piece::King && (next - sq) == dir))
                    return true;
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

            if(piece::is_same_sign(ntm, board[next]))
            {
                int8_t p = ntm * board[next];
                if(p == piece::Bishop || p == piece::Queen ||
                  (p == piece::Pawn && (next - sq) == dir && dir * ntm < 0) ||
                  (p == piece::King && (next - sq) == dir))
                    return true;
            }
            break;
        }
    }

    for(auto i{0}; i < 8; i++)
    {
        int8_t next = sq + direction::knight_jumps[i];
        if(!(next & square::inside) && ntm * board[next] == piece::Knight)
            return true;
    }

    return false;
}

void MoveGenerator::generate_non_king_check_eliminating_moves(int8_t king_pos, int8_t stm, const std::pair<std::vector<Attack>, std::vector<Pin>>& attpin, std::vector<Move> &moves) const
{
    int8_t attack_dir = attpin.first[0].attack_dir;
    int8_t attacker_pos = attpin.first[0].attacker_loc;

    int8_t next {king_pos};
    while((next = next + attack_dir) != attacker_pos)
        generate_non_king_to_square_moves(next, stm, attpin.second, Move_flag::Quite, moves);

    generate_non_king_to_square_moves(attacker_pos, stm, attpin.second, Move_flag::Capture, moves);
}

int8_t MoveGenerator::get_pin_direction(int8_t sq, std::vector<Pin> const & pins) const
{
    for(auto it = pins.begin(); it != pins.end(); it++)
        if(it->pinned_loc == sq)
            return it->pin_dir;

    return direction::ND;
}

void MoveGenerator::generate_non_king_to_square_moves(int8_t sq, int8_t stm, const std::vector<Pin>& pins, Move_flag flag, std::vector<Move> &moves) const
{
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
                if((p == piece::Rook || p == piece::Queen) && get_pin_direction(next, pins) == direction::ND)
                    moves.emplace_back(next, sq, flag);
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
                if((p == piece::Bishop || p == piece::Queen) && get_pin_direction(next, pins) == direction::ND)
                    moves.emplace_back(next, sq, flag);
            }
            break;
        }
    }

    for(auto i{0}; i < 8; i++)
    {
        int8_t next = sq + direction::knight_jumps[i];
        if(!(next & square::inside) && stm * board[next] == piece::Knight && get_pin_direction(next, pins) == direction::ND)
            moves.emplace_back(next, sq, flag);
    }

    if(flag == Move_flag::Quite)
    {
        int8_t pos1 = sq + stm * direction::S;
        if(stm * board[pos1] == piece::Pawn && get_pin_direction(pos1, pins) == direction::ND)
        {
            int8_t rank = square::rank(sq);
            if((stm > 0 && rank == 7) || (stm < 0 && rank == 0))
            {
                moves.emplace_back(pos1, sq, Move_flag::Queen_Promotion);
                moves.emplace_back(pos1, sq, Move_flag::Knight_Promotion);
                moves.emplace_back(pos1, sq, Move_flag::Bishop_Promotion);
                moves.emplace_back(pos1, sq, Move_flag::Rook_Promotion);
            }
            else
            {
                moves.emplace_back(pos1, sq, flag);
            }
        }

        int8_t pos2 = pos1 + stm * direction::S;
        if(stm * board[pos2] == piece::Pawn && board[pos1] == piece::eM && get_pin_direction(pos2, pins) == direction::ND &&
                ((stm > 0 && square::rank(pos2) == 1) || (stm < 0 && square::rank(pos2) == 6)))
            moves.emplace_back(pos2, sq, Move_flag::Double_Pawn_Push);
    }
    else if(flag == Move_flag::Capture)
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
                    moves.emplace_back(pos[i], sq, Move_flag::Queen_Promotion_Capture);
                    moves.emplace_back(pos[i], sq, Move_flag::Rook_Promotion_Capture);
                    moves.emplace_back(pos[i], sq, Move_flag::Bishop_Promotion_Capture);
                    moves.emplace_back(pos[i], sq, Move_flag::Knight_Promotion_Capture);
                }
                else
                {
                    moves.emplace_back(pos[i], sq, flag);
                }
            }
        }
    }
}

void MoveGenerator::generate_king_moves(int8_t king_pos, int8_t stm, bool under_check, std::vector<Move>& moves) const
{
    int8_t next_pos {0};
    // int8_t ntm = static_cast<int8_t>(-stm);
    for(int8_t i{0}; i < 8; i++)
    {
        next_pos = king_pos + direction::all_dirs[i];
        if((next_pos & square::inside) || piece::is_same_sign(board[next_pos], stm) || is_under_attack(next_pos, stm))
            continue;
        Move_flag flag = board[next_pos] == piece::eM ? Move_flag::Quite : Move_flag::Capture;
        moves.emplace_back(Move(king_pos, next_pos, flag));
    }

    if(under_check) return;

    if(stm > 0)
    {
        if(board_obj.query_castling_rights(Castling::white_king_side) &&
           board[square::f1] == piece::eM && board[square::g1] == piece::eM &&
           !is_under_attack(square::f1, stm) && !is_under_attack(square::g1, stm))
            moves.emplace_back(Move(king_pos, square::g1, Move_flag::King_Side_Castle));

        if(board_obj.query_castling_rights(Castling::white_queen_side) &&
           board[square::d1] == piece::eM && board[square::c1] == piece::eM &&
           !is_under_attack(square::d1, stm) && !is_under_attack(square::c1, stm))
            moves.emplace_back(Move(king_pos, square::c1, Move_flag::Queen_Side_Castle));
    }
    else
    {
        if(board_obj.query_castling_rights(Castling::black_king_side) &&
           board[square::f8] == piece::eM && board[square::g8] == piece::eM &&
           !is_under_attack(square::f8, stm) && !is_under_attack(square::g8, stm))
            moves.emplace_back(Move(king_pos, square::g8, Move_flag::King_Side_Castle));

        if(board_obj.query_castling_rights(Castling::black_queen_side) &&
           board[square::d8] == piece::eM && board[square::c8] == piece::eM &&
           !is_under_attack(square::d8, stm) && !is_under_attack(square::c8, stm))
            moves.emplace_back(Move(king_pos, square::c8, Move_flag::Queen_Side_Castle));
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
    int8_t dir{0};
    for(int i{0}; i < size; i++)
    {
        dir = dir_arr[i];
        if(pin_dir != direction::ND && (dir != pin_dir || dir != -pin_dir))
            continue;

        int8_t next{sq};
        while(!((next = next + dir) & square::inside))
        {
            if(piece::is_same_sign(stm, board[next]))
                break;

            if(board[next] == piece::eM)
            {
                moves.emplace_back(sq, next, Move_flag::Quite);
            }
            else
            {
                moves.emplace_back(sq, next, Move_flag::Capture);
                break;
            }
        }
    }
}

void MoveGenerator::generate_knight_moves(int8_t sq, int8_t stm, const std::vector<Pin> &pins, std::vector<Move>& moves) const
{
    if(get_pin_direction(sq, pins) != direction::ND) return;
    for(int i = 0; i < 8; i++)
    {
        int8_t next = sq + direction::knight_jumps[i];
        if(!(next & square::inside))
        {
            if(piece::is_same_sign(stm, board[next])) continue;
            if(board[next] == piece::eM) moves.emplace_back(sq, next, Move_flag::Quite);
            else moves.emplace_back(sq, next, Move_flag::Capture);
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
            moves.emplace_back(Move(sq, next, Move_flag::Queen_Promotion));
            moves.emplace_back(Move(sq, next, Move_flag::Knight_Promotion));
            moves.emplace_back(Move(sq, next, Move_flag::Bishop_Promotion));
            moves.emplace_back(Move(sq, next, Move_flag::Rook_Promotion));
        }
        else
        {
            moves.emplace_back(Move(sq, next, Move_flag::Quite));
        }

        if((stm > 0 && rank == 1) || (stm < 0 && rank == 6))
        {
            int8_t next2 = next + stm * direction::N;
            if(board[next2] == piece::eM)
                moves.emplace_back(Move(sq,next2,Move_flag::Double_Pawn_Push));
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
            moves.emplace_back(Move(sq, next, Move_flag::Queen_Promotion_Capture));
            moves.emplace_back(Move(sq, next, Move_flag::Knight_Promotion_Capture));
            moves.emplace_back(Move(sq, next, Move_flag::Bishop_Promotion_Capture));
            moves.emplace_back(Move(sq, next, Move_flag::Rook_Promotion_Capture));
        }
        else
        {
            moves.emplace_back(Move(sq, next, Move_flag::Capture));
        }
    }

    next = sq + stm * direction::NW;
    if (!(next & square::inside) && piece::is_different_sign(board[next], stm) && 
        (pin_dir == direction::ND || pin_dir == stm * direction::NW))
    {
        int8_t rank = square::rank(sq);
        if ((stm > 0 && rank == 6) || (stm < 0 && rank == 1))
        {
            moves.emplace_back(Move(sq, next, Move_flag::Queen_Promotion_Capture));
            moves.emplace_back(Move(sq, next, Move_flag::Knight_Promotion_Capture));
            moves.emplace_back(Move(sq, next, Move_flag::Bishop_Promotion_Capture));
            moves.emplace_back(Move(sq, next, Move_flag::Rook_Promotion_Capture));
        }
        else
        {
            moves.emplace_back(Move(sq, next, Move_flag::Capture));
        }
    }

    // en-passant captures
    int8_t ep = board_obj.get_en_passant_loc();
    if(ep != def::none && (sq == (ep + stm * direction::SW) || sq == (ep + stm * direction::SE)) && 
       (pin_dir == direction::ND || pin_dir == (ep - sq)))
        moves.emplace_back(Move(sq, ep, Move_flag::En_Passant_Capture));
}

std::vector<Move> MoveGenerator::generate_moves() const
{
   std::vector<Move> moves;
   auto ploc = board_obj.get_piece_locations();

   int8_t stm = static_cast<int8_t>(board_obj.get_side_to_move());
   int8_t king_pos = ploc.find(stm * piece::King)->second;
   auto attpin = compute_attacks_and_pins(king_pos, stm);

   // generate king moves:
   bool under_check { attpin.first.size() > 0 };
   generate_king_moves(king_pos, stm, under_check, moves);

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
