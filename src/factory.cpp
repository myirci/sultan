#include "factory.hpp"

#include "fen.hpp"
#include "square.hpp"
#include "piece.hpp"

std::unique_ptr<Board> Factory::CreateBoard(std::string_view fen_str)
{   
    Fen f{ fen_str };
    
    auto b = std::unique_ptr<Board>{ new Board() };
    auto pl = std::unique_ptr<PieceLocation> { new PieceLocation() };
    auto ad = std::unique_ptr<AttackDetector>{ new AttackDetector(*b, *pl) };
    b->attacks = std::move(ad);

    // piece placement
    size_t start{ 0 }, pos{ 0 };
    int idx{ 0 };
    std::string row{ "" }, pp{ f.piece_placement + "/" };
    while ((pos = pp.find('/', start)) != std::string::npos)
    {
        row = pp.substr(start, pos - start);
        start = pos + 1;
        for (char c : row)
        {
            if (std::isdigit(c))
            {
                for (int i = 0; i < static_cast<int>(c - '0'); i++)
                    b->board[square::order[idx++]] = piece::eM;
            }
            else
            {
                int8_t loc{ square::order[idx++] };
                int8_t piece = piece::char_to_piece.at(c);
                b->board[loc] = piece;
                pl->piece_loc.insert(std::make_pair(piece, loc));
            }
        }
    }

    b->ploc = std::move(pl);

    // active color
    b->side_to_move = f.active_color == "w" ? color::white : color::black;

    // castling availability
    if (f.castling_availability.find('K') != std::string::npos) b->set_castling_rights(Castling::white_king_side, true);
    if (f.castling_availability.find('Q') != std::string::npos) b->set_castling_rights(Castling::white_queen_side, true);
    if (f.castling_availability.find('k') != std::string::npos) b->set_castling_rights(Castling::black_king_side, true);
    if (f.castling_availability.find('q') != std::string::npos) b->set_castling_rights(Castling::black_queen_side, true);

    // en passant
    b->en_passant_loc = f.en_passant == "-"
        ? def::none 
        : square::sq(static_cast<int8_t>(f.en_passant[1] - '1'), static_cast<int8_t>(f.en_passant[0] - 'a'));

    // half_move_clock
    b->half_move_counter = f.half_move_clock;

    // full_move_number
    b->full_move_counter = f.full_move_number;

    // compute checks and pins
    b->attacks->compute_checks_and_pins();

    return std::move(b);
}

std::unique_ptr<Board> Factory::CloneBoard(Board const& brd) 
{
    auto b = std::unique_ptr<Board>{ new Board(brd) };
    auto pl = std::unique_ptr<PieceLocation>{ new PieceLocation(brd.ploc->piece_loc) };
    auto ad = std::unique_ptr<AttackDetector>{ new AttackDetector(*b, *pl, brd.attacks->checks_and_pins) };
    b->ploc = std::move(pl);
    b->attacks = std::move(ad);
    return std::move(b);
}

std::unique_ptr<Game> Factory::CreateGame(Board* brd)
{
    auto game = std::unique_ptr<Game>(new Game{ brd });
    return std::move(game);
}

std::unique_ptr<Perft> Factory::CreatePerft(Board* brd)
{
    auto perft = std::unique_ptr<Perft>(new Perft{ brd });
    return std::move(perft);
}

std::unique_ptr<Analyzer> Factory::CreateAnalyzer(Board* brd)
{
    auto analyzer = std::unique_ptr<Analyzer>(new Analyzer{ brd });
    return std::move(analyzer);
}