#include "move_generator_tests.hpp"
#include "../board.hpp"
#include "../fen.hpp"
#include "../utility.hpp"
#include "../square.hpp"
#include "../piece.hpp"

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
            test31, test32, test33, test34, test35, test36, test37, test38, test39, test40,
            test41, test42, test43, test44, test45, test46, test47, test48
        };
		TestBase::run_test("Move Generator", tests);
	}

    void MoveGeneratorTests::debug_func()
    {
        Board b;
        Fen f("r1bqkbnr/ppp1pppp/2np4/8/4P3/5Q2/PPPP1PPP/RNB1KBNR w KQkq - 0 1");
        Utility::fen_to_board(f, b);
        Utility::generate_and_print_moves(b);
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
        // test: find king position >> for white and black
        Board b;
        Fen f("r1bq1bnr/p1p1p1pp/1p6/2n1p2K/Pk2Pp2/7N/1PPP1PPP/RNBQ1B1R w - - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        return mg.find_king_pos(color::white) == square::h5 && mg.find_king_pos(color::black) == square::b4;
    }

    bool MoveGeneratorTests::test2()
    {
        // test: compute checks and pins for black, no white attacks, pieces: queen, rook, bishop, knight
        Board b;
        Fen f("3qk3/pp2pppb/b6n/2n2P2/2N5/r1PKB2r/P1PRN1PP/1q1r1b1R w - - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
        
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

        if (mg.attack_info.size() != attacks.size())
        {
            std::cout << "Number of dectected attacks is not correct: " << mg.attack_info.size() << "(expected: " << attacks.size() << ")" << std::endl;
            return false;
        }

        bool double_check_processed{ false };
        for (size_t i = 0; i < attacks.size(); i++)
        {
            if (attacks[i][2] == square::d3) 
            {
                if (double_check_processed)
                    continue;
                double_check_processed = true;

                auto r = mg.attack_info.equal_range(square::d3);
                for (auto it = r.first; it != r.second; it++) 
                {
                    if (it->second.first != square::d8 && it->second.first != square::c5)  return false;
                    if (it->second.second != direction::N && it->second.second != direction::ND) return false;
                }
            }
            else 
            {
                auto it = mg.attack_info.find(attacks[i][2]);
                if (it == mg.attack_info.end())
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

    bool MoveGeneratorTests::test3()
    {
        // test: compute checks and pins for white, no black attacks, pieces: bishop, knight and pawn
        Board b;
        Fen f("Q2R2Q1/1p1N1b2/2Ppp3/RPnk1pqR/2Pn4/1q1r1p2/B2R4/K6B b - - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();

        // expected outcome
        std::vector<std::vector<int8_t>> attacks
        {
            {square::h1, direction::SE, square::f3},
            {square::c4, direction::SW, square::d5}
        };

        if (mg.attack_info.size() != attacks.size())
        {
            std::cout << "Number of dectected attacks is not correct: " << mg.attack_info.size() << "(expected: " << attacks.size() << ")" << std::endl;
            return false;
        }

        for (size_t i = 0; i < attacks.size(); i++)
        {
            auto it = mg.attack_info.find(attacks[i][2]);
            if (it == mg.attack_info.end())
            {
                std::cout << "Target location could not be found" << std::endl;
                return false;
            }
            if (attacks[i][0] != it->second.first)  return false;
            if (attacks[i][1] != it->second.second) return false;
        }

        return true;
    }

    bool MoveGeneratorTests::test4()
    {
        // test: compute checks and pins for white and black pawn attacks
        Board b;
        Fen f("8/8/1PPP2pp/1PkP1pKp/2PP1ppp/8/8/8 w - - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();

        // expected outcome
        std::vector<std::vector<int8_t>> attacks
        {
            {square::d4, direction::SE, square::c5},
            {square::h6, direction::NE, square::g5}
        };

        if (mg.attack_info.size() != attacks.size())
        {
            std::cout << "Number of dectected attacks is not correct: " << mg.attack_info.size() << "(expected: " << attacks.size() << ")" << std::endl;
            return false;
        }

        for (size_t i = 0; i < attacks.size(); i++)
        {
            auto it = mg.attack_info.find(attacks[i][2]);
            if (it == mg.attack_info.end())
            {
                std::cout << "Target location could not be found" << std::endl;
                return false;
            }
            if (attacks[i][0] != it->second.first)  return false;
            if (attacks[i][1] != it->second.second) return false;
        }
        return true;
    }

    bool MoveGeneratorTests::test5()
    {
        // test: no safe place for white king
        Board b;
        Fen f("7q/8/2r5/1p4n1/8/3K1p2/5k2/4b3 w - - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
        std::vector<int8_t> target_squares{square::c4, square::d4, square::e4, square::c3, square::e3, square::c2, square::d2, square::e2};
        return std::all_of(target_squares.begin(), target_squares.end(), [&](int8_t ts) {return mg.is_under_attack(color::black, ts, square::d3); });
    }

    bool MoveGeneratorTests::test6()
    {
        //test: no safe place for black king
        Board b;
        Fen f("8/8/N6R/1P1k4/5P2/3K3B/8/8 w - - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
        std::vector<int8_t> target_squares{ square::c6, square::d6, square::e6, square::c5, square::e5, square::c4, square::d4, square::e4 };
        return std::all_of(target_squares.begin(), target_squares.end(), [&](int8_t ts) {return mg.is_under_attack(color::white, ts, square::d5); });
    }

    bool MoveGeneratorTests::test7() 
    {
        // test: white king can move to limited squares
        Board b;
        Fen f("4q3/1b6/8/8/1k2K2r/8/8/8 w - - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
        std::vector<int8_t> safe_target_squares{ square::d3, square::f5 };
        std::vector<int8_t> unsafe_target_squares{ square::d5, square::e5, square::d4, square::f4, square::e3, square::f3 };
        return 
            std::all_of(unsafe_target_squares.begin(), unsafe_target_squares.end(), [&](int8_t ts) {return mg.is_under_attack(color::black, ts, square::e4); }) &&
            std::all_of(safe_target_squares.begin(), safe_target_squares.end(), [&](int8_t ts) {return !mg.is_under_attack(color::black, ts, square::e4); });
    }

    bool MoveGeneratorTests::test8()
    {
        // test: black king can move to limited squares
        Board b;
        Fen f("2R5/8/2k1P3/5N2/8/8/8/K6Q w - - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
        std::vector<int8_t> safe_target_squares{ square::b5, square::b6 };
        std::vector<int8_t> unsafe_target_squares{ square::b7, square::c7, square::d7, square::d6, square::c5, square::d5 };
        return
            std::all_of(unsafe_target_squares.begin(), unsafe_target_squares.end(), [&](int8_t ts) {return mg.is_under_attack(color::white, ts, square::c6); }) &&
            std::all_of(safe_target_squares.begin(), safe_target_squares.end(), [&](int8_t ts) {return !mg.is_under_attack(color::white, ts, square::c6); });
    }

    bool MoveGeneratorTests::test9()
    {
        // test: white and black attacked squares
        Board b;
        Fen f("8/1k2p1p1/6P1/p7/8/8/2P1K2P/8 w - - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
        std::unordered_set<int8_t> black_attacked_squares 
        {
            square::a8, square::b8, square::c8, square::a7, square::c7, square::b4, square::a6, square::b6, square::c6, square::d6, square::f6, square::h6
        };

        std::unordered_set<int8_t> white_attacked_squares 
        { 
            square::f7, square::h7, square::b3, square::d3, square::e3, square::f3, square::g3, square::d2, square::f2, square::d1,square::e1, square::f1
        };
        

        for (auto i = 0; i < 64; i++)
        {
            auto sq = square::order[i];
            if (black_attacked_squares.find(sq) != black_attacked_squares.end())
            {
                if (!mg.is_under_attack(color::black, sq))
                {
                    std::cout << square::square_to_string(sq) << " is attacked by black but the attack cannot be detected" << std::endl;
                    return false;
                }
            }
            else
            {
                if (mg.is_under_attack(color::black, sq))
                {
                    std::cout << square::square_to_string(sq) << " is not attacked by black but it is detected" << std::endl;
                    return false;
                }
            }

            if (white_attacked_squares.find(sq) != white_attacked_squares.end())
            {
                if (!mg.is_under_attack(color::white, sq))
                {
                    std::cout << square::square_to_string(sq) << " is attacked by white but the attack cannot be detected" << std::endl;
                    return false;
                }
            }
            else
            {
                if (mg.is_under_attack(color::white, sq))
                {
                    std::cout << square::square_to_string(sq) << " is not attacked by white but it is detected" << std::endl;
                    return false;
                }
            }
        }

        return true;
    }

    bool MoveGeneratorTests::test10()
    {
        // test: white and black attacked squares
        Board b;
        Fen f("3k2nq/3b1rp1/3N3n/2B5/4N2b/PPR5/Q2PB3/6K1 w - - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();

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
                if (!mg.is_under_attack(color::black, sq))
                {
                    std::cout << square::square_to_string(sq) << " is attacked by black but the attack cannot be detected" << std::endl;
                    return false;
                }
            }
            else
            {
                if (mg.is_under_attack(color::black, sq))
                {
                    std::cout << square::square_to_string(sq) << " is not attacked by black but it is detected" << std::endl;
                    return false;
                }
            }

            if (white_attacked_squares.find(sq) != white_attacked_squares.end())
            {
                if (!mg.is_under_attack(color::white, sq))
                {
                    std::cout << square::square_to_string(sq) << " is attacked by white but the attack cannot be detected" << std::endl;
                    return false;
                }
            }
            else
            {
                if (mg.is_under_attack(color::white, sq))
                {
                    std::cout << square::square_to_string(sq) << " is not attacked by white but it is detected" << std::endl;
                    return false;
                }
            }
        }

        return true;
    }

    bool MoveGeneratorTests::test11() 
    {
        // white king moves
        Board b;
        Fen f("r3k2r/p3p3/8/8/1q6/8/P2P3P/R3K2R w KQkq - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
        std::vector<Move> moves;

        std::vector<Move> expected_moves
        {
            Move(square::e1, square::e2, MoveType::Quite, def::none),
            Move(square::e1, square::f1, MoveType::Quite, def::none),
            Move(square::e1, square::d1, MoveType::Quite, def::none),
            Move(square::e1, square::f2, MoveType::Quite, def::none),
            Move(square::e1, square::g1, MoveType::King_Side_Castle, def::none),
            Move(square::e1, square::c1, MoveType::Queen_Side_Castle, def::none),
        };

        mg.generate_king_moves(color::white, moves);
        // Utility::print_moves(moves);

        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test12()
    {
        // black king moves
        Board b;
        Fen f("r3k2r/1P6/8/8/8/8/3n4/R3K2R w KQkq - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
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
        mg.generate_king_moves(color::black, moves);
        // Utility::print_moves(moves);

        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test13()
    {
        // white king moves
        Board b;
        Fen f("r3k2r/1P6/8/8/8/8/3n4/R3K2R w KQkq - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
        std::vector<Move> moves;

        std::vector<Move> expected_moves
        {
            Move(square::e1, square::e2, MoveType::Quite, def::none),
            Move(square::e1, square::d1, MoveType::Quite, def::none),
            Move(square::e1, square::f2, MoveType::Quite, def::none),
            Move(square::e1, square::d2, MoveType::Capture, piece::bN),
            Move(square::e1, square::c1, MoveType::Queen_Side_Castle, def::none),
        };

        mg.generate_king_moves(color::white, moves);
        // Utility::print_moves(moves);

        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test14()
    {
        // white king moves
        Board b;
        Fen f("8/b7/4k3/8/3nKn2/8/8/8 w - - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
        std::vector<Move> moves;

        std::vector<Move> expected_moves
        {
            Move(square::e4, square::f4, MoveType::Capture, piece::bN),
            Move(square::e4, square::e3, MoveType::Quite, def::none)
        };

        mg.generate_king_moves(color::white, moves);
        // Utility::print_moves(moves);

        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test15()
    {
        // pin directions for white king
        Board b;
        Fen f("4r3/q1k5/7b/8/3PPP2/r2NKN1r/3QRN2/2b1q1b1 w - - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
        return 
            mg.get_pin_direction(square::d4) == direction::NW &&
            mg.get_pin_direction(square::e4) == direction::N &&
            mg.get_pin_direction(square::f4) == direction::NE &&
            mg.get_pin_direction(square::d3) == direction::W &&
            mg.get_pin_direction(square::f3) == direction::E &&
            mg.get_pin_direction(square::d2) == direction::SW &&
            mg.get_pin_direction(square::e2) == direction::S &&
            mg.get_pin_direction(square::f2) == direction::SE;
    }

    bool MoveGeneratorTests::test16()
    {
        // pin directions for black king
        Board b;
        Fen f("3R4/3n4/2rkn3/2p1p3/3p4/Q7/6K1/3R4 w - - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
        return
            mg.get_pin_direction(square::c5) == direction::SW &&
            mg.get_pin_direction(square::d4) == direction::S &&
            mg.get_pin_direction(square::d7) == direction::N &&
            mg.get_pin_direction(square::c6) == direction::ND &&
            mg.get_pin_direction(square::e6) == direction::ND &&
            mg.get_pin_direction(square::e7) == direction::ND;
    }

    bool MoveGeneratorTests::test17() 
    {
        // test: generate to square moves for white
        Board b;
        Fen f("rnb1kbnr/ppp1qppp/2pP1p2/8/1P1P2P1/2N2P2/PPP4R/R1BQKBN1 w Qkq - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
        std::vector<Move> expected_moves
        {
            Move(square::h2, square::e2, MoveType::Quite, def::none),
            Move(square::f1, square::e2, MoveType::Quite, def::none),
            Move(square::d1, square::e2, MoveType::Quite, def::none),
            Move(square::g1, square::e2, MoveType::Quite, def::none),
            Move(square::c3, square::e2, MoveType::Quite, def::none)
        };

        std::vector<Move> moves;
        mg.generate_to_square_moves(color::white, square::e2, moves);
        // Utility::print_moves(moves);

        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test18()
    {
        // test: generate to square moves for black
        Board b;
        Fen f("3q4/4pk2/2p2n2/7r/8/8/B7/KN1r1R1b w - - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
        std::vector<Move> expected_moves
        {
            Move(square::d8, square::d5, MoveType::Quite, def::none),
            Move(square::h5, square::d5, MoveType::Quite, def::none),
            Move(square::d1, square::d5, MoveType::Quite, def::none),
            Move(square::h1, square::d5, MoveType::Quite, def::none)
        };

        std::vector<Move> moves;
        mg.generate_to_square_moves(color::black, square::d5, moves);
        // Utility::print_moves(moves);

        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test19()
    {
        // test: generate to square moves for white
        Board b;
        Fen f("2q3K1/1P6/1N6/8/2R5/7B/8/k7 w - - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
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
        mg.generate_to_square_moves(color::white, square::c8, moves);
        // Utility::print_moves(moves);

        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test20()
    {
        // test: generate to square moves for black
        Board b;
        Fen f("2R4K/8/8/q7/7b/2bn4/4p3/2k4R w - - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
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
        mg.generate_to_square_moves(color::black, square::e1, moves);
        // Utility::print_moves(moves);

        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test21() 
    {
        // test: generate check eliminating non-king moves, white
        Board b;
        Fen f("k3B3/8/p7/1q3B2/P6R/N7/2PPK3/1R6 w - - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
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
        mg.generate_check_eliminating_moves(color::white, square::e2, moves);
        // Utility::print_moves(moves);

        return compare_moves(moves, expected_moves);

    }

    bool MoveGeneratorTests::test22()
    {
        // test: generate check eliminating non-king moves, black
        Board b;
        Fen f("3K4/8/8/8/8/8/n2bp1p1/1k5R b - - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
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
        mg.generate_check_eliminating_moves(color::black, square::b1, moves);
        // Utility::print_moves(moves);

        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test23()
    {
        // test: knight moves - white
        Board b;
        Fen f("n3k2N/2R2R2/4p3/1n5P/5N2/1pp5/2p1K3/n6N w - - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
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
        mg.generate_knight_moves(color::white, moves);
        // Utility::print_moves(moves);

        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test24()
    {
        // test: knight moves - white
        Board b;
        Fen f("n3k2N/2R2R2/4p3/1n5P/5N2/1pp5/2p1K3/n6N w - - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
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
        mg.generate_knight_moves(color::black, moves);
        // Utility::print_moves(moves);

        return compare_moves(moves, expected_moves);
    }
    
    bool MoveGeneratorTests::test25()
    {
        // test: sliding piece moves - white
        Board b;
        Fen f("6kB/1B3ppp/1n1P4/2qQ4/1Q3p2/PP2Pn2/R1PR1B2/nRK1N3 w - - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
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
        mg.generate_sliding_piece_moves(color::white, piece::Queen, moves);
        mg.generate_sliding_piece_moves(color::white, piece::Rook, moves);
        mg.generate_sliding_piece_moves(color::white, piece::Bishop, moves);
        // Utility::print_moves(moves);
        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test26()
    {
        // test: sliding piece moves - black
        Board b;
        Fen f("brbrbbbb/bbrbpppp/kN2p3/1ppp4/2p5/8/8/4K3 b - - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
        std::vector<Move> expected_moves
        {
            Move(square::c7, square::c6, MoveType::Quite, def::none),
            Move(square::a7, square::b6, MoveType::Capture, piece::wN),
            Move(square::b7, square::c6, MoveType::Quite, def::none),
            Move(square::d7, square::c6, MoveType::Quite, def::none),
        };

        std::vector<Move> moves;
        mg.generate_sliding_piece_moves(color::black, piece::Queen, moves);
        mg.generate_sliding_piece_moves(color::black, piece::Rook, moves);
        mg.generate_sliding_piece_moves(color::black, piece::Bishop, moves);
        // Utility::print_moves(moves);
        return compare_moves(moves, expected_moves);
    }
    
    bool MoveGeneratorTests::test27()
    {
        // test en-passant capture: white
        Board b;
        Fen f("4k3/8/8/1pP5/8/8/8/4K3 w - b6 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
        std::vector<Move> expected_moves
        {
            Move(square::c5, square::c6, MoveType::Quite, def::none),
            Move(square::c5, square::b6, MoveType::En_Passant_Capture, piece::bP)
        };
        
        std::vector<Move> moves;
        mg.generate_pawn_moves(color::white, moves);
        
        // Utility::print_moves(moves);
        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test28()
    {
        // test en-passant capture: white
        Board b;
        Fen f1("4k3/8/8/2Pp4/8/8/8/4K3 w - d6 0 1");
        Utility::fen_to_board(f1, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
        std::vector<Move> expected_moves
        {
            Move(square::c5, square::c6, MoveType::Quite, def::none),
            Move(square::c5, square::d6, MoveType::En_Passant_Capture, piece::bP)
        };

        std::vector<Move> moves;
        mg.generate_pawn_moves(color::white, moves);
        // Utility::print_moves(moves);
        
        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test29()
    {
        // test en-passant capture: black
        Board b;
        Fen f("4k3/8/8/8/4pP2/8/8/4K3 b - f3 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
        std::vector<Move> expected_moves
        {
            Move(square::e4, square::e3, MoveType::Quite, def::none),
            Move(square::e4, square::f3, MoveType::En_Passant_Capture, piece::wP)
        };

        std::vector<Move> moves;
        mg.generate_pawn_moves(color::black, moves);
        // Utility::print_moves(moves);
        
        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test30()
    {
        // test en-passant capture: black
        Board b;
        Fen f1("4k3/8/8/8/3Pp3/8/8/4K3 b - d3 0 1");
        Utility::fen_to_board(f1, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
        std::vector<Move> expected_moves
        {
            Move(square::e4, square::e3, MoveType::Quite, def::none),
            Move(square::e4, square::d3, MoveType::En_Passant_Capture, piece::wP)
        };

        std::vector<Move> moves;
        mg.generate_pawn_moves(color::black, moves);
        // Utility::print_moves(moves);

        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test31()
    {
        // test: en-passant capture in the pinned direction: white
        Board b;
        Fen f("7k/q7/8/1pP5/3K4/8/8/8 w - b6 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
        std::vector<Move> expected_moves{ Move(square::c5, square::b6, MoveType::En_Passant_Capture, piece::bP) };

        std::vector<Move> moves;
        mg.generate_pawn_moves(color::white, moves);
        // Utility::print_moves(moves);

        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test32()
    {
        // test: prevented en-passant capture due to pin: white
        Board b;
        Fen f("7k/q7/8/2Pp4/3K4/8/8/8 w - d6 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg(b);
        mg.compute_attacks();

        std::vector<Move> moves;
        mg.generate_pawn_moves(color::white, moves);
        // Utility::print_moves(moves);
        return moves.empty();
    }

    bool MoveGeneratorTests::test33()
    {
        // test: en-passant capture in the pinned direction: black
        Board b;
        Fen f("8/7k/8/8/3Pp3/8/8/1B2K3 b - d3 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg(b);
        mg.compute_attacks();
        std::vector<Move> expected_moves{ Move(square::e4, square::d3, MoveType::En_Passant_Capture, piece::wP) };

        std::vector<Move> moves;
        mg.generate_pawn_moves(color::black, moves);
        // Utility::print_moves(moves);

        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test34()
    {
        // test: prevented en-passant capture due to pin: black
        Board b;
        Fen f("8/1k6/8/8/3Pp3/8/8/4K2B b - d3 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg(b);
        mg.compute_attacks();
        std::vector<Move> moves;
        mg.generate_pawn_moves(color::black, moves);
        // Utility::print_moves(moves);
        return moves.empty();
    }

    bool MoveGeneratorTests::test35()
    {
        // test: pawn moves, white
        Board b;
        Fen f("1r1n1n2/P1P2P1P/4r3/2k4b/2b5/3PPP2/1P2K3/8 w - - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
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
        mg.generate_pawn_moves(color::white, moves);
        // Utility::print_moves(moves);
        
        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test36()
    {
        // test: pawn moves, black
        Board b;
        Fen f("2k5/2p5/8/4p3/3PPP2/8/pK1p4/NNQ1B1R1 b - - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
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
        mg.generate_pawn_moves(color::black, moves);
        // Utility::print_moves(moves);
        
        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test37()
    {
        // test: sliding piece moves under pins
        Board b;
        Fen f("2r2nk1/5ppb/8/2R5/4Q3/8/2K1Q1r1/8 w - - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
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
        mg.generate_sliding_piece_moves(color::white, piece::Rook, moves);
        mg.generate_sliding_piece_moves(color::white, piece::Queen, moves);
        // Utility::print_moves(moves);

        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test38()
    {
        // moves under check - for white
        Board b;
        Fen f("rnbqkb1r/pppppppp/8/8/4n3/3P4/PPPKPPPP/RNBQ1BNR w kq - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
        std::vector<Move> expected_moves
        {
            Move(square::d2, square::e3, MoveType::Quite, def::none),
            Move(square::d2, square::e1, MoveType::Quite, def::none),
            Move(square::d3, square::e4, MoveType::Capture, piece::bN)
        };

        auto moves = mg.generate_moves();
        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test39()
    {
        // moves under check - for white
        Board b;
        Fen f("rnb1kbnr/pp1ppppp/2p5/q7/8/3P4/PPPKPPPP/RNBQ1BNR w kq - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
        std::vector<Move> expected_moves
        {
            Move(square::d2, square::e3, MoveType::Quite, def::none),
            Move(square::b1, square::c3, MoveType::Quite, def::none),
            Move(square::c2, square::c3, MoveType::Quite, def::none),
            Move(square::b2, square::b4, MoveType::Double_Pawn_Push, def::none)
        };
        auto moves = mg.generate_moves();
        // Utility::print_moves(moves);
        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test40()
    {
        // double check: check mate!
        Board b;
        Fen f("rnbqkb1r/ppp1ppp1/3N1n1p/3p4/Q7/2P5/PP1PPPPP/R1B1KBNR b KQkq - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg { b };
        mg.compute_attacks();
        auto moves = mg.generate_moves();
        return moves.size() == 0;
    }

    bool MoveGeneratorTests::test41()
    {
        // double check!
        Board b;
        Fen f("rnbqk2r/ppp1ppp1/3N1n1p/3p4/Q7/2P5/PP1PPPPP/R1B1KBNR b KQkq - 0 1");
        Utility::fen_to_board(f, b);
        std::vector<Move> expected_moves { Move(square::e8, square::f8, MoveType::Quite, def::none) };
        MoveGenerator mg{ b };
        mg.compute_attacks();
        auto moves = mg.generate_moves();
        return compare_moves(moves, expected_moves);
    }

    bool MoveGeneratorTests::test42()
    {
        // test: no attacks
        Board b;
        Fen f("rnbqkbnr/p1pppppp/1p6/8/8/5P2/PPPPPKPP/RNBQ1BNR b kq - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
        return mg.attack_info.size() == 0;
    }

    bool MoveGeneratorTests::test43()
    {
        // test: no attacks
        Board b;
        Fen f("rnbqkbnr/p1pppppp/8/8/1p6/2NP4/PPP1PPPP/R1BQKBNR w KQkq - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
        return mg.attack_info.size() == 0;
    }

    bool MoveGeneratorTests::test44()
    {
        // test: no attacks
        Board b;
        Fen f("r1bqkbnr/pppppppp/8/8/3n4/8/PPPKPPPP/RNBQ1BNR w kq - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
        return mg.attack_info.size() == 0;
    }

    bool MoveGeneratorTests::test45() 
    {
        // test: en-passant pin
        Board b;
        Fen f("8/2p5/3p4/KP5r/1R3pPk/8/4P3/8 b - g3 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
        return mg.generate_moves().size() == 17;
    }

    bool MoveGeneratorTests::test46()
    {
        // test: en-passant pin
        Board b;
        Fen f("8/8/3p4/1Pp4r/1KR2pk1/8/4P1P1/8 w - c6 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
        return mg.generate_moves().size() == 7;
    }
    
    bool MoveGeneratorTests::test47()
    {
        // test: moving in the direction of pin
        Board b;
        Fen f("8/2p5/3p4/KP3R1r/5p2/8/4PkP1/8 b - - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
        return mg.generate_moves().size() == 20;
    }

    bool MoveGeneratorTests::test48()
    {
        // test: en passant capture under pin
        Board b;
        Fen f("8/2p5/3p4/KP4kr/5pP1/8/4P3/6R1 b - g3 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        mg.compute_attacks();
        return mg.generate_moves().size() == 16;
    }
}