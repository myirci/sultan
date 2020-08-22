#include "utility_tests.hpp"
#include "../utility.hpp"
#include "../board.hpp"
#include "../fen.hpp"
#include "../piece.hpp"
#include "../square.hpp"

#include <iostream>

namespace test
{
	void UtilityTests::run_test()
	{
		std::vector<bool (*)()> tests{ test1, test2, test3, test4, test5, test6 };
		TestBase::run_test("Utility", tests);
	}

    bool UtilityTests::test1()
    {
        Board b;
        Fen f;
        Utility::fen_to_board(f, b);
        int8_t board[Board::BOARDSIZE] =
        {
            piece::wR, piece::wN, piece::wB, piece::wQ, piece::wK, piece::wB, piece::wN, piece::wR, def::none, def::none, def::none, def::none, def::none, def::none, def::none, def::none,
            piece::wP, piece::wP, piece::wP, piece::wP, piece::wP, piece::wP, piece::wP, piece::wP, def::none, def::none, def::none, def::none, def::none, def::none, def::none, def::none,
            piece::eM, piece::eM, piece::eM, piece::eM, piece::eM, piece::eM, piece::eM, piece::eM, def::none, def::none, def::none, def::none, def::none, def::none, def::none, def::none,
            piece::eM, piece::eM, piece::eM, piece::eM, piece::eM, piece::eM, piece::eM, piece::eM, def::none, def::none, def::none, def::none, def::none, def::none, def::none, def::none,
            piece::eM, piece::eM, piece::eM, piece::eM, piece::eM, piece::eM, piece::eM, piece::eM, def::none, def::none, def::none, def::none, def::none, def::none, def::none, def::none,
            piece::eM, piece::eM, piece::eM, piece::eM, piece::eM, piece::eM, piece::eM, piece::eM, def::none, def::none, def::none, def::none, def::none, def::none, def::none, def::none,
            piece::bP, piece::bP, piece::bP, piece::bP, piece::bP, piece::bP, piece::bP, piece::bP, def::none, def::none, def::none, def::none, def::none, def::none, def::none, def::none,
            piece::bR, piece::bN, piece::bB, piece::bQ, piece::bK, piece::bB, piece::bN, piece::bR, def::none, def::none, def::none, def::none, def::none, def::none, def::none, def::none
        };

        auto bd = b.get_board();

        for (auto i = 0; i < Board::BOARDSIZE; i++)
        {
            if (board[i] != bd[i])
            {
                std::cout << "Board, mismatch at location " << i << " : " << static_cast<int>(board[i]) << " != " << static_cast<int>(bd[i]) << std::endl;
                return false;
            }
        }

        if (b.get_piece_locations().size() != 32) return false;

        std::vector<int8_t> white_pawn_loc{ square::a2, square::b2, square::c2, square::d2, square::e2, square::f2, square::g2, square::h2 };
        if (!check_piece_locations(b, piece::wP, white_pawn_loc)) return false;

        std::vector<int8_t> black_pawn_loc{ square::a7, square::b7, square::c7, square::d7, square::e7, square::f7, square::g7, square::h7 };
        if (!check_piece_locations(b, piece::bP, black_pawn_loc)) return false;

        std::vector<int8_t> white_rook_loc{ square::a1, square::h1 };
        if (!check_piece_locations(b, piece::wR, white_rook_loc)) return false;

        std::vector<int8_t> black_rook_loc{ square::a8, square::h8 };
        if (!check_piece_locations(b, piece::bR, black_rook_loc)) return false;

        std::vector<int8_t> white_knight_loc{ square::b1, square::g1 };
        if (!check_piece_locations(b, piece::wN, white_knight_loc)) return false;

        std::vector<int8_t> black_knight_loc{ square::b8, square::g8 };
        if (!check_piece_locations(b, piece::bN, black_knight_loc)) return false;

        std::vector<int8_t> white_bishop_loc{ square::c1, square::f1 };
        if (!check_piece_locations(b, piece::wB, white_bishop_loc)) return false;

        std::vector<int8_t> black_bishop_loc{ square::c8, square::f8 };
        if (!check_piece_locations(b, piece::bB, black_bishop_loc)) return false;

        return
            b.get_piece_locations().find(piece::wK)->second == square::e1 && b.get_piece_locations().find(piece::bK)->second == square::e8 &&
            b.get_piece_locations().find(piece::wQ)->second == square::d1 && b.get_piece_locations().find(piece::bQ)->second == square::d8 &&
            b.get_en_passant_loc() == def::none && b.get_castling_rights() == 15 && b.get_side_to_move() == color::white &&
            b.get_half_move_counter() == 0 && b.get_full_move_counter() == 1;
    }

    bool UtilityTests::test2()
    {
        Board b;
        Fen f;
        Utility::fen_to_board(f, b);
        auto fstr = Utility::board_to_fen_string(b);
        return fstr == f.get_fen();
    }

    bool UtilityTests::test3()
    {
        Board b;
        Fen f("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b KQkq c6 1 2");
        Utility::fen_to_board(f, b);
        auto fstr = Utility::board_to_fen_string(b);
        return fstr == f.get_fen();
    }

    bool UtilityTests::test4()
    {
        Board b;
        Fen f("5Q1r/2pb2pp/1pn1p2n/rP2Pp1k/p1Pp1P1q/P1bP1NPB/1B1N3P/R3K1R1 w Q - 0 1");
        Utility::fen_to_board(f, b);
        auto fstr = Utility::board_to_fen_string(b);
        return fstr == f.get_fen();
    }

    bool UtilityTests::test5()
    {
        Board b;
        Fen f("8/3k4/8/8/8/2K5/8/8 w - - 0 1");
        Utility::fen_to_board(f, b);
        auto fstr = Utility::board_to_fen_string(b);
        return fstr == f.get_fen();
    }

    bool UtilityTests::test6()
    {
        Board b;
        Fen f("8/3k3P/P6P/7p/P3q2p/2K4p/7p/8 w - - 0 1");
        Utility::fen_to_board(f, b);
        auto fstr = Utility::board_to_fen_string(b);
        return fstr == f.get_fen();
    }

    bool UtilityTests::check_piece_locations(Board const& b, int8_t piece, std::vector<int8_t> expected_loc)
    {
        auto r = b.get_piece_locations().equal_range(piece);
        int k = 0;
        for (auto it = r.first; it != r.second; it++)
        {
            if (it->second != expected_loc[k++])
            {
                k--;
                std::cout << "Piece location error! expected: " << expected_loc[k] << " got: " << it->second << std::endl;
                return false;
            }
        }
        return true;
    }

}