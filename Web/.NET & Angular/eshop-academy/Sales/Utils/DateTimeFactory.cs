namespace Sales.Utils
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
