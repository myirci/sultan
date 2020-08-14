using System;
using System.Collections.Generic;

namespace Deneme
{
    class Program
    {
        static void Main(string[] args)
        {
            // MakeUnMakeMoveAndFenStringTest();
            // MoveGeneratorTest();
            ZobristBasicTest();
        }

        static void MakeUnMakeMoveAndFenStringTest()
        {
            var moves = new List<Move>
            {
                new Move(Square.e2, Square.e4, MoveType.DoublePawnPush),
                new Move(Square.e7, Square.e5, MoveType.DoublePawnPush),
                new Move(Square.g1, Square.f3, MoveType.Quite),
                new Move(Square.b8, Square.c6, MoveType.Quite),
                new Move(Square.d2, Square.d4, MoveType.DoublePawnPush),
                new Move(Square.e5, Square.d4, MoveType.Capture, Piece.wP),
                new Move(Square.f3, Square.d4, MoveType.Capture, Piece.bP),
                new Move(Square.c6, Square.d4, MoveType.Capture, Piece.wN),
                new Move(Square.d1, Square.d4, MoveType.Capture, Piece.bN),
                new Move(Square.d7, Square.d6, MoveType.Quite),
                new Move(Square.f1, Square.b5, MoveType.Quite),
                new Move(Square.c8, Square.d7, MoveType.Quite),
                new Move(Square.b5, Square.d7, MoveType.Capture, Piece.bB),
                new Move(Square.d8, Square.d7, MoveType.Capture, Piece.wB),
                new Move(Square.e1, Square.g1, MoveType.KingSideCastle),
                new Move(Square.e8, Square.c8, MoveType.QueenSideCastle),
            };

            var fens = new List<String>
            {
                "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1",
                "rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w KQkq e6 0 2",
                "rnbqkbnr/pppp1ppp/8/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2",
                "r1bqkbnr/pppp1ppp/2n5/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 2 3",
                "r1bqkbnr/pppp1ppp/2n5/4p3/3PP3/5N2/PPP2PPP/RNBQKB1R b KQkq d3 0 3",
                "r1bqkbnr/pppp1ppp/2n5/8/3pP3/5N2/PPP2PPP/RNBQKB1R w KQkq - 0 4",
                "r1bqkbnr/pppp1ppp/2n5/8/3NP3/8/PPP2PPP/RNBQKB1R b KQkq - 0 4",
                "r1bqkbnr/pppp1ppp/8/8/3nP3/8/PPP2PPP/RNBQKB1R w KQkq - 0 5",
                "r1bqkbnr/pppp1ppp/8/8/3QP3/8/PPP2PPP/RNB1KB1R b KQkq - 0 5",
                "r1bqkbnr/ppp2ppp/3p4/8/3QP3/8/PPP2PPP/RNB1KB1R w KQkq - 0 6",
                "r1bqkbnr/ppp2ppp/3p4/1B6/3QP3/8/PPP2PPP/RNB1K2R b KQkq - 1 6",
                "r2qkbnr/pppb1ppp/3p4/1B6/3QP3/8/PPP2PPP/RNB1K2R w KQkq - 2 7",
                "r2qkbnr/pppB1ppp/3p4/8/3QP3/8/PPP2PPP/RNB1K2R b KQkq - 0 7",
                "r3kbnr/pppq1ppp/3p4/8/3QP3/8/PPP2PPP/RNB1K2R w KQkq - 0 8",
                "r3kbnr/pppq1ppp/3p4/8/3QP3/8/PPP2PPP/RNB2RK1 b kq - 1 8",
                "2kr1bnr/pppq1ppp/3p4/8/3QP3/8/PPP2PPP/RNB2RK1 w - - 2 9"
            };
            
            Console.WriteLine();
            Console.WriteLine("-----------------------------------------------------------------");
            Console.WriteLine();

            var gm = new Game();
            gm.Print();
            var fen = gm.ToFenString();
            Console.WriteLine(fen);

            Console.WriteLine();
            Console.WriteLine("-----------------------------------------------------------------");
            Console.WriteLine();

            for (int i = 0; i < moves.Count; i++)
            {
                gm.MakeMove(moves[i]);
                fen = gm.ToFenString();
                if (fen != fens[i])
                {
                    Console.WriteLine();
                    Console.WriteLine("FEN string mismatch");
                    Console.WriteLine();
                }

                gm.Print();

                Console.WriteLine();
                Console.WriteLine("-----------------------------------------------------------------");
                Console.WriteLine();
            }

            Console.WriteLine();
            Console.WriteLine("************************************************************************");
            Console.WriteLine();

            for (int i = 0; i < moves.Count; i++)
            {
                gm.UnMakeMove();
                gm.Print();

                Console.WriteLine();
                Console.WriteLine("-----------------------------------------------------------------");
                Console.WriteLine();
            }

            Console.WriteLine();
            Console.WriteLine("************************************************************************");
            Console.WriteLine();

            gm.Load("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
            gm.Print();

            Console.WriteLine();
            Console.WriteLine("************************************************************************");
            Console.WriteLine();

            Game g = new Game("8/PPP4k/8/8/8/8/4Kppp/8 w - - 0 1");
            g.Print();

        }

        static void MoveGeneratorTest()
        {
            // var gm = new Game();
            var gm = new Game("8/6pp/8/p5p1/Pp6/1P3p2/pPK4P/krQ4R w - - 0 1");
            for(int i = 0; i < 7; i++)
                Console.WriteLine(MoveGenerator.Perft(gm, i));

            /*
            gm.MakeMove(new Move(Square.e1, Square.f1, MoveType.Quite));
            gm.MakeMove(new Move(Square.h3, Square.g2, MoveType.Capture, Piece.wP));
            gm.MakeMove(new Move(Square.f1, Square.g1, MoveType.Quite));
            var moveList = MoveGenerator.GenerateMoves(gm);
            
           
           foreach (var mv in moveList)
           {
                mv.Print();
                gm.MakeMove(mv);
                // Console.WriteLine(MoveGenerator.Perft(gm, 1));
                gm.UnMakeMove();
           }
           */
        }

        static void ZobristBasicTest()
        {
            // Utility.GenerateZobristKeysFile("../../zobrist.txt", 749);
            // Utility.ReadAndPrintZobristKeysFile("../../zobrist.txt");
            // Zobrist.Print();

            var moves = new List<Move>
            {
                new Move(Square.e2, Square.e4, MoveType.DoublePawnPush),
                new Move(Square.e7, Square.e5, MoveType.DoublePawnPush),
                new Move(Square.g1, Square.f3, MoveType.Quite),
                new Move(Square.b8, Square.c6, MoveType.Quite),
                new Move(Square.d2, Square.d4, MoveType.DoublePawnPush),
                new Move(Square.e5, Square.d4, MoveType.Capture, Piece.wP),
                new Move(Square.f3, Square.d4, MoveType.Capture, Piece.bP),
                new Move(Square.c6, Square.d4, MoveType.Capture, Piece.wN),
                new Move(Square.d1, Square.d4, MoveType.Capture, Piece.bN),
                new Move(Square.d7, Square.d6, MoveType.Quite),
                new Move(Square.f1, Square.b5, MoveType.Quite),
                new Move(Square.c8, Square.d7, MoveType.Quite),
                new Move(Square.b5, Square.d7, MoveType.Capture, Piece.bB),
                new Move(Square.d8, Square.d7, MoveType.Capture, Piece.wB),
                new Move(Square.e1, Square.g1, MoveType.KingSideCastle),
                new Move(Square.e8, Square.c8, MoveType.QueenSideCastle),
            };

            var gm = new Game();
            Console.WriteLine(Zobrist.HashValue(gm));

            foreach (var mv in moves)
            {
                gm.MakeMove(mv);
                Console.WriteLine(Zobrist.HashValue(gm));
            }


            Console.WriteLine();
            Console.WriteLine("************************************************************************");
            Console.WriteLine();

            for (int i = 0; i < moves.Count; i++)
            {
                gm.UnMakeMove();
                Console.WriteLine(Zobrist.HashValue(gm));
            }
        }
    }
}
