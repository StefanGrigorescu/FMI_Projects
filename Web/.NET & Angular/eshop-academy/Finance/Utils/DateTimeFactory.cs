using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Finance.Utils
{
    public interface IDateTimeFactory
    {
        DateTime UtcNow();
        DateTime Now();
    }

    public sealed class DateTimeFactory : IDateTimeFactory
    {
        public DateTime UtcNow() => DateTime.UtcNow;
        public DateTime Now() => DateTime.Now;
    }
}
