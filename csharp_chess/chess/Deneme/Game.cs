using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Text;

namespace Deneme
{
    public enum Color : byte
    {
        White,
        Black,
        Invalid
    }

    public class Game
    {
        public Board GameBoard { get; set; }
        public Color SideToMove { get; set; }
        public Square EnPassantTargetSquare { get; set; }
        public int HalfMoveClock { get; set; }
        public int FullMoveCounter { get; set; }

        public WhitePieces WPieces { get; private set; }
        public BlackPieces BPieces { get; private set; }

        private BitVector32 castlingRights;
        private int wKS;
        private int wQS;
        private int bKS;
        private int bQS;
        
        Stack<int> HalfMoveClockHistory { get; set; }
        Stack<Square> EnPassantTargetSquareHistory { get; set; }
        Stack<BitVector32> CastlingRightsHistory { get; set; }
        Stack<Move> MoveHistory { get; set; }

        public Game(string fen = null)
        {
            GameBoard = new Board();
            WPieces = new WhitePieces(GameBoard);
            BPieces = new BlackPieces(GameBoard);

            wKS = BitVector32.CreateMask();
            wQS = BitVector32.CreateMask(wKS);
            bKS = BitVector32.CreateMask(wQS);
            bQS = BitVector32.CreateMask(bKS);

            CastlingRightsHistory = new Stack<BitVector32>();
            HalfMoveClockHistory = new Stack<int>();
            EnPassantTargetSquareHistory = new Stack<Square>();
            MoveHistory = new Stack<Move>();

            if (fen == null)
            {
                SideToMove = Color.White;
                EnPassantTargetSquare = Square.iv;
                HalfMoveClock = 0;
                FullMoveCounter = 1;
                castlingRights[wKS] = castlingRights[wQS] = castlingRights[bKS] = castlingRights[bQS] = true;
            }
            else
            {
                Load(fen);
            }
        }

        public void Load(string fen)
        {
            var fields = fen.Split(new char[] { ' ' });
            if (fields.Length != 6)
                throw new Exception(String.Format("FEN string has {0} fields", fields.Length));

            // set piece positions
            GameBoard.SetPosition(fields[0]);
            WPieces.SetPieces(GameBoard);
            BPieces.SetPieces(GameBoard);

            // side to move
            SideToMove = (fields[1] == "w") ? Color.White : Color.Black;

            // castling rights
            castlingRights[wKS] = fields[2].Any(c => c == 'K');
            castlingRights[wQS] = fields[2].Any(c => c == 'Q');
            castlingRights[bKS] = fields[2].Any(c => c == 'k');
            castlingRights[bQS] = fields[2].Any(c => c == 'q');

            // en passant square
            EnPassantTargetSquare = Utility.StringToSquare[fields[3]];

            // half move clock
            HalfMoveClock = Int32.Parse(fields[4]);

            // full move counter
            FullMoveCounter = Int32.Parse(fields[5]);
        }

        public bool KSCastle(Color c) => castlingRights[(c == Color.White) ? wKS : bKS];
        public bool QSCastle(Color c) => castlingRights[(c == Color.White) ? wQS : bQS];
        public void KSCastle(Color c, bool v) => castlingRights[(c == Color.White) ? wKS : bKS] = v;
        public void QSCastle(Color c, bool v) => castlingRights[(c == Color.White) ? wQS : bQS] = v;

        public void Print()
        {
            Console.WriteLine("Side to move     : {0}", SideToMove.ToString());
            Console.WriteLine("Castling rights  : {0}", GetCastlingRights());
            Console.WriteLine("En passant square: {0}", Utility.SquareToString[EnPassantTargetSquare]);
            Console.WriteLine("Half-move clock  : {0}", HalfMoveClock);
            Console.WriteLine("Full-move counter: {0}", FullMoveCounter);
            GameBoard.Print();
            WPieces.Print();
            Console.WriteLine();
            BPieces.Print();
        }

        public string GetCastlingRights()
        {
            string str = "";
            if (KSCastle(Color.White)) str += "K";
            if (QSCastle(Color.White)) str += "Q";
            if (KSCastle(Color.Black)) str += "k";
            if (QSCastle(Color.Black)) str += "q";
            return (str == "") ? "-" : str;
        }

        public void MakeMove(Move mv)
        {
            // settings w.r.t side to move
            PieceSet movingSide, movedSide;
            if (SideToMove == Color.White)
            {
                movingSide = WPieces;
                movedSide = BPieces;
                SideToMove = Color.Black;
            }
            else
            {
                movingSide = BPieces;
                movedSide = WPieces;
                SideToMove = Color.White;
                FullMoveCounter++;
            }
            
            // piece placement
            var movingPiece = GameBoard.BoardArray[(int)mv.From];
            GameBoard.BoardArray[(int)mv.From] = Piece.eP;

            if (mv.Type == MoveType.Promotion || mv.Type == MoveType.PromotionAndCapture)
            {
                GameBoard.BoardArray[(int)mv.To] = mv.Promoted;
                movingSide.RemovePiece(mv.From, movingPiece);
                movingSide.AddPiece(mv.To, mv.Promoted);
            }
            else
            {
                GameBoard.BoardArray[(int)mv.To] = movingPiece;
                movingSide.UpdatePiecePosition(mv.From, mv.To, movingPiece);
            }

            if (mv.Type == MoveType.Capture || mv.Type == MoveType.PromotionAndCapture)
                movedSide.RemovePiece(mv.To, mv.Captured);
                       
            if (mv.Type == MoveType.EnPassantCapture)
            {
                int sq = (int)EnPassantTargetSquare + movingSide.FileStep;
                GameBoard.BoardArray[sq] = Piece.eP;
                movedSide.RemovePiece((Square)sq, mv.Captured);
            }
            else if (mv.Type == MoveType.KingSideCastle)
            {
                GameBoard.BoardArray[(int)movingSide.KingSideRookPos] = Piece.eP;
                GameBoard.BoardArray[(int)movingSide.KingSideCastledRookPos] = movingSide.Rook;
                movingSide.UpdatePiecePosition(movingSide.KingSideRookPos, movingSide.KingSideCastledRookPos, movingSide.Rook);
            }
            else if (mv.Type == MoveType.QueenSideCastle)
            {
                GameBoard.BoardArray[(int)movingSide.QueenSideRookPos] = Piece.eP;
                GameBoard.BoardArray[(int)movingSide.QueenSideCastledRookPos] = movingSide.Rook;
                movingSide.UpdatePiecePosition(movingSide.QueenSideRookPos, movingSide.QueenSideCastledRookPos, movingSide.Rook);
            }

            // castling rights
            CastlingRightsHistory.Push(castlingRights);
            if (movingPiece == movingSide.King)
            {
                KSCastle(movingSide.Color, false);
                QSCastle(movingSide.Color, false);
            }
            else if (movingPiece == movingSide.Rook)
            {
                if (mv.From == movingSide.QueenSideRookPos)     QSCastle(movingSide.Color, false);
                else if (mv.From == movingSide.KingSideRookPos) KSCastle(movingSide.Color, false);
            }
            
            if (mv.Captured == movedSide.Rook)
            {
                if (mv.To == movedSide.QueenSideRookPos)     QSCastle(movedSide.Color, false);
                else if (mv.To == movedSide.KingSideRookPos) KSCastle(movedSide.Color, false);
            }
            
            // en passant target square
            EnPassantTargetSquareHistory.Push(EnPassantTargetSquare);
            EnPassantTargetSquare = (mv.Type == MoveType.DoublePawnPush) ? 
                (Square)((int)mv.From + movingSide.FileStep) : Square.iv;

            // half move clock
            HalfMoveClockHistory.Push(HalfMoveClock);
            HalfMoveClock = (movingPiece == movingSide.Pawn || mv.Type == MoveType.Capture) ? 0 : HalfMoveClock + 1;

            // store the move
            MoveHistory.Push(mv);
        }

        public void UnMakeMove()
        {
            // get the last move from the history
            Move mv = MoveHistory.Peek();

            PieceSet movedSide, movingSide;
            if (SideToMove == Color.White)
            {
                movingSide = WPieces;
                movedSide = BPieces;
                SideToMove = Color.Black;
                FullMoveCounter--;
            }
            else
            {
                movedSide = WPieces;
                movingSide = BPieces;
                SideToMove = Color.White;
            }

            // piece placement
            if (mv.Type == MoveType.Quite || mv.Type == MoveType.DoublePawnPush)
            {
                movedSide.UpdatePiecePosition(mv.To, mv.From, GameBoard.BoardArray[(int)mv.To]);
                GameBoard.BoardArray[(int)mv.From] = GameBoard.BoardArray[(int)mv.To];
                GameBoard.BoardArray[(int)mv.To] = Piece.eP;
            }
            else if (mv.Type == MoveType.Capture)
            {
                movedSide.UpdatePiecePosition(mv.To, mv.From, GameBoard.BoardArray[(int)mv.To]);
                movingSide.AddPiece(mv.To, mv.Captured);
                GameBoard.BoardArray[(int)mv.From] = GameBoard.BoardArray[(int)mv.To];
                GameBoard.BoardArray[(int)mv.To] = mv.Captured;
            }
            else if (mv.Type == MoveType.KingSideCastle)
            {
                movedSide.UpdatePiecePosition(mv.To, mv.From, movedSide.King);
                GameBoard.BoardArray[(int)mv.From] = movedSide.King;
                GameBoard.BoardArray[(int)mv.To] = Piece.eP;

                movedSide.UpdatePiecePosition(movedSide.KingSideCastledRookPos, movedSide.KingSideRookPos, movedSide.Rook);
                GameBoard.BoardArray[(int)movedSide.KingSideCastledRookPos] = Piece.eP;
                GameBoard.BoardArray[(int)movedSide.KingSideRookPos] = movedSide.Rook;
            }
            else if (mv.Type == MoveType.QueenSideCastle)
            {
                movedSide.UpdatePiecePosition(mv.To, mv.From, movedSide.King);
                GameBoard.BoardArray[(int)mv.From] = movedSide.King;
                GameBoard.BoardArray[(int)mv.To] = Piece.eP;
                                
                movedSide.UpdatePiecePosition(movedSide.QueenSideCastledRookPos, movedSide.QueenSideRookPos, movedSide.Rook);
                GameBoard.BoardArray[(int)movedSide.QueenSideCastledRookPos] = Piece.eP;
                GameBoard.BoardArray[(int)movedSide.QueenSideRookPos] = movedSide.Rook;
            }
            else if (mv.Type == MoveType.Promotion)
            {
                movedSide.RemovePiece(mv.To, GameBoard.BoardArray[(int)mv.To]);
                movedSide.AddPiece(mv.From, movedSide.Pawn);
                GameBoard.BoardArray[(int)mv.From] = movedSide.Pawn;
                GameBoard.BoardArray[(int)mv.To] = Piece.eP;
            }
            else if (mv.Type == MoveType.PromotionAndCapture)
            {
                movedSide.RemovePiece(mv.To, GameBoard.BoardArray[(int)mv.To]);
                movedSide.AddPiece(mv.From, movedSide.Pawn);
                movingSide.AddPiece(mv.To, mv.Captured);
                GameBoard.BoardArray[(int)mv.From] = movingSide.Pawn;
                GameBoard.BoardArray[(int)mv.To] = mv.Captured;
            }
            else if (mv.Type == MoveType.EnPassantCapture)
            {
                movedSide.UpdatePiecePosition(mv.To, mv.From, movedSide.Pawn);
                GameBoard.BoardArray[(int)mv.From] = movedSide.Pawn;
                GameBoard.BoardArray[(int)mv.To] = Piece.eP;
                GameBoard.BoardArray[(int)mv.To + movingSide.FileStep] = movingSide.Pawn;
                movingSide.AddPiece((Square)((int)mv.To + movingSide.FileStep), movingSide.Pawn);
            }

            // castling rights
            castlingRights = CastlingRightsHistory.Peek();
            CastlingRightsHistory.Pop();

            // en passant target square
            EnPassantTargetSquare = EnPassantTargetSquareHistory.Peek();
            EnPassantTargetSquareHistory.Pop();

            // half move clock
            HalfMoveClock = HalfMoveClockHistory.Peek();
            HalfMoveClockHistory.Pop();

            // remove move from the history
            MoveHistory.Pop();
        }

        public string ToFenString()
        {
            var fen = new StringBuilder("");
            
            int idx = 0;
            int emptyCount = 0;
            Piece p = Piece.eP;
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    idx = i * 8 + j;
                    p = GameBoard.BoardArray[Board.VisitOrderTB[idx]];
                    if (p == Piece.eP)
                    {
                        emptyCount++;
                    }
                    else
                    {
                        if (emptyCount != 0)
                            fen.Append(emptyCount.ToString());
                        fen.Append(Utility.PieceToChar[p]);
                        emptyCount = 0;
                    }
                }

                if (emptyCount != 0)
                    fen.Append(emptyCount.ToString());
                emptyCount = 0;

                fen.Append((i < 7) ? "/" : " ");
            }

            fen.Append(SideToMove == Color.White ? "w " : "b ");
            fen.Append(GetCastlingRights());
            fen.Append(" ");
            fen.Append(Utility.SquareToString[EnPassantTargetSquare]);
            fen.Append(" ");
            fen.Append(HalfMoveClock);
            fen.Append(" ");
            fen.Append(FullMoveCounter);
            return fen.ToString();
        }
    }
}
