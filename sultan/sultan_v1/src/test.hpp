#pragma once

#include <string_view>
#include <vector>

#include "move_generator.hpp"

class Test
{
public:

    Test() = delete;

    // fen string tests
    static bool test1();
    static bool test2();
    static bool test3();
    static bool test4();
    static bool test5();
    static bool test6_();

    // move tests
    static bool test6();
    static bool test7();
    static bool test8();
    static bool test9();
    static bool test10();
    static bool test11();
    static bool test12();
    static bool test13();
    static bool test14();
    static bool test15();

    // piece tests
    static bool test16();

    // board tests
    static bool test17();
    static bool test18();
    static bool test19();
    static bool test20();
    static bool test21();
    static bool test22();
    static bool test23();

    // utility tests
    static bool test24();
    static bool test25();
    static bool test26();
    static bool test27();
    static bool test28();
    static bool test29();

    // move generator tests
    static bool test30();
    static bool test31();
    static bool test32();
    static bool test33();
    static bool test34();
    static bool test35();
    static bool test36();
    static bool test37();
    static bool test38();
    static bool test39();
    static bool test40();
    static bool test41();
    static bool test42();
    static bool test43();
    static bool test44();
    static bool test45();

    // make-unmake move tests
    static bool test46();
    static bool test47();
    static bool test48();
    static bool test49();
    static bool test50();
    static bool test51();
    static bool test52();
    static bool test53();
    static bool test54();
    static bool test55();
    static bool test56();
    static bool test57();
    static bool test58();

    // perft
   
    // test runners
    static void run_fen_tests();
    static void run_move_tests();
    static void run_piece_tests();
    static void run_board_tests();
    static void run_utility_tests();
    static void run_move_generator_tests();
    static void run_make_move_tests();
    static void run_perft_tests();
    
private:
    static void run_test(std::string_view, const std::vector<bool (*)()>&);
    static void print_attack_and_pins(std::pair<std::vector<MoveGenerator::Attack>, std::vector<MoveGenerator::Pin>> const & ap);

    // helpers
    static bool check_piece_locations(Board const & b, int8_t piece, std::vector<int8_t> expected_loc);
};

