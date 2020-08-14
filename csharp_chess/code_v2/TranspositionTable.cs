using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Deneme
{
    public class TableData
    {
        public long HashValue { get; private set; }
    }

    public static class Transposition<TData>
        where TData : TableData
    {
        private static Dictionary<long, TData> HashTable { get; set; }
    }
}
