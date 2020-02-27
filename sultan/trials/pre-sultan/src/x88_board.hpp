#ifndef X88_BOARD_HPP
#define X88_BOARD_HPP

#include "pieces.hpp"
#include "move.hpp"
#include "piece_traits.hpp"
#include <iostream>
#include <vector>
#include <cstddef>

enum class State : std::int8_t {
    active_color    = 1,
    wks_castling    = 2,
    wqs_castling    = 4,
    bks_castling    = 8,
    bqs_castling    = 16,
    fifty_rule_draw = 32
};

enum class Castling_type : std::int8_t {
    king_side = 1,
    queen_side = 2
};

class X88_board {
private:
    // iterator definiton
    template <typename T> class iterator_base {
    protected:
        T* current;
        T* begin;
        iterator_base(T* init) : begin(init), current(init) { }
        iterator_base(T* b, T* c) : begin(b), current(c) { }
    public:
        T& operator*() { return *(this->current); }
    };
    template <typename T> class piterator : public iterator_base<T> {
    public:
        piterator(T* init = nullptr) : iterator_base<T>(init) { }
        piterator(T* b, T* c) : iterator_base<T>(b, c) { }
        piterator& operator++() {
            if(this->current != nullptr) {
                std::ptrdiff_t diff = this->current - this->begin;
                if(diff == 7) { this->current = this->begin + 16; }
                else if(diff == 23) { this->current = nullptr; }
                else { ++(this->current); }
            }
            return *this;
        }
        piterator operator++(int) {
            piterator temp = *this;
            ++(*this);
            return temp;
        }
        bool operator==(const piterator& x) const { return (this->current == x.current); }
        bool operator!=(const piterator& x) const { return (this->current != x.current); }
    };
    template <typename T> class biterator : public iterator_base<T> {
    public:
        biterator(T* init = nullptr) : iterator_base<T>(init) { }
        biterator(T* b, T* c) : iterator_base<T>(b, c) { }
        biterator& operator++() {
            if(this->current != nullptr) {
                std::ptrdiff_t diff = this->current - this->begin;
                if(diff == 119) { this->current = nullptr; }
                else if(((diff + 1) & 0x88) == 0) { ++(this->current); }
                else { this->current += 9;}
            }
            return *this;
        }
        biterator operator++(int) {
            biterator temp = *this;
            ++(*this);
            return temp;
        }
        bool operator==(const biterator& x) const { return (this->current == x.current); }
        bool operator!=(const biterator& x) const { return (this->current != x.current); }
        void set_to_row_beginning(int row) {
            if(row < 8) { this->current = this->begin + 16 * row; }
        }
    };
public:
    // iterator typedefs
    typedef piterator<std::int8_t>          piece_iterator;
    typedef piterator<const std::int8_t>    const_piece_iterator;
    typedef biterator<std::int8_t>          board_iterator;
    typedef biterator<const std::int8_t>    const_board_iterator;

    // iterator methods:
    piece_iterator white_pieces_begin()             { return piece_iterator(&board[white_pieces_start]); }
    const_piece_iterator white_pieces_begin() const { return const_piece_iterator(&board[white_pieces_start]); }
    piece_iterator white_pieces_end()               { return piece_iterator(&board[white_pieces_start], nullptr); }
    const_piece_iterator white_pieces_end() const   { return const_piece_iterator(&board[white_pieces_start], nullptr); }
    piece_iterator black_pieces_begin()             { return piece_iterator(&board[black_pieces_start]); }
    const_piece_iterator black_pieces_begin() const { return const_piece_iterator(&board[black_pieces_start]); }
    piece_iterator black_pieces_end()               { return piece_iterator(&board[black_pieces_start], nullptr); }
    const_piece_iterator black_pieces_end() const   { return const_piece_iterator(&board[black_pieces_start], nullptr); }
    board_iterator board_begin()                    { return board_iterator(&board[board_start]); }
    const_board_iterator board_begin() const        { return const_board_iterator(&board[board_start]); }
    board_iterator board_end()                      { return board_iterator(&board[board_start], nullptr); }
    const_board_iterator board_end() const          { return const_board_iterator(&board[board_start], nullptr); }

    // static members
    const static std::int8_t dummy;
    const static std::int8_t en_passant_loc;
    const static std::int8_t state1;
    const static std::int8_t hmove_num_upper;
    const static std::int8_t hmove_num_lower;
    const static std::int8_t move_num_upper;
    const static std::int8_t move_num_lower;
    const static std::int8_t white_pieces_start;
    const static std::int8_t black_pieces_start;
    const static std::int8_t board_start;

    // constructor and initialization functions
    X88_board();
    void initialize_to_starting_position();

    // getters & setters for data members
    std::int8_t const * const get_board() const;
    std::int8_t * const get_board();
    void set_square(std::int8_t loc, Pieces p);
    bool get_state_property(State s) const;
    void set_state_property(State s, bool b);
    std::int16_t get_move_num() const;
    void set_move_num(std::int16_t mn);
    void increment_move_num();
    std::int16_t get_half_move_num() const;
    void set_half_move_num(std::int16_t hmn);
    void increment_half_move_num();
    std::int8_t get_en_passant_loc() const;
    void set_en_passant_loc(std::int8_t ep_loc);

    // related to move
    void generate_moves(std::vector<Move>& moves);

    template <typename T> void apply_move(const Move& m) {
        std::int8_t piece_loc = m.from_loc;
        std::int8_t new_piece_loc = m.to_loc;
        std::uint8_t flags = m.flags;

        // common to all kinds of moves
        Pieces piece_type = static_cast<Pieces>(board[piece_loc]);      // piece to move
        board[piece_loc]  = static_cast<std::int8_t>(Pieces::empty);    // from location will be empty
        piece_iterator index;

        switch(flags) {
        case static_cast<std::uint8_t>(Move_flag::no_flag):
            // quite move;
            board[new_piece_loc] = static_cast<std::int8_t>(piece_type);
            break;
/*        case Move_flag::en_passant:
            board[new_piece_loc] = static_cast<std::int8_t>(piece_type);
            if(ColorTraits<T>::color == 1) { set_en_passant_loc(piece_loc + 16); }
            else { set_en_passant_loc(piece_loc - 16); }
            break;
        case Move_flag::king_side_castling:
            board[new_piece_loc] = static_cast<std::int8_t>(piece_type);
            if(ColorTraits<T>::color == 1) {
                board[new_piece_loc - 1] = static_cast<std::int8_t>(Pieces::wR);
                index = find_piece_index<T>(7);
                *index = new_piece_loc - 1;
            }
            else {
                board[new_piece_loc - 1] = static_cast<std::int8_t>(Pieces::bR);
                index = find_piece_index<T>(119);
                *index = new_piece_loc - 1;
            }
            break;
        case Move_flag::queen_side_castling:
            board[new_piece_loc] = static_cast<std::int8_t>(piece_type);
            if(ColorTraits<T>::color == 1) {
                board[new_piece_loc + 1] = static_cast<std::int8_t>(Pieces::wR);
                index = find_piece_index<T>(0);
                *index = new_piece_loc + 1;
            }
            else {
                board[new_piece_loc + 1] = static_cast<std::int8_t>(Pieces::bR);
                index = find_piece_index<T>(112);
                *index = new_piece_loc + 1;
            }
            break;
        case Move_flag::promote_to_queen:
            if(ColorTraits<T>::color == 1) {
                board[new_piece_loc] = static_cast<std::int8_t>(Pieces::wQ);
            }
            else {
                board[new_piece_loc] = static_cast<std::int8_t>(Pieces::bQ);
            }
            break;
        case Move_flag::promote_to_knight:
            if(ColorTraits<T>::color == 1) {
                board[new_piece_loc] = static_cast<std::int8_t>(Pieces::wN);
            }
            else {
                board[new_piece_loc] = static_cast<std::int8_t>(Pieces::bN);
            }
            break;
        case Move_flag::promote_to_rook:
            if(ColorTraits<T>::color == 1) {
                board[new_piece_loc] = static_cast<std::int8_t>(Pieces::wR);
            }
            else {
                board[new_piece_loc] = static_cast<std::int8_t>(Pieces::bR);
            }
            break;
        case Move_flag::promote_to_bishop:
            if(ColorTraits<T>::color == 1) {
                board[new_piece_loc] = static_cast<std::int8_t>(Pieces::wB);
            }
            else {
                board[new_piece_loc] = static_cast<std::int8_t>(Pieces::bB);
            }
            break;
        case Move_flag::capture:
            index = find_piece_index<T>(new_piece_loc);
            *index = dummy;
            board[new_piece_loc] = static_cast<std::int8_t>(piece_type);
            break;
        case Move_flag::capture_promote_to_queen:
            if(ColorTraits<T>::color == 1) {
                index = find_piece_index<T>(new_piece_loc);
                *index = dummy;
                board[new_piece_loc] = static_cast<std::int8_t>(Pieces::wQ);
            }
            else {
                index = find_piece_index<T>(new_piece_loc);
                *index = dummy;
                board[new_piece_loc] = static_cast<std::int8_t>(Pieces::bQ);
            }
            break;
        case Move_flag::capture_promote_to_knight:
            if(ColorTraits<T>::color == 1) {
                index = find_piece_index<T>(new_piece_loc);
                *index = dummy;
                board[new_piece_loc] = static_cast<std::int8_t>(Pieces::wN);
            }
            else {
                index = find_piece_index<T>(new_piece_loc);
                *index = dummy;
                board[new_piece_loc] = static_cast<std::int8_t>(Pieces::bN);
            }
            break;
        case Move_flag::capture_promote_to_rook:
            if(ColorTraits<T>::color == 1) {
                index = find_piece_index<T>(new_piece_loc);
                *index = dummy;
                board[new_piece_loc] = static_cast<std::int8_t>(Pieces::wR);
            }
            else {
                piece_iterator index = find_piece_index<T>(new_piece_loc);
                *index = dummy;
                board[new_piece_loc] = static_cast<std::int8_t>(Pieces::bR);
            }
            break;
        case Move_flag::capture_promote_to_bishop:
            if(ColorTraits<T>::color == 1) {
                index = find_piece_index<T>(new_piece_loc);
                *index = dummy;
                board[new_piece_loc] = static_cast<std::int8_t>(Pieces::wB);
            }
            else {
                index = find_piece_index<T>(new_piece_loc);
                *index = dummy;
                board[new_piece_loc] = static_cast<std::int8_t>(Pieces::bB);
            }
            break;
        default:
            std::cout << "Error in the move!" << std::endl;
            break;
        }
        */
        // update the piece location
        piece_iterator piece_index = find_piece_index<T>(piece_loc);
        *piece_index = new_piece_loc;
    }
 }
private:
    // data members:
    std::int8_t board[128];

    // non-template member functions:
    inline bool check_sliding_pieces_for_attack(std::int8_t piece_loc, std::int8_t sq_loc, std::int8_t val) const;
    inline bool check_diagonal_sliding_pieces_for_attack(std::int8_t piece_loc, std::int8_t sq_loc) const;
    inline bool check_straight_sliding_pieces_for_attack(std::int8_t piece_loc, std::int8_t sq_loc) const;

    // template memeber functions
    template <typename T> inline std::int8_t king_location() const {
        std::int8_t piece =  0;
        const_piece_iterator pit_begin, pit_end;
        if(ColorTraits<T>::color == 1) {
            piece = static_cast<std::int8_t>(Pieces::wK);
            pit_begin = this->white_pieces_begin();
            pit_end = this->white_pieces_end();
        }
        else {
            piece = static_cast<std::int8_t>(Pieces::bK);
            pit_begin = this->black_pieces_begin();
            pit_end = this->black_pieces_end();
        }

        for(const_piece_iterator pit = pit_begin; pit != pit_end; ++pit) {
            if((*pit & 0x88) == 0) {
                if(board[*pit] == piece) {
                    return *pit;
                }
            }
        }
        return dummy;
    }
    template <typename T> inline X88_board::piece_iterator find_piece_index(std::int8_t index) {
        piece_iterator pit_begin, pit_end;
        if(ColorTraits<T>::color == 1) {
            pit_begin = this->white_pieces_begin();
            pit_end = this->white_pieces_end();
        }
        else {
            pit_begin = this->black_pieces_begin();
            pit_end = this->black_pieces_end();
        }

        for(piece_iterator pit = pit_begin; pit != pit_end; ++pit) {
            if(*pit == index) { return pit; }
        }
        std::cerr << "Piece with the index: " << index << "cannot be found" << std::endl;
        return pit_end;
    }
    template <typename T> inline bool is_promote(std::int8_t pawn_loc) {
        if(ColorTraits<T>::color == 1) {
            if(pawn_loc >= 112 && pawn_loc <= 119) { return true; }
            return false;
        }
        else {
            if(pawn_loc >= 0 && pawn_loc <= 7) { return true; }
            return false;
        }
    }
    template <typename T> inline bool is_two_square_move_possibble(std::int8_t pawn_loc) {
        if(ColorTraits<T>::color == 1) {
            if(pawn_loc >= 16 &&
               pawn_loc <= 23 &&
               board[pawn_loc + 16] == static_cast<std::int8_t>(Pieces::empty) &&
               board[pawn_loc + 32] == static_cast<std::int8_t>(Pieces::empty)) { return true; }
            return false;
        }
        else {
            if(pawn_loc >= 96 &&
               pawn_loc <= 103 &&
               board[pawn_loc - 16] == static_cast<std::int8_t>(Pieces::empty) &&
               board[pawn_loc - 32] == static_cast<std::int8_t>(Pieces::empty)) { return true; }
            return false;
        }
    }
    template <typename T> void generate_moves_new(std::vector<Move>& moves) {
        piece_iterator pit_begin, pit_end;
        if(ColorTraits<T>::color == 1) {
            pit_begin = this->white_pieces_begin();
            pit_end = this->white_pieces_end();
        }
        else {
            pit_begin = this->black_pieces_begin();
            pit_end = this->black_pieces_end();
        }
        Pieces curr_piece_type, dest_piece_type;
        std::int8_t new_piece_loc;
        std::int8_t sign;
        std::int8_t king_loc = king_location<T>();
        std::int8_t piece_loc = 0;
        piece_iterator piece_index;
        for(piece_iterator pit = pit_begin; pit != pit_end; ++pit) {
            piece_loc = *pit;
            if((piece_loc & 0x88) == 0) {
                switch(static_cast<Pieces>(board[piece_loc])) {
                case Pieces::wP:
                case Pieces::bP:
                    curr_piece_type = PieceTraits<Pawn, T>::piece_type();
                    sign = (ColorTraits<T>::color == 1 ? 1 : -1);
                    // Pawn Move Type-1: One Square Forward
                    new_piece_loc = piece_loc + sign*16;
                    if((new_piece_loc & 0x88) == 0) {
                        if(board[new_piece_loc] == static_cast<std::int8_t>(Pieces::empty)) {
                            board[piece_loc] = static_cast<std::int8_t>(Pieces::empty);
                            board[new_piece_loc] = static_cast<std::int8_t>(curr_piece_type);
                            *pit = new_piece_loc;
                            if(!(is_under_attack<T>(king_loc))) {
                                if(is_promote<T>(new_piece_loc)) {
                                    moves.push_back(Move(piece_loc, new_piece_loc, (ColorTraits<T>::color == 1 ? Pieces::wQ : Pieces::bQ), Move_flag::promote));
                                    moves.push_back(Move(piece_loc, new_piece_loc, (ColorTraits<T>::color == 1 ? Pieces::wN : Pieces::bN), Move_flag::promote));
                                    moves.push_back(Move(piece_loc, new_piece_loc, (ColorTraits<T>::color == 1 ? Pieces::wR : Pieces::bR), Move_flag::promote));
                                    moves.push_back(Move(piece_loc, new_piece_loc, (ColorTraits<T>::color == 1 ? Pieces::wB : Pieces::bB), Move_flag::promote));
                                }
                                else {
                                    moves.push_back(Move(piece_loc, new_piece_loc));
                                }
                            }
                            board[piece_loc] = static_cast<std::int8_t>(curr_piece_type);
                            board[new_piece_loc] = static_cast<std::int8_t>(Pieces::empty);
                            *pit = piece_loc;
                        }
                    }
                    // Pawn Move Type-2: Capture
                    for(int i = 15; i <= 17; i+=2) {
                        new_piece_loc = piece_loc + sign*i;
                        if((new_piece_loc & 0x88) == 0) {
                            dest_piece_type = static_cast<Pieces>(board[new_piece_loc]);
                            if(is_reverse_color<T>(dest_piece_type)) {
                                board[piece_loc] = static_cast<std::int8_t>(Pieces::empty);
                                board[new_piece_loc] = static_cast<std::int8_t>(curr_piece_type);
                                *pit = new_piece_loc;
                                piece_index = find_piece_index<T>(new_piece_loc);
                                *piece_index = dummy;
                                if(!(is_under_attack<T>(king_loc))) {
                                    if(is_promote<T>(new_piece_loc)) {
                                        moves.push_back(Move(piece_loc, new_piece_loc, dest_piece_type, (ColorTraits<T>::color == 1 ? Pieces::wQ : Pieces::bQ)));
                                        moves.push_back(Move(piece_loc, new_piece_loc, dest_piece_type, (ColorTraits<T>::color == 1 ? Pieces::wN : Pieces::bN)));
                                        moves.push_back(Move(piece_loc, new_piece_loc, dest_piece_type, (ColorTraits<T>::color == 1 ? Pieces::wR : Pieces::bR)));
                                        moves.push_back(Move(piece_loc, new_piece_loc, dest_piece_type, (ColorTraits<T>::color == 1 ? Pieces::wB : Pieces::bB)));
                                    }
                                    else {
                                        moves.push_back(Move(piece_loc, new_piece_loc, Move_flag::capture));
                                    }
                                }
                                board[piece_loc] = static_cast<std::int8_t>(curr_piece_type);
                                board[new_piece_loc] = static_cast<std::int8_t>(dest_piece_type);
                                *pit = piece_loc;
                                *piece_index = new_piece_loc;
                            }
                        }
                    }
                    // Pawn Move Type-3: Two Squares Forward
                    new_piece_loc = piece_loc + sign*32;
                    if(is_two_square_move_possibble<T>(piece_loc)) {
                        board[piece_loc] = static_cast<std::int8_t>(Pieces::empty);
                        board[new_piece_loc] = static_cast<std::int8_t>(curr_piece_type);
                        *pit = new_piece_loc;
                        if(!is_under_attack<T>(king_loc)) {
                            moves.push_back(Move(piece_loc, new_piece_loc, Move_flag::en_passant));
                        }
                        board[piece_loc] = static_cast<std::int8_t>(curr_piece_type);
                        board[new_piece_loc] = static_cast<std::int8_t>(Pieces::empty);
                        *pit = piece_loc;
                    }
                    // Pawn Move Type-5: En-passant Capture
                    if((en_passant_loc & 0x88) == 0) {
                        if((piece_loc == (en_passant_loc - sign*15)) || (piece_loc == (en_passant_loc - sign*17))) {
                            board[piece_loc] = static_cast<std::int8_t>(Pieces::empty);
                            board[en_passant_loc] = static_cast<std::int8_t>(curr_piece_type);
                            *pit = en_passant_loc;
                            board[en_passant_loc - sign*16] = static_cast<std::int8_t>(Pieces::empty);
                            piece_index = find_piece_index<T>(en_passant_loc - sign*16);
                            *piece_index = dummy;
                            if(!is_under_attack<T>(king_loc)) {
                                moves.push_back(Move(piece_loc, en_passant_loc, Move_flag::en_passant_capture));
                            }
                            board[piece_loc] = static_cast<std::int8_t>(curr_piece_type);
                            board[en_passant_loc - sign*16] = static_cast<std::int8_t>(Pieces::bP);
                            *pit = piece_loc;
                            *piece_index = en_passant_loc - sign*16;
                        }
                    }
                    break;
                case Pieces::wK:
                case Pieces::bK:
                    // King Move Type-1:
                    for(auto mit = King::king_queen_moves.begin(); mit != King::king_queen_moves.end(); ++mit) {
                        new_piece_loc = piece_loc + *mit;
                        if(((new_piece_loc & 0x88) == 0) && !(is_under_attack<T>(new_piece_loc))) {
                            dest_piece_type = static_cast<Pieces>(board[new_piece_loc]);
                            if(dest_piece_type == Pieces::empty) {
                                moves.push_back(Move(piece_loc, new_piece_loc));
                            }
                            else if(is_reverse_color<T>(dest_piece_type)) {
                                moves.push_back(Move(piece_loc, new_piece_loc, dest_piece_type, Move_flag::capture));
                            }
                        }
                    }
                    // King Move Type-2: Castling
                    if(get_castling_rights<T>(Castling_type::king_side)) {
                        if(board[king_loc + 1] == static_cast<std::int8_t>(Pieces::empty) &&
                           board[king_loc + 2] == static_cast<std::int8_t>(Pieces::empty) &&
                           !(is_under_attack<T>(king_loc)) &&
                           !(is_under_attack<T>(king_loc + 1)) &&
                           !(is_under_attack<T>(king_loc + 2))) {
                           moves.push_back(Move(king_loc, king_loc + 2, Move_flag::king_side_castling));
                        }
                    }
                    if(get_castling_rights<T>(Castling_type::queen_side)) {
                        if(board[king_loc - 1] == static_cast<std::int8_t>(Pieces::empty) &&
                           board[king_loc - 2] == static_cast<std::int8_t>(Pieces::empty) &&
                           board[king_loc - 3] == static_cast<std::int8_t>(Pieces::empty) &&
                           !(is_under_attack<T>(king_loc)) &&
                           !(is_under_attack<T>(king_loc - 1)) &&
                           !(is_under_attack<T>(king_loc - 2))) {
                           moves.push_back(Move(king_loc, king_loc - 2, Move_flag::queen_side_castling));
                        }
                    }
                    break;
                case Pieces::wR:
                case Pieces::bR:
                    generate_sliding_and_jumping_piece_moves<Rook,T>(piece_loc, king_loc, pit, moves);
                    break;
                case Pieces::wB:
                case Pieces::bB:
                    generate_sliding_and_jumping_piece_moves<Bishop,T>(piece_loc, king_loc, pit, moves);
                    break;
                case Pieces::wQ:
                case Pieces::bQ:
                    generate_sliding_and_jumping_piece_moves<Queen,T>(piece_loc, king_loc, pit, moves);
                    break;
                case Pieces::wN:
                case Pieces::bN:
                    generate_sliding_and_jumping_piece_moves<Knight,T>(piece_loc, king_loc, pit, moves);
                    break;
                }
            }
        }
    }
    template <typename T> inline bool get_castling_rights(Castling_type s) {
        if(ColorTraits<T>::color == 1) {
            if(s == Castling_type::king_side) {
                return get_state_property(State::wks_castling);
            }
            else if(s == Castling_type::queen_side) {
                return get_state_property(State::wqs_castling);
            }
        }
        else {
            if(s == Castling_type::king_side) {
                return get_state_property(State::bks_castling);
            }
            else if(s == Castling_type::queen_side) {
                return get_state_property(State::bqs_castling);
            }
        }
    }
    template <typename T> inline bool is_reverse_color(Pieces p) {
        if(ColorTraits<T>::color == 1) {
            if(p == Pieces::bP || p == Pieces::bR || p == Pieces::bN || p == Pieces::bB || p == Pieces::bQ) {
                return true;
            }
            return false;
        }
        else {
            if(p == Pieces::wP || p == Pieces::wR || p == Pieces::wN || p == Pieces::wB || p == Pieces::wQ) {
                return true;
            }
            return false;
        }
    }
    template <typename T> bool is_under_attack(std::uint8_t sq_loc) const {
        const_piece_iterator pit_begin, pit_end;
        if(ColorTraits<T>::color == 1) {                    // intentionally reversed. Do not confuse later!
            pit_begin = this->black_pieces_begin();
            pit_end = this->black_pieces_end();
        }
        else {
            pit_begin = this->white_pieces_begin();
            pit_end = this->white_pieces_end();
        }
        std::int8_t piece_loc = 0;
        for(const_piece_iterator pit = pit_begin; pit != pit_end; pit++) {
            piece_loc = *pit;
            if((piece_loc & 0x88) == 0) {
                if(sq_loc == piece_loc) { continue; }
            }
            switch(static_cast<Pieces>(board[piece_loc])) {
            case Pieces::wP:
            case Pieces::bP:
                if(std::abs(sq_loc - piece_loc) == 17 || std::abs(sq_loc - piece_loc) == 15) { return true; }
                break;
            case Pieces::wN:
            case Pieces::bN:
                for(auto it = Knight::knight_moves.begin(); it != Knight::knight_moves.end(); ++it) {
                    if(sq_loc - piece_loc == *it) { return true; }
                }
                break;
            case Pieces::wR:
            case Pieces::bR:
                if(check_straight_sliding_pieces_for_attack(piece_loc, sq_loc)) { return true; }
                break;
            case Pieces::wB:
            case Pieces::bB:
                if(check_diagonal_sliding_pieces_for_attack(piece_loc, sq_loc)) { return true; }
                break;
            case Pieces::wQ:
            case Pieces::bQ:
                if(check_straight_sliding_pieces_for_attack(piece_loc, sq_loc)) { return true; }
                if(check_diagonal_sliding_pieces_for_attack(piece_loc, sq_loc)) { return true; }
                break;
            case Pieces::wK:
            case Pieces::bK:
                for(auto it = King::king_queen_moves.begin(); it != King::king_queen_moves.end(); ++it) {
                    if(sq_loc - piece_loc == *it) { return true; }
                }
                break;
            }
        }
        return false;
    }
    template <typename T1, typename T2> inline void generate_sliding_and_jumping_piece_moves(
            std::int8_t piece_loc, std::int8_t king_loc, piece_iterator& pit, std::vector<Move>& moves) {
        std::vector<std::int8_t> const & move_vec = PieceTraits<T1>::move_rights();
        Pieces curr_piece_type = PieceTraits<T1,T2>::piece_type();
        std::int8_t new_piece_loc;
        Pieces dest_piece_type;
        piece_iterator piece_index;
        for(auto mit = move_vec.begin(); mit != move_vec.end(); ++mit) {
            new_piece_loc = piece_loc + *mit;
            while((new_piece_loc & 0x88) == 0) {
                dest_piece_type = static_cast<Pieces>(board[new_piece_loc]);
                if(dest_piece_type == Pieces::empty) {
                    board[piece_loc] = static_cast<std::int8_t>(Pieces::empty);
                    board[new_piece_loc] = static_cast<std::int8_t>(curr_piece_type);
                    *pit = new_piece_loc;
                    if(!is_under_attack<T2>(king_loc)) {
                        moves.push_back(Move(piece_loc, new_piece_loc));
                    }
                   board[piece_loc] = static_cast<std::int8_t>(curr_piece_type);
                   board[new_piece_loc] = static_cast<std::int8_t>(dest_piece_type);
                   *pit = piece_loc;
                   if(curr_piece_type == Pieces::wN || curr_piece_type == Pieces::bN) { break; }
                }
                else if(is_reverse_color<T2>(dest_piece_type)) {
                    board[piece_loc] = static_cast<std::int8_t>(Pieces::empty);
                    board[new_piece_loc] = static_cast<std::int8_t>(curr_piece_type);
                    *pit = new_piece_loc;
                    piece_index = find_piece_index<T2>(new_piece_loc);
                    *piece_index = dummy;
                    if(!is_under_attack<T2>(king_loc)) {
                        moves.push_back(Move(piece_loc, new_piece_loc, dest_piece_type, Move_flag::capture));
                    }
                    board[piece_loc] = static_cast<std::int8_t>(curr_piece_type);
                    board[new_piece_loc] = static_cast<std::int8_t>(dest_piece_type);
                    *pit = piece_loc;
                    *piece_index = new_piece_loc;
                    break;
                }
                else {
                    break;
                }
                new_piece_loc += *mit;
            }
        }
    }
};
#endif
