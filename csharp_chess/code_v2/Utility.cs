using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;


namespace Deneme
{
    public static class Utility
    {
        public static Dictionary<Piece, Char> PieceToChar;
        public static Dictionary<Char, Piece> CharToPiece;
        public static Dictionary<Square, string> SquareToString;
        public static Dictionary<string, Square> StringToSquare;

        static Utility()
        {
            PieceToChar = new Dictionary<Piece, char>
            {
                { Piece.eP, '-'},
                { Piece.wP, 'P'}, { Piece.wR, 'R'}, { Piece.wN, 'N'}, { Piece.wB, 'B'}, { Piece.wQ, 'Q'}, { Piece.wK, 'K'},
                { Piece.bP, 'p'}, { Piece.bR, 'r'}, { Piece.bN, 'n'}, { Piece.bB, 'b'}, { Piece.bQ, 'q'}, { Piece.bK, 'k'}
            };

            CharToPiece = PieceToChar.ToDictionary(p => p.Value, p => p.Key);

            SquareToString = new Dictionary<Square, string>
            {
                { Square.a1, "a1" }, { Square.a2, "a2" }, { Square.a3, "a3" }, { Square.a4, "a4" }, { Square.a5, "a5" }, { Square.a6, "a6" }, { Square.a7, "a7" }, { Square.a8, "a8" },
                { Square.b1, "b1" }, { Square.b2, "b2" }, { Square.b3, "b3" }, { Square.b4, "b4" }, { Square.b5, "b5" }, { Square.b6, "b6" }, { Square.b7, "b7" }, { Square.b8, "b8" },
                { Square.c1, "c1" }, { Square.c2, "c2" }, { Square.c3, "c3" }, { Square.c4, "c4" }, { Square.c5, "c5" }, { Square.c6, "c6" }, { Square.c7, "c7" }, { Square.c8, "c8" },
                { Square.d1, "d1" }, { Square.d2, "d2" }, { Square.d3, "d3" }, { Square.d4, "d4" }, { Square.d5, "d5" }, { Square.d6, "d6" }, { Square.d7, "d7" }, { Square.d8, "d8" },
                { Square.e1, "e1" }, { Square.e2, "e2" }, { Square.e3, "e3" }, { Square.e4, "e4" }, { Square.e5, "e5" }, { Square.e6, "e6" }, { Square.e7, "e7" }, { Square.e8, "e8" },
                { Square.f1, "f1" }, { Square.f2, "f2" }, { Square.f3, "f3" }, { Square.f4, "f4" }, { Square.f5, "f5" }, { Square.f6, "f6" }, { Square.f7, "f7" }, { Square.f8, "f8" },
                { Square.g1, "g1" }, { Square.g2, "g2" }, { Square.g3, "g3" }, { Square.g4, "g4" }, { Square.g5, "g5" }, { Square.g6, "g6" }, { Square.g7, "g7" }, { Square.g8, "g8" },
                { Square.h1, "h1" }, { Square.h2, "h2" }, { Square.h3, "h3" }, { Square.h4, "h4" }, { Square.h5, "h5" }, { Square.h6, "h6" }, { Square.h7, "h7" }, { Square.h8, "h8" },
                { Square.iv, "-" }
            };

            StringToSquare = SquareToString.ToDictionary(p => p.Value, p => p.Key);

        }

        public static int Rank(Square sq) => (int)sq / 16;

        public static int File(Square sq) => (int)sq & 7;

        public static void Swap(ref int x, ref int y)
        {
            int temp = x;
            x = y;
            y = temp;
        }

        public static long MakeARandomLong(Random rnd)
        {
            var bArr = new byte[8];
            rnd.NextBytes(bArr);
            long l = 0;
            for (int i = 0, j = 0; i < 8; i++, j += 8)
                l |= ((long)bArr[i] << j);
            return l;
        }

        public static void GenerateZobristKeysFile(string fPath, int size)
        {
            var fileStream = new FileStream(fPath, FileMode.Create, FileAccess.Write);
            var streamWriter = new StreamWriter(fileStream, Encoding.ASCII);
            var rnd = new Random();
            var longArr = new long[size];
            for (int i = 0; i < size; i++)
            {
                longArr[i] = MakeARandomLong(rnd);
                streamWriter.WriteLine(longArr[i]);
            }

            streamWriter.Flush();
            fileStream.Close();
        }

        public static void ReadAndPrintZobristKeysFile(string fPath)
        {
            var fileStream = new FileStream(fPath, FileMode.Open, FileAccess.Read);
            var streamReader = new StreamReader(fileStream, Encoding.ASCII);
            string line;
            while ((line = streamReader.ReadLine()) != null)
            {
                long x = Convert.ToInt64(line);
                Console.WriteLine(x);
            }
            fileStream.Close();
        }

        /*
        public static void LoadGamesFromPGNFile(string fPath, List<Game> games)
        {
            var fileStream = new FileStream(fPath, FileMode.Open, FileAccess.Read);
            var streamReader = new StreamReader(fileStream, Encoding.UTF8);
            string line;
            while ((line = streamReader.ReadLine()) != null)
                Console.WriteLine(line);
        }
        */
    }
}
