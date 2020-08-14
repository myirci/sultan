using System.Collections.Generic;

namespace Deneme
{
    public class PgnRecord
    {
        Dictionary<string, string> Tags { get; set; }
        List<Move> Moves { get; set; }

        public PgnRecord()
        {
            Tags = new Dictionary<string, string>();
            Moves = new List<Move>();
        }
    }
}
