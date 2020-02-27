using System;

namespace Deneme
{
    public enum MoveType : byte
    {
        Quite,
        Capture,
        DoublePawnPush,
        KingSideCastle,
        QueenSideCastle,
        Promotion,
        PromotionAndCapture,
        EnPassantCapture
    }

    public class Move
    {
        public Square From { get; set; }
        public Square To { get; set; }
        public MoveType Type { get; set; }
        public Piece Captured { get; set; }
        public Piece Promoted { get; set; }

        public Move(Square origin, Square destination, MoveType type, Piece capturedPiece = Piece.eP, Piece promotedPiece = Piece.eP)
        {
            From = origin;
            To = destination;
            Type = type;
            Captured = capturedPiece;
            Promoted = promotedPiece;
        }

        public void Print()
        {
            Console.Write("{0} : {1} => {2}", 
                Utility.SquareToString[From], Utility.SquareToString[To], Type.ToString());
            if (Captured != Piece.eP)
                Console.Write(", Captured: {0}", Captured.ToString());
            if (Promoted != Piece.eP)
                Console.Write(", Promoted: {0}", Promoted.ToString());
            Console.Write("\n");
        }

    }
}
