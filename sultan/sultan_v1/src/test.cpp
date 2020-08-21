#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <unordered_set>
#include <algorithm>
#include <stdexcept>

#include "test.hpp"
#include "fen.hpp"
#include "piece.hpp"
#include "utility.hpp"
#include "engine.hpp"
#include "definitions.hpp"
#include "move.hpp"
#include "square.hpp"
#include "board.hpp"


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

bool Test::test6() 
{
    int8_t f{ 23 }, t{ 54 };
    Move m { 23, 54, Move_flag::Quite };

    if (m.get_from() != f) 
    {
        std::cout << "from: " << static_cast<int>(m.get_from()) << " expected: " << f << std::endl;
        return false;
    }
    
    if (m.get_to() != t) 
    {
        std::cout << "to: " << static_cast<int>(m.get_to()) << " expected: " << t << std::endl;
        return false;
    }

    if (m.get_move_flag() != Move_flag::Quite) 
    {
        std::cout << "move flag: " << Utility::move_flag_to_chararr.find(m.get_move_flag())->second
                  << " expected: " << Utility::move_flag_to_chararr.find(Move_flag::Quite)->second << std::endl;
        return false;
    }
    return true;
}

bool Test::test7()
{
    Move m { 0, 119, Move_flag::Capture };
    return m.get_from() == 0 && m.get_to() == 119 && m.is_capture() == true;
}

bool Test::test8()
{
    Move m { 119, 0, Move_flag::King_Side_Castle };
    return m.get_from() == 119 && m.get_to() == 0 && m.is_capture() == false;
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
    return piece::eM == 0 && def::none == -7 && piece::wP == -piece::bP &&
           piece::wR == -piece::bR && piece::wN == -piece::bN &&
           piece::wB == -piece::bB && piece::wQ == -piece::bQ &&
           piece::wK == -piece::bK;
}

bool Test::test17()
{
    Board b;
    auto func = [](int8_t v) { return v == def::none; };
    return  std::all_of(b.board, b.board+Board::BOARDSIZE, func) && b.piece_loc.empty() &&
            b.en_passant_loc == def::none && b.stored_en_passant_loc == def::none &&
            b.castling_rights == 0 && b.stored_castling_rights == 0 && 
            b.side_to_move == def::none && b.stored_captured_piece == def::none &&
            b.half_move_counter == -1 && b.full_move_counter == -1;
}

bool Test::test18()
{
    Board b;
    b.set_side_to_move(color::white);
    return b.get_side_to_move() == color::white;
}

bool Test::test19()
{
    Board b;
    b.set_side_to_move(color::black);
    return b.get_side_to_move() == color::black;
}

bool Test::test20()
{
    Board b;
    b.set_castling_rights(Castling::white_king_side, true);
    bool res1 { b.castling_rights == 1 &&
                b.query_castling_rights(Castling::white_king_side) == true &&
                b.query_castling_rights(Castling::white_queen_side) == false &&
                b.query_castling_rights(Castling::black_king_side) == false &&
                b.query_castling_rights(Castling::black_queen_side) == false };

    b.set_castling_rights(Castling::white_king_side, false);
    bool res2 { b.castling_rights == 0 &&
                b.query_castling_rights(Castling::white_king_side) == false &&
                b.query_castling_rights(Castling::white_queen_side) == false &&
                b.query_castling_rights(Castling::black_king_side) == false &&
                b.query_castling_rights(Castling::black_queen_side) == false };
    return res1 && res2;
}

bool Test::test21()
{
    Board b;
    b.set_castling_rights(Castling::white_queen_side, true);
    bool res1 { b.castling_rights == 2 &&
                b.query_castling_rights(Castling::white_king_side) == false &&
                b.query_castling_rights(Castling::white_queen_side) == true &&
                b.query_castling_rights(Castling::black_king_side) == false &&
                b.query_castling_rights(Castling::black_queen_side) == false };

    b.set_castling_rights(Castling::white_queen_side, false);
    bool res2 { b.castling_rights == 0 &&
                b.query_castling_rights(Castling::white_king_side) == false &&
                b.query_castling_rights(Castling::white_queen_side) == false &&
                b.query_castling_rights(Castling::black_king_side) == false &&
                b.query_castling_rights(Castling::black_queen_side) == false };
    return res1 && res2;
}

bool Test::test22()
{
    Board b;
    b.set_castling_rights(Castling::black_king_side, true);
    bool res1 { b.castling_rights == 4 &&
                b.query_castling_rights(Castling::white_king_side) == false &&
                b.query_castling_rights(Castling::white_queen_side) == false &&
                b.query_castling_rights(Castling::black_king_side) == true &&
                b.query_castling_rights(Castling::black_queen_side) == false };

    b.set_castling_rights(Castling::black_king_side, false);
    bool res2 { b.castling_rights == 4 &&
                b.query_castling_rights(Castling::white_king_side) == false &&
                b.query_castling_rights(Castling::white_queen_side) == false &&
                b.query_castling_rights(Castling::black_king_side) == false &&
                b.query_castling_rights(Castling::black_queen_side) == false };
    return res1 && res2;
}

bool Test::test23()
{
    Board b;
    b.set_castling_rights(Castling::black_queen_side, true);
    bool res1 { b.castling_rights == 8 &&
                b.query_castling_rights(Castling::white_king_side) == false &&
                b.query_castling_rights(Castling::white_queen_side) == false &&
                b.query_castling_rights(Castling::black_king_side) == false &&
                b.query_castling_rights(Castling::black_queen_side) == true };

    b.set_castling_rights(Castling::black_queen_side, false);
    bool res2 { b.castling_rights == 0 &&
                b.query_castling_rights(Castling::white_king_side) == false &&
                b.query_castling_rights(Castling::white_queen_side) == false &&
                b.query_castling_rights(Castling::black_king_side) == false &&
                b.query_castling_rights(Castling::black_queen_side) == false };
    return res1 && res2;
}

bool Test::test24()
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

    for (auto i = 0; i < Board::BOARDSIZE; i++)
    {
        if (board[i] != b.board[i])
        {
            std::cout << "Board, mismatch at location " << i << " : " << static_cast<int>(board[i]) << " != " << static_cast<int>(b.board[i]) << std::endl;
            return false;
        }
    }

    if (b.piece_loc.size() != 32) return false;

    std::vector<int8_t> white_pawn_loc { square::a2, square::b2, square::c2, square::d2, square::e2, square::f2, square::g2, square::h2 };
    if(!check_piece_locations(b, piece::wP, white_pawn_loc)) return false;

    std::vector<int8_t> black_pawn_loc { square::a7, square::b7, square::c7, square::d7, square::e7, square::f7, square::g7, square::h7 };
    if(!check_piece_locations(b, piece::bP, black_pawn_loc)) return false;

    std::vector<int8_t> white_rook_loc { square::a1, square::h1 };
    if (!check_piece_locations(b, piece::wR, white_rook_loc)) return false;

    std::vector<int8_t> black_rook_loc { square::a8, square::h8 };
    if (!check_piece_locations(b, piece::bR, black_rook_loc)) return false;

    std::vector<int8_t> white_knight_loc { square::b1, square::g1 };
    if (!check_piece_locations(b, piece::wN, white_knight_loc)) return false;

    std::vector<int8_t> black_knight_loc { square::b8, square::g8 };
    if (!check_piece_locations(b, piece::bN, black_knight_loc)) return false;

    std::vector<int8_t> white_bishop_loc { square::c1, square::f1 };
    if (!check_piece_locations(b, piece::wB, white_bishop_loc)) return false;

    std::vector<int8_t> black_bishop_loc { square::c8, square::f8 };
    if (!check_piece_locations(b, piece::bB, black_bishop_loc)) return false;

    return 
        b.piece_loc.find(piece::wK)->second == square::e1 && b.piece_loc.find(piece::bK)->second == square::e8 &&
        b.piece_loc.find(piece::wQ)->second == square::d1 && b.piece_loc.find(piece::bQ)->second == square::d8 &&
        b.en_passant_loc == def::none && b.castling_rights == 15 && b.side_to_move == color::white &&
        b.half_move_counter == 0 && b.full_move_counter == 1;
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
    // test: attacks and pins detection
    Board b;
    Fen f("3qk3/pp2pppb/b6n/2n2P2/2N5/r1PKB2r/P2RN1PP/3r1b1R w - - 0 1");
    Utility::fen_to_board(f, b);

    std::vector<MoveGenerator::Attack> attacks
    {
        MoveGenerator::Attack{square::d8, direction::N},
        MoveGenerator::Attack{square::c5, direction::ND}
    };

    std::vector<MoveGenerator::Pin> pins
    {
        MoveGenerator::Pin{square::h3, square::e3, direction::E},
        MoveGenerator::Pin{square::d1, square::d2, direction::S},
        MoveGenerator::Pin{square::a3, square::c3, direction::W},
        MoveGenerator::Pin{square::h7, square::f5, direction::NE},
        MoveGenerator::Pin{square::f1, square::e2, direction::SE},
        MoveGenerator::Pin{square::a6, square::c4, direction::NW}
    };

    MoveGenerator mg(b);
    auto ap = mg.compute_attacks_and_pins(square::d3, 1);

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
    // test: attacks and pins detection
    Board b;
    Fen f("Q2R2Q1/1p3b2/3p4/R1nk2qR/8/1q1r1p2/B2R4/7B b - - 0 1");
    Utility::fen_to_board(f, b);

    std::vector<MoveGenerator::Attack> attacks {};
    std::vector<MoveGenerator::Pin> pins
    {
        MoveGenerator::Pin{square::d8, square::d6, direction::N},
        MoveGenerator::Pin{square::h5, square::g5, direction::E},
        MoveGenerator::Pin{square::d2, square::d3, direction::S},
        MoveGenerator::Pin{square::a5, square::c5, direction::W},
        MoveGenerator::Pin{square::g8, square::f7, direction::NE},
        MoveGenerator::Pin{square::h1, square::f3, direction::SE},
        MoveGenerator::Pin{square::a2, square::b3, direction::SW},
        MoveGenerator::Pin{square::a8, square::b7, direction::NW}
    };

    MoveGenerator mg(b);
    auto ap = mg.compute_attacks_and_pins(square::d5, -1);

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
    // test: attacks and pins detection
    Board b;
    Fen f("Q2R2Q1/1p1NNb2/2Ppp3/RPnk1pqR/2Pn4/1q1r1p2/B2R2P1/7B b - - 0 1");
    Utility::fen_to_board(f, b);

    std::vector<MoveGenerator::Attack> attacks
    {
        MoveGenerator::Attack{square::c4, direction::SW},
        MoveGenerator::Attack{square::e7, direction::ND}
    };

    std::vector<MoveGenerator::Pin> pins { };

    MoveGenerator mg(b);
    auto ap = mg.compute_attacks_and_pins(square::d5, -1);

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
    // test: white and black attacked squares
    Board b;
    Fen f("8/1k2p1p1/6P1/p7/8/8/2P1K2P/8 w - - 0 1");
    Utility::fen_to_board(f, b);

    std::unordered_set<int8_t> black_attacked_squares
    {
        square::a8, square::b8, square::c8, square::a7, square::c7, square::b4,
        square::a6, square::b6, square::c6, square::d6, square::f6, square::h6
    };

    std::unordered_set<int8_t> white_attacked_squares
    {
        square::f7, square::h7, square::b3, square::d3, square::e3, square::f3,
        square::g3, square::d2, square::f2, square::d1,square::e1,square::f1
    };

    MoveGenerator mg(b);
    for(auto i = 0; i < 64; i++)
    {
        auto sq = square::order[i];
        if(black_attacked_squares.find(sq) != black_attacked_squares.end())
        {
            if(!mg.is_under_attack(sq, 1))
            {
                std::cout << square::square_to_string(sq) << " is attacked by black but the attack cannot be detected" << std::endl;
                return false;
            }
        }
        else
        {
            if(mg.is_under_attack(sq, 1))
            {
                std::cout << square::square_to_string(sq) << " is not attacked by black but it is detected" << std::endl;
                return false;
            }
        }

        if(white_attacked_squares.find(sq) != white_attacked_squares.end())
        {
            if(!mg.is_under_attack(sq, -1))
            {
                std::cout << square::square_to_string(sq) << " is attacked by white but the attack cannot be detected" << std::endl;
                return false;
            }
        }
        else
        {
            if(mg.is_under_attack(sq, -1))
            {
                std::cout << square::square_to_string(sq) << " is not attacked by white but it is detected" << std::endl;
                return false;
            }
        }
    }

    return true;
}

bool Test::test34()
{
    // test: white and black attacked squares
    Board b;
    Fen f("3k2nq/3b1rp1/3N3n/2B5/4N2b/PPR5/Q2PB3/6K1 w - - 0 1");
    Utility::fen_to_board(f, b);
    MoveGenerator mg(b);

    std::unordered_set<int8_t> black_attacked_squares
    {
        square::c8, square::d8, square::e8, square::f8, square::g8,
        square::c7, square::d7, square::e7, square::f7, square::g7, square::h7,
        square::c6, square::e6, square::f6, square::h6,
        square::b5, square::f5, square::g5,
        square::a4, square::f4, square::g4,
        square::f3, square::g3, square::h3,
        square::f2,
        square::e1, square::f1
    };

    std::unordered_set<int8_t> white_attacked_squares
    {
        square::c8, square::e8,
        square::a7, square::b7, square::f7,
        square::a6, square::b6, square::d6, square::f6,
        square::b5, square::c5, square::f5, square::g5, square::h5,
        square::a4, square::b4, square::c4, square::d4, square::e4, square::g4,
        square::a3, square::b3, square::c3, square::d3, square::e3, square::f3, square::g3, square::h3,
        square::b2, square::c2, square::d2, square::f2, square::g2, square::h2,
        square::a1, square::b1, square::c1, square::d1, square::f1, square::g1, square::h1
    };

    for(auto i = 0; i < 64; i++)
    {
        auto sq = square::order[i];
        if(black_attacked_squares.find(sq) != black_attacked_squares.end())
        {
            if(!mg.is_under_attack(sq, 1))
            {
                std::cout << square::square_to_string(sq) << " is attacked by black but the attack cannot be detected" << std::endl;
                return false;
            }
        }
        else
        {
            if(mg.is_under_attack(sq, 1))
            {
                std::cout << square::square_to_string(sq) << " is not attacked by black but it is detected" << std::endl;
                return false;
            }
        }

        if(white_attacked_squares.find(sq) != white_attacked_squares.end())
        {
            if(!mg.is_under_attack(sq, -1))
            {
                std::cout << square::square_to_string(sq) << " is attacked by white but the attack cannot be detected" << std::endl;
                return false;
            }
        }
        else
        {
            if(mg.is_under_attack(sq, -1))
            {
                std::cout << square::square_to_string(sq) << " is not attacked by white but it is detected" << std::endl;
                return false;
            }
        }
    }

    return true;
}

bool Test::test35()
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

    for (size_t i = 0; i < 24; i+=3) 
    {
        mg.generate_pawn_moves(wsq[i], 1, wattpin.second, moves);
        // Utility::print_moves(moves);
        if (moves.size() != 2 ||
            !Utility::is_equal(moves[0], Move(wsq[i], wsq[i + 1], Move_flag::Quite)) ||
            !Utility::is_equal(moves[1], Move(wsq[i], wsq[i + 2], Move_flag::Double_Pawn_Push)))
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
            !Utility::is_equal(moves[0], Move(bsq[i], bsq[i + 1], Move_flag::Quite)) ||
            !Utility::is_equal(moves[1], Move(bsq[i], bsq[i + 2], Move_flag::Double_Pawn_Push)))
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
    mg.generate_king_moves(square::e1, 1, false, moves);
    if (!moves.empty()) return false;
    mg.generate_king_moves(square::e8, -1, false, moves);
    if (!moves.empty()) return false;

    // Knight moves
    mg.generate_knight_moves(square::b1, 1, wattpin.second, moves);
    // Utility::print_moves(moves);
    if (moves.size() != 2 ||
        !Utility::is_equal(moves[0], Move(square::b1, square::c3, Move_flag::Quite)) ||
        !Utility::is_equal(moves[1], Move(square::b1, square::a3, Move_flag::Quite)))
        return false;

    moves.clear();
    mg.generate_knight_moves(square::g1, 1, wattpin.second, moves);
    // Utility::print_moves(moves);
    if (moves.size() != 2 ||
        !Utility::is_equal(moves[0], Move(square::g1, square::h3, Move_flag::Quite)) ||
        !Utility::is_equal(moves[1], Move(square::g1, square::f3, Move_flag::Quite)))
        return false;

    moves.clear();
    mg.generate_knight_moves(square::b8, -1, battpin.second, moves);
    // Utility::print_moves(moves);
    if (moves.size() != 2 ||
        !Utility::is_equal(moves[0], Move(square::b8, square::c6, Move_flag::Quite)) ||
        !Utility::is_equal(moves[1], Move(square::b8, square::a6, Move_flag::Quite)))
        return false;
    
    moves.clear();
    mg.generate_knight_moves(square::g8, -1, battpin.second, moves);
    // Utility::print_moves(moves);
    if (moves.size() != 2 ||
        !Utility::is_equal(moves[0], Move(square::g8, square::h6, Move_flag::Quite)) ||
        !Utility::is_equal(moves[1], Move(square::g8, square::f6, Move_flag::Quite)))
        return false;

    return true;
}

bool Test::test36() 
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
        !Utility::is_equal(moves[0], Move(square::c5, square::c6, Move_flag::Quite)) ||
        !Utility::is_equal(moves[1], Move(square::c5, square::b6, Move_flag::En_Passant_Capture)))
        return false;
    return true;
}

bool Test::test37()
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
        !Utility::is_equal(moves[0], Move(square::c5, square::c6, Move_flag::Quite)) ||
        !Utility::is_equal(moves[1], Move(square::c5, square::d6, Move_flag::En_Passant_Capture)))
        return false;
    return true;
}

bool Test::test38()
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
        !Utility::is_equal(moves[0], Move(square::e4, square::e3, Move_flag::Quite)) ||
        !Utility::is_equal(moves[1], Move(square::e4, square::f3, Move_flag::En_Passant_Capture)))
        return false;
    return true;
}

bool Test::test39()
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
        !Utility::is_equal(moves[0], Move(square::e4, square::e3, Move_flag::Quite)) ||
        !Utility::is_equal(moves[1], Move(square::e4, square::d3, Move_flag::En_Passant_Capture)))
        return false;
    return true;
}

bool Test::test40() 
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
    if(moves.size() != 1 || !Utility::is_equal(moves[0], Move(square::c5, square::b6, Move_flag::En_Passant_Capture)))
        return false;
    return true;
}

bool Test::test41()
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
    if(!moves.empty())
        return false;
    return true;
}

bool Test::test42()
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
    if (moves.size() != 1 || !Utility::is_equal(moves[0], Move(square::e4, square::d3, Move_flag::En_Passant_Capture)))
        return false;
    return true;
}

bool Test::test43()
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

bool Test::test44() 
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
    if(moves.size() != 8 || 
        !Utility::is_equal(moves[0], Move(square::a7, square::a8, Move_flag::Queen_Promotion)) ||
        !Utility::is_equal(moves[1], Move(square::a7, square::a8, Move_flag::Knight_Promotion)) ||
        !Utility::is_equal(moves[2], Move(square::a7, square::a8, Move_flag::Bishop_Promotion)) ||
        !Utility::is_equal(moves[3], Move(square::a7, square::a8, Move_flag::Rook_Promotion)) ||
        !Utility::is_equal(moves[4], Move(square::a7, square::b8, Move_flag::Queen_Promotion_Capture)) ||
        !Utility::is_equal(moves[5], Move(square::a7, square::b8, Move_flag::Knight_Promotion_Capture)) ||
        !Utility::is_equal(moves[6], Move(square::a7, square::b8, Move_flag::Bishop_Promotion_Capture)) ||
        !Utility::is_equal(moves[7], Move(square::a7, square::b8, Move_flag::Rook_Promotion_Capture)))
        return false;
    moves.clear();

    mg.generate_pawn_moves(square::c7, 1, wattpin.second, moves);
    // Utility::print_moves(moves);
    if (moves.size() != 12 ||
        !Utility::is_equal(moves[0], Move(square::c7, square::c8, Move_flag::Queen_Promotion)) ||
        !Utility::is_equal(moves[1], Move(square::c7, square::c8, Move_flag::Knight_Promotion)) ||
        !Utility::is_equal(moves[2], Move(square::c7, square::c8, Move_flag::Bishop_Promotion)) ||
        !Utility::is_equal(moves[3], Move(square::c7, square::c8, Move_flag::Rook_Promotion)) ||
        !Utility::is_equal(moves[4], Move(square::c7, square::d8, Move_flag::Queen_Promotion_Capture)) ||
        !Utility::is_equal(moves[5], Move(square::c7, square::d8, Move_flag::Knight_Promotion_Capture)) ||
        !Utility::is_equal(moves[6], Move(square::c7, square::d8, Move_flag::Bishop_Promotion_Capture)) ||
        !Utility::is_equal(moves[7], Move(square::c7, square::d8, Move_flag::Rook_Promotion_Capture)) ||
        !Utility::is_equal(moves[8], Move(square::c7, square::b8, Move_flag::Queen_Promotion_Capture)) ||
        !Utility::is_equal(moves[9], Move(square::c7, square::b8, Move_flag::Knight_Promotion_Capture)) ||
        !Utility::is_equal(moves[10], Move(square::c7, square::b8, Move_flag::Bishop_Promotion_Capture)) ||
        !Utility::is_equal(moves[11], Move(square::c7, square::b8, Move_flag::Rook_Promotion_Capture)))
        return false;
    moves.clear();

    mg.generate_pawn_moves(square::h7, 1, wattpin.second, moves);
    // Utility::print_moves(moves);
    if (moves.size() != 4 ||
        !Utility::is_equal(moves[0], Move(square::h7, square::h8, Move_flag::Queen_Promotion)) ||
        !Utility::is_equal(moves[1], Move(square::h7, square::h8, Move_flag::Knight_Promotion)) ||
        !Utility::is_equal(moves[2], Move(square::h7, square::h8, Move_flag::Bishop_Promotion)) ||
        !Utility::is_equal(moves[3], Move(square::h7, square::h8, Move_flag::Rook_Promotion)))
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
        !Utility::is_equal(moves[0], Move(square::d3, square::c4, Move_flag::Capture)))
        return false;
    moves.clear();

    mg.generate_pawn_moves(square::e3, 1, wattpin.second, moves);
    // Utility::print_moves(moves);
    if (moves.size() != 1 ||
        !Utility::is_equal(moves[0], Move(square::e3, square::e4, Move_flag::Quite)))
        return false;
    moves.clear();
    return true;
}

bool Test::test45()
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
        !Utility::is_equal(moves[0], Move(square::a2, square::b1, Move_flag::Queen_Promotion_Capture)) || 
        !Utility::is_equal(moves[1], Move(square::a2, square::b1, Move_flag::Knight_Promotion_Capture)) || 
        !Utility::is_equal(moves[2], Move(square::a2, square::b1, Move_flag::Bishop_Promotion_Capture)) || 
        !Utility::is_equal(moves[3], Move(square::a2, square::b1, Move_flag::Rook_Promotion_Capture)))
        return false;
    moves.clear();

    mg.generate_pawn_moves(square::d2, -1, battpin.second, moves);
    // Utility::print_moves(moves);
    if (moves.size() != 12 ||
        !Utility::is_equal(moves[0], Move(square::d2, square::d1, Move_flag::Queen_Promotion)) ||
        !Utility::is_equal(moves[1], Move(square::d2, square::d1, Move_flag::Knight_Promotion)) ||
        !Utility::is_equal(moves[2], Move(square::d2, square::d1, Move_flag::Bishop_Promotion)) ||
        !Utility::is_equal(moves[3], Move(square::d2, square::d1, Move_flag::Rook_Promotion)) ||
        !Utility::is_equal(moves[4], Move(square::d2, square::c1, Move_flag::Queen_Promotion_Capture)) ||
        !Utility::is_equal(moves[5], Move(square::d2, square::c1, Move_flag::Knight_Promotion_Capture)) ||
        !Utility::is_equal(moves[6], Move(square::d2, square::c1, Move_flag::Bishop_Promotion_Capture)) ||
        !Utility::is_equal(moves[7], Move(square::d2, square::c1, Move_flag::Rook_Promotion_Capture)) ||
        !Utility::is_equal(moves[8], Move(square::d2, square::e1, Move_flag::Queen_Promotion_Capture)) ||
        !Utility::is_equal(moves[9], Move(square::d2, square::e1, Move_flag::Knight_Promotion_Capture)) ||
        !Utility::is_equal(moves[10], Move(square::d2, square::e1, Move_flag::Bishop_Promotion_Capture)) ||
        !Utility::is_equal(moves[11], Move(square::d2, square::e1, Move_flag::Rook_Promotion_Capture)))
        return false;
    moves.clear();

    mg.generate_pawn_moves(square::e5, -1, battpin.second, moves);
    // Utility::print_moves(moves);
    if (moves.size() != 2 ||
        !Utility::is_equal(moves[0], Move(square::e5, square::d4, Move_flag::Capture)) ||
        !Utility::is_equal(moves[1], Move(square::e5, square::f4, Move_flag::Capture)))
        return false;
    moves.clear();
    return true;
}

bool Test::test46()
{
    // test: quite move 
    Board b;
    std::string fen1{ "8/3k4/2r5/8/7B/8/1K4P1/8 w - - 0 1" };
    std::string fen2{ "8/3k4/2r5/8/8/8/1K3BP1/8 b - - 1 1" };
    Fen f(fen1);
    Utility::fen_to_board(f, b);
    Move m(square::h4, square::f2, Move_flag::Quite);
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

bool Test::test47()
{
    // test: double pawn push move 
    Board b;
    std::string fen1{ "8/3k4/2r5/8/7B/8/1K4P1/8 w - - 0 1" };
    std::string fen2{ "8/3k4/2r5/8/6PB/8/1K6/8 b - g3 1 1" };
    Fen f(fen1);
    Utility::fen_to_board(f, b);
    Move m(square::g2, square::g4, Move_flag::Double_Pawn_Push);
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

bool Test::test48()
{
    // test: capture move 
    Board b;
    std::string fen1{ "8/3k3q/2r5/8/6PB/8/1K2Q3/8 b - - 0 1" };
    std::string fen2{ "8/3k4/2r5/8/6Pq/8/1K2Q3/8 w - - 1 2" };
    Fen f(fen1);
    Utility::fen_to_board(f, b);
    Move m(square::h7, square::h4, Move_flag::Capture);
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

bool Test::test49()
{
    // test: en passant capture move - white 
    Board b;
    std::string fen1{ "4k3/8/8/2PpP3/6p1/8/5P1P/4K3 w - d6 0 1" };
    std::string fen2{ "4k3/8/3P4/4P3/6p1/8/5P1P/4K3 b - - 1 1" };
    Fen f(fen1);
    Utility::fen_to_board(f, b);
    Move m(square::c5, square::d6, Move_flag::En_Passant_Capture);
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

bool Test::test50()
{
    // test: en passant capture move - white 
    Board b;
    std::string fen1{ "4k3/8/8/2PpP3/6p1/8/5P1P/4K3 w - d6 0 1" };
    std::string fen2{ "4k3/8/3P4/2P5/6p1/8/5P1P/4K3 b - - 1 1" };
    Fen f(fen1);
    Utility::fen_to_board(f, b);
    Move m(square::e5, square::d6, Move_flag::En_Passant_Capture);
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

bool Test::test51()
{
    // test: en passant capture move - black 
    Board b;
    std::string fen1{ "4k3/8/8/2PpP3/6pP/8/5P2/4K3 b - h3 0 1" };
    std::string fen2{ "4k3/8/8/2PpP3/8/7p/5P2/4K3 w - - 1 2" };
    Fen f(fen1);
    Utility::fen_to_board(f, b);
    Move m(square::g4, square::h3, Move_flag::En_Passant_Capture);
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

bool Test::test52()
{
    // test: en passant capture move - black 
    Board b;
    std::string fen1{ "4k3/8/8/2PpP3/5PpP/8/8/4K3 b - f3 0 1" };
    std::string fen2{ "4k3/8/8/2PpP3/7P/5p2/8/4K3 w - - 1 2" };
    Fen f(fen1);
    Utility::fen_to_board(f, b);
    Move m(square::g4, square::f3, Move_flag::En_Passant_Capture);
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

bool Test::test53()
{
    // test: king side castle white and black
    Board b;
    std::string fen1{ "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1" }; 
    std::string fen2{ "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R4RK1 b kq - 1 1" };
    std::string fen3{ "r4rk1/pppppppp/8/8/8/8/PPPPPPPP/R4RK1 w - - 2 2" };

    Fen f(fen1);
    Utility::fen_to_board(f, b);

    // white king side castle
    Move m1(square::e1, square::g1, Move_flag::King_Side_Castle);
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
    Move m2(square::e8, square::g8, Move_flag::King_Side_Castle);
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

/*
bool Test::test46() 
{
    Engine e("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    auto res = e.perft(0);
    if (res != 1) 
    {
        std::cout << "Perft error! expected: 1, got: " << res << std::endl;
    }
    return true;
}

bool Test::test47() 
{
    Engine e("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    auto res = e.perft(1);
    if (res != 20)
    {
        std::cout << "Perft error! expected: 20, got: " << res << std::endl;
    }
    return true;
}

bool Test::test48()
{
    Engine e("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    auto res = e.perft(2);
    if (res != 400)
    {
        std::cout << "Perft error! expected: 400, got: " << res << std::endl;
    }
    return true;
}

bool Test::test49() 
{
    Engine e("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    auto res = e.perft(3);
    if (res != 8902)
    {
        std::cout << "Perft error! expected: 8902, got: " << res << std::endl;
    }
    return true;
}

bool Test::test50() 
{
    
    Engine e("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    // auto res = e.perft(4);
    auto res = e.perft_divide(4);
    if (res != 197281)
    {
        std::cout << "Perft error! expected: 197281, got: " << res << std::endl;
        return false;
    }
    return true;
    
    /*
    Engine e("rnbqkbnr/pppppppp/8/8/8/5P2/PPPPP1PP/RNBQKBNR b KQkq - 0 1");
    // auto res = e.perft(4);
    auto res = e.perft_divide(3);
    if (res != 8457)
    {
        std::cout << "Perft error! expected: 8457, got: " << res << std::endl;
        return false;
    }
    return true;
    
}

bool Test::test51()
{
    
    Engine e("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    auto res = e.perft(5);
    if (res != 4865609)
    {
        std::cout << "Perft error! expected: 4865609, got: " << res << std::endl;
    }
    
    return true;
}
*/


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
    std::vector<bool (*)()> tests { test30, test31, test32, test33, test34, test35, test36, test37, test38, test39, test40, test41,
     test42, test43, test44 , test45};
    run_test("Move Generator", tests);
}

void Test::run_make_move_tests()
{
    std::vector<bool (*)()> tests{ test46, test47, test48, test49, test50, test51, test52, test53};
    run_test("Make Move Tests", tests);
}

void Test::run_perft_tests()
{
    std::vector<bool (*)()> tests{  };
    run_test("Perft Tests", tests);
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
        std::cout << "Attacker loc: " << square::square_to_string(it->attacker_loc)
                  << " Attack dir: " << direction::direction_to_chararray.find(it->attack_dir)->second
                  << std::endl;

    std::cout << "-----------------------------------------\n";

    for(auto it = ap.second.begin(); it != ap.second.end(); it++)
        std::cout << "Pinner loc: " << square::square_to_string(it->pinner_loc)
                  << " Pinned loc: " << square::square_to_string(it->pinned_loc)
                  << " Pin dir: " << direction::direction_to_chararray.find(it->pin_dir)->second
                  << std::endl;
}

bool Test::check_piece_locations(Board const& b, int8_t piece, std::vector<int8_t> expected_loc)
{
    auto r = b.piece_loc.equal_range(piece);
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
