using System;
using System.IO;
using System.Text;
using System.Collections.Generic;

namespace Deneme
{
    public static class Zobrist
    {
        public static long[][] PieceKeys { get; private set; }
        public static long[] EnPassantKeys { get; private set; }
        public static long WhiteKSCastle { get; private set; }
        public static long WhiteQSCastle { get; private set; }
        public static long BlackKSCastle { get; private set; }
        public static long BlackQSCastle { get; private set; }
        public static long BlackToMove { get; private set; }

        public static readonly int[] VisitOrder = new int[128]
       {
            0,   1,  2,  3,  4,  5,  6,  7, -1, -1, -1, -1, -1, -1, -1, -1,
            8,   9, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1,
            16, 17, 18, 19, 20, 21, 22, 23, -1, -1, -1, -1, -1, -1, -1, -1,
            24, 25, 26, 27, 28, 29, 30, 31, -1, -1, -1, -1, -1, -1, -1, -1,
            32, 33, 34, 35, 36, 37, 38, 39, -1, -1, -1, -1, -1, -1, -1, -1,
            40, 41, 42, 43, 44, 45, 46, 47, -1, -1, -1, -1, -1, -1, -1, -1,
            48, 49, 50, 51, 52, 53, 54, 55, -1, -1, -1, -1, -1, -1, -1, -1,
            56, 57, 58, 59, 60, 61, 62, 63, -1, -1, -1, -1, -1, -1, -1, -1,
       };

        static Zobrist()
        {
            PieceKeys = new long[13][];
            for(int i = 0; i < 6; i++)
                PieceKeys[i] = new long[64];
            for (int i = 7; i < 13; i++)
                PieceKeys[i] = new long[64];
            EnPassantKeys = new long[8];
            LoadKeys("../../zobrist_keys_1.txt");
        }

        private static void LoadKeys(string fPath)
        {
            var fileStream = new FileStream(fPath, FileMode.Open, FileAccess.Read);
            var streamReader = new StreamReader(fileStream, Encoding.ASCII);
            string line;
            var zKeys = new List<long>();
            while ((line = streamReader.ReadLine()) != null)
                zKeys.Add(Convert.ToInt64(line));

            zKeys.CopyTo(0, PieceKeys[(int)Piece.wP], 8, 48);
            zKeys.CopyTo(48, PieceKeys[(int)Piece.wR], 0, 64);
            zKeys.CopyTo(112, PieceKeys[(int)Piece.wN], 0, 64);
            zKeys.CopyTo(176, PieceKeys[(int)Piece.wB], 0, 64);
            zKeys.CopyTo(240, PieceKeys[(int)Piece.wQ], 0, 64);
            zKeys.CopyTo(304, PieceKeys[(int)Piece.wK], 0, 64);
            zKeys.CopyTo(368, PieceKeys[(int)Piece.bP], 8, 48);
            zKeys.CopyTo(416, PieceKeys[(int)Piece.bR], 0, 64);
            zKeys.CopyTo(480, PieceKeys[(int)Piece.bN], 0, 64);
            zKeys.CopyTo(544, PieceKeys[(int)Piece.bB], 0, 64);
            zKeys.CopyTo(608, PieceKeys[(int)Piece.bQ], 0, 64);
            zKeys.CopyTo(672, PieceKeys[(int)Piece.bK], 0, 64);
            zKeys.CopyTo(736, EnPassantKeys, 0, 8);
            WhiteKSCastle = zKeys[744];
            WhiteQSCastle = zKeys[745];
            BlackKSCastle = zKeys[746];
            BlackQSCastle = zKeys[747];
            BlackToMove = zKeys[748];
            fileStream.Close();
        }

        public static long HashValue(Game game)
        {
            Piece p = Piece.eP;
            long hash = 0;
            for(int i = 0; i < 64; i++)
            {
                p = game.GameBoard.BoardArray[Board.VisitOrderBT[i]];
                if (p != Piece.eP)
                    hash = hash ^ PieceKeys[(int)p][i];  
            }

            if (game.EnPassantTargetSquare != Square.iv)
                hash = hash ^ EnPassantKeys[Utility.File(game.EnPassantTargetSquare)];

            if (game.KSCastle(Color.White))
                hash = hash ^ WhiteKSCastle;

            if (game.QSCastle(Color.White))
                hash = hash ^ WhiteQSCastle;

            if (game.KSCastle(Color.Black))
                hash = hash ^ BlackKSCastle;

            if (game.QSCastle(Color.Black))
                hash = hash ^ BlackQSCastle;

            if (game.SideToMove == Color.Black)
                hash = hash ^ BlackToMove;

            return hash;
        }

        public static void Print()
        {
            for (int i = 8; i < 56; i++)
                Console.WriteLine(PieceKeys[0][i]);
            
            for (int i = 1; i < 6; i++)
                for(int j = 0; j < 64; j++)
                    Console.WriteLine(PieceKeys[i][j]);

            for (int i = 8; i < 56; i++)
                Console.WriteLine(PieceKeys[7][i]);

            for (int i = 8; i < 13; i++)
                for (int j = 0; j < 64; j++)
                    Console.WriteLine(PieceKeys[i][j]);

            for(int i = 0; i < 8; i++)
                Console.WriteLine(EnPassantKeys[i]);

            Console.WriteLine(WhiteKSCastle);
            Console.WriteLine(WhiteQSCastle);
            Console.WriteLine(BlackKSCastle);
            Console.WriteLine(BlackQSCastle);
            Console.WriteLine(BlackToMove);
        }
    }
}
