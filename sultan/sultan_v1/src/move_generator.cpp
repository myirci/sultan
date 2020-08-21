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
        dir = Square::Directions::flat_dirs[i];
        int8_t next{target}, pinned {def::none};
        while(!((next = next + dir) & Square::inside))
        {
            if(board[next] == Piece::eM) continue;

            if(Piece::is_same_sign(ntm, board[next]))
            {
                int8_t p = ntm * board[next];
                if(p == Piece::Rook || p == Piece::Queen)
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
        dir = Square::Directions::diagonal_dirs[i];
        int8_t next {target}, pinned {def::none};
        while(!((next = next + dir) & Square::inside))
        {
            if(board[next] == Piece::eM) continue;

            if(Piece::is_same_sign(ntm, board[next]))
            {
                int8_t p = ntm * board[next];
                if(p == Piece::Bishop || p == Piece::Queen || (p == Piece::Pawn && (next - target) == dir && dir * ntm < 0))
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
        int8_t next = target + Square::Directions::knight_jumps[i];
        if(!(next & Square::inside) && ntm * board[next] == Piece::Knight)
            attacks.push_back(Attack{next, Square::Directions::ND});
    }

    return std::make_pair(std::move(attacks), std::move(pins));
}

bool MoveGenerator::is_under_attack(int8_t sq, int8_t stm) const
{
    int8_t dir{ 0 }, ntm { -stm };

    for(auto i {0}; i < 4; i++)
    {
        int8_t next{sq};
        dir = Square::Directions::flat_dirs[i];
        while(!((next = next + dir) & Square::inside))
        {
            if(board[next] == Piece::eM) continue;

            if(Piece::is_same_sign(ntm, board[next]))
            {
                int8_t p = ntm * board[next];
                if(p == Piece::Rook || p == Piece::Queen || (p == Piece::King && (next - sq) == dir))
                    return true;
            }
            break;
        }
    }

    for(auto i{0}; i < 4; i++)
    {
        int8_t next{sq};
        dir = Square::Directions::diagonal_dirs[i];
        while(!((next = next + dir) & Square::inside))
        {
            if(board[next] == Piece::eM) continue;

            if(Piece::is_same_sign(ntm, board[next]))
            {
                int8_t p = ntm * board[next];
                if(p == Piece::Bishop || p == Piece::Queen ||
                  (p == Piece::Pawn && (next - sq) == dir && dir * ntm < 0) ||
                  (p == Piece::King && (next - sq) == dir))
                    return true;
            }
            break;
        }
    }

    for(auto i{0}; i < 8; i++)
    {
        int8_t next = sq + Square::Directions::knight_jumps[i];
        if(!(next & Square::inside) && ntm * board[next] == Piece::Knight)
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

    return Square::Directions::ND;
}

void MoveGenerator::generate_non_king_to_square_moves(int8_t sq, int8_t stm, const std::vector<Pin>& pins, Move_flag flag, std::vector<Move> &moves) const
{
    int8_t dir{0};
    for(auto i{0}; i < 4; i++)
    {
        int8_t next{sq};
        dir = Square::Directions::flat_dirs[i];
        while(!((next = next + dir) & Square::inside))
        {
            if(board[next] == Piece::eM) continue;

            if(Piece::is_same_sign(stm, board[next]))
            {
                int8_t p = stm * board[next];
                if((p == Piece::Rook || p == Piece::Queen) && get_pin_direction(next, pins) == Square::Directions::ND)
                    moves.emplace_back(next, sq, flag);
            }
            break;
        }
    }

    for(auto i{0}; i < 4; i++)
    {
        int8_t next{sq};
        dir = Square::Directions::diagonal_dirs[i];
        while(!((next = next + dir) & Square::inside))
        {
            if(board[next] == Piece::eM) continue;

            if(Piece::is_same_sign(stm, board[next]))
            {
                int8_t p = stm * board[next];
                if((p == Piece::Bishop || p == Piece::Queen) && get_pin_direction(next, pins) == Square::Directions::ND)
                    moves.emplace_back(next, sq, flag);
            }
            break;
        }
    }

    for(auto i{0}; i < 8; i++)
    {
        int8_t next = sq + Square::Directions::knight_jumps[i];
        if(!(next & Square::inside) && stm * board[next] == Piece::Knight && get_pin_direction(next, pins) == Square::Directions::ND)
            moves.emplace_back(next, sq, flag);
    }

    if(flag == Move_flag::Quite)
    {
        int8_t pos1 = sq + stm * Square::Directions::S;
        if(stm * board[pos1] == Piece::Pawn && get_pin_direction(pos1, pins) == Square::Directions::ND)
        {
            int8_t rank = Square::rank(sq);
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

        int8_t pos2 = pos1 + stm * Square::Directions::S;
        if(stm * board[pos2] == Piece::Pawn && board[pos1] == Piece::eM && get_pin_direction(pos2, pins) == Square::Directions::ND &&
                ((stm > 0 && Square::rank(pos2) == 1) || (stm < 0 && Square::rank(pos2) == 6)))
            moves.emplace_back(pos2, sq, Move_flag::Double_Pawn_Push);
    }
    else if(flag == Move_flag::Capture)
    {
        int8_t pos[2] = {
            static_cast<int8_t>(sq + stm * Square::Directions::SE),
            static_cast<int8_t>(sq + stm * Square::Directions::SW) };

        for(auto i = 0; i < 2; i++)
        {
            if(!(pos[i] & Square::inside) && stm * board[pos[i]] == Piece::Pawn && get_pin_direction(pos[i], pins) == Square::Directions::ND)
            {
                int8_t rank = Square::rank(sq);
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
        next_pos = king_pos + Square::Directions::all_dirs[i];
        if((next_pos & Square::inside) || Piece::is_same_sign(board[next_pos], stm) || is_under_attack(next_pos, stm))
            continue;
        Move_flag flag = board[next_pos] == Piece::eM ? Move_flag::Quite : Move_flag::Capture;
        moves.emplace_back(Move(king_pos, next_pos, flag));
    }

    if(under_check) return;

    if(stm > 0)
    {
        if(board_obj.query_castling_rights(Castling::white_king_side) &&
           board[Square::f1] == Piece::eM && board[Square::g1] == Piece::eM &&
           !is_under_attack(Square::f1, stm) && !is_under_attack(Square::g1, stm))
            moves.emplace_back(Move(king_pos, Square::g1, Move_flag::King_Side_Castle));

        if(board_obj.query_castling_rights(Castling::white_queen_side) &&
           board[Square::d1] == Piece::eM && board[Square::c1] == Piece::eM &&
           !is_under_attack(Square::d1, stm) && !is_under_attack(Square::c1, stm))
            moves.emplace_back(Move(king_pos, Square::c1, Move_flag::Queen_Side_Castle));
    }
    else
    {
        if(board_obj.query_castling_rights(Castling::black_king_side) &&
           board[Square::f8] == Piece::eM && board[Square::g8] == Piece::eM &&
           !is_under_attack(Square::f8, stm) && !is_under_attack(Square::g8, stm))
            moves.emplace_back(Move(king_pos, Square::g8, Move_flag::King_Side_Castle));

        if(board_obj.query_castling_rights(Castling::black_queen_side) &&
           board[Square::d8] == Piece::eM && board[Square::c8] == Piece::eM &&
           !is_under_attack(Square::d8, stm) && !is_under_attack(Square::c8, stm))
            moves.emplace_back(Move(king_pos, Square::c8, Move_flag::Queen_Side_Castle));
    }
}

void MoveGenerator::generate_sliding_piece_moves(int8_t sq, int8_t ptype, int8_t stm, const std::vector<Pin> &pins, std::vector<Move> &moves) const
{
    int8_t const * dir_arr = Square::Directions::all_dirs;
    int size {8};
    switch (ptype)
    {
        case Piece::Rook:
            dir_arr = Square::Directions::flat_dirs;
            size = 4;
            break;
        case Piece::Bishop:
            dir_arr = Square::Directions::diagonal_dirs;
            size = 4;
            break;
        case Piece::Queen:
            break;
        default:
            throw std::logic_error("Not a sliding piece!");
    }

    int8_t pin_dir = get_pin_direction(sq, pins);
    int8_t dir{0};
    for(int i{0}; i < size; i++)
    {
        dir = dir_arr[i];
        if(pin_dir != Square::Directions::ND && (dir != pin_dir || dir != -pin_dir))
            continue;

        int8_t next{sq};
        while(!((next = next + dir) & Square::inside))
        {
            if(Piece::is_same_sign(stm, board[next]))
                break;

            if(board[next] == Piece::eM)
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
    if(get_pin_direction(sq, pins) != Square::Directions::ND) return;
    for(int i = 0; i < 8; i++)
    {
        int8_t next = sq + Square::Directions::knight_jumps[i];
        if(!(next & Square::inside))
        {
            if(Piece::is_same_sign(stm, board[next])) continue;
            if(board[next] == Piece::eM) moves.emplace_back(sq, next, Move_flag::Quite);
            else moves.emplace_back(sq, next, Move_flag::Capture);
        }
    }
}

void MoveGenerator::generate_pawn_moves(int8_t sq, int8_t stm, const std::vector<Pin>& pins, std::vector<Move>& moves) const
{
    int8_t pin_dir = get_pin_direction(sq, pins);

    // single push, double push and single push-promotion moves
    int8_t next = sq + stm * Square::Directions::N;
    if(board[next] == Piece::eM && (pin_dir == Square::Directions::ND || pin_dir == stm * Square::Directions::N))
    {
        int8_t rank = Square::rank(sq);
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
            int8_t next2 = next + stm * Square::Directions::N;
            if(board[next2] == Piece::eM)
                moves.emplace_back(Move(sq,next2,Move_flag::Double_Pawn_Push));
        }
    }

    // captures, capture-promotions
    next = sq + stm * Square::Directions::NE;
    if (!(next & Square::inside) && Piece::is_different_sign(board[next], stm) && 
        (pin_dir == Square::Directions::ND || pin_dir == stm * Square::Directions::NE))
    {
        int8_t rank = Square::rank(sq);
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

    next = sq + stm * Square::Directions::NW;
    if (!(next & Square::inside) && Piece::is_different_sign(board[next], stm) && 
        (pin_dir == Square::Directions::ND || pin_dir == stm * Square::Directions::NW))
    {
        int8_t rank = Square::rank(sq);
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
    if(ep != def::none && (sq == (ep + stm * Square::Directions::SW) || sq == (ep + stm * Square::Directions::SE)) && 
       (pin_dir == Square::Directions::ND || pin_dir == (ep - sq)))
        moves.emplace_back(Move(sq, ep, Move_flag::En_Passant_Capture));
}

std::vector<Move> MoveGenerator::generate_moves() const
{
   std::vector<Move> moves;
   auto ploc = board_obj.get_piece_locations();

   int8_t stm = static_cast<int8_t>(board_obj.get_side_to_move());
   int8_t king_pos = ploc.find(stm * Piece::King)->second;
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
   auto r = ploc.equal_range(stm * Piece::Queen);
   for (auto it = r.first; it != r.second; it++) 
       generate_sliding_piece_moves(it->second, Piece::Queen, stm, attpin.second, moves);

   r = ploc.equal_range(stm * Piece::Rook);
   for (auto it = r.first; it != r.second; it++)
       generate_sliding_piece_moves(it->second, Piece::Rook, stm, attpin.second, moves);

   r = ploc.equal_range(stm * Piece::Bishop);
   for (auto it = r.first; it != r.second; it++)
       generate_sliding_piece_moves(it->second, Piece::Bishop, stm, attpin.second, moves);
   
   r = ploc.equal_range(stm * Piece::Knight);
   for (auto it = r.first; it != r.second; it++)
       generate_knight_moves(it->second, stm, attpin.second, moves);

   r = ploc.equal_range(stm * Piece::Pawn);
   for (auto it = r.first; it != r.second; it++)
       generate_pawn_moves(it->second, stm, attpin.second, moves);

   return moves;
}