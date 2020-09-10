#include "move_generator_tests.hpp"
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
	void MoveGeneratorTests::run_test()
	{
		std::vector<bool (*)()> tests
        { 
            test1, test2, test3, test4, test5, test6, test7, test8, test9, test10,
            test11, test12, test13, test14, test15, test16, test17, test18, test19, test20,
            test21, test22, test23, test24, test25, test26, test27, test28, test29, test30,
            test31, test32, test33
        };
		TestBase::run_test("Move Generator", tests);
	}

    void MoveGeneratorTests::debug_func()
    {
        // auto b = std::make_shared<Board>();
        // Fen f("r1bqkbnr/ppp1pppp/2np4/8/4P3/5Q2/PPPP1PPP/RNB1KBNR w KQkq - 0 1");
        // Utility::fen_to_board(f, *b);
        // Utility::generate_and_print_moves(b);
        /*
        MoveGenerator mg{ b };
        // std::cout << mg.attack_info.size() << std::endl;
        
        auto moves = mg.generate_moves();
        Utility::print_moves(moves);
        
        for (auto it = moves.begin(); it != moves.end(); it++) 
        {
            Utility::print_move(*it);
            auto st = b.make_move(*it);
            mg.compute_attacks();
            // std::cout << mg.attack_info.size() << std::endl;
            b.unmake_move(*it, st);
            mg.compute_attacks();
            // std::cout << mg.attack_info.size() << std::endl;
        }
        */
    }

    bool MoveGeneratorTests::compare_moves(std::vector<Move> const& moves, std::vector<Move> const& expected_moves)
    {
        if (moves.size() != expected_moves.size()) 
            return false;
        
        for (size_t i{ 0 }; i < moves.size(); i++)
            if (!Utility::is_equal(moves[i], expected_moves[i]))
                return false;
        return true;
    }

    bool MoveGeneratorTests::test1()
    {
        // white king moves
        auto b = Factory::CreateBoard("r3k2r/p3p3/8/8/1q6/8/P2P3P/R3K2R w KQkq - 0 1");
        std::vector<Move> expected_moves
        {
            Move(square::e1, square::e2, MoveType::Quite, def::none),
            Move(square::e1, square::f1, MoveType::Quite, def::none),
            Move(square::e1, square::d1, MoveType::Quite, def::none),
            Move(square::e1, square::f2, MoveType::Quite, def::none),
            Move(square::e1, square::g1, MoveType::King_Side_Castle, def::none),
            Move(square::e1, square::c1, MoveType::Queen_Side_Castle, def::none),
        };
        std::vector<Move> moves;
        MoveGenerator::generate_king_moves(*b, moves);
        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test2()
    {
        // black king moves
        auto b = Factory::CreateBoard("r3k2r/1P6/8/8/8/8/3n4/R3K2R b KQkq - 0 1");
        std::vector<Move> expected_moves
        {
            Move(square::e8, square::f8, MoveType::Quite, def::none),
            Move(square::e8, square::e7, MoveType::Quite, def::none),
            Move(square::e8, square::d8, MoveType::Quite, def::none),
            Move(square::e8, square::f7, MoveType::Quite, def::none),
            Move(square::e8, square::d7, MoveType::Quite, def::none),
            Move(square::e8, square::g8, MoveType::King_Side_Castle, def::none)
        };
        std::vector<Move> moves;
        MoveGenerator::generate_king_moves(*b, moves);
        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test3()
    {
        // white king moves
        auto b = Factory::CreateBoard("r3k2r/1P6/8/8/8/8/3n4/R3K2R w KQkq - 0 1");
        std::vector<Move> expected_moves
        {
            Move(square::e1, square::e2, MoveType::Quite, def::none),
            Move(square::e1, square::d1, MoveType::Quite, def::none),
            Move(square::e1, square::f2, MoveType::Quite, def::none),
            Move(square::e1, square::d2, MoveType::Capture, piece::bN),
            Move(square::e1, square::c1, MoveType::Queen_Side_Castle, def::none),
        };
        std::vector<Move> moves;
        MoveGenerator::generate_king_moves(*b, moves);
        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test4()
    {
        // white king moves
        auto b = Factory::CreateBoard("8/b7/4k3/8/3nKn2/8/8/8 w - - 0 1");
        std::vector<Move> expected_moves
        {
            Move(square::e4, square::f4, MoveType::Capture, piece::bN),
            Move(square::e4, square::e3, MoveType::Quite, def::none)
        };
        std::vector<Move> moves;
        MoveGenerator::generate_king_moves(*b, moves);
        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test5()
    {
        // test: generate to square moves for white
        auto b = Factory::CreateBoard("rnb1kbnr/ppp1qppp/2pP1p2/8/1P1P2P1/2N2P2/PPP4R/R1BQKBN1 w Qkq - 0 1");
        std::vector<Move> expected_moves
        {
            Move(square::h2, square::e2, MoveType::Quite, def::none),
            Move(square::f1, square::e2, MoveType::Quite, def::none),
            Move(square::d1, square::e2, MoveType::Quite, def::none),
            Move(square::g1, square::e2, MoveType::Quite, def::none),
            Move(square::c3, square::e2, MoveType::Quite, def::none)
        };
        std::vector<Move> moves;
        MoveGenerator::generate_to_square_moves(*b, square::e2, moves);
        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test6()
    {
        // test: generate to square moves for black
        auto b = Factory::CreateBoard("3q4/4pk2/2p2n2/7r/8/8/B7/KN1r1R1b b - - 0 1");
        std::vector<Move> expected_moves
        {
            Move(square::d8, square::d5, MoveType::Quite, def::none),
            Move(square::h5, square::d5, MoveType::Quite, def::none),
            Move(square::d1, square::d5, MoveType::Quite, def::none),
            Move(square::h1, square::d5, MoveType::Quite, def::none)
        };
        std::vector<Move> moves;
        MoveGenerator::generate_to_square_moves(*b, square::d5, moves);
        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test7() 
    {
        // test: generate to square moves for white
        auto b = Factory::CreateBoard("2q3K1/1P6/1N6/8/2R5/7B/8/k7 w - - 0 1");
        std::vector<Move> expected_moves
        {
            Move(square::c4, square::c8, MoveType::Capture, piece::bQ),
            Move(square::h3, square::c8, MoveType::Capture, piece::bQ),
            Move(square::b6, square::c8, MoveType::Capture, piece::bQ),
            Move(square::b7, square::c8, MoveType::Queen_Promotion_Capture, piece::bQ),
            Move(square::b7, square::c8, MoveType::Rook_Promotion_Capture, piece::bQ),
            Move(square::b7, square::c8, MoveType::Bishop_Promotion_Capture, piece::bQ),
            Move(square::b7, square::c8, MoveType::Knight_Promotion_Capture, piece::bQ)
        };
        std::vector<Move> moves;
        MoveGenerator::generate_to_square_moves(*b, square::c8, moves);
        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test8()
    {
        // test: generate to square moves for black
        auto b = Factory::CreateBoard("2R4K/8/8/q7/7b/2bn4/4p3/2k4R b - - 0 1");        
        std::vector<Move> expected_moves
        {
            Move(square::h4, square::e1, MoveType::Quite, def::none),
            Move(square::d3, square::e1, MoveType::Quite, def::none),
            Move(square::e2, square::e1, MoveType::Queen_Promotion, def::none),
            Move(square::e2, square::e1, MoveType::Rook_Promotion, def::none),
            Move(square::e2, square::e1, MoveType::Bishop_Promotion, def::none),
            Move(square::e2, square::e1, MoveType::Knight_Promotion, def::none)
        };
        std::vector<Move> moves;
        MoveGenerator::generate_to_square_moves(*b, square::e1, moves);
        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test9()
    {
        // test: generate check eliminating non-king moves, white
        auto b = Factory::CreateBoard("k3B3/8/p7/1q3B2/P6R/N7/2PPK3/1R6 w - - 0 1");
        std::vector<Move> expected_moves
        {
            Move(square::f5, square::d3, MoveType::Quite, def::none),
            Move(square::d2, square::d3, MoveType::Quite, def::none),
            Move(square::h4, square::c4, MoveType::Quite, def::none),
            Move(square::a3, square::c4, MoveType::Quite, def::none),
            Move(square::c2, square::c4, MoveType::Double_Pawn_Push, def::none),
            Move(square::b1, square::b5, MoveType::Capture, piece::bQ),
            Move(square::e8, square::b5, MoveType::Capture, piece::bQ),
            Move(square::a3, square::b5, MoveType::Capture, piece::bQ),
            Move(square::a4, square::b5, MoveType::Capture, piece::bQ)
        };
        std::vector<Move> moves;
        MoveGenerator::generate_check_eliminating_moves(*b, square::e2, moves);
        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test10()
    {
        // test: generate check eliminating non-king moves, black
        auto b = Factory::CreateBoard("3K4/8/8/8/8/8/n2bp1p1/1k5R b - - 0 1");
        std::vector<Move> expected_moves
        {
            Move(square::d2, square::c1, MoveType::Quite, def::none),
            Move(square::a2, square::c1, MoveType::Quite, def::none),
            Move(square::d2, square::e1, MoveType::Quite, def::none),
            Move(square::e2, square::e1, MoveType::Queen_Promotion, def::none),
            Move(square::e2, square::e1, MoveType::Rook_Promotion, def::none),
            Move(square::e2, square::e1, MoveType::Bishop_Promotion, def::none),
            Move(square::e2, square::e1, MoveType::Knight_Promotion, def::none),
            Move(square::g2, square::g1, MoveType::Queen_Promotion, def::none),
            Move(square::g2, square::g1, MoveType::Rook_Promotion, def::none),
            Move(square::g2, square::g1, MoveType::Bishop_Promotion, def::none),
            Move(square::g2, square::g1, MoveType::Knight_Promotion, def::none),
            Move(square::g2, square::h1, MoveType::Queen_Promotion_Capture, piece::wR),
            Move(square::g2, square::h1, MoveType::Rook_Promotion_Capture, piece::wR),
            Move(square::g2, square::h1, MoveType::Bishop_Promotion_Capture, piece::wR),
            Move(square::g2, square::h1, MoveType::Knight_Promotion_Capture, piece::wR)
        };
        std::vector<Move> moves;
        MoveGenerator::generate_check_eliminating_moves(*b, square::b1, moves);
        return compare_moves(moves, expected_moves);
    }
    
    bool MoveGeneratorTests::test11() 
    {
        // test: knight moves - white
        auto b = Factory::CreateBoard("n3k2N/2R2R2/4p3/1n5P/5N2/1pp5/2p1K3/n6N w - - 0 1");
        std::vector<Move> expected_moves
        {
            Move(square::h8, square::g6, MoveType::Quite, def::none),
            Move(square::f4, square::e6, MoveType::Capture, piece::bP),
            Move(square::f4, square::g6, MoveType::Quite, def::none),
            Move(square::f4, square::h3, MoveType::Quite, def::none),
            Move(square::f4, square::g2, MoveType::Quite, def::none),
            Move(square::f4, square::d3, MoveType::Quite, def::none),
            Move(square::f4, square::d5, MoveType::Quite, def::none),
            Move(square::h1, square::g3, MoveType::Quite, def::none),
            Move(square::h1, square::f2, MoveType::Quite, def::none)
        };
        std::vector<Move> moves;
        MoveGenerator::generate_knight_moves(*b, moves);
        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test12()
    {
        // test: knight moves - black
        auto b = Factory::CreateBoard("n3k2N/2R2R2/4p3/1n5P/5N2/1pp5/2p1K3/n6N b - - 0 1");
        std::vector<Move> expected_moves
        {
            Move(square::a8, square::c7, MoveType::Capture, piece::wR),
            Move(square::a8, square::b6, MoveType::Quite, def::none),
            Move(square::b5, square::a7, MoveType::Quite, def::none),
            Move(square::b5, square::c7, MoveType::Capture, piece::wR),
            Move(square::b5, square::d6, MoveType::Quite, def::none),
            Move(square::b5, square::d4, MoveType::Quite, def::none),
            Move(square::b5, square::a3, MoveType::Quite, def::none)
        };
        std::vector<Move> moves;
        MoveGenerator::generate_knight_moves(*b, moves);
        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test13()
    {
        // test: sliding piece moves - white
        auto b = Factory::CreateBoard("6kB/1B3ppp/1n1P4/2qQ4/1Q3p2/PP2Pn2/R1PR1B2/nRK1N3 w - - 0 1");
        std::vector<Move> expected_moves
        {
            Move(square::d5, square::e5, MoveType::Quite, def::none),
            Move(square::d5, square::f5, MoveType::Quite, def::none),
            Move(square::d5, square::g5, MoveType::Quite, def::none),
            Move(square::d5, square::h5, MoveType::Quite, def::none),
            Move(square::d5, square::d4, MoveType::Quite, def::none),
            Move(square::d5, square::d3, MoveType::Quite, def::none),
            Move(square::d5, square::c5, MoveType::Capture, piece::bQ),
            Move(square::d5, square::e6, MoveType::Quite, def::none),
            Move(square::d5, square::f7, MoveType::Capture, piece::bP),
            Move(square::d5, square::e4, MoveType::Quite, def::none),
            Move(square::d5, square::f3, MoveType::Capture, piece::bN),
            Move(square::d5, square::c4, MoveType::Quite, def::none),
            Move(square::d5, square::c6, MoveType::Quite, def::none),
            Move(square::b4, square::b5, MoveType::Quite, def::none),
            Move(square::b4, square::b6, MoveType::Capture, piece::bN),
            Move(square::b4, square::c4, MoveType::Quite, def::none),
            Move(square::b4, square::d4, MoveType::Quite, def::none),
            Move(square::b4, square::e4, MoveType::Quite, def::none),
            Move(square::b4, square::f4, MoveType::Capture, piece::bP),
            Move(square::b4, square::a4, MoveType::Quite, def::none),
            Move(square::b4, square::c5, MoveType::Capture, piece::bQ),
            Move(square::b4, square::c3, MoveType::Quite, def::none),
            Move(square::b4, square::a5, MoveType::Quite, def::none),
            Move(square::a2, square::b2, MoveType::Quite, def::none),
            Move(square::a2, square::a1, MoveType::Capture, piece::bN),
            Move(square::d2, square::d3, MoveType::Quite, def::none),
            Move(square::d2, square::d4, MoveType::Quite, def::none),
            Move(square::d2, square::e2, MoveType::Quite, def::none),
            Move(square::d2, square::d1, MoveType::Quite, def::none),
            Move(square::b1, square::b2, MoveType::Quite, def::none),
            Move(square::b1, square::a1, MoveType::Capture, piece::bN),
            Move(square::h8, square::g7, MoveType::Capture, piece::bP),
            Move(square::b7, square::c8, MoveType::Quite, def::none),
            Move(square::b7, square::c6, MoveType::Quite, def::none),
            Move(square::b7, square::a6, MoveType::Quite, def::none),
            Move(square::b7, square::a8, MoveType::Quite, def::none),
            Move(square::f2, square::g3, MoveType::Quite, def::none),
            Move(square::f2, square::h4, MoveType::Quite, def::none),
            Move(square::f2, square::g1, MoveType::Quite, def::none),
        };
        std::vector<Move> moves;
        MoveGenerator::generate_sliding_piece_moves(*b, piece::Queen, moves);
        MoveGenerator::generate_sliding_piece_moves(*b, piece::Rook, moves);
        MoveGenerator::generate_sliding_piece_moves(*b, piece::Bishop, moves);
        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test14()
    {
        // test: sliding piece moves - black
        auto b = Factory::CreateBoard("brbrbbbb/bbrbpppp/kN2p3/1ppp4/2p5/8/8/4K3 b - - 0 1");
        std::vector<Move> expected_moves
        {
            Move(square::c7, square::c6, MoveType::Quite, def::none),
            Move(square::a7, square::b6, MoveType::Capture, piece::wN),
            Move(square::b7, square::c6, MoveType::Quite, def::none),
            Move(square::d7, square::c6, MoveType::Quite, def::none),
        };
        std::vector<Move> moves;
        MoveGenerator::generate_sliding_piece_moves(*b, piece::Queen, moves);
        MoveGenerator::generate_sliding_piece_moves(*b, piece::Rook, moves);
        MoveGenerator::generate_sliding_piece_moves(*b, piece::Bishop, moves);
        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test15()
    {
        // test en-passant capture: white
        auto b = Factory::CreateBoard("4k3/8/8/1pP5/8/8/8/4K3 w - b6 0 1");
        std::vector<Move> expected_moves
        {
            Move(square::c5, square::c6, MoveType::Quite, def::none),
            Move(square::c5, square::b6, MoveType::En_Passant_Capture, piece::bP)
        };
        std::vector<Move> moves;
        MoveGenerator::generate_pawn_moves(*b, moves);
        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test16()
    {
        // test en-passant capture: white
        auto b = Factory::CreateBoard("4k3/8/8/2Pp4/8/8/8/4K3 w - d6 0 1");
        std::vector<Move> expected_moves
        {
            Move(square::c5, square::c6, MoveType::Quite, def::none),
            Move(square::c5, square::d6, MoveType::En_Passant_Capture, piece::bP)
        };
        std::vector<Move> moves;
        MoveGenerator::generate_pawn_moves(*b, moves);
        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test17() 
    {
        // test en-passant capture: black
        auto b = Factory::CreateBoard("4k3/8/8/8/4pP2/8/8/4K3 b - f3 0 1");
        std::vector<Move> expected_moves
        {
            Move(square::e4, square::e3, MoveType::Quite, def::none),
            Move(square::e4, square::f3, MoveType::En_Passant_Capture, piece::wP)
        };
        std::vector<Move> moves;
        MoveGenerator::generate_pawn_moves(*b, moves);
        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test18()
    {
        // test en-passant capture: black
        auto b = Factory::CreateBoard("4k3/8/8/8/3Pp3/8/8/4K3 b - d3 0 1");
        std::vector<Move> expected_moves
        {
            Move(square::e4, square::e3, MoveType::Quite, def::none),
            Move(square::e4, square::d3, MoveType::En_Passant_Capture, piece::wP)
        };
        std::vector<Move> moves;
        MoveGenerator::generate_pawn_moves(*b, moves);
        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test19()
    {
        // test: en-passant capture in the pinned direction: white
        auto b = Factory::CreateBoard("7k/q7/8/1pP5/3K4/8/8/8 w - b6 0 1");
        std::vector<Move> expected_moves{ Move(square::c5, square::b6, MoveType::En_Passant_Capture, piece::bP) };
        std::vector<Move> moves;
        MoveGenerator::generate_pawn_moves(*b, moves);
        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test20()
    {
        // test: prevented en-passant capture due to pin: white
        auto b = Factory::CreateBoard("7k/q7/8/2Pp4/3K4/8/8/8 w - d6 0 1");
        std::vector<Move> moves;
        MoveGenerator::generate_pawn_moves(*b, moves);
        return moves.empty();
    }

    bool MoveGeneratorTests::test21() 
    {
        // test: en-passant capture in the pinned direction: black
        auto b = Factory::CreateBoard("8/7k/8/8/3Pp3/8/8/1B2K3 b - d3 0 1");
        std::vector<Move> expected_moves{ Move(square::e4, square::d3, MoveType::En_Passant_Capture, piece::wP) };
        std::vector<Move> moves;
        MoveGenerator::generate_pawn_moves(*b, moves);
        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test22()
    {
        // test: prevented en-passant capture due to pin: black
        auto b = Factory::CreateBoard("8/1k6/8/8/3Pp3/8/8/4K2B b - d3 0 1");
        std::vector<Move> moves;
        MoveGenerator::generate_pawn_moves(*b, moves);
        return moves.empty();
    }

    bool MoveGeneratorTests::test23()
    {
        // test: pawn moves, white
        auto b = Factory::CreateBoard("1r1n1n2/P1P2P1P/4r3/2k4b/2b5/3PPP2/1P2K3/8 w - - 0 1");
        std::vector<Move> expected_moves
        {
            Move(square::a7, square::a8, MoveType::Queen_Promotion, def::none),
            Move(square::a7, square::a8, MoveType::Knight_Promotion, def::none),
            Move(square::a7, square::a8, MoveType::Bishop_Promotion, def::none),
            Move(square::a7, square::a8, MoveType::Rook_Promotion, def::none),
            Move(square::a7, square::b8, MoveType::Queen_Promotion_Capture, piece::bR),
            Move(square::a7, square::b8, MoveType::Knight_Promotion_Capture, piece::bR),
            Move(square::a7, square::b8, MoveType::Bishop_Promotion_Capture, piece::bR),
            Move(square::a7, square::b8, MoveType::Rook_Promotion_Capture, piece::bR),
            Move(square::c7, square::c8, MoveType::Queen_Promotion, def::none),
            Move(square::c7, square::c8, MoveType::Knight_Promotion, def::none),
            Move(square::c7, square::c8, MoveType::Bishop_Promotion, def::none),
            Move(square::c7, square::c8, MoveType::Rook_Promotion, def::none),
            Move(square::c7, square::d8, MoveType::Queen_Promotion_Capture, piece::bN),
            Move(square::c7, square::d8, MoveType::Knight_Promotion_Capture, piece::bN),
            Move(square::c7, square::d8, MoveType::Bishop_Promotion_Capture, piece::bN),
            Move(square::c7, square::d8, MoveType::Rook_Promotion_Capture, piece::bN),
            Move(square::c7, square::b8, MoveType::Queen_Promotion_Capture, piece::bR),
            Move(square::c7, square::b8, MoveType::Knight_Promotion_Capture, piece::bR),
            Move(square::c7, square::b8, MoveType::Bishop_Promotion_Capture, piece::bR),
            Move(square::c7, square::b8, MoveType::Rook_Promotion_Capture, piece::bR),
            Move(square::h7, square::h8, MoveType::Queen_Promotion, def::none),
            Move(square::h7, square::h8, MoveType::Knight_Promotion, def::none),
            Move(square::h7, square::h8, MoveType::Bishop_Promotion, def::none),
            Move(square::h7, square::h8, MoveType::Rook_Promotion, def::none),
            Move(square::d3, square::c4, MoveType::Capture, piece::bB),
            Move(square::e3, square::e4, MoveType::Quite, def::none),
            Move(square::b2, square::b3, MoveType::Quite, def::none),
            Move(square::b2, square::b4, MoveType::Double_Pawn_Push, def::none),
        };
        std::vector<Move> moves;
        MoveGenerator::generate_pawn_moves(*b, moves);
        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test24()
    {
        // test: pawn moves, black
        auto b = Factory::CreateBoard("2k5/2p5/8/4p3/3PPP2/8/pK1p4/NNQ1B1R1 b - - 0 1");
        std::vector<Move> expected_moves
        {
            Move(square::c7, square::c6, MoveType::Quite, def::none),
            Move(square::c7, square::c5, MoveType::Double_Pawn_Push, def::none),
            Move(square::e5, square::d4, MoveType::Capture, piece::wP),
            Move(square::e5, square::f4, MoveType::Capture, piece::wP),
            Move(square::a2, square::b1, MoveType::Queen_Promotion_Capture, piece::wN),
            Move(square::a2, square::b1, MoveType::Knight_Promotion_Capture, piece::wN),
            Move(square::a2, square::b1, MoveType::Bishop_Promotion_Capture, piece::wN),
            Move(square::a2, square::b1, MoveType::Rook_Promotion_Capture, piece::wN),
            Move(square::d2, square::d1, MoveType::Queen_Promotion, def::none),
            Move(square::d2, square::d1, MoveType::Knight_Promotion, def::none),
            Move(square::d2, square::d1, MoveType::Bishop_Promotion, def::none),
            Move(square::d2, square::d1, MoveType::Rook_Promotion, def::none),
            Move(square::d2, square::c1, MoveType::Queen_Promotion_Capture, piece::wQ),
            Move(square::d2, square::c1, MoveType::Knight_Promotion_Capture, piece::wQ),
            Move(square::d2, square::c1, MoveType::Bishop_Promotion_Capture, piece::wQ),
            Move(square::d2, square::c1, MoveType::Rook_Promotion_Capture, piece::wQ),
            Move(square::d2, square::e1, MoveType::Queen_Promotion_Capture, piece::wB),
            Move(square::d2, square::e1, MoveType::Knight_Promotion_Capture, piece::wB),
            Move(square::d2, square::e1, MoveType::Bishop_Promotion_Capture, piece::wB),
            Move(square::d2, square::e1, MoveType::Rook_Promotion_Capture, piece::wB)
        };
        std::vector<Move> moves;
        MoveGenerator::generate_pawn_moves(*b, moves);
        return compare_moves(moves, expected_moves);
    }
    
    bool MoveGeneratorTests::test25()
    {
        // test: sliding piece moves under pins
        auto b = Factory::CreateBoard("2r2nk1/5ppb/8/2R5/4Q3/8/2K1Q1r1/8 w - - 0 1");
        std::vector<Move> expected_moves
        {
            Move(square::c5, square::c6, MoveType::Quite, def::none),
            Move(square::c5, square::c7, MoveType::Quite, def::none),
            Move(square::c5, square::c8, MoveType::Capture, piece::bR),
            Move(square::c5, square::c4, MoveType::Quite, def::none),
            Move(square::c5, square::c3, MoveType::Quite, def::none),
            Move(square::e4, square::f5, MoveType::Quite, def::none),
            Move(square::e4, square::g6, MoveType::Quite, def::none),
            Move(square::e4, square::h7, MoveType::Capture, piece::bB),
            Move(square::e4, square::d3, MoveType::Quite, def::none),
            Move(square::e2, square::f2, MoveType::Quite, def::none),
            Move(square::e2, square::g2, MoveType::Capture, piece::bR),
            Move(square::e2, square::d2, MoveType::Quite, def::none),
        };

        std::vector<Move> moves;
        MoveGenerator::generate_sliding_piece_moves(*b, piece::Rook, moves);
        MoveGenerator::generate_sliding_piece_moves(*b, piece::Queen, moves);
        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test26()
    {
        // moves under check - for white
        auto b = Factory::CreateBoard("rnbqkb1r/pppppppp/8/8/4n3/3P4/PPPKPPPP/RNBQ1BNR w kq - 0 1");
        std::vector<Move> expected_moves
        {
            Move(square::d2, square::e3, MoveType::Quite, def::none),
            Move(square::d2, square::e1, MoveType::Quite, def::none),
            Move(square::d3, square::e4, MoveType::Capture, piece::bN)
        };
        auto moves = MoveGenerator::generate_moves(*b);
        return compare_moves(moves, expected_moves);
    }
    
    bool MoveGeneratorTests::test27()
    {
        // moves under check - for white
        auto b = Factory::CreateBoard("rnb1kbnr/pp1ppppp/2p5/q7/8/3P4/PPPKPPPP/RNBQ1BNR w kq - 0 1");
        std::vector<Move> expected_moves
        {
            Move(square::d2, square::e3, MoveType::Quite, def::none),
            Move(square::b1, square::c3, MoveType::Quite, def::none),
            Move(square::c2, square::c3, MoveType::Quite, def::none),
            Move(square::b2, square::b4, MoveType::Double_Pawn_Push, def::none)
        };
        auto moves = MoveGenerator::generate_moves(*b);
        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test28()
    {
        // double check: check mate!
        auto b = Factory::CreateBoard("rnbqkb1r/ppp1ppp1/3N1n1p/3p4/Q7/2P5/PP1PPPPP/R1B1KBNR b KQkq - 0 1");
        return MoveGenerator::generate_moves(*b).size() == 0;
    }

    bool MoveGeneratorTests::test29()
    {
        // double check!
        auto b = Factory::CreateBoard("rnbqk2r/ppp1ppp1/3N1n1p/3p4/Q7/2P5/PP1PPPPP/R1B1KBNR b KQkq - 0 1");
        std::vector<Move> expected_moves{ Move(square::e8, square::f8, MoveType::Quite, def::none) };
        auto moves = MoveGenerator::generate_moves(*b);
        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test30()
    {
        // test: en-passant pin
        auto b = Factory::CreateBoard("8/2p5/3p4/KP5r/1R3pPk/8/4P3/8 b - g3 0 1");
        return MoveGenerator::generate_moves(*b).size() == 17;
    }

    bool MoveGeneratorTests::test31()
    {
        // test: en-passant pin
        auto b = Factory::CreateBoard("8/8/3p4/1Pp4r/1KR2pk1/8/4P1P1/8 w - c6 0 1");
        return MoveGenerator::generate_moves(*b).size() == 7;
    }

    bool MoveGeneratorTests::test32()
    {
        // test: moving in the direction of pin
        auto b = Factory::CreateBoard("8/2p5/3p4/KP3R1r/5p2/8/4PkP1/8 b - - 0 1");
        return MoveGenerator::generate_moves(*b).size() == 20;
    }

    bool MoveGeneratorTests::test33()
    {
        // test: en passant capture under pin
        auto b = Factory::CreateBoard("8/2p5/3p4/KP4kr/5pP1/8/4P3/6R1 b - g3 0 1");
        return MoveGenerator::generate_moves(*b).size() == 16;
    }
}