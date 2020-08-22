#include "board_tests.hpp"
#include "../board.hpp"
#include "../fen.hpp"
#include "../utility.hpp"
#include "../square.hpp"
#include "../piece.hpp"

#include <iostream>
#include <algorithm>

namespace test 
{
	void BoardTests::run_test() 
	{
		std::vector<bool (*)()> tests{ test1, test2, test3, test4, test5, test6, test7, test8, test8, test10,
        test11, test12, test13, test14, test15};
		TestBase::run_test("Board", tests);
	}

    bool BoardTests::test1()
    {
        // test: default constructor, clear func
        Board b;
        auto func = [](int8_t v) { return v == def::none; };
        return
            std::all_of(b.get_board(), b.get_board() + Board::BOARDSIZE, func) &&
            b.get_piece_locations().empty() &&
            b.get_en_passant_loc() == def::none &&
            b.get_castling_rights() == 0 &&
            b.get_side_to_move() == def::none &&
            b.get_half_move_counter() == -1 &&
            b.get_full_move_counter() == -1;
    }

    bool BoardTests::test2()
    {
        Board b;
        b.set_side_to_move(color::white);
        return b.get_side_to_move() == color::white;
    }

    bool BoardTests::test3()
    {
        Board b;
        b.set_side_to_move(color::black);
        return b.get_side_to_move() == color::black;
    }

    bool BoardTests::test4()
    {
        Board b;
        b.set_castling_rights(Castling::white_king_side, true);
        bool res1{ b.get_castling_rights() == 1 &&
                    b.query_castling_rights(Castling::white_king_side) == true &&
                    b.query_castling_rights(Castling::white_queen_side) == false &&
                    b.query_castling_rights(Castling::black_king_side) == false &&
                    b.query_castling_rights(Castling::black_queen_side) == false };

        b.set_castling_rights(Castling::white_king_side, false);
        bool res2{ b.get_castling_rights() == 0 &&
                    b.query_castling_rights(Castling::white_king_side) == false &&
                    b.query_castling_rights(Castling::white_queen_side) == false &&
                    b.query_castling_rights(Castling::black_king_side) == false &&
                    b.query_castling_rights(Castling::black_queen_side) == false };
        return res1 && res2;
    }

    bool BoardTests::test5()
    {
        Board b;
        b.set_castling_rights(Castling::white_queen_side, true);
        bool res1{ b.get_castling_rights() == 2 &&
                    b.query_castling_rights(Castling::white_king_side) == false &&
                    b.query_castling_rights(Castling::white_queen_side) == true &&
                    b.query_castling_rights(Castling::black_king_side) == false &&
                    b.query_castling_rights(Castling::black_queen_side) == false };

        b.set_castling_rights(Castling::white_queen_side, false);
        bool res2{ b.get_castling_rights() == 0 &&
                    b.query_castling_rights(Castling::white_king_side) == false &&
                    b.query_castling_rights(Castling::white_queen_side) == false &&
                    b.query_castling_rights(Castling::black_king_side) == false &&
                    b.query_castling_rights(Castling::black_queen_side) == false };
        return res1 && res2;
    }

    bool BoardTests::test6()
    {
        Board b;
        b.set_castling_rights(Castling::black_king_side, true);
        bool res1{ b.get_castling_rights() == 4 &&
                    b.query_castling_rights(Castling::white_king_side) == false &&
                    b.query_castling_rights(Castling::white_queen_side) == false &&
                    b.query_castling_rights(Castling::black_king_side) == true &&
                    b.query_castling_rights(Castling::black_queen_side) == false };

        b.set_castling_rights(Castling::black_king_side, false);
        bool res2{ b.get_castling_rights() == 0 &&
                    b.query_castling_rights(Castling::white_king_side) == false &&
                    b.query_castling_rights(Castling::white_queen_side) == false &&
                    b.query_castling_rights(Castling::black_king_side) == false &&
                    b.query_castling_rights(Castling::black_queen_side) == false };
        return res1 && res2;
    }

    bool BoardTests::test7()
    {
        Board b;
        b.set_castling_rights(Castling::black_queen_side, true);
        bool res1{ b.get_castling_rights() == 8 &&
                    b.query_castling_rights(Castling::white_king_side) == false &&
                    b.query_castling_rights(Castling::white_queen_side) == false &&
                    b.query_castling_rights(Castling::black_king_side) == false &&
                    b.query_castling_rights(Castling::black_queen_side) == true };

        b.set_castling_rights(Castling::black_queen_side, false);
        bool res2{ b.get_castling_rights() == 0 &&
                    b.query_castling_rights(Castling::white_king_side) == false &&
                    b.query_castling_rights(Castling::white_queen_side) == false &&
                    b.query_castling_rights(Castling::black_king_side) == false &&
                    b.query_castling_rights(Castling::black_queen_side) == false };
        return res1 && res2;
    }


    bool BoardTests::test8()
    {
        // test: quite move 
        Board b;
        std::string fen1{ "8/3k4/2r5/8/7B/8/1K4P1/8 w - - 0 1" };
        std::string fen2{ "8/3k4/2r5/8/8/8/1K3BP1/8 b - - 1 1" };
        Fen f(fen1);
        Utility::fen_to_board(f, b);
        Move m(square::h4, square::f2, MoveType::Quite, def::none);
        b.make_move(m);
        auto fen3 = Utility::board_to_fen_string(b);
        if (fen3 != fen2)
        {
            std::cout << "Board is not correct after make move!" << std::endl;
            std::cout << "Expected: " << fen2 << std::endl;
            std::cout << "Got     : " << fen3 << std::endl;
            return false;
        }

        b.unmake_move(m);
        auto fen4 = Utility::board_to_fen_string(b);
        if (fen4 != fen1)
        {
            std::cout << "Board is not correct after unmake move!" << std::endl;
            std::cout << "Expected: " << fen2 << std::endl;
            std::cout << "Got     : " << fen4 << std::endl;
            return false;
        }
        return true;
    }

    bool BoardTests::test9()
    {
        // test: double pawn push move 
        Board b;
        std::string fen1{ "8/3k4/2r5/8/7B/8/1K4P1/8 w - - 0 1" };
        std::string fen2{ "8/3k4/2r5/8/6PB/8/1K6/8 b - g3 1 1" };
        Fen f(fen1);
        Utility::fen_to_board(f, b);
        Move m(square::g2, square::g4, MoveType::Double_Pawn_Push, def::none);
        b.make_move(m);
        auto fen3 = Utility::board_to_fen_string(b);
        if (fen3 != fen2)
        {
            std::cout << "Board is not correct after make move!" << std::endl;
            std::cout << "Expected: " << fen2 << std::endl;
            std::cout << "Got     : " << fen3 << std::endl;
            return false;
        }

        b.unmake_move(m);
        auto fen4 = Utility::board_to_fen_string(b);
        if (fen4 != fen1)
        {
            std::cout << "Board is not correct after unmake move!" << std::endl;
            std::cout << "Expected: " << fen2 << std::endl;
            std::cout << "Got     : " << fen4 << std::endl;
            return false;
        }
        return true;
    }

    bool BoardTests::test10()
    {
        // test: capture move 
        Board b;
        std::string fen1{ "8/3k3q/2r5/8/6PB/8/1K2Q3/8 b - - 0 1" };
        std::string fen2{ "8/3k4/2r5/8/6Pq/8/1K2Q3/8 w - - 1 2" };
        Fen f(fen1);
        Utility::fen_to_board(f, b);
        Move m(square::h7, square::h4, MoveType::Capture, piece::wB);
        b.make_move(m);
        auto fen3 = Utility::board_to_fen_string(b);
        if (fen3 != fen2)
        {
            std::cout << "Board is not correct after make move!" << std::endl;
            std::cout << "Expected: " << fen2 << std::endl;
            std::cout << "Got     : " << fen3 << std::endl;
            return false;
        }

        b.unmake_move(m);
        auto fen4 = Utility::board_to_fen_string(b);
        if (fen4 != fen1)
        {
            std::cout << "Board is not correct after unmake move!" << std::endl;
            std::cout << "Expected: " << fen2 << std::endl;
            std::cout << "Got     : " << fen4 << std::endl;
            return false;
        }
        return true;
    }

    bool BoardTests::test11()
    {
        // test: en passant capture move - white 
        Board b;
        std::string fen1{ "4k3/8/8/2PpP3/6p1/8/5P1P/4K3 w - d6 0 1" };
        std::string fen2{ "4k3/8/3P4/4P3/6p1/8/5P1P/4K3 b - - 1 1" };
        Fen f(fen1);
        Utility::fen_to_board(f, b);
        Move m(square::c5, square::d6, MoveType::En_Passant_Capture, piece::bP);
        b.make_move(m);
        auto fen3 = Utility::board_to_fen_string(b);
        if (fen3 != fen2)
        {
            std::cout << "Board is not correct after make move!" << std::endl;
            std::cout << "Expected: " << fen2 << std::endl;
            std::cout << "Got     : " << fen3 << std::endl;
            return false;
        }

        b.unmake_move(m);
        auto fen4 = Utility::board_to_fen_string(b);
        if (fen4 != fen1)
        {
            std::cout << "Board is not correct after unmake move!" << std::endl;
            std::cout << "Expected: " << fen2 << std::endl;
            std::cout << "Got     : " << fen4 << std::endl;
            return false;
        }
        return true;
    }

    bool BoardTests::test12()
    {
        // test: en passant capture move - white 
        Board b;
        std::string fen1{ "4k3/8/8/2PpP3/6p1/8/5P1P/4K3 w - d6 0 1" };
        std::string fen2{ "4k3/8/3P4/2P5/6p1/8/5P1P/4K3 b - - 1 1" };
        Fen f(fen1);
        Utility::fen_to_board(f, b);
        Move m(square::e5, square::d6, MoveType::En_Passant_Capture, piece::bP);
        b.make_move(m);
        auto fen3 = Utility::board_to_fen_string(b);
        if (fen3 != fen2)
        {
            std::cout << "Board is not correct after make move!" << std::endl;
            std::cout << "Expected: " << fen2 << std::endl;
            std::cout << "Got     : " << fen3 << std::endl;
            return false;
        }

        b.unmake_move(m);
        auto fen4 = Utility::board_to_fen_string(b);
        if (fen4 != fen1)
        {
            std::cout << "Board is not correct after unmake move!" << std::endl;
            std::cout << "Expected: " << fen2 << std::endl;
            std::cout << "Got     : " << fen4 << std::endl;
            return false;
        }
        return true;
    }

    bool BoardTests::test13()
    {
        // test: en passant capture move - black 
        Board b;
        std::string fen1{ "4k3/8/8/2PpP3/6pP/8/5P2/4K3 b - h3 0 1" };
        std::string fen2{ "4k3/8/8/2PpP3/8/7p/5P2/4K3 w - - 1 2" };
        Fen f(fen1);
        Utility::fen_to_board(f, b);
        Move m(square::g4, square::h3, MoveType::En_Passant_Capture, piece::wP);
        b.make_move(m);
        auto fen3 = Utility::board_to_fen_string(b);
        if (fen3 != fen2)
        {
            std::cout << "Board is not correct after make move!" << std::endl;
            std::cout << "Expected: " << fen2 << std::endl;
            std::cout << "Got     : " << fen3 << std::endl;
            return false;
        }

        b.unmake_move(m);
        auto fen4 = Utility::board_to_fen_string(b);
        if (fen4 != fen1)
        {
            std::cout << "Board is not correct after unmake move!" << std::endl;
            std::cout << "Expected: " << fen2 << std::endl;
            std::cout << "Got     : " << fen4 << std::endl;
            return false;
        }
        return true;
    }

    bool BoardTests::test14()
    {
        // test: en passant capture move - black 
        Board b;
        std::string fen1{ "4k3/8/8/2PpP3/5PpP/8/8/4K3 b - f3 0 1" };
        std::string fen2{ "4k3/8/8/2PpP3/7P/5p2/8/4K3 w - - 1 2" };
        Fen f(fen1);
        Utility::fen_to_board(f, b);
        Move m(square::g4, square::f3, MoveType::En_Passant_Capture, piece::wP);
        b.make_move(m);
        auto fen3 = Utility::board_to_fen_string(b);
        if (fen3 != fen2)
        {
            std::cout << "Board is not correct after make move!" << std::endl;
            std::cout << "Expected: " << fen2 << std::endl;
            std::cout << "Got     : " << fen3 << std::endl;
            return false;
        }

        b.unmake_move(m);
        auto fen4 = Utility::board_to_fen_string(b);
        if (fen4 != fen1)
        {
            std::cout << "Board is not correct after unmake move!" << std::endl;
            std::cout << "Expected: " << fen2 << std::endl;
            std::cout << "Got     : " << fen4 << std::endl;
            return false;
        }
        return true;
    }

    bool BoardTests::test15()
    {
        // test: king side castle white and black
        Board b;
        std::string fen1{ "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1" };
        std::string fen2{ "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R4RK1 b kq - 1 1" };
        std::string fen3{ "r4rk1/pppppppp/8/8/8/8/PPPPPPPP/R4RK1 w - - 2 2" };

        Fen f(fen1);
        Utility::fen_to_board(f, b);

        // white king side castle
        Move m1(square::e1, square::g1, MoveType::King_Side_Castle, def::none);
        b.make_move(m1);
        auto fen4 = Utility::board_to_fen_string(b);
        if (fen4 != fen2)
        {
            std::cout << "Board is not correct after make move!" << std::endl;
            std::cout << "Expected: " << fen2 << std::endl;
            std::cout << "Got     : " << fen4 << std::endl;
            return false;
        }

        // black king side castle
        Move m2(square::e8, square::g8, MoveType::King_Side_Castle, def::none);
        b.make_move(m2);
        auto fen5 = Utility::board_to_fen_string(b);
        if (fen5 != fen3)
        {
            std::cout << "Board is not correct after make move!" << std::endl;
            std::cout << "Expected: " << fen3 << std::endl;
            std::cout << "Got     : " << fen5 << std::endl;
            return false;
        }

        b.unmake_move(m2);
        auto fen6 = Utility::board_to_fen_string(b);
        if (fen6 != fen2)
        {
            std::cout << "Board is not correct after unmake move!" << std::endl;
            std::cout << "Expected: " << fen2 << std::endl;
            std::cout << "Got     : " << fen6 << std::endl;
            return false;
        }

        b.unmake_move(m1);
        auto fen7 = Utility::board_to_fen_string(b);
        if (fen7 != fen1)
        {
            std::cout << "Board is not correct after unmake move!" << std::endl;
            std::cout << "Expected: " << fen1 << std::endl;
            std::cout << "Got     : " << fen7 << std::endl;
            return false;
        }
        return true;
    }

}