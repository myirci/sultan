#include "attack_detector.hpp"
#include "board.hpp"
#include "piece_location.hpp"
#include "piece.hpp"
#include "square.hpp"
#include "definitions.hpp"

#include <stdexcept>

AttackDetector::AttackDetector(Board const& b, PieceLocation const& pl) : board{ b }, ploc { pl } { }

AttackDetector::AttackDetector(Board const& b, PieceLocation const& pl, std::unordered_multimap<int8_t, std::pair<int8_t, int8_t>> const& cap)
    : board{ b }, ploc{ pl }, checks_and_pins{ cap } { }

void AttackDetector::compute_checks_and_pins()
{
    checks_and_pins.clear();
    compute_checks_and_pins(color::white);
    compute_checks_and_pins(color::black);
}

int8_t AttackDetector::num_checkers(int8_t king_pos) const
{
    auto r = checks_and_pins.equal_range(king_pos);
    return static_cast<int8_t>(std::distance(r.first, r.second));
}

std::pair<int8_t, int8_t> AttackDetector::get_checker_positions(int8_t king_pos) const
{
    auto r = checks_and_pins.equal_range(king_pos);
    auto ncheckers = std::distance(r.first, r.second);
    if(ncheckers == 0)
        return std::make_pair(def::none, def::none);
    else if(ncheckers == 1)
        return std::make_pair(r.first->first, def::none);
    return std::make_pair(r.first->first, r.second->first);
}

std::pair<int8_t, int8_t> AttackDetector::get_attack_info(int8_t pos) const
{
    auto it = checks_and_pins.find(pos);
    if (it != checks_and_pins.end())
        return it->second;
    throw std::logic_error("Attacker could not be found!");
}

int8_t AttackDetector::get_pin_direction(int8_t sq) const
{
    auto it = checks_and_pins.find(sq);
    return it == checks_and_pins.end() ? direction::ND : it->second.second;
}

void AttackDetector::compute_checks_and_pins(int8_t attacking_side)
{
    auto brd = board.get_board();
    int8_t opponent_king_pos = ploc.find_piece_location(-attacking_side * piece::King);

    // compute attacks to the opponent_king_position from the ranks and files
    for (int i{ 0 }; i < 4; i++)
    {
        int8_t next{ opponent_king_pos }, target_sq{ opponent_king_pos };
        while (!((next = next + direction::flat_dirs[i]) & square::inside))
        {
            if (brd[next] == piece::eM) continue;

            if (piece::is_same_sign(attacking_side, brd[next]))
            {
                int8_t p = attacking_side * brd[next];
                if (p == piece::Rook || p == piece::Queen)
                    checks_and_pins.emplace(std::make_pair(target_sq, std::make_pair(next, direction::flat_dirs[i])));
                break;
            }
            else
            {
                if (target_sq == opponent_king_pos) target_sq = next;
                else break;
            }
        }
    }

    // compute attacks to the opponent_king_position from the diagonals
    for (int i{ 0 }; i < 4; i++)
    {
        int8_t next{ opponent_king_pos }, target_sq{ opponent_king_pos };
        while (!((next = next + direction::diagonal_dirs[i]) & square::inside))
        {
            if (brd[next] == piece::eM) continue;

            if (piece::is_same_sign(attacking_side, brd[next]))
            {
                int8_t p = attacking_side * brd[next];
                if (p == piece::Bishop || p == piece::Queen ||
                    (p == piece::Pawn && (next - target_sq) == direction::diagonal_dirs[i] && direction::diagonal_dirs[i] * attacking_side < 0) && target_sq == opponent_king_pos)
                    checks_and_pins.emplace(std::make_pair(target_sq, std::make_pair(next, direction::diagonal_dirs[i])));
                break;
            }
            else
            {
                if (target_sq == opponent_king_pos) target_sq = next;
                else break;
            }
        }
    }

    // compute attacks to the opponent king position by the knights
    for (int i{ 0 }; i < 8; i++)
    {
        int8_t next = opponent_king_pos + direction::knight_jumps[i];
        if (!(next & square::inside) && attacking_side * brd[next] == piece::Knight)
            checks_and_pins.emplace(std::make_pair(opponent_king_pos, std::make_pair(next, direction::ND)));
    }
}

// behaves as if brd[exclude_pos1] and brd[exclude_pos2] are empty if they are different than def::none
bool AttackDetector::is_under_attack(int8_t attacking_side, int8_t target_sq, int8_t exclude_pos1, int8_t exclude_pos2) const
{
    auto brd = board.get_board();

    // check attacks from the ranks and files
    for (int i{ 0 }; i < 4; i++)
    {
        int8_t next{ target_sq };
        while (!((next = next + direction::flat_dirs[i]) & square::inside))
        {
            if (brd[next] == piece::eM || next == exclude_pos1 || next == exclude_pos2) continue;

            if (piece::is_same_sign(attacking_side, brd[next]))
            {
                int8_t p = attacking_side * brd[next];
                if (p == piece::Rook || p == piece::Queen || (p == piece::King && (next - target_sq) == direction::flat_dirs[i]))
                    return true;
            }
            break;
        }
    }

    // check attacks from the diagonals
    for (int i{ 0 }; i < 4; i++)
    {
        int8_t next{ target_sq };
        while (!((next = next + direction::diagonal_dirs[i]) & square::inside))
        {
            if (brd[next] == piece::eM || next == exclude_pos1 || next == exclude_pos2) continue;

            if (piece::is_same_sign(attacking_side, brd[next]))
            {
                int8_t p = attacking_side * brd[next];
                if (p == piece::Bishop || p == piece::Queen ||
                    (p == piece::Pawn && (next - target_sq) == direction::diagonal_dirs[i] && direction::diagonal_dirs[i] * attacking_side < 0) ||
                    (p == piece::King && (next - target_sq) == direction::diagonal_dirs[i]))
                    return true;
            }
            break;
        }
    }

    for (int i{ 0 }; i < 8; i++)
    {
        int8_t next = target_sq + direction::knight_jumps[i];
        if (!(next & square::inside) && attacking_side * brd[next] == piece::Knight)
            return true;
    }

    return false;
}

