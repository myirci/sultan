using System;
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
            var moves = new List<Move>();

            PieceSet movingSide = game.WPieces;
            PieceSet movedSide = game.BPieces;
            if (game.SideToMove == Color.Black)
            {
                movingSide = game.BPieces;
                movedSide = game.WPieces;
            }

            // finding checking pieces
            var kingAttackers = new Dictionary<Square, Piece>();
            AttackingPieces(game, movedSide, movingSide.KingPos, kingAttackers);
            int checkCount = kingAttackers.Count;

            GenerateKingMoves(game, movingSide, movedSide, checkCount, moves);

            if (checkCount == 1)
            {
                // generate capturing the checking piece moves
                var e = kingAttackers.GetEnumerator();
                e.MoveNext();
                var attackers = new Dictionary<Square, Piece>();
                AttackingPieces(game, movingSide, e.Current.Key, attackers);
                foreach (var p in attackers)
                    moves.Add(new Move(p.Key, e.Current.Key, MoveType.Capture, e.Current.Value));
                
                // generate blocking moves
                if (e.Current.Value != movedSide.Knight || e.Current.Value != movedSide.Pawn)
                {
                    int bigger = (int)movingSide.KingPos;
                    int smaller = (int)e.Current.Key;
                    if (smaller > bigger)
                        Utility.Swap(ref smaller, ref bigger);
                    int diff = bigger - smaller;
                    int step = 1;
                    if (diff % 15 == 0) step = 15;
                    else if (diff % 16 == 0) step = 16;
                    else if (diff % 17 == 0) step = 17;

                    smaller += step;
                    while (smaller != bigger)
                    {
                        attackers.Clear();
                        BlockingPieces(game, movingSide, (Square)smaller, attackers);
                        foreach (var p in attackers)
                        {
                            if (p.Value != movingSide.King)
                            {
                                MoveType mt = MoveType.Quite;
                                if (p.Value == movedSide.Pawn && (Math.Abs(smaller - (int)p.Key) > 16))
                                    mt = MoveType.DoublePawnPush;
                                moves.Add(new Move(p.Key, (Square)smaller, mt));
                            }
                        }
                        smaller += step;
                    }
                }
            }
            else if (checkCount == 0)
            {
                if (movingSide.PawnCount() != 0)
                    GeneratePawnMoves(game, movingSide, movedSide, moves);

                GeneratePieceMoves(game, movingSide, movedSide, moves);
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

        private static void AttackingPieces(Game game, PieceSet attackingSide, Square sq, Dictionary<Square, Piece> attackingPieces)
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
                            attackingPieces.Add((Square)dest, p);
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
                            attackingPieces.Add((Square)dest, p);
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
                            attackingPieces.Add((Square)dest, p);
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
                        attackingPieces.Add((Square)dest, attackingSide.Knight);
                }
            }

            // Pawn attacks
            if (attackingSide.PawnCount() > 0)
            {
                foreach (int i in attackingSide.NegatedDiagStep)
                {
                    dest = (int)sq + i;
                    if ((dest & 0x88) == 0 && game.GameBoard.BoardArray[dest] == attackingSide.Pawn)
                        attackingPieces.Add((Square)dest, attackingSide.Pawn);
                }
            }

            // King attacks
            foreach (int i in SlidingPieceSteps)
            {
                dest = (int)sq + i;
                if ((dest & 0x88) == 0 && game.GameBoard.BoardArray[dest] == attackingSide.King)
                    attackingPieces.Add((Square)dest, attackingSide.King);
            }
        }

        private static void BlockingPieces(Game game, PieceSet attackingSide, Square sq, Dictionary<Square, Piece> blockingPieces)
        {
            int dest = -1;
            Piece p = Piece.eP;

            // Queen blocks
            if (attackingSide.QueenCount() > 0)
            {
                foreach (int i in SlidingPieceSteps)
                {
                    dest = (int)sq + i;
                    while ((dest & 0x88) == 0)
                    {
                        p = game.GameBoard.BoardArray[dest];
                        if (p == attackingSide.Queen)
                            blockingPieces.Add((Square)dest, p);
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
                            blockingPieces.Add((Square)dest, p);
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
                            blockingPieces.Add((Square)dest, p);
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
                        blockingPieces.Add((Square)dest, attackingSide.Knight);
                }
            }

            // Pawn attacks
            if (attackingSide.PawnCount() > 0)
            {
                foreach (int i in attackingSide.NegatedDiagStep)
                {
                    dest = (int)sq + i;
                    if ((dest & 0x88) == 0 && game.GameBoard.BoardArray[dest] == attackingSide.Pawn 
                        && game.EnPassantTargetSquare == sq)
                        blockingPieces.Add((Square)dest, attackingSide.Pawn);
                }
            }

            if (attackingSide.PawnCount() > 0)
            {
                dest = (int)sq - attackingSide.FileStep;
                if ((dest & 0x88) == 0 && game.GameBoard.BoardArray[dest] == attackingSide.Pawn)
                    blockingPieces.Add((Square)dest, attackingSide.Pawn);
                dest -= attackingSide.FileStep;
                if ((dest & 0x88) == 0 && game.GameBoard.BoardArray[dest] == attackingSide.Pawn && Utility.Rank((Square)dest) == attackingSide.PawnStartRank)
                    blockingPieces.Add((Square)dest, attackingSide.Pawn);
            }
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
                foreach (int i in attackingSide.NegatedDiagStep)
                {
                    dest = (int)sq + i;
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

        private static void GenerateKingMoves(Game game, PieceSet movingSide, PieceSet movedSide, int checkCount, List<Move> moves)
        {
            int dest = -1;
            Piece p = Piece.eP;
            foreach (var i in SlidingPieceSteps)
            {
                dest = (int)movingSide.KingPos + i;
                if ((0x88 & dest) == 0 && !IsUnderAttack(game, movedSide, (Square)dest))
                {
                    p = game.GameBoard.BoardArray[dest];
                    if (p == Piece.eP)
                        moves.Add(new Move(movingSide.KingPos, (Square)dest, MoveType.Quite));
                    else if (movingSide.IsOppositeColor(p))
                        moves.Add(new Move(movingSide.KingPos, (Square)dest, MoveType.Capture, p));
                }
            }

            if (checkCount == 0)
            {
                if (game.KSCastle(movingSide.Color) &&
                    game.GameBoard.BoardArray[(int)movingSide.KingSideCastleKingPath[0]] == Piece.eP &&
                    game.GameBoard.BoardArray[(int)movingSide.KingSideCastleKingPath[1]] == Piece.eP &&
                    !IsUnderAttack(game, movedSide, movingSide.KingSideCastleKingPath[0]) &&
                    !IsUnderAttack(game, movedSide, movingSide.KingSideCastleKingPath[1]))
                    moves.Add(new Move(movingSide.KingPos, movingSide.KingSideCastleKingPath[1], MoveType.KingSideCastle));

                if (game.QSCastle(movingSide.Color) &&
                    game.GameBoard.BoardArray[(int)movingSide.QueenSideCastleKingPath[0]] == Piece.eP &&
                    game.GameBoard.BoardArray[(int)movingSide.QueenSideCastleKingPath[1]] == Piece.eP &&
                    !IsUnderAttack(game, movedSide, movingSide.QueenSideCastleKingPath[0]) &&
                    !IsUnderAttack(game, movedSide, movingSide.QueenSideCastleKingPath[1]))
                    moves.Add(new Move(movingSide.KingPos, movingSide.QueenSideCastleKingPath[1], MoveType.QueenSideCastle));
            }
        }
        
        private static void GeneratePawnMoves(Game game, PieceSet movingSide, PieceSet movedSide, List<Move> moves)
        {
            int dest = -1;
            int rank = -1;
            foreach (var sq in movingSide.PawnPositions)
            {
                var pin = IsPinned(game, movedSide, movingSide.KingPos, sq);
                if (pin.Item1)
                {
                    if (pin.Item2 == movingSide.FileStep)
                    {
                        // one square forward
                        dest = (int)sq + movingSide.FileStep;
                        if (game.GameBoard.BoardArray[dest] == Piece.eP)
                            moves.Add(new Move(sq, (Square)dest, MoveType.Quite));
                    }
                    else if (pin.Item2 == movingSide.DiagStep[0] || pin.Item2 == movingSide.DiagStep[1])
                    {
                        // one square diagonally forward and capture
                        foreach (int st in movingSide.DiagStep)
                        {
                            dest = (int)sq + st;
                            if ((dest & 0x88) == 0 && movingSide.IsOppositeColor(game.GameBoard.BoardArray[dest]))
                                moves.Add(new Move(sq, (Square)dest, MoveType.Capture, game.GameBoard.BoardArray[dest]));
                        }
                    }
                }
                else
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
                    var pin = IsPinned(game, movedSide, movedSide.KingPos, sq);
                    if (pin.Item1)
                        continue;

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
                    var pin = IsPinned(game, movedSide, movedSide.KingPos, sq);
                    if (pin.Item1)
                    {
                        dest = (int)sq + pin.Item2;
                        while ((dest & 0x88) == 0)
                        {
                            p = game.GameBoard.BoardArray[dest];
                            if (p == Piece.eP)
                            {
                                moves.Add(new Move(sq, (Square)dest, MoveType.Quite));
                                dest += pin.Item2;
                            }
                            else
                            {
                                if (movingSide.IsOppositeColor(p))
                                    moves.Add(new Move(sq, (Square)dest, MoveType.Capture, p));
                                break;
                            }
                        }
                    }
                    else
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
            }
            
            if (movingSide.RookCount() > 0)
            {
                foreach (var sq in movingSide.RookPositions)
                {
                    var pin = IsPinned(game, movedSide, movedSide.KingPos, sq);
                    if (pin.Item1)
                    {
                        dest = (int)sq + pin.Item2;
                        while ((dest & 0x88) == 0)
                        {
                            p = game.GameBoard.BoardArray[dest];
                            if (p == Piece.eP)
                            {
                                moves.Add(new Move(sq, (Square)dest, MoveType.Quite));
                                dest += pin.Item2;
                            }
                            else
                            {
                                if (movingSide.IsOppositeColor(p))
                                    moves.Add(new Move(sq, (Square)dest, MoveType.Capture, p));
                                break;
                            }
                        }
                    }
                    else
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
            }

            if (movingSide.BishopCount() > 0)
            {
                foreach (var sq in movingSide.BishopPositions)
                {
                    var pin = IsPinned(game, movedSide, movedSide.KingPos, sq);
                    if (pin.Item1)
                    {
                        dest = (int)sq + pin.Item2;
                        while ((dest & 0x88) == 0)
                        {
                            p = game.GameBoard.BoardArray[dest];
                            if (p == Piece.eP)
                            {
                                moves.Add(new Move(sq, (Square)dest, MoveType.Quite));
                                dest += pin.Item2;
                            }
                            else
                            {
                                if (movingSide.IsOppositeColor(p))
                                    moves.Add(new Move(sq, (Square)dest, MoveType.Capture, p));
                                break;
                            }
                        }
                    }
                    else
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

        private static Tuple<bool, int> IsPinned(Game game, PieceSet attackingSide, Square kingPos, Square pieceLoc)
        {
            int smaller = (int)kingPos;
            int bigger = (int)pieceLoc;
            if (smaller > bigger)
                Utility.Swap(ref smaller, ref bigger);

            int step = -1;
            int diff = bigger - smaller;
            if (diff < 8) step = 1;
            else if (diff % 15 == 0) step = 15;
            else if (diff % 16 == 0) step = 16;
            else if (diff % 17 == 0) step = 17;

            if (step == -1)
                return new Tuple<bool, int>(false, 0);

            smaller += step;
            while (smaller != bigger)
            {
                if (game.GameBoard.BoardArray[smaller] != Piece.eP)
                    return new Tuple<bool, int>(false, 0);
                smaller += step;
            }

            if (kingPos > pieceLoc)
                step *= -1;

            Piece p = Piece.eP;
            if (step == 1 || step == 16 || step == -1 || step == -16)
            {
                int loc = (int)pieceLoc + step;
                while ((loc & 0x88) == 0)
                {
                    p = game.GameBoard.BoardArray[loc];
                    if (p == attackingSide.Rook || p == attackingSide.Queen)
                        return new Tuple<bool, int>(true, step);
                    else if(p != Piece.eP)
                        return new Tuple<bool, int>(false, 0);
                    loc += step;
                }
            }
            else
            {
                int loc = (int)pieceLoc + step;
                while ((loc & 0x88) == 0)
                {
                    p = game.GameBoard.BoardArray[loc];
                    if (p == attackingSide.Bishop || p == attackingSide.Queen)
                        return new Tuple<bool, int>(true, step);
                    else if (p != Piece.eP)
                        return new Tuple<bool, int>(false, 0);
                    loc += step;
                }
            }

            return new Tuple<bool, int>(false, 0);
        }
    }
}
