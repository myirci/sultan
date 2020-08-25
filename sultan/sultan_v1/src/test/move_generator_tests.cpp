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
		std::vector<bool (*)()> tests{ 
            test1, test2, test3, test4, test5, test6, test7, test8, test9, test10,
            test11, test12, test13, test14, test15, test16, test17, test18, test19, test20,
		    test21, test22, // , test13  test16,  test19, test20
        };
		TestBase::run_test("Move Generator", tests);
	}

    void MoveGeneratorTests::debug_func()
    {
        Board b;
        Fen f("7q/8/2r5/1p4n1/8/3K1p2/5k2/4b3 w - - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        std::vector<int8_t> target_squares{ square::c4, square::d4, square::e4, square::c3, square::e3, square::c2, square::d2, square::e2 };
        for (size_t i{ 0 }; i < target_squares.size(); i++) 
        {
            std::cout << mg.is_under_attack(color::black, target_squares[i], square::d3) << std::endl;
        }
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
        // Utility::print_attacks(mg.battacks);
        
        // expected outcome
        std::vector<attack::AttackInfo> battacks
        {
            {square::d8, direction::N, square::d3},     // check
            {square::h3, direction::E, square::e3},     // pin
            {square::d1, direction::S, square::d2},     // pin
            {square::a3, direction::W, square::c3},     // pin
            {square::h7, direction::NE, square::f5},    // pin
            {square::f1, direction::SE, square::e2},    // pin
            {square::b1, direction::SW, square::c2},    // pin
            {square::a6, direction::NW, square::c4},    // pin
            {square::c5, direction::ND, square::d3}    // check   
        };
        
        if (!mg.wattacks.empty())
        {
            std::cout << "Number of dectected attacks for white is not correct: " << mg.wattacks.size() << "(expected:0)" << std::endl;
            return false;
        }

        if (mg.battacks.size() != battacks.size())
        {
            std::cout << "Number of dectected attacks for black is not correct: " << mg.battacks.size() << "(expected: " << battacks.size() << ")" << std::endl;
            return false;
        }

        for (size_t i = 0; i < battacks.size(); i++)
        {
            if (battacks[i].attacker_loc != mg.battacks[i].attacker_loc)    return false;
            if (battacks[i].attack_dir != mg.battacks[i].attack_dir)        return false;
            if (battacks[i].target_loc != mg.battacks[i].target_loc)        return false;
        }
        
        return true;
    }

    bool MoveGeneratorTests::test3()
    {
        // test: compute checks and pins for white, no black attacks, pieces: bishop, knight and pawn
        Board b;
        Fen f("Q2R2Q1/1p1NNb2/2Ppp3/RPnk1pqR/2Pn4/1q1r1p2/B2R4/K6B b - - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        // Utility::print_attacks(mg.wattacks);

        // expected outcome
        std::vector<attack::AttackInfo> wattacks
        {
            {square::h1, direction::SE, square::f3},
            {square::c4, direction::SW, square::d5},
            {square::e7, direction::ND, square::d5}
        };

        if (!mg.battacks.empty())
        {
            std::cout << "Number of dectected attacks for black is not correct: " << mg.battacks.size() << "(expected: 0)" << std::endl;
            return false;
        }

        if (mg.wattacks.size() != wattacks.size())
        {
            std::cout << "Number of dectected attacks for white is not correct: " << mg.wattacks.size() << "(expected: " << wattacks.size() << ")" << std::endl;
            return false;
        }

        for (size_t i = 0; i < wattacks.size(); i++)
        {
            if (wattacks[i].attacker_loc != mg.wattacks[i].attacker_loc)    return false;
            if (wattacks[i].attack_dir != mg.wattacks[i].attack_dir)        return false;
            if (wattacks[i].target_loc != mg.wattacks[i].target_loc)        return false;
        }

        return true;
    }

    bool MoveGeneratorTests::test4()
    {
        // test: compute checks and pins for white and black pawn attacks
        Board b;
        Fen f("8/8/1PPP1ppp/1PkP1pKp/1PPP1ppp/8/8/8 w - - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };
        // Utility::print_attacks(mg.wattacks);
        // Utility::print_attacks(mg.battacks);

        // expected outcome
        std::vector<attack::AttackInfo> wattacks
        {
            {square::d4, direction::SE, square::c5},
            {square::b4, direction::SW, square::c5}
        };

        std::vector<attack::AttackInfo> battacks
        {
            {square::h6, direction::NE, square::g5},
            {square::f6, direction::NW, square::g5}
        };

        if (mg.wattacks.size() != wattacks.size())
        {
            std::cout << "Number of dectected attacks for white is not correct: " << mg.wattacks.size() << "(expected: " << wattacks.size() << ")" << std::endl;
            return false;
        }

        for (size_t i = 0; i < wattacks.size(); i++)
        {
            if (wattacks[i].attacker_loc != mg.wattacks[i].attacker_loc)    return false;
            if (wattacks[i].attack_dir != mg.wattacks[i].attack_dir)        return false;
            if (wattacks[i].target_loc != mg.wattacks[i].target_loc)        return false;
        }

        if (mg.battacks.size() != battacks.size())
        {
            std::cout << "Number of dectected attacks for black is not correct: " << mg.battacks.size() << "(expected: 0)" << std::endl;
            return false;
        }

        for (size_t i = 0; i < battacks.size(); i++)
        {
            if (battacks[i].attacker_loc != mg.battacks[i].attacker_loc)    return false;
            if (battacks[i].attack_dir != mg.battacks[i].attack_dir)        return false;
            if (battacks[i].target_loc != mg.battacks[i].target_loc)        return false;
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
        MoveGenerator mg(b);

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

        mg.generate_king_moves(color::white, false, moves);
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
        mg.generate_king_moves(color::black, false, moves);
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
        std::vector<Move> moves;

        std::vector<Move> expected_moves
        {
            Move(square::e1, square::e2, MoveType::Quite, def::none),
            Move(square::e1, square::d1, MoveType::Quite, def::none),
            Move(square::e1, square::f2, MoveType::Quite, def::none),
            Move(square::e1, square::d2, MoveType::Capture, piece::bN),
            Move(square::e1, square::c1, MoveType::Queen_Side_Castle, def::none),
        };

        mg.generate_king_moves(color::white, false, moves);
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
        std::vector<Move> moves;

        std::vector<Move> expected_moves
        {
            Move(square::e4, square::f4, MoveType::Capture, piece::bN),
            Move(square::e4, square::e3, MoveType::Quite, def::none)
        };

        mg.generate_king_moves(color::white, false, moves);
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
        
        return 
            mg.get_pin_direction(color::white, square::d4) == direction::NW &&
            mg.get_pin_direction(color::white, square::e4) == direction::N &&
            mg.get_pin_direction(color::white, square::f4) == direction::NE &&
            mg.get_pin_direction(color::white, square::d3) == direction::W &&
            mg.get_pin_direction(color::white, square::f3) == direction::E &&
            mg.get_pin_direction(color::white, square::d2) == direction::SW &&
            mg.get_pin_direction(color::white, square::e2) == direction::S &&
            mg.get_pin_direction(color::white, square::f2) == direction::SE;
    }

    bool MoveGeneratorTests::test16()
    {
        // pin directions for black king
        Board b;
        Fen f("3R4/3n4/2rkn3/2p1p3/3p4/Q7/6K1/3R4 w - - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };

        return
            mg.get_pin_direction(color::black, square::c5) == direction::SW &&
            mg.get_pin_direction(color::black, square::d4) == direction::S &&
            mg.get_pin_direction(color::black, square::d7) == direction::N &&
            mg.get_pin_direction(color::black, square::c6) == direction::ND &&
            mg.get_pin_direction(color::black, square::e6) == direction::ND &&
            mg.get_pin_direction(color::white, square::e7) == direction::ND;
    }

    bool MoveGeneratorTests::test17() 
    {
        // test: generate to square moves for white
        Board b;
        Fen f("rnb1kbnr/ppp1qppp/2pP1p2/8/1P1P2P1/2N2P2/PPP4R/R1BQKBN1 w Qkq - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg{ b };

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
        
    }

    /*
    bool MoveGeneratorTests::test6()
    {
        // test: moves for black and white at the initial position
        Board b;
        Fen f;
        Utility::fen_to_board(f, b);
        MoveGenerator mg(b);

        std::vector<Move> moves;

        auto wattpin = mg.compute_attacks_and_pins(square::e1, 1);
        auto battpin = mg.compute_attacks_and_pins(square::e8, -1);

        // white pawn moves
        std::vector<int8_t> wsq
        {
            square::a2, square::a3, square::a4,
            square::b2, square::b3, square::b4,
            square::c2, square::c3, square::c4,
            square::d2, square::d3, square::d4,
            square::e2, square::e3, square::e4,
            square::f2, square::f3, square::f4,
            square::g2, square::g3, square::g4,
            square::h2, square::h3, square::h4
        };

        for (size_t i = 0; i < 24; i += 3)
        {
            mg.generate_pawn_moves(wsq[i], 1, wattpin.second, moves);
            // Utility::print_moves(moves);
            if (moves.size() != 2 ||
                !Utility::is_equal(moves[0], Move(wsq[i], wsq[i + 1], MoveType::Quite, def::none)) ||
                !Utility::is_equal(moves[1], Move(wsq[i], wsq[i + 2], MoveType::Double_Pawn_Push, def::none)))
                return false;
            moves.clear();
        }

        // black pawn moves
        std::vector<int8_t> bsq
        {
            square::a7, square::a6, square::a5,
            square::b7, square::b6, square::b5,
            square::c7, square::c6, square::c5,
            square::d7, square::d6, square::d5,
            square::e7, square::e6, square::e5,
            square::f7, square::f6, square::f5,
            square::g7, square::g6, square::g5,
            square::h7, square::h6, square::h5
        };

        for (size_t i = 0; i < 24; i += 3)
        {
            mg.generate_pawn_moves(bsq[i], -1, battpin.second, moves);
            // Utility::print_moves(moves);
            if (moves.size() != 2 ||
                !Utility::is_equal(moves[0], Move(bsq[i], bsq[i + 1], MoveType::Quite, def::none)) ||
                !Utility::is_equal(moves[1], Move(bsq[i], bsq[i + 2], MoveType::Double_Pawn_Push, def::none)))
                return false;
            moves.clear();
        }

        // Rook moves:
        mg.generate_sliding_piece_moves(square::a1, piece::Rook, 1, wattpin.second, moves);
        if (!moves.empty()) return false;
        mg.generate_sliding_piece_moves(square::h1, piece::Rook, 1, wattpin.second, moves);
        if (!moves.empty()) return false;
        mg.generate_sliding_piece_moves(square::a8, piece::Rook, -1, battpin.second, moves);
        if (!moves.empty()) return false;
        mg.generate_sliding_piece_moves(square::h8, piece::Rook, -1, battpin.second, moves);
        if (!moves.empty()) return false;

        // Bishop moves
        mg.generate_sliding_piece_moves(square::c1, piece::Bishop, 1, wattpin.second, moves);
        if (!moves.empty()) return false;
        mg.generate_sliding_piece_moves(square::f1, piece::Bishop, 1, wattpin.second, moves);
        if (!moves.empty()) return false;
        mg.generate_sliding_piece_moves(square::c8, piece::Bishop, -1, battpin.second, moves);
        if (!moves.empty()) return false;
        mg.generate_sliding_piece_moves(square::f8, piece::Bishop, -1, battpin.second, moves);
        if (!moves.empty()) return false;

        // Queen moves
        mg.generate_sliding_piece_moves(square::d1, piece::Queen, 1, wattpin.second, moves);
        if (!moves.empty()) return false;
        mg.generate_sliding_piece_moves(square::d8, piece::Queen, -1, battpin.second, moves);
        if (!moves.empty()) return false;

        // King moves
        mg.generate_king_moves(square::e1, 1, wattpin.first, moves);
        if (!moves.empty()) return false;
        mg.generate_king_moves(square::e8, -1, wattpin.first, moves);
        if (!moves.empty()) return false;

        // Knight moves
        mg.generate_knight_moves(square::b1, 1, wattpin.second, moves);
        // Utility::print_moves(moves);
        if (moves.size() != 2 ||
            !Utility::is_equal(moves[0], Move(square::b1, square::c3, MoveType::Quite, def::none)) ||
            !Utility::is_equal(moves[1], Move(square::b1, square::a3, MoveType::Quite, def::none)))
            return false;

        moves.clear();
        mg.generate_knight_moves(square::g1, 1, wattpin.second, moves);
        // Utility::print_moves(moves);
        if (moves.size() != 2 ||
            !Utility::is_equal(moves[0], Move(square::g1, square::h3, MoveType::Quite, def::none)) ||
            !Utility::is_equal(moves[1], Move(square::g1, square::f3, MoveType::Quite, def::none)))
            return false;

        moves.clear();
        mg.generate_knight_moves(square::b8, -1, battpin.second, moves);
        // Utility::print_moves(moves);
        if (moves.size() != 2 ||
            !Utility::is_equal(moves[0], Move(square::b8, square::c6, MoveType::Quite, def::none)) ||
            !Utility::is_equal(moves[1], Move(square::b8, square::a6, MoveType::Quite, def::none)))
            return false;

        moves.clear();
        mg.generate_knight_moves(square::g8, -1, battpin.second, moves);
        // Utility::print_moves(moves);
        if (moves.size() != 2 ||
            !Utility::is_equal(moves[0], Move(square::g8, square::h6, MoveType::Quite, def::none)) ||
            !Utility::is_equal(moves[1], Move(square::g8, square::f6, MoveType::Quite, def::none)))
            return false;

        return true;
    }

    bool MoveGeneratorTests::test7()
    {
        // test en-passant capture: white
        Board b;
        Fen f("4k3/8/8/1pP5/8/8/8/4K3 w - b6 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg(b);
        std::vector<Move> moves;
        auto wattpin = mg.compute_attacks_and_pins(square::e1, 1);
        mg.generate_pawn_moves(square::c5, 1, wattpin.second, moves);
        // Utility::print_moves(moves);
        if (moves.size() != 2 ||
            !Utility::is_equal(moves[0], Move(square::c5, square::c6, MoveType::Quite, def::none)) ||
            !Utility::is_equal(moves[1], Move(square::c5, square::b6, MoveType::En_Passant_Capture, piece::bP)))
            return false;
        return true;
    }

    bool MoveGeneratorTests::test8()
    {
        // test en-passant capture: white
        Board b;
        Fen f1("4k3/8/8/2Pp4/8/8/8/4K3 w - d6 0 1");
        Utility::fen_to_board(f1, b);
        MoveGenerator mg(b);
        std::vector<Move> moves;
        auto wattpin = mg.compute_attacks_and_pins(square::e1, 1);
        mg.generate_pawn_moves(square::c5, 1, wattpin.second, moves);
        // Utility::print_moves(moves);
        if (moves.size() != 2 ||
            !Utility::is_equal(moves[0], Move(square::c5, square::c6, MoveType::Quite, def::none)) ||
            !Utility::is_equal(moves[1], Move(square::c5, square::d6, MoveType::En_Passant_Capture, piece::bP)))
            return false;
        return true;
    }

    bool MoveGeneratorTests::test9()
    {
        // test en-passant capture: black
        Board b;
        Fen f("4k3/8/8/8/4pP2/8/8/4K3 b - f3 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg(b);
        std::vector<Move> moves;
        auto wattpin = mg.compute_attacks_and_pins(square::e8, -1);
        mg.generate_pawn_moves(square::e4, -1, wattpin.second, moves);
        // Utility::print_moves(moves);
        if (moves.size() != 2 ||
            !Utility::is_equal(moves[0], Move(square::e4, square::e3, MoveType::Quite, def::none)) ||
            !Utility::is_equal(moves[1], Move(square::e4, square::f3, MoveType::En_Passant_Capture, piece::wP)))
            return false;
        return true;
    }

    bool MoveGeneratorTests::test10()
    {
        // test en-passant capture: black
        Board b;
        Fen f1("4k3/8/8/8/3Pp3/8/8/4K3 b - d3 0 1");
        Utility::fen_to_board(f1, b);
        MoveGenerator mg(b);
        std::vector<Move> moves;
        auto wattpin = mg.compute_attacks_and_pins(square::e8, -1);
        mg.generate_pawn_moves(square::e4, -1, wattpin.second, moves);
        // Utility::print_moves(moves);
        if (moves.size() != 2 ||
            !Utility::is_equal(moves[0], Move(square::e4, square::e3, MoveType::Quite, def::none)) ||
            !Utility::is_equal(moves[1], Move(square::e4, square::d3, MoveType::En_Passant_Capture, piece::wP)))
            return false;
        return true;
    }

    bool MoveGeneratorTests::test11()
    {
        // test: en-passant capture in the pinned direction: white
        Board b;
        Fen f("8/q7/8/1pP5/3K4/8/8/8 w - b6 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg(b);

        std::vector<Move> moves;
        auto wattpin = mg.compute_attacks_and_pins(square::d4, 1);
        mg.generate_pawn_moves(square::c5, 1, wattpin.second, moves);
        // Utility::print_moves(moves);
        if (moves.size() != 1 || !Utility::is_equal(moves[0], Move(square::c5, square::b6, MoveType::En_Passant_Capture, piece::bP)))
            return false;
        return true;
    }

    bool MoveGeneratorTests::test12()
    {
        // test: prevented en-passant capture due to pin: white
        Board b;
        Fen f("8/q7/8/2Pp4/3K4/8/8/8 w - d6 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg(b);

        std::vector<Move> moves;
        auto wattpin = mg.compute_attacks_and_pins(square::d4, 1);
        mg.generate_pawn_moves(square::c5, 1, wattpin.second, moves);
        Utility::print_moves(moves);
        if (!moves.empty())
            return false;
        return true;
    }

    bool MoveGeneratorTests::test13()
    {
        // test: en-passant capture in the pinned direction: black
        Board b;
        Fen f("8/7k/8/8/3Pp3/8/8/1B2K3 b - d3 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg(b);

        std::vector<Move> moves;
        auto battpin = mg.compute_attacks_and_pins(square::h7, -1);
        mg.generate_pawn_moves(square::e4, -1, battpin.second, moves);
        // Utility::print_moves(moves);
        if (moves.size() != 1 || !Utility::is_equal(moves[0], Move(square::e4, square::d3, MoveType::En_Passant_Capture, piece::wP)))
            return false;
        return true;
    }

    bool MoveGeneratorTests::test14()
    {
        // test: prevented en-passant capture due to pin: black
        Board b;
        Fen f("8/1k6/8/8/3Pp3/8/8/4K2B b - d3 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg(b);

        std::vector<Move> moves;
        auto battpin = mg.compute_attacks_and_pins(square::b7, -1);
        mg.generate_pawn_moves(square::e4, -1, battpin.second, moves);
        Utility::print_moves(moves);
        if (!moves.empty())
            return false;
        return true;
    }

    bool MoveGeneratorTests::test15()
    {
        // test: pawn promotions with and without capture: white
        Board b;
        Fen f("1r1n1n2/P1P1PP1P/4r3/2k4b/2b5/3PPP2/4K3/8 w - - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg(b);
        auto wattpin = mg.compute_attacks_and_pins(square::e2, 1);
        std::vector<Move> moves;

        mg.generate_pawn_moves(square::a7, 1, wattpin.second, moves);
        // Utility::print_moves(moves);
        if (moves.size() != 8 ||
            !Utility::is_equal(moves[0], Move(square::a7, square::a8, MoveType::Queen_Promotion, def::none)) ||
            !Utility::is_equal(moves[1], Move(square::a7, square::a8, MoveType::Knight_Promotion, def::none)) ||
            !Utility::is_equal(moves[2], Move(square::a7, square::a8, MoveType::Bishop_Promotion, def::none)) ||
            !Utility::is_equal(moves[3], Move(square::a7, square::a8, MoveType::Rook_Promotion, def::none)) ||
            !Utility::is_equal(moves[4], Move(square::a7, square::b8, MoveType::Queen_Promotion_Capture, piece::bR)) ||
            !Utility::is_equal(moves[5], Move(square::a7, square::b8, MoveType::Knight_Promotion_Capture, piece::bR)) ||
            !Utility::is_equal(moves[6], Move(square::a7, square::b8, MoveType::Bishop_Promotion_Capture, piece::bR)) ||
            !Utility::is_equal(moves[7], Move(square::a7, square::b8, MoveType::Rook_Promotion_Capture, piece::bR)))
            return false;
        moves.clear();

        mg.generate_pawn_moves(square::c7, 1, wattpin.second, moves);
        // Utility::print_moves(moves);
        if (moves.size() != 12 ||
            !Utility::is_equal(moves[0], Move(square::c7, square::c8, MoveType::Queen_Promotion, def::none)) ||
            !Utility::is_equal(moves[1], Move(square::c7, square::c8, MoveType::Knight_Promotion, def::none)) ||
            !Utility::is_equal(moves[2], Move(square::c7, square::c8, MoveType::Bishop_Promotion, def::none)) ||
            !Utility::is_equal(moves[3], Move(square::c7, square::c8, MoveType::Rook_Promotion, def::none)) ||
            !Utility::is_equal(moves[4], Move(square::c7, square::d8, MoveType::Queen_Promotion_Capture, piece::bN)) ||
            !Utility::is_equal(moves[5], Move(square::c7, square::d8, MoveType::Knight_Promotion_Capture, piece::bN)) ||
            !Utility::is_equal(moves[6], Move(square::c7, square::d8, MoveType::Bishop_Promotion_Capture, piece::bN)) ||
            !Utility::is_equal(moves[7], Move(square::c7, square::d8, MoveType::Rook_Promotion_Capture, piece::bN)) ||
            !Utility::is_equal(moves[8], Move(square::c7, square::b8, MoveType::Queen_Promotion_Capture, piece::bR)) ||
            !Utility::is_equal(moves[9], Move(square::c7, square::b8, MoveType::Knight_Promotion_Capture, piece::bR)) ||
            !Utility::is_equal(moves[10], Move(square::c7, square::b8, MoveType::Bishop_Promotion_Capture, piece::bR)) ||
            !Utility::is_equal(moves[11], Move(square::c7, square::b8, MoveType::Rook_Promotion_Capture, piece::bR)))
            return false;
        moves.clear();

        mg.generate_pawn_moves(square::h7, 1, wattpin.second, moves);
        // Utility::print_moves(moves);
        if (moves.size() != 4 ||
            !Utility::is_equal(moves[0], Move(square::h7, square::h8, MoveType::Queen_Promotion, def::none)) ||
            !Utility::is_equal(moves[1], Move(square::h7, square::h8, MoveType::Knight_Promotion, def::none)) ||
            !Utility::is_equal(moves[2], Move(square::h7, square::h8, MoveType::Bishop_Promotion, def::none)) ||
            !Utility::is_equal(moves[3], Move(square::h7, square::h8, MoveType::Rook_Promotion, def::none)))
            return false;
        moves.clear();

        mg.generate_pawn_moves(square::f7, 1, wattpin.second, moves);
        if (!moves.empty())
            return false;

        mg.generate_pawn_moves(square::f3, 1, wattpin.second, moves);
        if (!moves.empty())
            return false;

        mg.generate_pawn_moves(square::d3, 1, wattpin.second, moves);
        // Utility::print_moves(moves);
        if (moves.size() != 1 ||
            !Utility::is_equal(moves[0], Move(square::d3, square::c4, MoveType::Capture, piece::bB)))
            return false;
        moves.clear();

        mg.generate_pawn_moves(square::e3, 1, wattpin.second, moves);
        // Utility::print_moves(moves);
        if (moves.size() != 1 ||
            !Utility::is_equal(moves[0], Move(square::e3, square::e4, MoveType::Quite, def::none)))
            return false;
        moves.clear();
        return true;
    }

    bool MoveGeneratorTests::test16()
    {
        // test: prevented en-passant capture due to pin: black
        Board b;
        Fen f("1k6/8/8/4p3/3PPP2/8/pK1p3p/NNQ1B1R1 b - - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg(b);

        std::vector<Move> moves;
        auto battpin = mg.compute_attacks_and_pins(square::b8, -1);
        mg.generate_pawn_moves(square::a2, -1, battpin.second, moves);
        // Utility::print_moves(moves);
        if (moves.size() != 4 ||
            !Utility::is_equal(moves[0], Move(square::a2, square::b1, MoveType::Queen_Promotion_Capture, piece::wN)) ||
            !Utility::is_equal(moves[1], Move(square::a2, square::b1, MoveType::Knight_Promotion_Capture, piece::wN)) ||
            !Utility::is_equal(moves[2], Move(square::a2, square::b1, MoveType::Bishop_Promotion_Capture, piece::wN)) ||
            !Utility::is_equal(moves[3], Move(square::a2, square::b1, MoveType::Rook_Promotion_Capture, piece::wN)))
            return false;
        moves.clear();

        mg.generate_pawn_moves(square::d2, -1, battpin.second, moves);
        // Utility::print_moves(moves);
        if (moves.size() != 12 ||
            !Utility::is_equal(moves[0], Move(square::d2, square::d1, MoveType::Queen_Promotion, def::none)) ||
            !Utility::is_equal(moves[1], Move(square::d2, square::d1, MoveType::Knight_Promotion, def::none)) ||
            !Utility::is_equal(moves[2], Move(square::d2, square::d1, MoveType::Bishop_Promotion, def::none)) ||
            !Utility::is_equal(moves[3], Move(square::d2, square::d1, MoveType::Rook_Promotion, def::none)) ||
            !Utility::is_equal(moves[4], Move(square::d2, square::c1, MoveType::Queen_Promotion_Capture, piece::wQ)) ||
            !Utility::is_equal(moves[5], Move(square::d2, square::c1, MoveType::Knight_Promotion_Capture, piece::wQ)) ||
            !Utility::is_equal(moves[6], Move(square::d2, square::c1, MoveType::Bishop_Promotion_Capture, piece::wQ)) ||
            !Utility::is_equal(moves[7], Move(square::d2, square::c1, MoveType::Rook_Promotion_Capture, piece::wQ)) ||
            !Utility::is_equal(moves[8], Move(square::d2, square::e1, MoveType::Queen_Promotion_Capture, piece::wB)) ||
            !Utility::is_equal(moves[9], Move(square::d2, square::e1, MoveType::Knight_Promotion_Capture, piece::wB)) ||
            !Utility::is_equal(moves[10], Move(square::d2, square::e1, MoveType::Bishop_Promotion_Capture, piece::wB)) ||
            !Utility::is_equal(moves[11], Move(square::d2, square::e1, MoveType::Rook_Promotion_Capture, piece::wB)))
            return false;
        moves.clear();

        mg.generate_pawn_moves(square::e5, -1, battpin.second, moves);
        // Utility::print_moves(moves);
        if (moves.size() != 2 ||
            !Utility::is_equal(moves[0], Move(square::e5, square::d4, MoveType::Capture, piece::wP)) ||
            !Utility::is_equal(moves[1], Move(square::e5, square::f4, MoveType::Capture, piece::wP)))
            return false;
        moves.clear();
        return true;
    }

    bool MoveGeneratorTests::test17() 
    {
        // moves under check - for white
        Board b;
        Fen f("rnbqkb1r/pppppppp/8/8/4n3/3P4/PPPKPPPP/RNBQ1BNR w kq - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg(b);
        auto moves = mg.generate_moves();

        return
            moves.size() == 3 &&
            Utility::is_equal(moves[0], Move(square::d2, square::e3, MoveType::Quite, def::none)) &&
            Utility::is_equal(moves[1], Move(square::d2, square::e1, MoveType::Quite, def::none)) &&
            Utility::is_equal(moves[2], Move(square::d3, square::e4, MoveType::Capture, piece::bN));
    }
    
    bool MoveGeneratorTests::test18()
    {
        // moves under check - for white
        Board b;
        Fen f("rnb1kbnr/pp1ppppp/2p5/q7/8/3P4/PPPKPPPP/RNBQ1BNR w kq - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg(b);
        auto moves = mg.generate_moves();

        return
            moves.size() == 4 &&
            Utility::is_equal(moves[0], Move(square::d2, square::e3, MoveType::Quite, def::none)) &&
            Utility::is_equal(moves[1], Move(square::b1, square::c3, MoveType::Quite, def::none)) &&
            Utility::is_equal(moves[2], Move(square::c2, square::c3, MoveType::Quite, def::none)) &&
            Utility::is_equal(moves[3], Move(square::b2, square::b4, MoveType::Double_Pawn_Push, def::none));
    }

    bool MoveGeneratorTests::test19()
    {
        // double check: check mate!
        Board b;
        Fen f("rnbqkb1r/ppp1ppp1/3N1n1p/3p4/Q7/2P5/PP1PPPPP/R1B1KBNR b KQkq - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg(b);
        auto moves = mg.generate_moves();

        return moves.size() == 0;
    }

    bool MoveGeneratorTests::test20()
    {
        // double check: check mate!
        Board b;
        Fen f("rnbqk2r/ppp1ppp1/3N1n1p/3p4/Q7/2P5/PP1PPPPP/R1B1KBNR b KQkq - 0 1");
        Utility::fen_to_board(f, b);
        MoveGenerator mg(b);
        auto moves = mg.generate_moves();

        return moves.size() == 1 && Utility::is_equal(moves[0], Move(square::e8, square::f8, MoveType::Quite, def::none));
    }

    

    */
}