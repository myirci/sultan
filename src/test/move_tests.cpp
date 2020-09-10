#include "move_tests.hpp"
#include "../move.hpp"
#include "../piece.hpp"
#include "../definitions.hpp"

namespace test
{
    void MoveTests::run_test()
    {
        std::vector<bool (*)()> tests{ test1, test2, test3, test4, test5, test6, test7, test8, test9 };
        TestBase::run_test("Move", tests);
    }

    bool MoveTests::test1()
    {
        int8_t f{ 23 }, t{ 54 }, cap{ def::none };
        MoveType mt{ MoveType::Quite };
        Move m{ f, t, mt, cap };
        return m.get_from() == f && m.get_to() == t && m.get_move_type() == mt && m.get_captured_piece() == cap &&
            m.is_capture() == false && m.is_promotion() == false;
    }

    bool MoveTests::test2()
    {
        int8_t f{ 0 }, t{ 119 }, cap{ piece::bP };
        MoveType mt{ MoveType::Capture };
        Move m{ f, t, mt, cap };
        return m.get_from() == f && m.get_to() == t && m.get_move_type() == mt && m.get_captured_piece() == cap &&
            m.is_capture() == true && m.is_promotion() == false;
    }

    bool MoveTests::test3()
    {
        int8_t f{ 119 }, t{ 0 }, cap{ def::none };
        MoveType mt{ MoveType::King_Side_Castle };
        Move m{ f, t, mt, cap };
        return m.get_from() == f && m.get_to() == t && m.get_move_type() == mt && m.get_captured_piece() == cap &&
            m.is_capture() == false && m.is_promotion() == false;
    }

    bool MoveTests::test4()
    {
        int8_t f{ 97 }, t{ 114 }, cap{ def::none };
        MoveType mt{ MoveType::Queen_Promotion };
        Move m{ f, t, mt, cap };
        return m.get_from() == f && m.get_to() == t && m.get_move_type() == mt && m.get_captured_piece() == cap &&
            m.is_capture() == false && m.is_promotion() == true;
    }

    bool MoveTests::test5()
    {
        int8_t f{ 97 }, t{ 114 }, cap{ def::none };
        MoveType mt{ MoveType::Knight_Promotion };
        Move m{ f, t, mt, cap };
        return m.get_from() == f && m.get_to() == t && m.get_move_type() == mt && m.get_captured_piece() == cap &&
            m.is_capture() == false && m.is_promotion() == true;
    }

    bool MoveTests::test6()
    {
        int8_t f{ 97 }, t{ 114 }, cap{ def::none };
        MoveType mt{ MoveType::Rook_Promotion };
        Move m{ f, t, mt, cap };
        return m.get_from() == f && m.get_to() == t && m.get_move_type() == mt && m.get_captured_piece() == cap &&
            m.is_capture() == false && m.is_promotion() == true;
    }

    bool MoveTests::test7()
    {
        int8_t f{ 97 }, t{ 114 }, cap{ piece::wN };
        MoveType mt{ MoveType::Bishop_Promotion_Capture };
        Move m{ f, t, mt, cap };
        return m.get_from() == f && m.get_to() == t && m.get_move_type() == mt && m.get_captured_piece() == cap &&
            m.is_capture() == true && m.is_promotion() == true;
    }

    bool MoveTests::test8()
    {
        int8_t f{ 103 }, t{ 119 }, cap{ def::none };
        MoveType mt{ MoveType::Double_Pawn_Push };
        Move m{ f, t, mt, cap };
        return m.get_from() == f && m.get_to() == t && m.get_move_type() == mt && m.get_captured_piece() == cap &&
            m.is_capture() == false && m.is_promotion() == false;
    }

    bool MoveTests::test9()
    {
        int8_t f{ 103 }, t{ 119 }, cap{ piece::bP };
        MoveType mt{ MoveType::En_Passant_Capture };
        Move m{ f, t, mt, cap };
        return m.get_from() == f && m.get_to() == t && m.get_move_type() == mt && m.get_captured_piece() == cap &&
            m.is_capture() == true && m.is_promotion() == false;
    }
}