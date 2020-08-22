#include "piece_tests.hpp"
#include "../piece.hpp"
#include "../definitions.hpp"

namespace test
{
    void PieceTests::run_test()
    {
        std::vector<bool (*)()> tests{ test1 };
        TestBase::run_test("Fen", tests);
    }

    bool PieceTests::test1()
    {
        return
            piece::eM == 0 &&
            def::none == -128 &&
            piece::wP == -piece::bP &&
            piece::wR == -piece::bR &&
            piece::wN == -piece::bN &&
            piece::wB == -piece::bB &&
            piece::wQ == -piece::bQ &&
            piece::wK == -piece::bK;
    }
}