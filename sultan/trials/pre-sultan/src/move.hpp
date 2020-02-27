#ifndef MOVE_HPP
#define MOVE_HPP

#include <iostream>
#include <cstdint>
#include <string>
#include "pieces.hpp"

std::string flag_to_string(std::uint8_t flags);

enum class Move_flag : std::uint8_t {
    no_flag             = 0x00,
    capture             = 0x80, // test by bit location
    promote             = 0x40, // test by bit location
    en_passant          = 0x20,
    en_passant_capture  = 0xA0,
    king_side_castling  = 0x10,
    queen_side_castling = 0x11,
    base_mask1          = 0xF8, // base mask for only capture and only promote
    base_mask2          = 0xF0, // base mask when both capture and promote occur at the same time
    base_mask1_inverse  = 0x07,
    base_mask2_inv_p1   = 0x03,
    base_mask2_inv_p2   = 0x0C,
    knight_mask         = 0x01,
    bishop_mask         = 0x02,
    queen_mask          = 0x03,
    pawn_mask           = 0x04
};

struct Move {
    std::int8_t from_loc;
    std::int8_t to_loc;
    std::uint8_t flags;

    // constructor for no-flag: quite move
    Move(std::int8_t fr, std::int8_t to) : from_loc(fr), to_loc(to), flags(0) { }
    // constructor for capture-only and promote-only:
    Move(std::int8_t fr, std::int8_t to, Pieces piece, Move_flag f) : from_loc(fr), to_loc(to), flags(0) {
        flags |= static_cast<std::uint8_t>(f);
        flags &= static_cast<std::uint8_t>(Move_flag::base_mask1);
        switch(piece) {
        case Pieces::wP:
        case Pieces::bP:
            flags |= static_cast<std::uint8_t>(Move_flag::pawn_mask);
            break;
        case Pieces::wN:
        case Pieces::bN:
            flags |= static_cast<std::uint8_t>(Move_flag::knight_mask);
            break;
        case Pieces::wB:
        case Pieces::bB:
            flags |= static_cast<std::uint8_t>(Move_flag::bishop_mask);
            break;
        case Pieces::wR:
        case Pieces::bR:
            break;
        case Pieces::wQ:
        case Pieces::bQ:
            flags |= static_cast<std::uint8_t>(Move_flag::queen_mask);
            break;
        default:
            std::cerr << "ERROR: No valid piece match!" << std::endl;
        }
    }
    // constreuctor for capture and promote at the same time
    Move(std::int8_t fr, std::int8_t to, Pieces cap_piece, Pieces pro_piece) : from_loc(fr), to_loc(to), flags(0) {
        flags |= static_cast<std::uint8_t>(Move_flag::capture);
        flags |= static_cast<std::uint8_t>(Move_flag::promote);
        flags &= static_cast<std::uint8_t>(Move_flag::base_mask2);
        switch(cap_piece) {
        case Pieces::wR:
        case Pieces::bR:
            break;
        case Pieces::wN:
        case Pieces::bN:
            flags |= static_cast<std::uint8_t>(Move_flag::knight_mask);
            break;
        case Pieces::wB:
        case Pieces::bB:
            flags |= static_cast<std::uint8_t>(Move_flag::bishop_mask);
            break;
        case Pieces::wQ:
        case Pieces::bQ:
            flags |= static_cast<std::uint8_t>(Move_flag::queen_mask);
            break;
        default:
            std::cerr << "ERROR: No valid piece match!" << std::endl;
        }
        switch(pro_piece) {
        case Pieces::wQ:
        case Pieces::bQ:
            flags |= (static_cast<std::uint8_t>(Move_flag::queen_mask) << 2);
            break;
        case Pieces::wR:
        case Pieces::bR:
            break;
        case Pieces::wN:
        case Pieces::bN:
            flags |= (static_cast<std::uint8_t>(Move_flag::knight_mask) << 2);
            break;
        case Pieces::wB:
        case Pieces::bB:
            flags |= (static_cast<std::uint8_t>(Move_flag::bishop_mask) << 2);
            break;
        default:
            std::cerr << "ERROR: No valid piece match!" << std::endl;
        }
    }
    // constructor for castling, en-passant capture and double square pawn move (en-passant)
    Move(std::int8_t fr, std::int8_t to, Move_flag f) : from_loc(fr), to_loc(to), flags(static_cast<std::uint8_t>(f)) { }
    // print move
    void print();
};

#endif
