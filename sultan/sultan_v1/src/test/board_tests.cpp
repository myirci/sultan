#include "board_tests.hpp"
#include "../board.hpp"
#include "../utility.hpp"
#include "../square.hpp"
#include "../piece.hpp"
#include "../factory.hpp"

#include <iostream>
#include <algorithm>
#include <unordered_set>

namespace test 
{
	void BoardTests::run_test() 
	{
		std::vector<bool (*)()> tests
        { 
            test1, test2, test3, test4, test5, test6, test7, test8, test9, test10,
            test11, test12, test13, test14, test15, test16, test17, test18, test19, test20, 
            test21, test22, test23, test24, test25, test26, test27, test28, test29, test30,
            test31, test32, test33, test34, test35, test36, test37, test38, test39, test40
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
        // test: default constructor
        Board b;
        auto func = [](int8_t v) { return v == def::none; };
        return
            std::all_of(b.get_board(), b.get_board() + Board::BOARDSIZE, func) &&
            b.get_en_passant_loc() == def::none &&
            b.get_castling_rights() == 0 &&
            b.get_side_to_move() == def::none &&
            b.get_half_move_counter() == -1 &&
            b.get_full_move_counter() == -1 &&
            b.ploc == nullptr &&
            b.attacks == nullptr;
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
        std::string fen1{ "8/3k4/2r5/8/7B/8/1K4P1/8 w - - 0 1" };
        std::string fen2{ "8/3k4/2r5/8/8/8/1K3BP1/8 b - - 1 1" };
        auto b = Factory::CreateBoard(fen1);
        
        auto m = Move(square::h4, square::f2, MoveType::Quite, def::none);
        auto st = b->make_move(m);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen2, 1)) 
            return false;
        
        b->unmake_move(m, st);
        return compare_fen(Utility::board_to_fen_string(*b), fen1, 2);
    }

    bool BoardTests::test9()
    {
        // test: double pawn push move black and white
        std::string fen1{ "8/3k1p2/2r5/8/7B/8/1K4P1/8 w - - 0 1" };
        std::string fen2{ "8/3k1p2/2r5/8/6PB/8/1K6/8 b - g3 0 1" };
        std::string fen3{ "8/3k4/2r5/5p2/6PB/8/1K6/8 w - f6 0 2" };
        auto b = Factory::CreateBoard(fen1);
        Move m1(square::g2, square::g4, MoveType::Double_Pawn_Push, def::none);
        auto st1 = b->make_move(m1);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen2, 1)) 
            return false;
        
        Move m2(square::f7, square::f5, MoveType::Double_Pawn_Push, def::none);
        auto st2 = b->make_move(m2);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen3, 2)) 
            return false;

        b->unmake_move(m2, st2);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen2, 3)) 
            return false;

        b->unmake_move(m1, st1);
        return compare_fen(Utility::board_to_fen_string(*b), fen1, 4);
    }

    bool BoardTests::test10()
    {
        // test: capture move 
        std::string fen1{ "8/3k3q/2r5/8/6PB/8/1K2Q3/8 b - - 0 1" };
        std::string fen2{ "8/3k4/2r5/8/6Pq/8/1K2Q3/8 w - - 0 2" };
        auto b = Factory::CreateBoard(fen1);
        
        Move m(square::h7, square::h4, MoveType::Capture, piece::wB);
        auto st = b->make_move(m);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen2, 1)) 
            return false;
        
        b->unmake_move(m, st);
        return compare_fen(Utility::board_to_fen_string(*b), fen1, 2);
    }

    bool BoardTests::test11()
    {
        // test: en passant capture move - white 
        std::string fen1{ "4k3/8/8/2PpP3/6p1/8/5P1P/4K3 w - d6 0 1" };
        std::string fen2{ "4k3/8/3P4/4P3/6p1/8/5P1P/4K3 b - - 0 1" };
        auto b = Factory::CreateBoard(fen1);
        
        Move m(square::c5, square::d6, MoveType::En_Passant_Capture, piece::bP);
        auto st = b->make_move(m);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen2, 1)) 
            return false;
        
        b->unmake_move(m, st);
        return compare_fen(Utility::board_to_fen_string(*b), fen1, 2);
    }

    bool BoardTests::test12()
    {
        // test: en passant capture move - white 
        std::string fen1{ "4k3/8/8/2PpP3/6p1/8/5P1P/4K3 w - d6 0 1" };
        std::string fen2{ "4k3/8/3P4/2P5/6p1/8/5P1P/4K3 b - - 0 1" };
        auto b = Factory::CreateBoard(fen1);
        
        Move m(square::e5, square::d6, MoveType::En_Passant_Capture, piece::bP);
        auto st = b->make_move(m);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen2, 1)) 
            return false;
        
        b->unmake_move(m, st);
        return compare_fen(Utility::board_to_fen_string(*b), fen1, 2);
    }

    bool BoardTests::test13()
    {
        // test: en passant capture move - black 
        std::string fen1{ "4k3/8/8/2PpP3/6pP/8/5P2/4K3 b - h3 0 1" };
        std::string fen2{ "4k3/8/8/2PpP3/8/7p/5P2/4K3 w - - 0 2" };
        auto b = Factory::CreateBoard(fen1);
        
        Move m(square::g4, square::h3, MoveType::En_Passant_Capture, piece::wP);
        auto st = b->make_move(m);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen2, 1)) 
            return false;
        
        b->unmake_move(m, st);
        auto fen4 = Utility::board_to_fen_string(*b);
        return compare_fen(Utility::board_to_fen_string(*b), fen1, 2);
    }

    bool BoardTests::test14()
    {
        // test: en passant capture move - black 
        std::string fen1{ "4k3/8/8/2PpP3/5PpP/8/8/4K3 b - f3 0 1" };
        std::string fen2{ "4k3/8/8/2PpP3/7P/5p2/8/4K3 w - - 0 2" };
        auto b = Factory::CreateBoard(fen1);
        
        Move m(square::g4, square::f3, MoveType::En_Passant_Capture, piece::wP);
        auto st = b->make_move(m);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen2, 1)) 
            return false;
        
        b->unmake_move(m, st);
        return compare_fen(Utility::board_to_fen_string(*b), fen1, 2);
    }

    bool BoardTests::test15()
    {
        // test: king side castle white and black
        std::string fen1{ "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1" };
        std::string fen2{ "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R4RK1 b kq - 1 1" };
        std::string fen3{ "r4rk1/pppppppp/8/8/8/8/PPPPPPPP/R4RK1 w - - 2 2" };
        auto b = Factory::CreateBoard(fen1);

        // white king side castle
        Move m1(square::e1, square::g1, MoveType::King_Side_Castle, def::none);
        auto st1 = b->make_move(m1);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen2, 1)) 
            return false;

        // black king side castle
        Move m2(square::e8, square::g8, MoveType::King_Side_Castle, def::none);
        auto st2 = b->make_move(m2);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen3, 2)) 
            return false;

        b->unmake_move(m2, st2);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen2, 3)) 
            return false;

        b->unmake_move(m1, st1);
        return compare_fen(Utility::board_to_fen_string(*b), fen1, 4);
    }

    bool BoardTests::test16()
    {
        // test: queen side castle white and black
        std::string fen1{ "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1" };
        std::string fen2{ "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/2KR3R b kq - 1 1" };
        std::string fen3{ "2kr3r/pppppppp/8/8/8/8/PPPPPPPP/2KR3R w - - 2 2" };
        auto b = Factory::CreateBoard(fen1);

        // white king side castle
        Move m1(square::e1, square::c1, MoveType::Queen_Side_Castle, def::none);
        auto st1 = b->make_move(m1);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen2, 1)) 
            return false;

        // black king side castle
        Move m2(square::e8, square::c8, MoveType::Queen_Side_Castle, def::none);
        auto st2 = b->make_move(m2);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen3, 2)) 
            return false;

        b->unmake_move(m2, st2);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen2, 3)) 
            return false;

        b->unmake_move(m1, st1);
        auto fen7 = Utility::board_to_fen_string(*b);
        return compare_fen(Utility::board_to_fen_string(*b), fen1, 4);
    }

    bool BoardTests::test17()
    {
        // test promotions
        std::string fen1{ "8/1P4P1/8/4k3/8/2K5/p4p2/8 w - - 0 1" };
        std::string fen2{ "1N6/6P1/8/4k3/8/2K5/p4p2/8 b - - 0 1" };  
        std::string fen3{ "1N6/6P1/8/4k3/8/2K5/5p2/r7 w - - 0 2" };
        std::string fen4{ "1N4Q1/8/8/4k3/8/2K5/5p2/r7 b - - 0 2" };
        std::string fen5{ "1N4Q1/8/8/4k3/8/2K5/8/r4b2 w - - 0 3" };
        auto b = Factory::CreateBoard(fen1);

        // white promotes to knight
        Move m1(square::b7, square::b8, MoveType::Knight_Promotion, def::none);
        auto st1 = b->make_move(m1);
        auto fen_2 = Utility::board_to_fen_string(*b);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen2, 1)) 
            return false;

        // black promotes to rook
        Move m2(square::a2, square::a1, MoveType::Rook_Promotion, def::none);
        auto st2 = b->make_move(m2);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen3, 2)) 
            return false;

        // white promotes to queen
        Move m3(square::g7, square::g8, MoveType::Queen_Promotion, def::none);
        auto st3 = b->make_move(m3);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen4, 3)) 
            return false;

        // black promotes to bishop
        Move m4(square::f2, square::f1, MoveType::Bishop_Promotion, def::none);        
        auto st4 = b->make_move(m4);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen5, 4)) 
            return false;

        // unmake move 4
        b->unmake_move(m4, st4);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen4, 5)) 
            return false;

        // unnmake move 3
        b->unmake_move(m3, st3);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen3, 6)) 
            return false;

        // unmake move 2
        b->unmake_move(m2, st2);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen2, 7)) 
            return false;

        // unmake move 1
        b->unmake_move(m1, st1);
        return compare_fen(Utility::board_to_fen_string(*b), fen1, 8);
    }

    bool BoardTests::test18()
    {
        // test promotions with capture
        std::string fen1{ "1n1b4/2P1P3/8/8/1K4k1/8/4p1p1/5N1B w - - 0 1" };
        std::string fen2{ "1n1Q4/2P5/8/8/1K4k1/8/4p1p1/5N1B b - - 0 1" };
        std::string fen3{ "1n1Q4/2P5/8/8/1K4k1/8/4p3/5N1r w - - 0 2" };
        std::string fen4{ "1N1Q4/8/8/8/1K4k1/8/4p3/5N1r b - - 0 2" };
        std::string fen5{ "1N1Q4/8/8/8/1K4k1/8/8/5b1r w - - 0 3" };
        auto b = Factory::CreateBoard(fen1);

        // white captures black bishop and promotes to queen
        Move m1(square::e7, square::d8, MoveType::Queen_Promotion_Capture, piece::bB);
        auto st1 = b->make_move(m1);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen2, 1))
            return false;

        // black caputes white bishop and promotes to rook
        Move m2(square::g2, square::h1, MoveType::Rook_Promotion_Capture, piece::wB);
        auto st2 = b->make_move(m2);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen3, 2)) 
            return false;

        // white captures black knight and promotes to knight
        Move m3(square::c7, square::b8, MoveType::Knight_Promotion_Capture, piece::bN);
        auto st3 = b->make_move(m3);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen4, 3)) 
            return false;

        // black captures white knight and promotes to bishop
        Move m4(square::e2, square::f1, MoveType::Bishop_Promotion_Capture, piece::wN);
        auto st4 = b->make_move(m4);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen5, 4)) 
            return false;

        // unmake move 4
        b->unmake_move(m4, st4);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen4, 5)) 
            return false;

        // unnmake move 3
        b->unmake_move(m3, st3);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen3, 6)) 
            return false;

        // unmake move 2
        b->unmake_move(m2, st2);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen2, 7)) 
            return false;

        // unmake move 1
        b->unmake_move(m1, st1);
        return compare_fen(Utility::board_to_fen_string(*b), fen1, 8);
    }

    bool BoardTests::test19() 
    {
        // test: castling rights when king moves
        std::string fen1{ "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1" };
        std::string fen2{ "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R2K3R b kq - 1 1" };
        std::string fen3{ "r4k1r/pppppppp/8/8/8/8/PPPPPPPP/R2K3R w - - 2 2" };
        auto b = Factory::CreateBoard(fen1);
        
        Move m1(square::e1, square::d1, MoveType::Quite, def::none);
        auto st1 = b->make_move(m1);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen2, 1)) 
            return false;

        Move m2(square::e8, square::f8, MoveType::Quite, def::none);
        auto st2 = b->make_move(m2);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen3, 2)) 
            return false;
        
        b->unmake_move(m2, st2);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen2, 3)) 
            return false;
        
        b->unmake_move(m1, st1);
        return compare_fen(Utility::board_to_fen_string(*b), fen1, 1);
    }

    bool BoardTests::test20()
    {
        // test: castling rights when rook moves
        std::string fen1{ "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1" };
        std::string fen2{ "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/1R2K2R b Kkq - 1 1" };
        std::string fen3{ "2r1k2r/pppppppp/8/8/8/8/PPPPPPPP/1R2K2R w Kk - 2 2" };
        std::string fen4{ "2r1k2r/pppppppp/8/8/8/8/PPPPPPPP/1R2KR2 b k - 3 2" };
        std::string fen5{ "2r1k1r1/pppppppp/8/8/8/8/PPPPPPPP/1R2KR2 w - - 4 3" };
        auto b = Factory::CreateBoard(fen1);
        
        Move m1(square::a1, square::b1, MoveType::Quite, def::none);
        auto st1 = b->make_move(m1);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen2, 1)) 
            return false;

        Move m2(square::a8, square::c8, MoveType::Quite, def::none);
        auto st2 =  b->make_move(m2);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen3, 2)) 
            return false;

        Move m3(square::h1, square::f1, MoveType::Quite, def::none);
        auto st3 = b->make_move(m3);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen4, 3)) 
            return false;

        Move m4(square::h8, square::g8, MoveType::Quite, def::none);
        auto st4 = b->make_move(m4);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen5, 4)) 
            return false;

        b->unmake_move(m4, st4);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen4, 5)) 
            return false;

        b->unmake_move(m3, st3);
        auto fen7 = Utility::board_to_fen_string(*b);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen3, 6)) 
            false;

        b->unmake_move(m2, st2);
        auto fen8 = Utility::board_to_fen_string(*b);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen2, 7)) 
            return false;

        b->unmake_move(m1, st1);
        auto fen9 = Utility::board_to_fen_string(*b);
        return compare_fen(Utility::board_to_fen_string(*b), fen1, 8);
    }

    bool BoardTests::test21() 
    {
        // test: castling rights when rook captured
        std::string fen1{ "r3k2r/p1pppp1p/8/3BB3/3qb3/8/P1PPPP1P/R3K2R w KQkq - 0 1" };
        std::string fen2{ "B3k2r/p1pppp1p/8/4B3/3qb3/8/P1PPPP1P/R3K2R b KQk - 0 1" };
        std::string fen3{ "B3k2r/p1pppp1p/8/4B3/3q4/8/P1PPPP1P/R3K2b w Qk - 0 2" };
        std::string fen4{ "B3k2B/p1pppp1p/8/8/3q4/8/P1PPPP1P/R3K2b b Q - 0 2" };
        std::string fen5{ "B3k2B/p1pppp1p/8/8/8/8/P1PPPP1P/q3K2b w - - 0 3" };
        auto b = Factory::CreateBoard(fen1);

        Move m1(square::d5, square::a8, MoveType::Capture, piece::bR);
        auto st1 = b->make_move(m1);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen2, 1)) 
            return false;

        Move m2(square::e4, square::h1, MoveType::Capture, piece::wR);
        auto st2 = b->make_move(m2);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen3, 2)) 
            return false;

        Move m3(square::e5, square::h8, MoveType::Capture, piece::bR);
        auto st3 = b->make_move(m3);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen4, 3)) 
            return false;

        Move m4(square::d4, square::a1, MoveType::Capture, piece::wR);
        auto st4 = b->make_move(m4);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen5, 4)) 
            return false;

        b->unmake_move(m4, st4);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen4, 5)) 
            return false;

        b->unmake_move(m3, st3);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen3, 6)) 
            return false;

        b->unmake_move(m2, st2);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen2, 7)) 
            return false;

        b->unmake_move(m1, st1);
        return compare_fen(Utility::board_to_fen_string(*b), fen1, 8);
    }

    bool BoardTests::test22() 
    {
        std::string fen1{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
        auto b = Factory::CreateBoard(fen1);

        Move m(square::a2, square::a4, MoveType::Double_Pawn_Push, def::none);
        auto s1 = b->ploc->get_number_of_pieces();
        auto st = b->make_move(m);
        auto s2 = b->ploc->get_number_of_pieces();
        b->unmake_move(m, st);
        auto s3 = b->ploc->get_number_of_pieces();
        return (s1 == 32) && (s2 == 32) && (s3 == 32);
    }

    bool BoardTests::test23()
    {
        std::string fen1{ "r1bqkbnr/pppppppp/2n5/P7/8/8/1PPPPPPP/RNBQKBNR b KQkq - 0 1" };      
        std::string fen2{ "r1bqkbnr/p1pppppp/2n5/Pp6/8/8/1PPPPPPP/RNBQKBNR w KQkq b6 0 2" };    // b7-b5
        std::string fen3{ "r1bqkbnr/p1pppppp/1Pn5/8/8/8/1PPPPPPP/RNBQKBNR b KQkq - 0 2" };      // a5xb6
        auto b = Factory::CreateBoard(fen1);
        
        Move m1(square::b7, square::b5, MoveType::Double_Pawn_Push, def::none);
        auto st1 = b->make_move(m1);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen2, 1)) 
            return false;

        Move m2(square::a5, square::b6, MoveType::En_Passant_Capture, piece::bP);
        auto st2 = b->make_move(m2);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen3, 2)) 
            return false;

        b->unmake_move(m2, st2);
        if (!compare_fen(Utility::board_to_fen_string(*b), fen2, 7)) 
            return false;

        b->unmake_move(m1, st1);
        return compare_fen(Utility::board_to_fen_string(*b), fen1, 8);
    }

    bool BoardTests::test24() 
    {
        // test: find king position >> for white and black
        auto b = Factory::CreateBoard("r1bq1bnr/p1p1p1pp/1p6/2n1p2K/Pk2Pp2/7N/1PPP1PPP/RNBQ1B1R w - - 0 1");
        return b->find_piece_location(piece::wK) == square::h5 && b->find_piece_location(piece::bK) == square::b4;
    }

    bool BoardTests::test25() 
    {
        // test: compute checks and pins for black, no white attacks, pieces: queen, rook, bishop, knight
        auto b = Factory::CreateBoard("3qk3/pp2pppb/b6n/2n2P2/2N5/r1PKB2r/P1PRN1PP/1q1r1b1R w - - 0 1");

        // expected outcome
        std::vector<std::vector<int8_t>> attacks
        {
            {square::d8, direction::N, square::d3},     // check
            {square::h3, direction::E, square::e3},     // pin
            {square::d1, direction::S, square::d2},     // pin
            {square::a3, direction::W, square::c3},     // pin
            {square::h7, direction::NE, square::f5},    // pin
            {square::f1, direction::SE, square::e2},    // pin
            {square::b1, direction::SW, square::c2},    // pin
            {square::a6, direction::NW, square::c4},    // pin
            {square::c5, direction::ND, square::d3}     // check   
        };

        bool double_check_processed{ false };
        for (size_t i = 0; i < attacks.size(); i++)
        {
            if (attacks[i][2] == square::d3)
            {
                if (double_check_processed)
                    continue;
                double_check_processed = true;

                auto r = b->attacks->checks_and_pins.equal_range(square::d3);
                for (auto it = r.first; it != r.second; it++)
                {
                    if (it->second.first != square::d8 && it->second.first != square::c5)  return false;
                    if (it->second.second != direction::N && it->second.second != direction::ND) return false;
                }
            }
            else
            {
                auto it = b->attacks->checks_and_pins.find(attacks[i][2]);
                if (it == b->attacks->checks_and_pins.end())
                {
                    std::cout << "Target location could not be found" << std::endl;
                    return false;
                }
                if (attacks[i][0] != it->second.first)  return false;
                if (attacks[i][1] != it->second.second) return false;
            }
        }

        return true;
    }

    bool BoardTests::test26() 
    {
        // test: compute checks and pins for white, no black attacks, pieces: bishop, knight and pawn
        auto b = Factory::CreateBoard("Q2R2Q1/1p1N1b2/2Ppp3/RPnk1pqR/2Pn4/1q1r1p2/B2R4/K6B b - - 0 1");
        
        // expected outcome
        std::vector<std::vector<int8_t>> attacks
        {
            {square::h1, direction::SE, square::f3},
            {square::c4, direction::SW, square::d5}
        };

        for (size_t i = 0; i < attacks.size(); i++)
        {
            auto it = b->attacks->checks_and_pins.find(attacks[i][2]);
            if (it == b->attacks->checks_and_pins.end())
            {
                std::cout << "Target location could not be found" << std::endl;
                return false;
            }
            if (attacks[i][0] != it->second.first)  return false;
            if (attacks[i][1] != it->second.second) return false;
        }

        return true;
    }

    bool BoardTests::test27() 
    {
        // test: compute checks and pins for white and black pawn attacks
        auto b = Factory::CreateBoard("8/8/1PPP2pp/1PkP1pKp/2PP1ppp/8/8/8 w - - 0 1");

        // expected outcome
        std::vector<std::vector<int8_t>> attacks
        {
            {square::d4, direction::SE, square::c5},
            {square::h6, direction::NE, square::g5}
        };

        for (size_t i = 0; i < attacks.size(); i++)
        {
            auto it = b->attacks->checks_and_pins.find(attacks[i][2]);
            if (it == b->attacks->checks_and_pins.end())
            {
                std::cout << "Target location could not be found" << std::endl;
                return false;
            }
            if (attacks[i][0] != it->second.first)  return false;
            if (attacks[i][1] != it->second.second) return false;
        }
        return true;
    }

    bool BoardTests::test28() 
    {
        // test: no safe place for white king
        auto b = Factory::CreateBoard("7q/8/2r5/1p4n1/8/3K1p2/5k2/4b3 w - - 0 1");
        std::vector<int8_t> target_squares{ square::c4, square::d4, square::e4, square::c3, square::e3, square::c2, square::d2, square::e2 };
        return std::all_of(target_squares.begin(), target_squares.end(), [&](int8_t ts) { return b->is_under_attack(color::black, ts, square::d3); });
    }

    bool BoardTests::test29()
    {
        //test: no safe place for black king
        auto b = Factory::CreateBoard("8/8/N6R/1P1k4/5P2/3K3B/8/8 w - - 0 1");
        std::vector<int8_t> target_squares{ square::c6, square::d6, square::e6, square::c5, square::e5, square::c4, square::d4, square::e4 };
        return std::all_of(target_squares.begin(), target_squares.end(), [&](int8_t ts) {return b->is_under_attack(color::white, ts, square::d5); });
    }

    bool BoardTests::test30()
    {
        // test: white king can move to limited squares
        auto b = Factory::CreateBoard("4q3/1b6/8/8/1k2K2r/8/8/8 w - - 0 1");
        std::vector<int8_t> safe_target_squares{ square::d3, square::f5 };
        std::vector<int8_t> unsafe_target_squares{ square::d5, square::e5, square::d4, square::f4, square::e3, square::f3 };
        return
            std::all_of(unsafe_target_squares.begin(), unsafe_target_squares.end(), [&](int8_t ts) {return b->is_under_attack(color::black, ts, square::e4); }) &&
            std::all_of(safe_target_squares.begin(), safe_target_squares.end(), [&](int8_t ts) {return !b->is_under_attack(color::black, ts, square::e4); });
    }

    bool BoardTests::test31()
    {
        // test: black king can move to limited squares
        auto b = Factory::CreateBoard("2R5/8/2k1P3/5N2/8/8/8/K6Q w - - 0 1");
        std::vector<int8_t> safe_target_squares{ square::b5, square::b6 };
        std::vector<int8_t> unsafe_target_squares{ square::b7, square::c7, square::d7, square::d6, square::c5, square::d5 };
        return
            std::all_of(unsafe_target_squares.begin(), unsafe_target_squares.end(), [&](int8_t ts) {return b->is_under_attack(color::white, ts, square::c6); }) &&
            std::all_of(safe_target_squares.begin(), safe_target_squares.end(), [&](int8_t ts) {return !b->is_under_attack(color::white, ts, square::c6); });
    }

    bool BoardTests::test32()
    {
        // test: white and black attacked squares
        auto b = Factory::CreateBoard("8/1k2p1p1/6P1/p7/8/8/2P1K2P/8 w - - 0 1");

        std::unordered_set<int8_t> black_attacked_squares {
            square::a8, square::b8, square::c8, square::a7, square::c7, square::b4, 
            square::a6, square::b6, square::c6, square::d6, square::f6, square::h6
        };

        std::unordered_set<int8_t> white_attacked_squares {
            square::f7, square::h7, square::b3, square::d3, square::e3, square::f3, 
            square::g3, square::d2, square::f2, square::d1, square::e1, square::f1
        };

        for (auto i = 0; i < 64; i++)
        {
            auto sq = square::order[i];
            if (black_attacked_squares.find(sq) != black_attacked_squares.end())
            {
                if (!b->is_under_attack(color::black, sq))
                {
                    std::cout << square::square_to_string(sq) << " is attacked by black but the attack cannot be detected" << std::endl;
                    return false;
                }
            }
            else
            {
                if (b->is_under_attack(color::black, sq))
                {
                    std::cout << square::square_to_string(sq) << " is not attacked by black but it is detected" << std::endl;
                    return false;
                }
            }

            if (white_attacked_squares.find(sq) != white_attacked_squares.end())
            {
                if (!b->is_under_attack(color::white, sq))
                {
                    std::cout << square::square_to_string(sq) << " is attacked by white but the attack cannot be detected" << std::endl;
                    return false;
                }
            }
            else
            {
                if (b->is_under_attack(color::white, sq))
                {
                    std::cout << square::square_to_string(sq) << " is not attacked by white but it is detected" << std::endl;
                    return false;
                }
            }
        }

        return true;
    }

    bool BoardTests::test33()
    {
        // test: white and black attacked squares
        auto b = Factory::CreateBoard("3k2nq/3b1rp1/3N3n/2B5/4N2b/PPR5/Q2PB3/6K1 w - - 0 1");
        
        std::unordered_set<int8_t> black_attacked_squares
        {
            square::c8, square::d8, square::e8, square::f8, square::g8, square::c7, square::d7, square::e7, square::f7, square::g7, square::h7,
            square::c6, square::e6, square::f6, square::h6, square::b5, square::f5, square::g5, square::a4, square::f4, square::g4, square::f3,
            square::g3, square::h3, square::f2, square::e1, square::f1
        };

        std::unordered_set<int8_t> white_attacked_squares
        {
            square::c8, square::e8, square::a7, square::b7, square::f7, square::a6, square::b6, square::d6, square::f6, square::b5, square::c5,
            square::f5, square::g5, square::h5, square::a4, square::b4, square::c4, square::d4, square::e4, square::g4, square::a3, square::b3,
            square::c3, square::d3, square::e3, square::f3, square::g3, square::h3, square::b2, square::c2, square::d2, square::f2, square::g2,
            square::h2, square::a1, square::b1, square::c1, square::d1, square::f1, square::g1, square::h1
        };

        for (auto i = 0; i < 64; i++)
        {
            auto sq = square::order[i];
            if (black_attacked_squares.find(sq) != black_attacked_squares.end())
            {
                if (!b->is_under_attack(color::black, sq))
                {
                    std::cout << square::square_to_string(sq) << " is attacked by black but the attack cannot be detected" << std::endl;
                    return false;
                }
            }
            else
            {
                if (b->is_under_attack(color::black, sq))
                {
                    std::cout << square::square_to_string(sq) << " is not attacked by black but it is detected" << std::endl;
                    return false;
                }
            }

            if (white_attacked_squares.find(sq) != white_attacked_squares.end())
            {
                if (!b->is_under_attack(color::white, sq))
                {
                    std::cout << square::square_to_string(sq) << " is attacked by white but the attack cannot be detected" << std::endl;
                    return false;
                }
            }
            else
            {
                if (b->is_under_attack(color::white, sq))
                {
                    std::cout << square::square_to_string(sq) << " is not attacked by white but it is detected" << std::endl;
                    return false;
                }
            }
        }

        return true;
    }

    bool BoardTests::test34()
    {
        // pin directions for white king
        auto b = Factory::CreateBoard("4r3/q1k5/7b/8/3PPP2/r2NKN1r/3QRN2/2b1q1b1 w - - 0 1");
        
        return
            b->get_pin_direction(square::d4) == direction::NW &&
            b->get_pin_direction(square::e4) == direction::N &&
            b->get_pin_direction(square::f4) == direction::NE &&
            b->get_pin_direction(square::d3) == direction::W &&
            b->get_pin_direction(square::f3) == direction::E &&
            b->get_pin_direction(square::d2) == direction::SW &&
            b->get_pin_direction(square::e2) == direction::S &&
            b->get_pin_direction(square::f2) == direction::SE;
    }

    bool BoardTests::test35()
    {
        // pin directions for black king
        auto b = Factory::CreateBoard("3R4/3n4/2rkn3/2p1p3/3p4/Q7/6K1/3R4 w - - 0 1");
        return
            b->get_pin_direction(square::c5) == direction::SW &&
            b->get_pin_direction(square::d4) == direction::S &&
            b->get_pin_direction(square::d7) == direction::N &&
            b->get_pin_direction(square::c6) == direction::ND &&
            b->get_pin_direction(square::e6) == direction::ND &&
            b->get_pin_direction(square::e7) == direction::ND;
    }

    bool BoardTests::test36() 
    {
        // test: no attacks
        auto b = Factory::CreateBoard("rnbqkbnr/p1pppppp/1p6/8/8/5P2/PPPPPKPP/RNBQ1BNR b kq - 0 1");
        return b->attacks->checks_and_pins.size() == 0;
    }

    bool BoardTests::test37()
    {
        // test: no attacks
        auto b = Factory::CreateBoard("rnbqkbnr/p1pppppp/8/8/1p6/2NP4/PPP1PPPP/R1BQKBNR w KQkq - 0 1");
        return b->attacks->checks_and_pins.size() == 0;
    }

    bool BoardTests::test38()
    {
        // test: no attacks
        auto b = Factory::CreateBoard("r1bqkbnr/pppppppp/8/8/3n4/8/PPPKPPPP/RNBQ1BNR w kq - 0 1");
        return b->attacks->checks_and_pins.size() == 0;
    }

    bool BoardTests::test39()
    {
        // test: clone
        auto b1 = Factory::CreateBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        auto b2 = Factory::CloneBoard(*b1);
        return Utility::is_equal(*b1, *b2);
    }

    bool BoardTests::test40()
    {
        // test: clone
        auto b1 = Factory::CreateBoard("4k2r/pb1p1p1p/r1p1p2n/Qpb1n1p1/PPNPP1P1/R3RN1q/KBP2P1P/8 b k d3 0 1");
        auto b2 = Factory::CloneBoard(*b1);
        return Utility::is_equal(*b1, *b2);
    }
}