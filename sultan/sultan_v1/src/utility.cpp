#include "utility.hpp"
#include "fen.hpp"
#include "piece.hpp"
#include "board_representation/square.hpp"
#include "board_representation/board.hpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>

std::unordered_map<Move_flag, char const *> const Utility::move_flag_to_chararr
{
    {Move_flag::Quite, "Quite"}, {Move_flag::Capture, "Capture"}, {Move_flag::Double_Pawn_Push, "Double Pawn Push"},
    {Move_flag::Bishop_Promotion, "Bishop Promotion"}, {Move_flag::Bishop_Promotion_Capture, "Bishop Promotion Capture"}, 
    {Move_flag::En_Passant_Capture, "En-passant Capture"}, {Move_flag::King_Side_Castle, "King Side Castle"}, 
    {Move_flag::Knight_Promotion, "Knight Promotion"}, {Move_flag::Knight_Promotion_Capture, "Knight Promotion Capture"},
    {Move_flag::Queen_Promotion, "Queen Promotion"}, {Move_flag::Queen_Promotion_Capture, "Queen Promotion Capture"}, 
    {Move_flag::Queen_Side_Castle, "Queen Side Castle"}, {Move_flag::Rook_Promotion, "Rook Promotion"}, 
    {Move_flag::Rook_Promotion_Capture, "Rook Promotion Capture"}
};

bool Utility::is_equal(Move m1, Move m2) 
{
    return m1.get_from() == m2.get_from() && 
           m1.get_to() == m2.get_to() && 
           m1.get_move_flag() == m2.get_move_flag();
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
                    board[Square::order[idx++]] = Piece::eM;
            }
            else
            {
                int8_t loc { Square::order[idx++] };
                int8_t piece = Piece::char_to_piece.at(c);
                board[loc] = piece;
                piece_loc.insert(std::make_pair(piece, loc));
            }
        }
    }
    b.set_piece_locations(std::move(piece_loc));

    // active color
    if (f.active_color == "w")
        b.set_side_to_move(Piece::white);
    else if(f.active_color == "b")
        b.set_side_to_move(Piece::black);
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
    if (f.en_passant == "-") b.set_en_passant_loc(Piece::nN);
    else b.set_en_passant_loc(Square::sq(static_cast<int8_t>(f.en_passant[1] - '1'), static_cast<int8_t>(f.en_passant[0] - 'a')));

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
        if(board[Square::order[i]] == Piece::eM)
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
           auto it = Piece::piece_to_char.find(board[Square::order[i]]);
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

    auto ac = b.get_side_to_move() == Piece::white ? " w" : " b";
    ss << ac;

    std::string castling{""};
    if(b.query_castling_rights(Castling::white_king_side)) castling.append("K");
    if(b.query_castling_rights(Castling::white_queen_side)) castling.append("Q");
    if(b.query_castling_rights(Castling::black_king_side)) castling.append("k");
    if(b.query_castling_rights(Castling::black_queen_side)) castling.append("q");
    if(castling == "") ss << " -";
    else ss << " " << castling;

    auto ep = b.get_en_passant_loc();
    if(ep == Piece::nN) ss << " -";
    else
    {
       auto p = Square::rank_and_file(ep);
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
            std::cout << std::setw(3) << static_cast<int>(board[Square::full_order[i]]) << " ";
        }
    }
    else
    {
        for (auto i = 0; i < 64; i++)
        {
            if(i % 8 == 0) std::cout << "\n";
            std::cout << std::setw(3) << static_cast<int>(board[Square::order[i]]) << " ";
        }
    }

    std::cout << "\n";    
}

void Utility::print_moves(std::vector<Move> const& moves) 
{
    for (auto it = moves.begin(); it != moves.end(); it++)
        print_move(*it);
}


void Utility::print_move(Move m) 
{
    std::cout
        << "From: " << Square::square_to_string(m.get_from())
        << " To: " << Square::square_to_string(m.get_to())
        << " Flag: " << Utility::move_flag_to_chararr.find(m.get_move_flag())->second
        << std::endl;
}