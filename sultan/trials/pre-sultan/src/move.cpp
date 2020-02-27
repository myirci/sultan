#include "move.hpp"
#include <iomanip>

std::string flag_to_string(std::uint8_t flags) {
    std::string str = "";

    if(flags == static_cast<std::uint8_t>(Move_flag::no_flag)) {
        str = "quite move";
    }
    else if(flags == static_cast<std::uint8_t>(Move_flag::en_passant_capture)) {
        str = "en-passant capture";
    }
    else if((static_cast<std::uint8_t>(Move_flag::capture) == (flags & static_cast<std::uint8_t>(Move_flag::capture))) &&
            (static_cast<std::uint8_t>(Move_flag::promote) != (flags & static_cast<std::uint8_t>(Move_flag::promote)))) {
        std::uint8_t f = static_cast<std::uint8_t>(Move_flag::base_mask1_inverse) & flags;
        if(f == static_cast<std::uint8_t>(Move_flag::pawn_mask)) {
            str = "pawn capture";
        }
        else if(f == static_cast<std::uint8_t>(Move_flag::knight_mask)) {
            str = "knight capture";
        }
        else if(f == static_cast<std::uint8_t>(Move_flag::bishop_mask)) {
            str = "bishop capture";
        }
        else if(f == static_cast<std::uint8_t>(Move_flag::queen_mask)) {
            str = "queen capture";
        }
        else if(f == 0) {
            str = "rook capture";
        }
    }
    else if(flags == static_cast<std::uint8_t>(Move_flag::en_passant)) {
        str = "double pawn move";
    }
    else if(flags == static_cast<std::uint8_t>(Move_flag::king_side_castling)) {
        str = "king side catling";
    }
    else if(flags == static_cast<std::uint8_t>(Move_flag::queen_side_castling)) {
        str = "queen side castling";
    }
    else if((static_cast<std::uint8_t>(Move_flag::capture) != (flags & static_cast<std::uint8_t>(Move_flag::capture))) &&
            (static_cast<std::uint8_t>(Move_flag::promote) == (flags & static_cast<std::uint8_t>(Move_flag::promote)))) {
        std::uint8_t f = static_cast<std::uint8_t>(Move_flag::base_mask1_inverse) & flags;
        if(f == static_cast<std::uint8_t>(Move_flag::queen_mask)) {
            str = "promote to queen";
        }
        else if(f == static_cast<std::uint8_t>(Move_flag::knight_mask)) {
            str = "promote to knight";
        }
        else if(f == static_cast<std::uint8_t>(Move_flag::bishop_mask)) {
            str = "promote to bishop";
        }
        else if(f == 0) {
            str = "promote to rook ";
        }
    }
    else if((static_cast<std::uint8_t>(Move_flag::capture) == (flags & static_cast<std::uint8_t>(Move_flag::capture))) &&
            (static_cast<std::uint8_t>(Move_flag::promote) == (flags & static_cast<std::uint8_t>(Move_flag::promote)))) {
        // determine the captured piece:
        std::uint8_t f = static_cast<std::uint8_t>(Move_flag::base_mask2_inv_p1) & flags;
        if(f == static_cast<std::uint8_t>(Move_flag::queen_mask)) {
            str = "capture queen and ";
        }
        else if(f == static_cast<std::uint8_t>(Move_flag::knight_mask)) {
            str = "capture knight and ";
        }
        else if(f == static_cast<std::uint8_t>(Move_flag::bishop_mask)) {
            str = "capture bishop and ";
        }
        else if(f == 0) {
            str = "capture rook and ";
        }
        // determine the promote:
        f = static_cast<std::uint8_t>(Move_flag::base_mask2_inv_p2) & flags;
        f = f >> 2;
        if(f == static_cast<std::uint8_t>(Move_flag::queen_mask)) {
            str += "promote to queen";
        }
        else if(f == static_cast<std::uint8_t>(Move_flag::knight_mask)) {
            str += "promote to knight";
        }
        else if(f == static_cast<std::uint8_t>(Move_flag::bishop_mask)) {
            str += "promote to bishop";
        }
        else if(f == 0) {
            str += "promote to rook";
        }
    }
    return str;
}

void Move::print() {
    std::cout << "(" << std::setw(3) << static_cast<unsigned int>(from_loc) << ","
              << std::setw(3) << static_cast<unsigned int>(to_loc) << ")\t"
              << flag_to_string(flags) << std::endl;
}

