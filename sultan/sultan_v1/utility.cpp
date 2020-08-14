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

void Utility::fen_to_board(const Fen& f, Board& b)
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
    b.set_piece_locations(piece_loc);

    // active color
    if (f.active_color == "w")
        b.set_side_to_move(Piece::white);
    else if(f.active_color == "b")
        b.set_side_to_move(Piece::black);
    else
        throw std::invalid_argument("active color parse error");

    // castling availability
    if (f.castling_availability.find('K') != std::string::npos)
        b.set_castling_availability(Castling::white_king_side, true);
    if (f.castling_availability.find('Q') != std::string::npos)
        b.set_castling_availability(Castling::white_queen_side, true);
    if (f.castling_availability.find('k') != std::string::npos)
        b.set_castling_availability(Castling::black_king_side, true);
    if (f.castling_availability.find('q') != std::string::npos)
        b.set_castling_availability(Castling::black_queen_side, true);

    // en passant
    if (f.en_passant == "-") b.set_en_passant_loc(Piece::nN);
    else b.set_en_passant_loc(Square::sq(static_cast<int8_t>(f.en_passant[1] - '1'), static_cast<int8_t>(f.en_passant[0] - 'a')));

    // half_move_clock
    b.set_half_move_counter(f.half_move_clock);

    // full_move_number
    b.set_full_move_counter(f.full_move_number);
}

std::string Utility::board_to_fen_string(const Board& b)
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
    if(b.query_castling_availability(Castling::white_king_side)) castling.append("K");
    if(b.query_castling_availability(Castling::white_queen_side)) castling.append("Q");
    if(b.query_castling_availability(Castling::black_king_side)) castling.append("k");
    if(b.query_castling_availability(Castling::black_queen_side)) castling.append("q");
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

void Utility::print_board(const Board &b, bool full)
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

    auto wp = b.get_white_piece_locations();
    std::cout << "White piece locations: " << std::endl;
    for(auto it = wp.begin(); it != wp.end(); it++)
        std::cout << Piece::piece_to_char.find(board[*it])->second << ": " << static_cast<int>(*it) << " ";
    std::cout << "\n";

    auto bp = b.get_black_piece_locations();
    std::cout << "Black piece locations: " << std::endl;
    for(auto it = bp.begin(); it != bp.end(); it++)
        std::cout << Piece::piece_to_char.find(board[*it])->second << ": " << static_cast<int>(*it) << " ";
    std::cout << "\n";

}