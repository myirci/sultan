#include "fen_tests.hpp"
#include "../fen.hpp"

#include <sstream>

namespace test 
{
    void FenTests::run_test()
    {
        std::vector<bool (*)()> tests { test1, test2 };
        TestBase::run_test("Fen", tests);
    }

    bool FenTests::test1()
    {
        Fen f;
        return f.active_color == "w" && f.castling_availability == "KQkq" && f.en_passant == "-" && f.full_move_number == 1 && 
            f.half_move_clock == 0 && f.piece_placement == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    }

    bool FenTests::test2()
    {
        Fen f("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b KQkq c6 1 2");
        return f.active_color == "b" && f.castling_availability == "KQkq" && f.en_passant == "c6" && f.full_move_number == 2 &&
            f.half_move_clock == 1 && f.piece_placement == "6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1";
    }
}