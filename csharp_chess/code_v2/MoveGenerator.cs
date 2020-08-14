using System.Collections.Generic;

namespace Deneme
{
    public static class MoveGenerator
    {
        private static readonly int[] KnightJumps = new int[8] { 14, 18, 31, 33, -14, -18, -31, -33 };
        private static readonly int[] SlidingPieceSteps = new int[8] { 16, -16, 1, -1, 15, -15, 17, -17 };
        private static readonly int[] FlatSlidingPieceSteps = new int[4] { 16, -16, 1, -1 };
        private static readonly int[] DiagSlidingPieceSteps = new int[4] { 15, -15, 17, -17 };

        public static List<Move> GenerateMoves(Game game)
        {
            var pseudoMoves = new List<Move>();

            PieceSet movingSide = game.WPieces;
            PieceSet movedSide = game.BPieces;
            if (game.SideToMove == Color.Black)
            {
                movingSide = game.BPieces;
                movedSide = game.WPieces;
            }

            GenerateKingMoves(game, movingSide, movedSide, pseudoMoves);
            if (movingSide.PawnCount() != 0)
                GeneratePawnMoves(game, movingSide, movedSide, pseudoMoves);
            GeneratePieceMoves(game, movingSide, movedSide, pseudoMoves);

            var moves = new List<Move>();
            foreach (var mv in pseudoMoves)
            {
                game.MakeMove(mv);                
                if (!IsUnderAttack(game, movedSide, movingSide.KingPos))
                    moves.Add(mv);
                game.UnMakeMove();
            }

            return moves;
        }

        public static long Perft(Game game, int depth)
        {
            if (depth == 0)
                return 1;

            long nodes = 0;
            var moves = GenerateMoves(game);
            foreach (var mv in moves)
            {
                game.MakeMove(mv);
                nodes += Perft(game, depth - 1);
                game.UnMakeMove();
            }
            return nodes;
        }

        private static bool IsUnderAttack(Game game, PieceSet attackingSide, Square sq)
        {
            int dest = -1;
            Piece p = Piece.eP;

            // Queen attacks
            if (attackingSide.QueenCount() > 0)
            {
                foreach (int i in SlidingPieceSteps)
                {
                    dest = (int)sq + i;
                    while ((dest & 0x88) == 0)
                    {
                        p = game.GameBoard.BoardArray[dest];
                        if (p == attackingSide.Queen)
                            return true;
                        if (p != Piece.eP)
                            break;
                        dest += i;
                    }
                }
            }

            // Rook attacks
            if (attackingSide.RookCount() > 0)
            {
                foreach (int i in FlatSlidingPieceSteps)
                {
                    dest = (int)sq + i;
                    while ((dest & 0x88) == 0)
                    {
                        p = game.GameBoard.BoardArray[dest];
                        if (p == attackingSide.Rook)
                            return true;
                        if (p != Piece.eP)
                            break;
                        dest += i;
                    }
                }
            }

            // Bishop attacks
            if (attackingSide.BishopCount() > 0)
            {
                foreach (int i in DiagSlidingPieceSteps)
                {
                    dest = (int)sq + i;
                    while ((dest & 0x88) == 0)
                    {
                        p = game.GameBoard.BoardArray[dest];
                        if (p == attackingSide.Bishop)
                            return true;
                        if (p != Piece.eP)
                            break;
                        dest += i;
                    }
                }
            }

            // Knight attacks
            if (attackingSide.KnightCount() > 0)
            {
                foreach (int i in KnightJumps)
                {
                    dest = (int)sq + i;
                    if ((dest & 0x88) == 0 && game.GameBoard.BoardArray[dest] == attackingSide.Knight)
                        return true;
                }
            }

            // Pawn attacks
            if (attackingSide.PawnCount() > 0)
            {
                foreach (int i in attackingSide.DiagStep)
                {
                    dest = (int)sq - i;
                    if ((dest & 0x88) == 0 && game.GameBoard.BoardArray[dest] == attackingSide.Pawn)
                        return true;
                }
            }

            // King attacks
            foreach (int i in SlidingPieceSteps)
            {
                dest = (int)sq + i;
                if ((dest & 0x88) == 0 && game.GameBoard.BoardArray[dest] == attackingSide.King)
                    return true;
            }

            return false;
        }

        private static void GenerateKingMoves(Game game, PieceSet movingSide, PieceSet movedSide, List<Move> moves)
        {
            int dest = -1;
            Piece p = Piece.eP;
            foreach (var i in SlidingPieceSteps)
            {
                dest = (int)movingSide.KingPos + i;
                if ((0x88 & dest) == 0)
                {
                    p = game.GameBoard.BoardArray[dest];
                    if (p == Piece.eP)
                        moves.Add(new Move(movingSide.KingPos, (Square)dest, MoveType.Quite));
                    else if (movingSide.IsOppositeColor(p))
                        moves.Add(new Move(movingSide.KingPos, (Square)dest, MoveType.Capture, p));
                }
            }

            if (!IsUnderAttack(game, movedSide, movingSide.KingPos))
            {
                if (game.KSCastle(movingSide.Color) &&
                    game.GameBoard.BoardArray[(int)movingSide.KSCastlingSquares[1]] == Piece.eP &&
                    game.GameBoard.BoardArray[(int)movingSide.KSCastlingSquares[2]] == Piece.eP &&
                    !IsUnderAttack(game, movedSide, movingSide.KSCastlingSquares[1]) &&
                    !IsUnderAttack(game, movedSide, movingSide.KSCastlingSquares[2]))
                    moves.Add(new Move(movingSide.KingPos, movingSide.KSCastlingSquares[2], MoveType.KingSideCastle));

                if (game.QSCastle(movingSide.Color) &&
                    game.GameBoard.BoardArray[(int)movingSide.QSCastlingSquares[1]] == Piece.eP &&
                    game.GameBoard.BoardArray[(int)movingSide.QSCastlingSquares[2]] == Piece.eP &&
                    game.GameBoard.BoardArray[(int)movingSide.QSCastlingSquares[3]] == Piece.eP &&
                    !IsUnderAttack(game, movedSide, movingSide.QSCastlingSquares[1]) &&
                    !IsUnderAttack(game, movedSide, movingSide.QSCastlingSquares[2]))
                    moves.Add(new Move(movingSide.KingPos, movingSide.QSCastlingSquares[2], MoveType.QueenSideCastle));
            }
        }

        private static void GeneratePawnMoves(Game game, PieceSet movingSide, PieceSet movedSide, List<Move> moves)
        {
            int dest = -1;
            int rank = -1;
            foreach (var sq in movingSide.PawnPositions)
            {
                rank = Utility.Rank(sq);
                if (rank == movingSide.PawnStartRank)
                {
                    // one square forward
                    dest = (int)sq + movingSide.FileStep;
                    if (game.GameBoard.BoardArray[dest] == Piece.eP)
                    {
                        moves.Add(new Move(sq, (Square)dest, MoveType.Quite));
                        // two squares forward
                        dest += movingSide.FileStep;
                        if (game.GameBoard.BoardArray[dest] == Piece.eP)
                            moves.Add(new Move(sq, (Square)dest, MoveType.DoublePawnPush));
                    }

                    // one square diagonally forward and capture
                    foreach (int st in movingSide.DiagStep)
                    {
                        dest = (int)sq + st;
                        if ((dest & 0x88) == 0 && movingSide.IsOppositeColor(game.GameBoard.BoardArray[dest]))
                            moves.Add(new Move(sq, (Square)dest, MoveType.Capture, game.GameBoard.BoardArray[dest]));
                    }
                }
                else if (rank == movingSide.PawnFinalRank)
                {
                    // one square forward and promote
                    dest = (int)sq + movingSide.FileStep;
                    if (game.GameBoard.BoardArray[dest] == Piece.eP)
                    {
                        moves.Add(new Move(sq, (Square)dest, MoveType.Promotion, promotedPiece: movingSide.Queen));
                        moves.Add(new Move(sq, (Square)dest, MoveType.Promotion, promotedPiece: movingSide.Rook));
                        moves.Add(new Move(sq, (Square)dest, MoveType.Promotion, promotedPiece: movingSide.Knight));
                        moves.Add(new Move(sq, (Square)dest, MoveType.Promotion, promotedPiece: movingSide.Bishop));
                    }

                    // one square diagonally forward, promote and capture
                    foreach (int st in movingSide.DiagStep)
                    {
                        dest = (int)sq + st;
                        if ((dest & 0x88) == 0 && movingSide.IsOppositeColor(game.GameBoard.BoardArray[dest]))
                        {
                            moves.Add(new Move(sq, (Square)dest, MoveType.PromotionAndCapture, game.GameBoard.BoardArray[dest], movingSide.Queen));
                            moves.Add(new Move(sq, (Square)dest, MoveType.PromotionAndCapture, game.GameBoard.BoardArray[dest], movingSide.Rook));
                            moves.Add(new Move(sq, (Square)dest, MoveType.PromotionAndCapture, game.GameBoard.BoardArray[dest], movingSide.Knight));
                            moves.Add(new Move(sq, (Square)dest, MoveType.PromotionAndCapture, game.GameBoard.BoardArray[dest], movingSide.Bishop));
                        }
                    }
                }
                else
                {
                    // one square forward
                    dest = (int)sq + movingSide.FileStep;
                    if (game.GameBoard.BoardArray[dest] == Piece.eP)
                        moves.Add(new Move(sq, (Square)dest, MoveType.Quite));

                    // one square diagonally forward and capture
                    foreach (int st in movingSide.DiagStep)
                    {
                        dest = (int)sq + st;
                        if ((dest & 0x88) == 0 && movingSide.IsOppositeColor(game.GameBoard.BoardArray[dest]))
                            moves.Add(new Move(sq, (Square)dest, MoveType.Capture, game.GameBoard.BoardArray[dest]));
                    }

                    // en passant capture
                    if (game.EnPassantTargetSquare != Square.iv)
                    {
                        foreach (var i in movingSide.DiagStep)
                        {
                            if((int)game.EnPassantTargetSquare - i == (int)sq)
                            {
                                moves.Add(new Move(sq, game.EnPassantTargetSquare, MoveType.EnPassantCapture, movedSide.Pawn));
                                break;
                            }
                        }
                    }
                }
            }
        }

        private static void GeneratePieceMoves(Game game, PieceSet movingSide, PieceSet movedSide, List<Move> moves)
        {
            int dest = -1;
            Piece p = Piece.eP;

            if (movingSide.KnightCount() > 0)
            {
                foreach (var sq in movingSide.KnightPositions)
                {
                    foreach (var i in KnightJumps)
                    {
                        dest = (int)sq + i;
                        if ((0x88 & dest) == 0)
                        {
                            p = game.GameBoard.BoardArray[dest];
                            if (p == Piece.eP)
                                moves.Add(new Move(sq, (Square)dest, MoveType.Quite));
                            else if (movingSide.IsOppositeColor(p))
                                moves.Add(new Move(sq, (Square)dest, MoveType.Capture, p));
                        }
                    }
                }
            }

            if (movingSide.QueenCount() > 0)
            {
                foreach (var sq in movingSide.QueenPositions)
                {
                    foreach (int i in SlidingPieceSteps)
                    {
                        dest = (int)sq + i;
                        while ((dest & 0x88) == 0)
                        {
                            p = game.GameBoard.BoardArray[dest];
                            if (p == Piece.eP)
                            {
                                moves.Add(new Move(sq, (Square)dest, MoveType.Quite));
                                dest += i;
                            }
                            else
                            {
                                if (movingSide.IsOppositeColor(p))
                                    moves.Add(new Move(sq, (Square)dest, MoveType.Capture, p));
                                break;
                            }
                        }
                    }

                }
            }

            if (movingSide.RookCount() > 0)
            {
                foreach (var sq in movingSide.RookPositions)
                {
                    foreach (int i in FlatSlidingPieceSteps)
                    {
                        dest = (int)sq + i;
                        while ((dest & 0x88) == 0)
                        {
                            p = game.GameBoard.BoardArray[dest];
                            if (p == Piece.eP)
                            {
                                moves.Add(new Move(sq, (Square)dest, MoveType.Quite));
                                dest += i;
                            }
                            else
                            {
                                if (movingSide.IsOppositeColor(p))
                                    moves.Add(new Move(sq, (Square)dest, MoveType.Capture, p));
                                break;
                            }
                        }
                    }

                }
            }

            if (movingSide.BishopCount() > 0)
            {
                foreach (var sq in movingSide.BishopPositions)
                {
                    foreach (int i in DiagSlidingPieceSteps)
                    {
                        dest = (int)sq + i;
                        while ((dest & 0x88) == 0)
                        {
                            p = game.GameBoard.BoardArray[dest];
                            if (p == Piece.eP)
                            {
                                moves.Add(new Move(sq, (Square)dest, MoveType.Quite));
                                dest += i;
                            }
                            else
                            {
                                if (movingSide.IsOppositeColor(p))
                                    moves.Add(new Move(sq, (Square)dest, MoveType.Capture, p));
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}
