#include "utility.hpp"
#include "fen.hpp"
#include "piece.hpp"
#include "definitions.hpp"
#include "square.hpp"
#include "board.hpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>

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

bool Utility::is_equal(Move m1, Move m2) 
{
    return m1.get_from() == m2.get_from() && 
           m1.get_to() == m2.get_to() && 
           m1.get_move_type() == m2.get_move_type() &&
           m1.get_captured_piece() == m2.get_captured_piece();
}

void Utility::fen_to_board(Fen const & f, Board& b)
{
    // piece placement
    size_t start{0}, pos{0};
    int idx{0};
    std::string row{""}, pp{f.piece_placement + "/" };
    std::unordered_multimap<int8_t, int8_t> piece_loc;
    auto board = b.get_board();
    while((pos = pp.find('/', start)) != std::string::npos)
    {
        row = pp.substr(start, pos - start);
        start = pos + 1;

        for(char c : row)
        {
            if(std::isdigit(c))
            {
                for(int i = 0; i < static_cast<int>(c - '0'); i++)
                    board[square::order[idx++]] = piece::eM;
            }
            else
            {
                int8_t loc { square::order[idx++] };
                int8_t piece = piece::char_to_piece.at(c);
                board[loc] = piece;
                piece_loc.insert(std::make_pair(piece, loc));
            }
        }
    }
    b.set_piece_locations(std::move(piece_loc));

    // active color
    if (f.active_color == "w")
        b.set_side_to_move(color::white);
    else if(f.active_color == "b")
        b.set_side_to_move(color::black);
    else
        throw std::invalid_argument("active color parse error");

    // castling availability
    if (f.castling_availability.find('K') != std::string::npos)
        b.set_castling_rights(Castling::white_king_side, true);
    if (f.castling_availability.find('Q') != std::string::npos)
        b.set_castling_rights(Castling::white_queen_side, true);
    if (f.castling_availability.find('k') != std::string::npos)
        b.set_castling_rights(Castling::black_king_side, true);
    if (f.castling_availability.find('q') != std::string::npos)
        b.set_castling_rights(Castling::black_queen_side, true);

    // en passant
    if (f.en_passant == "-") b.set_en_passant_loc(def::none);
    else b.set_en_passant_loc(square::sq(static_cast<int8_t>(f.en_passant[1] - '1'), static_cast<int8_t>(f.en_passant[0] - 'a')));

    // half_move_clock
    b.set_half_move_counter(f.half_move_clock);

    // full_move_number
    b.set_full_move_counter(f.full_move_number);
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
    std::cout << "\n";
    if(full)
    {
        for (auto i = 0; i < 128; i++)
        {
            if(i % 16 == 0) std::cout << "\n";
            std::cout << std::setw(3) << static_cast<int>(board[square::full_order[i]]) << " ";
        }
    }
    else
    {
        for (auto i = 0; i < 64; i++)
        {
            if(i % 8 == 0) std::cout << "\n";
            std::cout << std::setw(3) << static_cast<int>(board[square::order[i]]) << " ";
        }
    }

    std::cout << "\n";

    std::cout << "\nPiece Locations" << std::endl;
    auto ploc = b.get_piece_locations();
    for (auto it = ploc.begin(); it != ploc.end(); it++) 
        std::cout << piece::piece_to_char.find(it->first)->second << " at " << square::square_to_string(it->second) << ", ";
    std::cout << std::endl;
}

void Utility::generate_and_print_moves(Board const& board) 
{
    MoveGenerator mg{ board };
    auto moves = mg.generate_moves();
    std::cout << moves.size() << " legal moves : ";
    for (auto it = moves.begin(); it != moves.end(); it++)
        std::cout << *it << " ";
    std::cout << std::endl;
}

void Utility::generate_and_print_moves(std::string const& fen_str) 
{
    Fen f{ fen_str };
    Board b;
    fen_to_board(f, b);
    generate_and_print_moves(b);
}


void Utility::print_moves(std::vector<Move> const& moves) 
{
    for (auto it = moves.begin(); it != moves.end(); it++)
        print_move(*it);
}

void Utility::print_move(Move m) 
{
    auto captured = m.is_capture() ? piece::piece_to_char.find(m.get_captured_piece())->second : ' ';
    std::cout
        << "From: " << square::square_to_string(m.get_from())
        << " To: " << square::square_to_string(m.get_to())
        << " Type: " << Utility::move_type_to_chararr.find(m.get_move_type())->second << " " << captured << std::endl;
}

/*
void Utility::print_attacks(std::vector<attack::AttackInfo> const& attack_infos)
{
    for (auto it = attack_infos.begin(); it != attack_infos.end(); it++) 
    {
        std::cout << "attacker loc:\t" << square::square_to_string(it->attacker_loc)
                  << ", \tattack dir:\t" << direction::direction_to_chararray.find(it->attack_dir)->second
                  << ", \ttarget square:\t" << square::square_to_string(it->target_loc)
                  << std::endl;
    } 
}
*/
