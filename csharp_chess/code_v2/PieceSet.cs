using System.Collections.Generic;
using System;
using System.Linq;

namespace Deneme
{
    public enum Piece : byte
    {
        wP, wR, wN, wB, wQ, wK, // 0 -> 5 
        eP, // empty = 6
        bP, bR, bN, bB, bQ, bK,  // 7 -> 12
    }

    public abstract class PieceSet
    {
        protected PieceSet()
        {
            PawnPositions = new List<Square>();
            RookPositions = new List<Square>();
            KnightPositions = new List<Square>();
            BishopPositions = new List<Square>();
            QueenPositions = new List<Square>();
        }

        public Color Color { get; protected set; }
        public Piece Pawn { get; protected set; }
        public Piece Rook { get; protected set; }
        public Piece Knight { get; protected set; }
        public Piece Bishop { get; protected set; }
        public Piece Queen { get; protected set; }
        public Piece King { get; protected set; }

        public Square KingPos { get; protected set; }
        public List<Square> PawnPositions { get; protected set; }
        public List<Square> RookPositions { get; protected set; }
        public List<Square> KnightPositions { get; protected set; }
        public List<Square> BishopPositions { get; protected set; }
        public List<Square> QueenPositions { get; protected set; }
        public Dictionary<Piece, List<Square>> PiecePositionMap;

        public Square[] KSCastlingSquares { get; protected set; }
        public Square[] QSCastlingSquares { get; protected set; }
   
        public int PawnStartRank { get; protected set; }
        public int PawnFinalRank { get; protected set; }
        public int FileStep { get; protected set; }
        public int[] DiagStep { get; protected set; }
                
        public int PawnCount() => PawnPositions.Count;
        public int RookCount() => RookPositions.Count;
        public int KnightCount() => KnightPositions.Count;
        public int BishopCount() => BishopPositions.Count;
        public int QueenCount() => QueenPositions.Count;

        public void UpdatePiecePosition(Square from, Square to, Piece p)
        {
            if (p == King)
            {
                KingPos = to;
            }
            else
            {
                var posLst = PiecePositionMap[p];
                for (int i = 0; i < posLst.Count; i++)
                {
                    if (posLst[i] == from)
                    {
                        posLst[i] = to;
                        return;
                    }
                }

                throw new Exception("Piece location could not be found");
            }            
        }

        public void AddPiece(Square loc, Piece p)
        {
            if (p == King || p == Piece.eP)
                throw new Exception("Add piece error!");
            else
                PiecePositionMap[p].Add(loc);
        }

        public void RemovePiece(Square loc, Piece p)
        {
            if (p == King || p == Piece.eP)
            {
                throw new Exception("Remove piece error!");
            }
            else
            {
                var posLst = PiecePositionMap[p];
                for (int i = 0; i < posLst.Count; i++)
                {
                    if (posLst[i] == loc)
                    {
                        posLst.RemoveAt(i);
                        return;
                    }
                }

                throw new Exception("Piece location could not be found");
            }
        }

        public virtual void Print()
        {
            Console.WriteLine("Pawns  : {0}", string.Join(",", PawnPositions.Select(s => Utility.SquareToString[s])));
            Console.WriteLine("Rooks  : {0}", string.Join(",", RookPositions.Select(s => Utility.SquareToString[s])));
            Console.WriteLine("Knights: {0}", string.Join(",", KnightPositions.Select(s => Utility.SquareToString[s])));
            Console.WriteLine("Bishops: {0}", string.Join(",", BishopPositions.Select(s => Utility.SquareToString[s])));
            Console.WriteLine("Queens : {0}", string.Join(",", QueenPositions.Select(s => Utility.SquareToString[s])));
            Console.WriteLine("King   : {0}", Utility.SquareToString[KingPos]);
        }

        public abstract bool IsSameColor(Piece p);
        public abstract bool IsOppositeColor(Piece p);

        public void SetPieces(Board b)
        {
            PawnPositions.Clear();
            RookPositions.Clear();
            KnightPositions.Clear();
            BishopPositions.Clear();
            QueenPositions.Clear();

            Piece p = Piece.eP;
            for (int i = 0; i < 64; i++)
            {
                p = b.BoardArray[Board.VisitOrderBT[i]];
                if (IsSameColor(p))
                    if (p == King)
                        KingPos = (Square)Board.VisitOrderBT[i];
                    else
                        PiecePositionMap[p].Add((Square)Board.VisitOrderBT[i]); 
            }
        }

        protected void InitializePiecePositionMap() => PiecePositionMap = 
            new Dictionary<Piece, List<Square>>
            {
                { Pawn, PawnPositions}, {Rook, RookPositions}, {Knight, KnightPositions},
                { Bishop, BishopPositions}, {Queen, QueenPositions}
            };
    }

    public class WhitePieces : PieceSet
    {
        public WhitePieces(Board brd) : base()
        {
            Color = Color.White;
            Pawn = Piece.wP;
            Rook = Piece.wR;
            Knight = Piece.wN;
            Bishop = Piece.wB;
            Queen = Piece.wQ;
            King = Piece.wK;

            KSCastlingSquares = new Square[4] { Square.e1, Square.f1, Square.g1, Square.h1};
            QSCastlingSquares = new Square[5] { Square.e1, Square.d1, Square.c1, Square.b1, Square.a1 };

            PawnStartRank = 1;
            PawnFinalRank = 6;
            FileStep = 16;
            DiagStep = new int[2] { 15, 17 };
                        
            InitializePiecePositionMap();
            SetPieces(brd);
    }

        public override void Print()
        {
            Console.WriteLine("White Pieces: ");
            base.Print();
        }

        public sealed override bool IsSameColor(Piece p) => (int)p < (int)Piece.eP;
        public sealed override bool IsOppositeColor(Piece p) => (int)p > (int)Piece.eP;
    }

    public class BlackPieces : PieceSet
    {
        public BlackPieces(Board brd) : base()
        {
            Color = Color.Black;
            Pawn = Piece.bP;
            Rook = Piece.bR;
            Knight = Piece.bN;
            Bishop = Piece.bB;
            Queen = Piece.bQ;
            King = Piece.bK;

            KSCastlingSquares = new Square[4] { Square.e8, Square.f8, Square.g8, Square.h8 };
            QSCastlingSquares = new Square[5] { Square.e8, Square.d8, Square.c8, Square.b8, Square.a8 };

            PawnStartRank = 6;
            PawnFinalRank = 1;
            FileStep = -16;
            DiagStep = new int[2] { -15, -17 };
                        
            InitializePiecePositionMap();
            SetPieces(brd);
        }

        public override void Print()
        {
            Console.WriteLine("Black Pieces: ");
            base.Print();
        }

        public sealed override bool IsSameColor(Piece p) => (int)p > (int)Piece.eP;
        public sealed override bool IsOppositeColor(Piece p) => (int)p < (int)Piece.eP;
    }
}
