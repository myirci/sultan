#include "utility_functions.hpp"
#include "x88_board.hpp"
#include <iostream>
#include <iomanip>

// black pieces are at the bottom
// pieces are printed with their numeric values
void print_board_bb_numeric(const X88_board& b) {
    std::int8_t const * const board = b.get_board();
    for(int i = 0; i < 8; ++i) {
        for(int j = 7; j >= 0; --j) {
            std::cout << std::setw(5) << static_cast<int>(board[i*16 + j]);
        }
        std::cout << std::endl;
    }
    print_other_info(b);
}

// black pieces are at the bottom
// pieces are printed with their character values
void print_board_bb_alphabetic(const X88_board& b) {

    int i = 0;
    for(auto it = b.board_begin(); it != b.board_end(); ++it, ++i) {
        if((i % 8) == 0) { std::cout << std::endl; }
        std::cout << std::setw(2) << piece_to_char(static_cast<Pieces>(*it));
    }

    /*
    std::int8_t const * const board = b.get_board();
    for(int i = 0; i < 8; ++i) {
        for(int j = 7; j >= 0; --j) {
            std::cout << std::setw(2) << piece_to_char(static_cast<Pieces>(board[i*16 + j]));
        }
        std::cout << std::endl;
    }
    */
    print_other_info(b);
}

void print_board_wb_numeric(const X88_board& b) {
    std::int8_t const * const board = b.get_board();
    for(int i = 7; i >= 0; --i) {
        for(int j = 0; j < 8; ++j) {
            std::cout << std::setw(5) << static_cast<int>(board[i*16 + j]);
        }
        std::cout << std::endl;
    }
    print_other_info(b);
}

void print_board_wb_alphabetic(const X88_board& b) {
    std::int8_t const * const board = b.get_board();
    for(int i = 7; i >= 0; --i) {
        for(int j = 0; j < 8; ++j) {
            std::cout << std::setw(2) << piece_to_char(static_cast<Pieces>(board[i*16 + j]));
        }
        std::cout << std::endl;
    }
    print_other_info(b);
}

void print_white_pieces(const X88_board& b) {
    std::cout << std::hex;
    for(X88_board::const_piece_iterator wpit = b.white_pieces_begin(); wpit != b.white_pieces_end(); ++wpit) {
        std::cout << std::setw(5) << static_cast<int>(*wpit) << " ";
    }
    std::cout << std::dec;
}

void print_black_pieces(const X88_board& b) {
    std::cout << std::hex;
    for(X88_board::const_piece_iterator bpit = b.black_pieces_begin(); bpit != b.black_pieces_end(); ++bpit) {
        std::cout << std::setw(5) << static_cast<int>(*bpit) << " ";
    }
    std::cout << std::dec;
}

void print_moves(std::vector<Move>& moves) {
    int c = 1;
    std::cout << "# of generated moves: " << moves.size() << std::left << std::endl;
    for(auto it = moves.begin(); it != moves.end(); ++it, ++c) {
        std::cout << "move-" << std::setw(3) << c << ": ";
        it->print();
    }
}

void print_other_info(const X88_board& b) {
    std::cout << std::endl;
    std::cout << "Active Color  : " << (b.get_state_property(State::active_color) ? "white" : "black") << "\n"
              << "En passant loc: " << std::hex << static_cast<int>(b.get_en_passant_loc()) << std::dec << "\n"
              << "Half Move Num : " << static_cast<int>(b.get_half_move_num()) << "\n"
              << "Move Number   : " << b.get_move_num() << "\n"
              << "Castling wqs  : " << (b.get_state_property(State::wqs_castling) ? "ok" : "not ok") << "\n"
              << "Castling wks  : " << (b.get_state_property(State::wks_castling) ? "ok" : "not ok") << "\n"
              << "Castling bqs  : " << (b.get_state_property(State::bqs_castling) ? "ok" : "not ok") << "\n"
              << "Castling bks  : " << (b.get_state_property(State::bks_castling) ? "ok" : "not ok") << "\n"
              << std::endl;
}

void generate_moves_iteratively(X88_board& b, int ply) {

//    int ply_count = ply;
//    std::uint64_t move_count = 0;
//    std::vector<move> moves;
//    int k = 1;
//    while (ply_count > 0) {
//        b.generate_moves(moves);
//        std::cout << "Ply-" << k << ": " << moves.size() << std::endl;
//        move_count += moves.size();
//        b.apply_move<White>(moves[0]);
//        print_board_wb_alphabetic(b);
////        for(auto it = moves.begin(); it != moves.end(); ++it) {
////            print_board_wb_alphabetic(b);
////        }
//        --ply_count;
//        ++k;
//    }
}
