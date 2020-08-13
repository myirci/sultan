#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <unordered_set>

#include "test.hpp"
#include "fen.hpp"
#include "move.hpp"
#include "piece.hpp"
#include "board_representation/square.hpp"
#include "board_representation/board.hpp"
#include "utility.hpp"

bool Test::test1()
{
    // tests: default constructor, get_fen()
    Fen f;
    return f.get_fen() == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
}

bool Test::test2()
{
    // tests: const string left reference constructor, get_fen()
    std::string pos { "6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b KQkq c6 1 2" };
    Fen f(pos);
    return f.get_fen() == pos;
}

bool Test::test3()
{
    // tests: string right reference constructor, get_fen
    std::string pos1 { "6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b KQkq c6 1 2" };
    auto pos2 = pos1;
    Fen f(std::move(pos1));
    return f.get_fen() == pos2;
}

bool Test::test4()
{
    // tests: const string left reference constructor, set_fen
    std::string pos1 { "6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b KQkq c6 1 2" };
    std::string pos2 { "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -" };
    Fen f(pos1);
    f.set_fen(pos2);
    return f.get_fen() == pos2;
}

bool Test::test5()
{
    // tests: string right reference constructor, parse, overloaded ouput stream operator
    Fen f("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b KQkq c6 1 2");
    std::stringstream ss;
    ss << f;
    auto res = R"(Piece Placement : 6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1
Active Color    : b
Castling Avail  : KQkq
En Passant      : c6
Half Move Clock : 1
Full Move Number: 2
)";
    return ss.str() == res;
}

bool Test::test6_()
{
    std::string pos1 { "6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b KQkq c6 1 2" };
    Fen f1(pos1);
    Fen f2(std::move(f1));
    return f2.get_fen() == pos1;
}

bool Test::test6() {

    // tests: constructor, set_from, get_from set_to, get_to, set_move_flag, get_move_flag
    Move m { 23, 54, Move_flag::Quite };
    return m.get_from() == 23 && m.get_to() == 54 && m.get_move_flag() == Move_flag::Quite;
}

bool Test::test7()
{
    Move m { 23, 54, Move_flag::Capture };
    return m.is_capture() == true;
}

bool Test::test8()
{
    Move m { 23, 54, Move_flag::King_Side_Castle };
    return m.is_capture() == false;
}

bool Test::test9()
{
    Move m { 97, 114, Move_flag::Queen_Promotion };
    return m.is_promotion() == true;
}

bool Test::test10()
{
    Move m { 103, 119, Move_flag::Knight_Promotion };
    return m.is_promotion() == true;
}

bool Test::test11()
{
    Move m { 103, 119, Move_flag::Rook_Promotion};
    return m.is_promotion() == true;
}

bool Test::test12()
{
    Move m { 103, 119, Move_flag::Bishop_Promotion };
    return m.is_promotion() == true;
}

bool Test::test13()
{
    Move m { 103, 119, Move_flag::Double_Pawn_Push};
    return m.is_promotion() == false;
}

bool Test::test14()
{
    Move m { 103, 119, Move_flag::En_Passant_Capture};
    return m.is_promotion() == false;
}

bool Test::test15()
{
    Move m { 51, 34, Move_flag::En_Passant_Capture};
    return m.is_capture() == true;
}

bool Test::test16()
{
    return Piece::eM == 0 && Piece::nN == -7 && Piece::wP == -Piece::bP &&
           Piece::wR == -Piece::bR && Piece::wN == -Piece::bN &&
           Piece::wB == -Piece::bB && Piece::wQ == -Piece::bQ &&
           Piece::wK == -Piece::bK;
}

bool Test::test17()
{
    Board b;
    auto func = [](int8_t v) { return v == Piece::nN; };
    return  std::all_of(b.board, b.board+Board::BOARDSIZE, func) &&
            b.white_pieces.empty() &&
            b.black_pieces.empty() &&
            std::all_of(b.black_pieces.begin(), b.black_pieces.end(), func) &&
            b.en_passant_loc == Piece::nN &&
            b.flags == 0 &&
            b.half_move_counter == -1 &&
            b.full_move_counter == -1;
}

bool Test::test18()
{
    Board b;
    b.set_side_to_move(Piece::white);
    return b.flags == 1 && b.get_side_to_move() == Piece::white;
}

bool Test::test19()
{
    Board b;
    b.set_side_to_move(Piece::black);
    return b.flags == 0 && b.get_side_to_move() == Piece::black;
}

bool Test::test20()
{
    Board b;
    b.set_castling_availability(Castling::white_king_side, true);
    bool res1 { b.flags == 2 &&
                b.query_castling_availability(Castling::white_king_side) == true &&
                b.query_castling_availability(Castling::white_queen_side) == false &&
                b.query_castling_availability(Castling::black_king_side) == false &&
                b.query_castling_availability(Castling::black_queen_side) == false };

    b.set_castling_availability(Castling::white_king_side, false);
    bool res2 { b.flags == 0 &&
                b.query_castling_availability(Castling::white_king_side) == false &&
                b.query_castling_availability(Castling::white_queen_side) == false &&
                b.query_castling_availability(Castling::black_king_side) == false &&
                b.query_castling_availability(Castling::black_queen_side) == false };
    return res1 && res2;
}

bool Test::test21()
{
    Board b;
    b.set_castling_availability(Castling::white_queen_side, true);
    bool res1 { b.flags == 4 &&
                b.query_castling_availability(Castling::white_king_side) == false &&
                b.query_castling_availability(Castling::white_queen_side) == true &&
                b.query_castling_availability(Castling::black_king_side) == false &&
                b.query_castling_availability(Castling::black_queen_side) == false };

    b.set_castling_availability(Castling::white_queen_side, false);
    bool res2 { b.flags == 0 &&
                b.query_castling_availability(Castling::white_king_side) == false &&
                b.query_castling_availability(Castling::white_queen_side) == false &&
                b.query_castling_availability(Castling::black_king_side) == false &&
                b.query_castling_availability(Castling::black_queen_side) == false };
    return res1 && res2;
}

bool Test::test22()
{
    Board b;
    b.set_castling_availability(Castling::black_king_side, true);
    bool res1 { b.flags == 8 &&
                b.query_castling_availability(Castling::white_king_side) == false &&
                b.query_castling_availability(Castling::white_queen_side) == false &&
                b.query_castling_availability(Castling::black_king_side) == true &&
                b.query_castling_availability(Castling::black_queen_side) == false };

    b.set_castling_availability(Castling::black_king_side, false);
    bool res2 { b.flags == 0 &&
                b.query_castling_availability(Castling::white_king_side) == false &&
                b.query_castling_availability(Castling::white_queen_side) == false &&
                b.query_castling_availability(Castling::black_king_side) == false &&
                b.query_castling_availability(Castling::black_queen_side) == false };
    return res1 && res2;
}

bool Test::test23()
{
    Board b;
    b.set_castling_availability(Castling::black_queen_side, true);
    bool res1 { b.flags == 16 &&
                b.query_castling_availability(Castling::white_king_side) == false &&
                b.query_castling_availability(Castling::white_queen_side) == false &&
                b.query_castling_availability(Castling::black_king_side) == false &&
                b.query_castling_availability(Castling::black_queen_side) == true };

    b.set_castling_availability(Castling::black_queen_side, false);
    bool res2 { b.flags == 0 &&
                b.query_castling_availability(Castling::white_king_side) == false &&
                b.query_castling_availability(Castling::white_queen_side) == false &&
                b.query_castling_availability(Castling::black_king_side) == false &&
                b.query_castling_availability(Castling::black_queen_side) == false };
    return res1 && res2;
}

bool Test::test24()
{
    Board b;
    Fen f;
    Utility::fen_to_board(f, b);
    int8_t board[Board::BOARDSIZE] =
    {
        Piece::wR, Piece::wN, Piece::wB, Piece::wQ, Piece::wK, Piece::wB, Piece::wN, Piece::wR, Piece::nN, Piece::nN, Piece::nN, Piece::nN, Piece::nN, Piece::nN, Piece::nN, Piece::nN,
        Piece::wP, Piece::wP, Piece::wP, Piece::wP, Piece::wP, Piece::wP, Piece::wP, Piece::wP, Piece::nN, Piece::nN, Piece::nN, Piece::nN, Piece::nN, Piece::nN, Piece::nN, Piece::nN,
        Piece::eM, Piece::eM, Piece::eM, Piece::eM, Piece::eM, Piece::eM, Piece::eM, Piece::eM, Piece::nN, Piece::nN, Piece::nN, Piece::nN, Piece::nN, Piece::nN, Piece::nN, Piece::nN,
        Piece::eM, Piece::eM, Piece::eM, Piece::eM, Piece::eM, Piece::eM, Piece::eM, Piece::eM, Piece::nN, Piece::nN, Piece::nN, Piece::nN, Piece::nN, Piece::nN, Piece::nN, Piece::nN,
        Piece::eM, Piece::eM, Piece::eM, Piece::eM, Piece::eM, Piece::eM, Piece::eM, Piece::eM, Piece::nN, Piece::nN, Piece::nN, Piece::nN, Piece::nN, Piece::nN, Piece::nN, Piece::nN,
        Piece::eM, Piece::eM, Piece::eM, Piece::eM, Piece::eM, Piece::eM, Piece::eM, Piece::eM, Piece::nN, Piece::nN, Piece::nN, Piece::nN, Piece::nN, Piece::nN, Piece::nN, Piece::nN,
        Piece::bP, Piece::bP, Piece::bP, Piece::bP, Piece::bP, Piece::bP, Piece::bP, Piece::bP, Piece::nN, Piece::nN, Piece::nN, Piece::nN, Piece::nN, Piece::nN, Piece::nN, Piece::nN,
        Piece::bR, Piece::bN, Piece::bB, Piece::bQ, Piece::bK, Piece::bB, Piece::bN, Piece::bR, Piece::nN, Piece::nN, Piece::nN, Piece::nN, Piece::nN, Piece::nN, Piece::nN, Piece::nN
    };

    for (auto i = 0; i < Board::BOARDSIZE; i++)
    {
        if (board[i] != b.board[i])
        {
            std::cout << "Board, mismatch at location " << i << " : " << static_cast<int>(board[i]) << " != " << static_cast<int>(b.board[i]) << std::endl;
            return false;
        }
    }

    if(b.white_pieces.size() != 16)
        return false;

    std::array<int8_t, 16> wpieces {
        Square::e1, Square::d1, Square::a1, Square::h1, Square::c1, Square::f1, Square::b1, Square::g1,
        Square::a2, Square::b2, Square::c2, Square::d2, Square::e2, Square::f2, Square::g2, Square::h2
    };

    for(size_t i = 0; i < b.white_pieces.size(); i++)
    {
        if(b.white_pieces[i] != wpieces[i])
        {
            std::cout << "White pieces, mismatch at location " << i << " : " << static_cast<int>(wpieces[i]) << " != " << static_cast<int>(b.white_pieces[i]) << std::endl;
            return false;
        }
    }

    if(b.black_pieces.size() != 16)
        return false;

    std::array<int8_t, 16> bpieces {
        Square::e8, Square::d8, Square::a8, Square::h8, Square::c8, Square::f8, Square::b8, Square::g8,
        Square::a7, Square::b7, Square::c7, Square::d7, Square::e7, Square::f7, Square::g7, Square::h7
    };

    for(size_t i = 0; i < b.black_pieces.size(); i++)
    {
        if(b.black_pieces[i] != bpieces[i])
        {
            std::cout << "Black pieces, mismatch at location " << i << " : " << static_cast<int>(bpieces[i]) << " != " << static_cast<int>(b.black_pieces[i]) << std::endl;
            return false;
        }
    }

    return
            b.en_passant_loc == Piece::nN &&
            b.flags == 31 &&
            b.half_move_counter == 0 &&
            b.full_move_counter == 1;
}

bool Test::test25()
{
    Board b;
    Fen f;
    Utility::fen_to_board(f, b);
    auto fstr = Utility::board_to_fen_string(b);
    return fstr == f.get_fen();
}

bool Test::test26()
{
    Board b;
    Fen f("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b KQkq c6 1 2");
    Utility::fen_to_board(f, b);
    auto fstr = Utility::board_to_fen_string(b);
    return fstr == f.get_fen();
}

bool Test::test27()
{
    Board b;
    Fen f("5Q1r/2pb2pp/1pn1p2n/rP2Pp1k/p1Pp1P1q/P1bP1NPB/1B1N3P/R3K1R1 w Q - 0 1");
    Utility::fen_to_board(f, b);
    auto fstr = Utility::board_to_fen_string(b);
    return fstr == f.get_fen();
}

bool Test::test28()
{
    Board b;
    Fen f("8/3k4/8/8/8/2K5/8/8 w - - 0 1");
    Utility::fen_to_board(f, b);
    auto fstr = Utility::board_to_fen_string(b);
    return fstr == f.get_fen();
}

bool Test::test29()
{
    Board b;
    Fen f("8/3k3P/P6P/7p/P3q2p/2K4p/7p/8 w - - 0 1");
    Utility::fen_to_board(f, b);
    auto fstr = Utility::board_to_fen_string(b);
    return fstr == f.get_fen();
}

bool Test::test30()
{
    Board b;
    Fen f("3qk3/pp2pppb/b6n/2n2P2/2N5/r1PKB2r/P2RN1PP/3r1b1R w - - 0 1");
    Utility::fen_to_board(f, b);

    std::vector<MoveGenerator::Attack> attacks
    {
        MoveGenerator::Attack{Square::d8, Square::Directions::N},
        MoveGenerator::Attack{Square::c5, Square::Directions::ND}
    };

    std::vector<MoveGenerator::Pin> pins
    {
        MoveGenerator::Pin{Square::h3, Square::e3, Square::Directions::E},
        MoveGenerator::Pin{Square::d1, Square::d2, Square::Directions::S},
        MoveGenerator::Pin{Square::a3, Square::c3, Square::Directions::W},
        MoveGenerator::Pin{Square::h7, Square::f5, Square::Directions::NE},
        MoveGenerator::Pin{Square::f1, Square::e2, Square::Directions::SE},
        MoveGenerator::Pin{Square::a6, Square::c4, Square::Directions::NW}
    };

    MoveGenerator mg(b);
    auto ap = mg.compute_attacks_and_pins(Square::d3, -1);

    if(ap.first.size() != attacks.size())
    {
        std::cout << "Number of dectected checks is not correct: "
                  << ap.first.size() << "(expected: " <<  attacks.size() << ")" << std::endl;
        return false;
    }
    else
    {
        for(size_t i = 0; i < attacks.size(); i++)
        {
            if(attacks[i].attacker_loc != ap.first[i].attacker_loc) return false;
            if(attacks[i].attack_dir != ap.first[i].attack_dir) return false;
        }
    }

    if(ap.second.size() != pins.size())
    {
        std::cout << "Number of dectected pins is not correct: "
                  << ap.second.size() << "(expected:" << pins.size() << ")" << std::endl;
        return false;
    }
    else
    {
        for(size_t i = 0; i < pins.size(); i++)
        {
            if(pins[i].pinner_loc != ap.second[i].pinner_loc) return false;
            if(pins[i].pinned_loc != ap.second[i].pinned_loc) return false;
            if(pins[i].pin_dir != ap.second[i].pin_dir) return false;
        }
    }

    return true;
}

bool Test::test31()
{
    Board b;
    Fen f("Q2R2Q1/1p3b2/3p4/R1nk2qR/8/1q1r1p2/B2R4/7B b - - 0 1");
    Utility::fen_to_board(f, b);

    std::vector<MoveGenerator::Attack> attacks {};
    std::vector<MoveGenerator::Pin> pins
    {
        MoveGenerator::Pin{Square::d8, Square::d6, Square::Directions::N},
        MoveGenerator::Pin{Square::h5, Square::g5, Square::Directions::E},
        MoveGenerator::Pin{Square::d2, Square::d3, Square::Directions::S},
        MoveGenerator::Pin{Square::a5, Square::c5, Square::Directions::W},
        MoveGenerator::Pin{Square::g8, Square::f7, Square::Directions::NE},
        MoveGenerator::Pin{Square::h1, Square::f3, Square::Directions::SE},
        MoveGenerator::Pin{Square::a2, Square::b3, Square::Directions::SW},
        MoveGenerator::Pin{Square::a8, Square::b7, Square::Directions::NW}
    };

    MoveGenerator mg(b);
    auto ap = mg.compute_attacks_and_pins(Square::d5, 1);

    if(ap.first.size() != attacks.size())
    {
        std::cout << "Number of dectected checks is not correct: "
                  << ap.first.size() << "(expected: " <<  attacks.size() << ")" << std::endl;
        return false;
    }

    if(ap.second.size() != pins.size())
    {
        std::cout << "Number of dectected pins is not correct: "
                  << ap.second.size() << "(expected:" << pins.size() << ")" << std::endl;
        return false;
    }
    else
    {
        for(size_t i = 0; i < pins.size(); i++)
        {
            if(pins[i].pinner_loc != ap.second[i].pinner_loc) return false;
            if(pins[i].pinned_loc != ap.second[i].pinned_loc) return false;
            if(pins[i].pin_dir != ap.second[i].pin_dir) return false;
        }
    }

    return true;
}

bool Test::test32()
{
    Board b;
    Fen f("Q2R2Q1/1p1NNb2/2Ppp3/RPnk1pqR/2Pn4/1q1r1p2/B2R2P1/7B b - - 0 1");
    Utility::fen_to_board(f, b);

    std::vector<MoveGenerator::Attack> attacks
    {
        MoveGenerator::Attack{Square::c4, Square::Directions::SW},
        MoveGenerator::Attack{Square::e7, Square::Directions::ND}
    };

    std::vector<MoveGenerator::Pin> pins { };

    MoveGenerator mg(b);
    auto ap = mg.compute_attacks_and_pins(Square::d5, 1);

    if(ap.first.size() != attacks.size())
    {
        std::cout << "Number of dectected checks is not correct: "
                  << ap.first.size() << "(expected: " <<  attacks.size() << ")" << std::endl;
        return false;
    }
    else
    {
        for(size_t i = 0; i < attacks.size(); i++)
        {
            if(attacks[i].attacker_loc != ap.first[i].attacker_loc) return false;
            if(attacks[i].attack_dir != ap.first[i].attack_dir) return false;
        }
    }

    if(ap.second.size() != pins.size())
    {
        std::cout << "Number of dectected pins is not correct: "
                  << ap.second.size() << "(expected:" << pins.size() << ")" << std::endl;
        return false;
    }

    return true;
}

bool Test::test33()
{
    Board b;
    Fen f("8/1k2p1p1/6P1/p7/8/8/2P1K2P/8 w - - 0 1");
    Utility::fen_to_board(f, b);

    std::unordered_set<int8_t> black_attacked_squares
    {
        Square::a8, Square::b8, Square::c8, Square::a7, Square::c7, Square::b4,
        Square::a6, Square::b6, Square::c6, Square::d6, Square::f6, Square::h6
    };

    std::unordered_set<int8_t> white_attacked_squares
    {
        Square::f7, Square::h7, Square::b3, Square::d3, Square::e3, Square::f3,
        Square::g3, Square::d2, Square::f2, Square::d1,Square::e1,Square::f1
    };

    MoveGenerator mg(b);
    for(auto i = 0; i < 64; i++)
    {
        auto sq = Square::order[i];
        if(black_attacked_squares.find(sq) != black_attacked_squares.end())
        {
            if(!mg.is_under_attack(sq, -1))
            {
                std::cout << Square::square_to_string(sq) << " is attacked by black but the attack cannot be detected" << std::endl;
                return false;
            }
        }
        else
        {
            if(mg.is_under_attack(sq, -1))
            {
                std::cout << Square::square_to_string(sq) << " is not attacked by black but it is detected" << std::endl;
                return false;
            }
        }

        if(white_attacked_squares.find(sq) != white_attacked_squares.end())
        {
            if(!mg.is_under_attack(sq, 1))
            {
                std::cout << Square::square_to_string(sq) << " is attacked by white but the attack cannot be detected" << std::endl;
                return false;
            }
        }
        else
        {
            if(mg.is_under_attack(sq, 1))
            {
                std::cout << Square::square_to_string(sq) << " is not attacked by white but it is detected" << std::endl;
                return false;
            }
        }
    }

    return true;
}

bool Test::test34()
{
    Board b;
    Fen f("3k2nq/3b1rp1/3N3n/2B5/4N2b/PPR5/Q2PB3/6K1 w - - 0 1");
    Utility::fen_to_board(f, b);

    std::unordered_set<int8_t> black_attacked_squares
    {
        Square::c8, Square::d8, Square::e8, Square::f8, Square::g8,
        Square::c7, Square::d7, Square::e7, Square::f7, Square::g7, Square::h7,
        Square::c6, Square::e6, Square::f6, Square::h6,
        Square::b5, Square::f5, Square::g5,
        Square::a4, Square::f4, Square::g4,
        Square::f3, Square::g3, Square::h3,
        Square::f2,
        Square::e1, Square::f1
    };

    std::unordered_set<int8_t> white_attacked_squares
    {
        Square::c8, Square::e8,
        Square::a7, Square::b7, Square::f7,
        Square::a6, Square::b6, Square::d6, Square::f6,
        Square::b5, Square::c5, Square::f5, Square::g5, Square::h5,
        Square::a4, Square::b4, Square::c4, Square::d4, Square::e4, Square::g4,
        Square::a3, Square::b3, Square::c3, Square::d3, Square::e3, Square::f3, Square::g3, Square::h3,
        Square::b2, Square::c2, Square::d2, Square::f2, Square::g2, Square::h2,
        Square::a1, Square::b1, Square::c1, Square::d1, Square::f1, Square::g1, Square::h1
    };

    MoveGenerator mg(b);
    for(auto i = 0; i < 64; i++)
    {
        auto sq = Square::order[i];
        if(black_attacked_squares.find(sq) != black_attacked_squares.end())
        {
            if(!mg.is_under_attack(sq, -1))
            {
                std::cout << Square::square_to_string(sq) << " is attacked by black but the attack cannot be detected" << std::endl;
                return false;
            }
        }
        else
        {
            if(mg.is_under_attack(sq, -1))
            {
                std::cout << Square::square_to_string(sq) << " is not attacked by black but it is detected" << std::endl;
                return false;
            }
        }

        if(white_attacked_squares.find(sq) != white_attacked_squares.end())
        {
            if(!mg.is_under_attack(sq, 1))
            {
                std::cout << Square::square_to_string(sq) << " is attacked by white but the attack cannot be detected" << std::endl;
                return false;
            }
        }
        else
        {
            if(mg.is_under_attack(sq, 1))
            {
                std::cout << Square::square_to_string(sq) << " is not attacked by white but it is detected" << std::endl;
                return false;
            }
        }
    }

    return true;
}

bool Test::test35()
{
    return false;
}

bool Test::test36() { return false; }
bool Test::test37() { return false; }
bool Test::test38() { return false; }
bool Test::test39() { return false; }
bool Test::test40() { return false; }

void Test::run_fen_tests()
{
    std::vector<bool(*)()> tests { test1, test2, test3, test4, test5, test6_};
    run_test("Fen", tests);
}

void Test::run_move_tests()
{
    std::vector<bool(*)()> tests { test6, test7, test8, test9, test10, test11, test12, test13, test14, test15 };
    run_test("Move", tests);
}

void Test::run_piece_tests()
{
    std::vector<bool (*)()> tests { test16 };
    run_test("Piece", tests);
}

void Test::run_board_tests()
{
    std::vector<bool (*)()> tests { test17, test18, test19, test20, test21, test22, test23 };
    run_test("Board", tests);
}

void Test::run_utility_tests()
{
    std::vector<bool (*)()> tests { test24, test25, test26, test27, test28, test29 };
    run_test("Utility", tests);
}

void Test::run_move_generator_tests()
{
    std::vector<bool (*)()> tests { test30, test31, test32, test33, test34 };
    run_test("Move Generator", tests);
}

void Test::run_test(std::string_view desc, const std::vector<bool (*)()>& tests)
{
    auto str{""};
    int i{0};
    std::cout << desc << " Tests : " << std::endl;
    for(auto tf : tests)
    {
        str = tf() ? " PASS" : " FAIL";
        std::cout <<"\tTest-" << ++i << " "<< str << std::endl;
    }
}

void Test::print_attack_and_pins(std::pair<std::vector<MoveGenerator::Attack>, std::vector<MoveGenerator::Pin>> const & ap)
{
    for(auto it = ap.first.begin(); it != ap.first.end(); it++)
        std::cout << "Attacker loc: " << Square::square_to_string(it->attacker_loc)
                  << " Attack dir: " << Square::Directions::direction_to_chararray.find(it->attack_dir)->second
                  << std::endl;

    std::cout << "-----------------------------------------\n";

    for(auto it = ap.second.begin(); it != ap.second.end(); it++)
        std::cout << "Pinner loc: " << Square::square_to_string(it->pinner_loc)
                  << " Pinned loc: " << Square::square_to_string(it->pinned_loc)
                  << " Pin dir: " << Square::Directions::direction_to_chararray.find(it->pin_dir)->second
                  << std::endl;
}
