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
		std::vector<bool (*)()> tests
        { 
            test1, test2, test3, test4, test5, test6, test7, test8, test9, test10,
            test11, test12, test13, test14, test15, test16, test17, test18, test19, test20, 
            test21, test22, test23 
        };
		TestBase::run_test("Board", tests);
	}

    bool BoardTests::compare_fen(std::string const& fen, std::string const& expected_fen, int id) 
    {
        if (fen != expected_fen)
        {
            std::cout << "Board is not correct, id : " << id << std::endl;
            std::cout << "Expected: " << expected_fen << std::endl;
            std::cout << "Got     : " << fen << std::endl;
            return false;
        }
        return true;
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
        auto m = Move(square::h4, square::f2, MoveType::Quite, def::none);
        auto st = b.make_move(m);
        if (!compare_fen(Utility::board_to_fen_string(b), fen2, 1)) return false;
        b.unmake_move(m, st);
        return compare_fen(Utility::board_to_fen_string(b), fen1, 2);
    }

    bool BoardTests::test9()
    {
        // test: double pawn push move black and white
        Board b;
        std::string fen1{ "8/3k1p2/2r5/8/7B/8/1K4P1/8 w - - 0 1" };
        std::string fen2{ "8/3k1p2/2r5/8/6PB/8/1K6/8 b - g3 0 1" };
        std::string fen3{ "8/3k4/2r5/5p2/6PB/8/1K6/8 w - f6 0 2" };
        Fen f(fen1);
        Utility::fen_to_board(f, b);
        Move m1(square::g2, square::g4, MoveType::Double_Pawn_Push, def::none);
        auto st1 = b.make_move(m1);
        if (!compare_fen(Utility::board_to_fen_string(b), fen2, 1)) return false;
        
        Move m2(square::f7, square::f5, MoveType::Double_Pawn_Push, def::none);
        auto st2 = b.make_move(m2);
        if (!compare_fen(Utility::board_to_fen_string(b), fen3, 2)) return false;

        b.unmake_move(m2, st2);
        if (!compare_fen(Utility::board_to_fen_string(b), fen2, 3)) return false;

        b.unmake_move(m1, st1);
        return compare_fen(Utility::board_to_fen_string(b), fen1, 4);
    }

    bool BoardTests::test10()
    {
        // test: capture move 
        Board b;
        std::string fen1{ "8/3k3q/2r5/8/6PB/8/1K2Q3/8 b - - 0 1" };
        std::string fen2{ "8/3k4/2r5/8/6Pq/8/1K2Q3/8 w - - 0 2" };
        Fen f(fen1);
        Utility::fen_to_board(f, b);
        Move m(square::h7, square::h4, MoveType::Capture, piece::wB);
        auto st = b.make_move(m);
        if (!compare_fen(Utility::board_to_fen_string(b), fen2, 1)) return false;

        b.unmake_move(m, st);
        return compare_fen(Utility::board_to_fen_string(b), fen1, 2);
    }

    bool BoardTests::test11()
    {
        // test: en passant capture move - white 
        Board b;
        std::string fen1{ "4k3/8/8/2PpP3/6p1/8/5P1P/4K3 w - d6 0 1" };
        std::string fen2{ "4k3/8/3P4/4P3/6p1/8/5P1P/4K3 b - - 0 1" };
        Fen f(fen1);
        Utility::fen_to_board(f, b);
        Move m(square::c5, square::d6, MoveType::En_Passant_Capture, piece::bP);
        auto st = b.make_move(m);
        if (!compare_fen(Utility::board_to_fen_string(b), fen2, 1)) return false;

        b.unmake_move(m, st);
        return compare_fen(Utility::board_to_fen_string(b), fen1, 2);
    }

    bool BoardTests::test12()
    {
        // test: en passant capture move - white 
        Board b;
        std::string fen1{ "4k3/8/8/2PpP3/6p1/8/5P1P/4K3 w - d6 0 1" };
        std::string fen2{ "4k3/8/3P4/2P5/6p1/8/5P1P/4K3 b - - 0 1" };
        Fen f(fen1);
        Utility::fen_to_board(f, b);
        Move m(square::e5, square::d6, MoveType::En_Passant_Capture, piece::bP);
        auto st = b.make_move(m);
        if (!compare_fen(Utility::board_to_fen_string(b), fen2, 1)) return false;

        b.unmake_move(m, st);
        return compare_fen(Utility::board_to_fen_string(b), fen1, 2);
    }

    bool BoardTests::test13()
    {
        // test: en passant capture move - black 
        Board b;
        std::string fen1{ "4k3/8/8/2PpP3/6pP/8/5P2/4K3 b - h3 0 1" };
        std::string fen2{ "4k3/8/8/2PpP3/8/7p/5P2/4K3 w - - 0 2" };
        Fen f(fen1);
        Utility::fen_to_board(f, b);
        Move m(square::g4, square::h3, MoveType::En_Passant_Capture, piece::wP);
        auto st = b.make_move(m);
        if (!compare_fen(Utility::board_to_fen_string(b), fen2, 1)) return false;

        b.unmake_move(m, st);
        auto fen4 = Utility::board_to_fen_string(b);
        return compare_fen(Utility::board_to_fen_string(b), fen1, 2);
    }

    bool BoardTests::test14()
    {
        // test: en passant capture move - black 
        Board b;
        std::string fen1{ "4k3/8/8/2PpP3/5PpP/8/8/4K3 b - f3 0 1" };
        std::string fen2{ "4k3/8/8/2PpP3/7P/5p2/8/4K3 w - - 0 2" };
        Fen f(fen1);
        Utility::fen_to_board(f, b);
        Move m(square::g4, square::f3, MoveType::En_Passant_Capture, piece::wP);
        auto st = b.make_move(m);
        if (!compare_fen(Utility::board_to_fen_string(b), fen2, 1)) return false;

        b.unmake_move(m, st);
        return compare_fen(Utility::board_to_fen_string(b), fen1, 2);
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
        auto st1 = b.make_move(m1);
        if (!compare_fen(Utility::board_to_fen_string(b), fen2, 1)) return false;

        // black king side castle
        Move m2(square::e8, square::g8, MoveType::King_Side_Castle, def::none);
        auto st2 = b.make_move(m2);
        if (!compare_fen(Utility::board_to_fen_string(b), fen3, 2)) return false;

        b.unmake_move(m2, st2);
        if (!compare_fen(Utility::board_to_fen_string(b), fen2, 3)) return false;

        b.unmake_move(m1, st1);
        return compare_fen(Utility::board_to_fen_string(b), fen1, 4);
    }

    bool BoardTests::test16()
    {
        // test: queen side castle white and black
        Board b;
        std::string fen1{ "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1" };
        std::string fen2{ "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/2KR3R b kq - 1 1" };
        std::string fen3{ "2kr3r/pppppppp/8/8/8/8/PPPPPPPP/2KR3R w - - 2 2" };

        Fen f(fen1);
        Utility::fen_to_board(f, b);

        // white king side castle
        Move m1(square::e1, square::c1, MoveType::Queen_Side_Castle, def::none);
        auto st1 = b.make_move(m1);
        if (!compare_fen(Utility::board_to_fen_string(b), fen2, 1)) return false;

        // black king side castle
        Move m2(square::e8, square::c8, MoveType::Queen_Side_Castle, def::none);
        auto st2 = b.make_move(m2);
        if (!compare_fen(Utility::board_to_fen_string(b), fen3, 2)) return false;

        b.unmake_move(m2, st2);
        if (!compare_fen(Utility::board_to_fen_string(b), fen2, 3)) return false;

        b.unmake_move(m1, st1);
        auto fen7 = Utility::board_to_fen_string(b);
        return compare_fen(Utility::board_to_fen_string(b), fen1, 4);
    }

    bool BoardTests::test17()
    {
        // test promotions
        Board b;
        std::string fen1{ "8/1P4P1/8/4k3/8/2K5/p4p2/8 w - - 0 1" };
        std::string fen2{ "1N6/6P1/8/4k3/8/2K5/p4p2/8 b - - 0 1" };  
        std::string fen3{ "1N6/6P1/8/4k3/8/2K5/5p2/r7 w - - 0 2" };
        std::string fen4{ "1N4Q1/8/8/4k3/8/2K5/5p2/r7 b - - 0 2" };
        std::string fen5{ "1N4Q1/8/8/4k3/8/2K5/8/r4b2 w - - 0 3" };

        Fen f(fen1);
        Utility::fen_to_board(f, b);

        // white promotes to knight
        Move m1(square::b7, square::b8, MoveType::Knight_Promotion, def::none);
        auto st1 = b.make_move(m1);
        auto fen_2 = Utility::board_to_fen_string(b);
        if (!compare_fen(Utility::board_to_fen_string(b), fen2, 1)) return false;

        // black promotes to rook
        Move m2(square::a2, square::a1, MoveType::Rook_Promotion, def::none);
        auto st2 = b.make_move(m2);
        if (!compare_fen(Utility::board_to_fen_string(b), fen3, 2)) return false;

        // white promotes to queen
        Move m3(square::g7, square::g8, MoveType::Queen_Promotion, def::none);
        auto st3 = b.make_move(m3);
        if (!compare_fen(Utility::board_to_fen_string(b), fen4, 3)) return false;

        // black promotes to bishop
        Move m4(square::f2, square::f1, MoveType::Bishop_Promotion, def::none);        
        auto st4 = b.make_move(m4);
        if (!compare_fen(Utility::board_to_fen_string(b), fen5, 4)) return false;

        // unmake move 4
        b.unmake_move(m4, st4);
        if (!compare_fen(Utility::board_to_fen_string(b), fen4, 5)) return false;

        // unnmake move 3
        b.unmake_move(m3, st3);
        if (!compare_fen(Utility::board_to_fen_string(b), fen3, 6)) return false;

        // unmake move 2
        b.unmake_move(m2, st2);
        if (!compare_fen(Utility::board_to_fen_string(b), fen2, 7)) return false;

        // unmake move 1
        b.unmake_move(m1, st1);
        return compare_fen(Utility::board_to_fen_string(b), fen1, 8);
    }

    bool BoardTests::test18()
    {
        // test promotions with capture
        Board b;
        std::string fen1{ "1n1b4/2P1P3/8/8/1K4k1/8/4p1p1/5N1B w - - 0 1" };
        std::string fen2{ "1n1Q4/2P5/8/8/1K4k1/8/4p1p1/5N1B b - - 0 1" };
        std::string fen3{ "1n1Q4/2P5/8/8/1K4k1/8/4p3/5N1r w - - 0 2" };
        std::string fen4{ "1N1Q4/8/8/8/1K4k1/8/4p3/5N1r b - - 0 2" };
        std::string fen5{ "1N1Q4/8/8/8/1K4k1/8/8/5b1r w - - 0 3" };

        Fen f(fen1);
        Utility::fen_to_board(f, b);

        // white captures black bishop and promotes to queen
        Move m1(square::e7, square::d8, MoveType::Queen_Promotion_Capture, piece::bB);
        auto st1 = b.make_move(m1);
        if (!compare_fen(Utility::board_to_fen_string(b), fen2, 1)) return false;

        // black caputes white bishop and promotes to rook
        Move m2(square::g2, square::h1, MoveType::Rook_Promotion_Capture, piece::wB);
        auto st2 = b.make_move(m2);
        if (!compare_fen(Utility::board_to_fen_string(b), fen3, 2)) return false;

        // white captures black knight and promotes to knight
        Move m3(square::c7, square::b8, MoveType::Knight_Promotion_Capture, piece::bN);
        auto st3 = b.make_move(m3);
        if (!compare_fen(Utility::board_to_fen_string(b), fen4, 3)) return false;

        // black captures white knight and promotes to bishop
        Move m4(square::e2, square::f1, MoveType::Bishop_Promotion_Capture, piece::wN);
        auto st4 = b.make_move(m4);
        if (!compare_fen(Utility::board_to_fen_string(b), fen5, 4)) return false;

        // unmake move 4
        b.unmake_move(m4, st4);
        if (!compare_fen(Utility::board_to_fen_string(b), fen4, 5)) return false;

        // unnmake move 3
        b.unmake_move(m3, st3);
        if (!compare_fen(Utility::board_to_fen_string(b), fen3, 6)) return false;

        // unmake move 2
        b.unmake_move(m2, st2);
        if (!compare_fen(Utility::board_to_fen_string(b), fen2, 7)) return false;

        // unmake move 1
        b.unmake_move(m1, st1);
        return compare_fen(Utility::board_to_fen_string(b), fen1, 8);
    }

    bool BoardTests::test19() 
    {
        // test: castling rights when king moves
        Board b;
        std::string fen1{ "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1" };
        std::string fen2{ "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R2K3R b kq - 1 1" };
        std::string fen3{ "r4k1r/pppppppp/8/8/8/8/PPPPPPPP/R2K3R w - - 2 2" };

        Fen f(fen1);
        Utility::fen_to_board(f, b);
        Move m1(square::e1, square::d1, MoveType::Quite, def::none);
        auto st1 = b.make_move(m1);
        if (!compare_fen(Utility::board_to_fen_string(b), fen2, 1)) return false;

        Move m2(square::e8, square::f8, MoveType::Quite, def::none);
        auto st2 = b.make_move(m2);
        if (!compare_fen(Utility::board_to_fen_string(b), fen3, 2)) return false;

        b.unmake_move(m2, st2);
        if (!compare_fen(Utility::board_to_fen_string(b), fen2, 3)) return false;

        b.unmake_move(m1, st1);
        return compare_fen(Utility::board_to_fen_string(b), fen1, 1);
    }

    bool BoardTests::test20()
    {
        // test: castling rights when rook moves
        Board b;
        std::string fen1{ "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1" };
        std::string fen2{ "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/1R2K2R b Kkq - 1 1" };
        std::string fen3{ "2r1k2r/pppppppp/8/8/8/8/PPPPPPPP/1R2K2R w Kk - 2 2" };
        std::string fen4{ "2r1k2r/pppppppp/8/8/8/8/PPPPPPPP/1R2KR2 b k - 3 2" };
        std::string fen5{ "2r1k1r1/pppppppp/8/8/8/8/PPPPPPPP/1R2KR2 w - - 4 3" };

        Fen f(fen1);
        Utility::fen_to_board(f, b);
        Move m1(square::a1, square::b1, MoveType::Quite, def::none);
        auto st1 = b.make_move(m1);
        if (!compare_fen(Utility::board_to_fen_string(b), fen2, 1)) return false;

        Move m2(square::a8, square::c8, MoveType::Quite, def::none);
        auto st2 =  b.make_move(m2);
        if (!compare_fen(Utility::board_to_fen_string(b), fen3, 2)) return false;

        Move m3(square::h1, square::f1, MoveType::Quite, def::none);
        auto st3 = b.make_move(m3);
        if (!compare_fen(Utility::board_to_fen_string(b), fen4, 3)) return false;

        Move m4(square::h8, square::g8, MoveType::Quite, def::none);
        auto st4 = b.make_move(m4);
        if (!compare_fen(Utility::board_to_fen_string(b), fen5, 4)) return false;

        b.unmake_move(m4, st4);
        if (!compare_fen(Utility::board_to_fen_string(b), fen4, 5)) return false;

        b.unmake_move(m3, st3);
        auto fen7 = Utility::board_to_fen_string(b);
        if (!compare_fen(Utility::board_to_fen_string(b), fen3, 6)) return false;

        b.unmake_move(m2, st2);
        auto fen8 = Utility::board_to_fen_string(b);
        if (!compare_fen(Utility::board_to_fen_string(b), fen2, 7)) return false;

        b.unmake_move(m1, st1);
        auto fen9 = Utility::board_to_fen_string(b);
        return compare_fen(Utility::board_to_fen_string(b), fen1, 8);
    }

    bool BoardTests::test21() 
    {
        // test: castling rights when rook captured
        Board b;
        std::string fen1{ "r3k2r/p1pppp1p/8/3BB3/3qb3/8/P1PPPP1P/R3K2R w KQkq - 0 1" };
        std::string fen2{ "B3k2r/p1pppp1p/8/4B3/3qb3/8/P1PPPP1P/R3K2R b KQk - 0 1" };
        std::string fen3{ "B3k2r/p1pppp1p/8/4B3/3q4/8/P1PPPP1P/R3K2b w Qk - 0 2" };
        std::string fen4{ "B3k2B/p1pppp1p/8/8/3q4/8/P1PPPP1P/R3K2b b Q - 0 2" };
        std::string fen5{ "B3k2B/p1pppp1p/8/8/8/8/P1PPPP1P/q3K2b w - - 0 3" };

        Fen f(fen1);
        Utility::fen_to_board(f, b);
        Move m1(square::d5, square::a8, MoveType::Capture, piece::bR);
        auto st1 = b.make_move(m1);
        if (!compare_fen(Utility::board_to_fen_string(b), fen2, 1)) return false;

        Move m2(square::e4, square::h1, MoveType::Capture, piece::wR);
        auto st2 = b.make_move(m2);
        if (!compare_fen(Utility::board_to_fen_string(b), fen3, 2)) return false;

        Move m3(square::e5, square::h8, MoveType::Capture, piece::bR);
        auto st3 = b.make_move(m3);
        if (!compare_fen(Utility::board_to_fen_string(b), fen4, 3)) return false;

        Move m4(square::d4, square::a1, MoveType::Capture, piece::wR);
        auto st4 = b.make_move(m4);
        if (!compare_fen(Utility::board_to_fen_string(b), fen5, 4)) return false;

        b.unmake_move(m4, st4);
        if (!compare_fen(Utility::board_to_fen_string(b), fen4, 5)) return false;

        b.unmake_move(m3, st3);
        if (!compare_fen(Utility::board_to_fen_string(b), fen3, 6)) return false;

        b.unmake_move(m2, st2);
        if (!compare_fen(Utility::board_to_fen_string(b), fen2, 7)) return false;

        b.unmake_move(m1, st1);
        return compare_fen(Utility::board_to_fen_string(b), fen1, 8);
    }

    bool BoardTests::test22() 
    {
        Board b;
        Fen f{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
        Utility::fen_to_board(f, b);
        Move m(square::a2, square::a4, MoveType::Double_Pawn_Push, def::none);

        auto s1 = b.get_piece_locations().size();
        auto st = b.make_move(m);
        auto s2 = b.get_piece_locations().size();
        b.unmake_move(m, st);
        auto s3 = b.get_piece_locations().size();
        return (s1 == 32) && (s2 == 32) && (s3 == 32);
    }

    bool BoardTests::test23()
    {
        Board b;
        std::string fen1{ "r1bqkbnr/pppppppp/2n5/P7/8/8/1PPPPPPP/RNBQKBNR b KQkq - 0 1" };      
        std::string fen2{ "r1bqkbnr/p1pppppp/2n5/Pp6/8/8/1PPPPPPP/RNBQKBNR w KQkq b6 0 2" };    // b7-b5
        std::string fen3{ "r1bqkbnr/p1pppppp/1Pn5/8/8/8/1PPPPPPP/RNBQKBNR b KQkq - 0 2" };      // a5xb6

        Fen f(fen1);
        Utility::fen_to_board(f, b);
        Move m1(square::b7, square::b5, MoveType::Double_Pawn_Push, def::none);
        auto st1 = b.make_move(m1);
        if (!compare_fen(Utility::board_to_fen_string(b), fen2, 1)) return false;

        Move m2(square::a5, square::b6, MoveType::En_Passant_Capture, piece::bP);
        auto st2 = b.make_move(m2);
        if (!compare_fen(Utility::board_to_fen_string(b), fen3, 2)) return false;

        b.unmake_move(m2, st2);
        if (!compare_fen(Utility::board_to_fen_string(b), fen2, 7)) return false;

        b.unmake_move(m1, st1);
        return compare_fen(Utility::board_to_fen_string(b), fen1, 8);

/*
        Board b;
        Fen f{ "rn1qkbnr/pbpppppp/1p6/P7/8/8/1PPPPPPP/RNBQKBNR w KQkq - 0 1" };
        Utility::fen_to_board(f, b);
        Move m(square::a5, square::b6, MoveType::Capture, piece::bP);
        auto s1 = b.get_piece_locations().size();
        Utility::print_board(b, true);
        auto st = b.make_move(m);
        std::cout << "-----------------------------------------\n";
        Utility::print_board(b, true);
        auto s2 = b.get_piece_locations().size();
        b.unmake_move(m, st);
        std::cout << "-----------------------------------------\n";
        Utility::print_board(b, true);
        auto s3 = b.get_piece_locations().size();
        return (s1 == 32) && (s2 == 31) && (s3 == 32);
        */
    }
}