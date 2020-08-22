#include "fen_tests.hpp"
#include "../fen.hpp"

#include <sstream>

namespace test 
{
    void FenTests::run_test()
    {
        std::vector<bool (*)()> tests{ test1, test2, test3, test4, test5, test6 };
        TestBase::run_test("Fen", tests);
    }

    bool FenTests::test1()
    {
        // tests: default constructor, get_fen()
        Fen f;
        return f.get_fen() == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    }

    bool FenTests::test2()
    {
        // tests: const string left reference constructor, get_fen()
        std::string pos{ "6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b KQkq c6 1 2" };
        Fen f(pos);
        return f.get_fen() == pos;
    }

    bool FenTests::test3()
    {
        // tests: string right reference constructor, get_fen
        std::string pos1{ "6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b KQkq c6 1 2" };
        auto pos2 = pos1;
        Fen f(std::move(pos1));
        return f.get_fen() == pos2;
    }

    bool FenTests::test4()
    {
        // tests: const string left reference constructor, set_fen
        std::string pos1{ "6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b KQkq c6 1 2" };
        std::string pos2{ "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -" };
        Fen f(pos1);
        f.set_fen(pos2);
        return f.get_fen() == pos2;
    }

    bool FenTests::test5()
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

    bool FenTests::test6()
    {
        std::string pos1{ "6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b KQkq c6 1 2" };
        Fen f1(pos1);
        Fen f2(std::move(f1));
        return f2.get_fen() == pos1;
    }
}