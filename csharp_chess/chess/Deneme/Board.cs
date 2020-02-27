using System;


namespace Deneme
{
    public enum Square : byte
    {
        // Real part
        a1 = 0, b1, c1, d1, e1, f1, g1, h1,
        a2 = 0x10, b2, c2, d2, e2, f2, g2, h2,
        a3 = 0x20, b3, c3, d3, e3, f3, g3, h3,
        a4 = 0x30, b4, c4, d4, e4, f4, g4, h4,
        a5 = 0x40, b5, c5, d5, e5, f5, g5, h5,
        a6 = 0x50, b6, c6, d6, e6, f6, g6, h6,
        a7 = 0x60, b7, c7, d7, e7, f7, g7, h7,
        a8 = 0x70, b8, c8, d8, e8, f8, g8, h8,
        iv // invalid square
    }

    public enum TwinSquare : byte
    {
        // Mirror part
        a1 = 8, b1, c1, d1, e1, f1, g1, h1,
        a2 = 0x18, b2, c2, d2, e2, f2, g2, h2,
        a3 = 0x28, b3, c3, d3, e3, f3, g3, h3,
        a4 = 0x38, b4, c4, d4, e4, f4, g4, h4,
        a5 = 0x48, b5, c5, d5, e5, f5, g5, h5,
        a6 = 0x58, b6, c6, d6, e6, f6, g6, h6,
        a7 = 0x68, b7, c7, d7, e7, f7, g7, h7,
        a8 = 0x78, b8, c8, d8, e8, f8, g8, h8,
        iv // invalid square
    }

    public class Board
    {
        public Piece[] BoardArray { get; set; }

        public static readonly int[] VisitOrderTB = new int[64]
        {
            (int)Square.a8, (int)Square.b8, (int)Square.c8, (int)Square.d8, (int)Square.e8, (int)Square.f8, (int)Square.g8, (int)Square.h8,
            (int)Square.a7, (int)Square.b7, (int)Square.c7, (int)Square.d7, (int)Square.e7, (int)Square.f7, (int)Square.g7, (int)Square.h7,
            (int)Square.a6, (int)Square.b6, (int)Square.c6, (int)Square.d6, (int)Square.e6, (int)Square.f6, (int)Square.g6, (int)Square.h6,
            (int)Square.a5, (int)Square.b5, (int)Square.c5, (int)Square.d5, (int)Square.e5, (int)Square.f5, (int)Square.g5, (int)Square.h5,
            (int)Square.a4, (int)Square.b4, (int)Square.c4, (int)Square.d4, (int)Square.e4, (int)Square.f4, (int)Square.g4, (int)Square.h4,
            (int)Square.a3, (int)Square.b3, (int)Square.c3, (int)Square.d3, (int)Square.e3, (int)Square.f3, (int)Square.g3, (int)Square.h3,
            (int)Square.a2, (int)Square.b2, (int)Square.c2, (int)Square.d2, (int)Square.e2, (int)Square.f2, (int)Square.g2, (int)Square.h2,
            (int)Square.a1, (int)Square.b1, (int)Square.c1, (int)Square.d1, (int)Square.e1, (int)Square.f1, (int)Square.g1, (int)Square.h1
        };
        
        public static readonly int[] VisitOrderBT = new int[64]
        {
            (int)Square.a1, (int)Square.b1, (int)Square.c1, (int)Square.d1, (int)Square.e1, (int)Square.f1, (int)Square.g1, (int)Square.h1,
            (int)Square.a2, (int)Square.b2, (int)Square.c2, (int)Square.d2, (int)Square.e2, (int)Square.f2, (int)Square.g2, (int)Square.h2,
            (int)Square.a3, (int)Square.b3, (int)Square.c3, (int)Square.d3, (int)Square.e3, (int)Square.f3, (int)Square.g3, (int)Square.h3,
            (int)Square.a4, (int)Square.b4, (int)Square.c4, (int)Square.d4, (int)Square.e4, (int)Square.f4, (int)Square.g4, (int)Square.h4,
            (int)Square.a5, (int)Square.b5, (int)Square.c5, (int)Square.d5, (int)Square.e5, (int)Square.f5, (int)Square.g5, (int)Square.h5,
            (int)Square.a6, (int)Square.b6, (int)Square.c6, (int)Square.d6, (int)Square.e6, (int)Square.f6, (int)Square.g6, (int)Square.h6,
            (int)Square.a7, (int)Square.b7, (int)Square.c7, (int)Square.d7, (int)Square.e7, (int)Square.f7, (int)Square.g7, (int)Square.h7,
            (int)Square.a8, (int)Square.b8, (int)Square.c8, (int)Square.d8, (int)Square.e8, (int)Square.f8, (int)Square.g8, (int)Square.h8
        };

        public static readonly int[] TwinVisitOrderTB = new int[64]
        {
            (int)TwinSquare.a8, (int)TwinSquare.b8, (int)TwinSquare.c8, (int)TwinSquare.d8, (int)TwinSquare.e8, (int)TwinSquare.f8, (int)TwinSquare.g8, (int)TwinSquare.h8,
            (int)TwinSquare.a7, (int)TwinSquare.b7, (int)TwinSquare.c7, (int)TwinSquare.d7, (int)TwinSquare.e7, (int)TwinSquare.f7, (int)TwinSquare.g7, (int)TwinSquare.h7,
            (int)TwinSquare.a6, (int)TwinSquare.b6, (int)TwinSquare.c6, (int)TwinSquare.d6, (int)TwinSquare.e6, (int)TwinSquare.f6, (int)TwinSquare.g6, (int)TwinSquare.h6,
            (int)TwinSquare.a5, (int)TwinSquare.b5, (int)TwinSquare.c5, (int)TwinSquare.d5, (int)TwinSquare.e5, (int)TwinSquare.f5, (int)TwinSquare.g5, (int)TwinSquare.h5,
            (int)TwinSquare.a4, (int)TwinSquare.b4, (int)TwinSquare.c4, (int)TwinSquare.d4, (int)TwinSquare.e4, (int)TwinSquare.f4, (int)TwinSquare.g4, (int)TwinSquare.h4,
            (int)TwinSquare.a3, (int)TwinSquare.b3, (int)TwinSquare.c3, (int)TwinSquare.d3, (int)TwinSquare.e3, (int)TwinSquare.f3, (int)TwinSquare.g3, (int)TwinSquare.h3,
            (int)TwinSquare.a2, (int)TwinSquare.b2, (int)TwinSquare.c2, (int)TwinSquare.d2, (int)TwinSquare.e2, (int)TwinSquare.f2, (int)TwinSquare.g2, (int)TwinSquare.h2,
            (int)TwinSquare.a1, (int)TwinSquare.b1, (int)TwinSquare.c1, (int)TwinSquare.d1, (int)TwinSquare.e1, (int)TwinSquare.f1, (int)TwinSquare.g1, (int)TwinSquare.h1
        };

        public static int[] TwinVisitOrderBT = new int[64]
        {
            (int)TwinSquare.a1, (int)TwinSquare.b1, (int)TwinSquare.c1, (int)TwinSquare.d1, (int)TwinSquare.e1, (int)TwinSquare.f1, (int)TwinSquare.g1, (int)TwinSquare.h1,
            (int)TwinSquare.a2, (int)TwinSquare.b2, (int)TwinSquare.c2, (int)TwinSquare.d2, (int)TwinSquare.e2, (int)TwinSquare.f2, (int)TwinSquare.g2, (int)TwinSquare.h2,
            (int)TwinSquare.a3, (int)TwinSquare.b3, (int)TwinSquare.c3, (int)TwinSquare.d3, (int)TwinSquare.e3, (int)TwinSquare.f3, (int)TwinSquare.g3, (int)TwinSquare.h3,
            (int)TwinSquare.a4, (int)TwinSquare.b4, (int)TwinSquare.c4, (int)TwinSquare.d4, (int)TwinSquare.e4, (int)TwinSquare.f4, (int)TwinSquare.g4, (int)TwinSquare.h4,
            (int)TwinSquare.a5, (int)TwinSquare.b5, (int)TwinSquare.c5, (int)TwinSquare.d5, (int)TwinSquare.e5, (int)TwinSquare.f5, (int)TwinSquare.g5, (int)TwinSquare.h5,
            (int)TwinSquare.a6, (int)TwinSquare.b6, (int)TwinSquare.c6, (int)TwinSquare.d6, (int)TwinSquare.e6, (int)TwinSquare.f6, (int)TwinSquare.g6, (int)TwinSquare.h6,
            (int)TwinSquare.a7, (int)TwinSquare.b7, (int)TwinSquare.c7, (int)TwinSquare.d7, (int)TwinSquare.e7, (int)TwinSquare.f7, (int)TwinSquare.g7, (int)TwinSquare.h7,
            (int)TwinSquare.a8, (int)TwinSquare.b8, (int)TwinSquare.c8, (int)TwinSquare.d8, (int)TwinSquare.e8, (int)TwinSquare.f8, (int)TwinSquare.g8, (int)TwinSquare.h8
        };

        public Board()
        {
            BoardArray = new Piece[128];
            SetToIntialPosition();
        }

        public void SetPosition(string piecePlacement)
        {
            var pieces = piecePlacement.Split(new char[] { '/' });
            int idx = 0;
            foreach (var str in pieces)
            {
                foreach (var ch in str)
                {
                    if (Char.IsDigit(ch))
                    {
                        int val = ch - '0';
                        for (int i = 0; i < val; ++i)
                            BoardArray[VisitOrderTB[idx++]] = Piece.eP;
                    }
                    else
                    {
                        BoardArray[VisitOrderTB[idx++]] = Utility.CharToPiece[ch];
                    }
                }
            }
        }

        public void Print(ConsoleColor dark = ConsoleColor.DarkBlue, ConsoleColor light = ConsoleColor.White)
        {
            Console.WriteLine();
            Console.Write("\t");

            char ch = ' ';
            int count = 0;
            int toggleVal = 0;
            for (int i = 0; i < 64; i++)
            {
                if (i % 2 == toggleVal)
                {
                    Console.BackgroundColor = light;
                    Console.ForegroundColor = dark;
                }
                else
                {
                    Console.BackgroundColor = dark;
                    Console.ForegroundColor = light;
                }

                ch = Utility.PieceToChar[BoardArray[VisitOrderTB[i]]];
                Console.Write(" {0} ", ch);

                if (++count % 8 == 0)
                {
                    Console.ResetColor();
                    Console.WriteLine();
                    Console.Write("\t");
                    toggleVal = (toggleVal == 0) ? 1 : 0;
                }
            }
            Console.ResetColor();
            Console.WriteLine();
        }

        private void SetToIntialPosition()
        {
            for (int i = 0; i < 128; i++)
                BoardArray[i] = Piece.eP;

            BoardArray[(byte)Square.a1] = BoardArray[(byte)Square.h1] = Piece.wR;
            BoardArray[(byte)Square.b1] = BoardArray[(byte)Square.g1] = Piece.wN;
            BoardArray[(byte)Square.c1] = BoardArray[(byte)Square.f1] = Piece.wB;
            BoardArray[(byte)Square.d1] = Piece.wQ;
            BoardArray[(byte)Square.e1] = Piece.wK;
            BoardArray[(byte)Square.a2] = BoardArray[(byte)Square.b2] =
            BoardArray[(byte)Square.c2] = BoardArray[(byte)Square.d2] =
            BoardArray[(byte)Square.e2] = BoardArray[(byte)Square.f2] =
            BoardArray[(byte)Square.g2] = BoardArray[(byte)Square.h2] = Piece.wP;

            BoardArray[(byte)Square.a8] = BoardArray[(byte)Square.h8] = Piece.bR;
            BoardArray[(byte)Square.b8] = BoardArray[(byte)Square.g8] = Piece.bN;
            BoardArray[(byte)Square.c8] = BoardArray[(byte)Square.f8] = Piece.bB;
            BoardArray[(byte)Square.d8] = Piece.bQ;
            BoardArray[(byte)Square.e8] = Piece.bK;
            BoardArray[(byte)Square.a7] = BoardArray[(byte)Square.b7] =
            BoardArray[(byte)Square.c7] = BoardArray[(byte)Square.d7] =
            BoardArray[(byte)Square.e7] = BoardArray[(byte)Square.f7] =
            BoardArray[(byte)Square.g7] = BoardArray[(byte)Square.h7] = Piece.bP;
        }
    }
}
