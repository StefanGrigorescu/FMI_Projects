namespace MFIS
{
    internal class Interval
    {
        // MinValue must be STRICTLY lower than MaxValue
        public static readonly double MinValue = -10.0;
        public static readonly double MaxValue = 10.0;
        public static double Span => MaxValue - MinValue;

        public double LeftBoundary { get; set; } = MinValue;
        public double RightBoundary { get; set; } = MaxValue;
        public bool ClosedLeft { get; init; } = false;
        public bool ClosedRight { get; init; } = false;

        public Interval()
        {
            if (LeftBoundary < MinValue)
            {
                LeftBoundary = MinValue;
            }

            if (RightBoundary > MaxValue)
            {
                RightBoundary = MaxValue;
            }
        }

        public bool Contains(double value)
        {
            if (value < LeftBoundary)
            {
                return false;
            }
            if (value > RightBoundary)
            {
                return false;
            }
            if (value == LeftBoundary && !ClosedLeft)
            {
                return false;
            }
            if (value == RightBoundary && !ClosedRight)
            {
                return false;
            }

            return true;
        }
    }
}
