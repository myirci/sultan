#include "utility.hpp"
#include "fen.hpp"
#include "piece.hpp"
#include "definitions.hpp"
#include "square.hpp"
#include "board.hpp"
#include "factory.hpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <random>

std::unordered_map<MoveType, char const *> const Utility::move_type_to_chararr
{
    { MoveType::Quite, "Quite" }, 
    { MoveType::Capture, "Capture" }, 
    { MoveType::Double_Pawn_Push, "Double Pawn Push" },
    { MoveType::Bishop_Promotion, "Bishop Promotion" }, 
    { MoveType::Bishop_Promotion_Capture, "Bishop Promotion Capture" }, 
    { MoveType::En_Passant_Capture, "En-passant Capture" }, 
    { MoveType::King_Side_Castle, "King Side Castle" }, 
    { MoveType::Knight_Promotion, "Knight Promotion" }, 
    { MoveType::Knight_Promotion_Capture, "Knight Promotion Capture" },
    { MoveType::Queen_Promotion, "Queen Promotion" }, 
    { MoveType::Queen_Promotion_Capture, "Queen Promotio n Capture" }, 
    { MoveType::Queen_Side_Castle, "Queen Side Castle" }, 
    { MoveType::Rook_Promotion, "Rook Promotion" }, 
    { MoveType::Rook_Promotion_Capture, "Rook Promotion Capture" }
};

bool Utility::is_equal(Move const& m1, Move const& m2)
{
    return m1.get_from() == m2.get_from() && 
           m1.get_to() == m2.get_to() && 
           m1.get_move_type() == m2.get_move_type() &&
           m1.get_captured_piece() == m2.get_captured_piece();
}

bool Utility::is_equal(Board const& b1, Board const& b2) 
{
    return b1.get_castling_rights() == b2.get_castling_rights() && b1.get_side_to_move() == b2.get_side_to_move() &&
           b1.get_full_move_counter() == b2.get_full_move_counter() && b1.get_half_move_counter() == b2.get_half_move_counter() &&
           b1.get_en_passant_loc() == b2.get_en_passant_loc() && std::equal(b1.board, b1.board + Board::BOARDSIZE, b2.board) &&
           b1.ploc->piece_loc == b2.ploc->piece_loc && b1.attacks->checks_and_pins == b2.attacks->checks_and_pins;
}

std::string Utility::board_to_fen_string(Board const & b)
{
    auto board = b.get_board();
    int count_empty{0};
    std::stringstream ss;
    for(size_t i = 0; i < 64; i++)
    {
        if(board[square::order[i]] == piece::eM)
        {
            count_empty++;
        }
        else
        {
           if(count_empty != 0)
           {
               ss << count_empty;
               count_empty = 0;
           }
           auto it = piece::piece_to_char.find(board[square::order[i]]);
           ss << it->second;
        }
        if((i+1) % 8 == 0)
        {
            if(count_empty != 0)
            {
                ss << count_empty;
                count_empty = 0;
            }

            if(i != 63) ss << "/";
        }
    }

    auto ac = b.get_side_to_move() == color::white ? " w" : " b";
    ss << ac;

    std::string castling{""};
    if(b.query_castling_rights(Castling::white_king_side)) castling.append("K");
    if(b.query_castling_rights(Castling::white_queen_side)) castling.append("Q");
    if(b.query_castling_rights(Castling::black_king_side)) castling.append("k");
    if(b.query_castling_rights(Castling::black_queen_side)) castling.append("q");
    if(castling == "") ss << " -";
    else ss << " " << castling;

    auto ep = b.get_en_passant_loc();
    if(ep == def::none) ss << " -";
    else
    {
       auto p = square::rank_and_file(ep);
       ss << " " << static_cast<char>(p.second + 'a') << static_cast<char>(p.first + '1');
    }

    ss << " " << b.get_half_move_counter() << " " << b.get_full_move_counter();
    return ss.str();
}

void Utility::print_board(Board const & b, bool full)
{
    auto board = b.get_board();
    if(full)
    {
        for (auto i = 0; i < 128; i++)
        {
            if(i % 16 == 0) std::cout << "\n";
            std::cout << std::setw(3) << piece::piece_to_char.find(board[square::full_order[i]])->second << " ";
        }
    }
    else
    {
        for (auto i = 0; i < 64; i++)
        {
            if(i % 8 == 0) std::cout << "\n";
            std::cout << std::setw(3) << piece::piece_to_char.find(board[square::order[i]])->second << " ";
        }
    }

    std::cout << "\n";
}

void Utility::print_piece_locations(Board const& b) 
{
    std::cout << std::endl;
    auto ploc = b.ploc->piece_loc;
    for (auto it = ploc.begin(); it != ploc.end(); it++)
        std::cout << piece::piece_to_char.find(it->first)->second << " at " << square::square_to_string(it->second) << "\n";
    std::cout << std::endl;
}

void Utility::generate_and_print_moves(Board const& board)
{
    auto moves = MoveGenerator::generate_moves(board);
    std::cout << std::endl;
    std::cout << moves.size() << " legal moves : ";
    for (auto it = moves.begin(); it != moves.end(); it++)
        std::cout << *it << " ";
    std::cout << std::endl;
}

void Utility::generate_and_print_moves(std::string_view fen_str) 
{
    auto b = Factory::CreateBoard(fen_str);
    generate_and_print_moves(*b);
}

void Utility::print_moves(std::vector<Move> const& moves) 
{
    for (auto it = moves.begin(); it != moves.end(); it++)
        print_move(*it);
}

void Utility::print_move(Move const& m) 
{
    auto captured = m.is_capture() ? piece::piece_to_char.find(m.get_captured_piece())->second : ' ';
    std::cout
        << "From: " << square::square_to_string(m.get_from())
        << " To: " << square::square_to_string(m.get_to())
        << " Type: " << Utility::move_type_to_chararr.find(m.get_move_type())->second << " " << captured << std::endl;
}

std::string Utility::to_string(Move const& mv) 
{
    std::string str = square::square_to_string(mv.get_from()) + square::square_to_string(mv.get_to());
    if (mv.is_promotion()) 
    {
        switch (mv.get_move_type()) 
        {
            case MoveType::Queen_Promotion:
            case MoveType::Queen_Promotion_Capture:
                str.push_back('q');
                break;
            case MoveType::Knight_Promotion:
            case MoveType::Knight_Promotion_Capture:
                str.push_back('n');
                break;
            case MoveType::Rook_Promotion:
            case MoveType::Rook_Promotion_Capture:
                str.push_back('r');
                break;
            case MoveType::Bishop_Promotion:
            case MoveType::Bishop_Promotion_Capture:
                str.push_back('b');
                break;
        }
    }
    return str;
}

void Utility::print_attacks(std::unordered_map<int8_t, std::pair<int8_t, int8_t>> const& attack_info)
{
    for (auto it = attack_info.begin(); it != attack_info.end(); it++) 
    {
        std::cout << "attacker loc:\t" << square::square_to_string(it->second.first)
                  << ", \tattack dir:\t" << direction::direction_to_chararray.find(it->second.second)->second
                  << ", \ttarget square:\t" << square::square_to_string(it->first)
                  << std::endl;
    } 
}

std::vector<std::string> Utility::tokenize(std::string const& str)
{
    std::vector<std::string> tokens;
    std::stringstream ss{ str };
    std::string token{ "" };
    while (ss >> token)
        tokens.push_back(token);
    return std::move(tokens);
}

Move Utility::to_move(std::string_view str, Board const& brd)
{
    if (str.length() != 4 && str.length() != 5)
        throw std::logic_error("Invalid move!");

    int8_t from{ square::string_to_square(str[0], str[1]) };
    int8_t to{ square::string_to_square(str[2], str[3]) };
    if (to == brd.get_en_passant_loc())
        return Move(from, to, MoveType::En_Passant_Capture, -brd.get_side_to_move() * piece::Pawn);
    int8_t cap = brd.board[to] == piece::eM ? def::none : brd.board[to];
    
    MoveType mt{ MoveType::Quite };
    if (str.length() == 5)
    {
        // promotion and promotion with capture
        if (str[4] == 'q')       mt = cap == def::none ? MoveType::Queen_Promotion : MoveType::Queen_Promotion_Capture;
        else if (str[4] == 'n')  mt = cap == def::none ? MoveType::Knight_Promotion : MoveType::Knight_Promotion_Capture;
        else if (str[4] == 'r')  mt = cap == def::none ? MoveType::Rook_Promotion : MoveType::Rook_Promotion_Capture;
        else if (str[4] == 'b')  mt = cap == def::none ? MoveType::Bishop_Promotion : MoveType::Bishop_Promotion_Capture;
    }
    else if (cap != def::none)
    {
        mt = MoveType::Capture;
    }
    else
    {
        // castle and double pawn push
        if ((brd.board[from] == piece::wP && (to - from) == (2 * direction::N)) ||
            (brd.board[from] == piece::bP && (to - from) == (2 * direction::S)))
        {
            mt = MoveType::Double_Pawn_Push;
        }
        else if (brd.board[from] == piece::wK && from == square::e1)
        {
            if (to == square::g1)      mt = MoveType::King_Side_Castle;
            else if (to == square::c1) mt = MoveType::Queen_Side_Castle;
        }
        else if (brd.board[from] == piece::bK && from == square::e8)
        {
            if (to == square::g8)      mt = MoveType::King_Side_Castle;
            else if (to == square::c8) mt = MoveType::Queen_Side_Castle;
        }
    }

    return Move(from, to, mt, cap);
}

int Utility::get_random(int lower_limit, int upper_limit) 
{
    std::random_device rd;
    std::default_random_engine generator{rd()};
    std::uniform_int_distribution<int> distribution(lower_limit, upper_limit);
    return distribution(generator);
}