using GeneticSharp;

namespace MFIS.Transitions
{
    internal class ConditionedTransition : ITransition
    {
        public Label Label { get; init; }
        /// <summary>
        /// The valid interval for the Input, as a real numbers interval.
        /// </summary>
        public Interval DomainInterval { get; init; }
        public double Input { get; init; }

        public ConditionedTransition(Label label, Gene input, Interval domainInterval) : base()
        {
            Label = label;
            Input = (double)input.Value;
            DomainInterval = domainInterval;
        }

        public bool PreconditionsPassed() => DomainInterval.Contains(Input);

        public double ObjectiveFunctionNormalized()
        {
            return ObjectiveFunction() / Interval.Span;
        }

        private double ObjectiveFunction()
        {
            if (PreconditionsPassed())
            {
                return 0;
            }

            if (Input <= DomainInterval.LeftBoundary)
            {
                return DomainInterval.LeftBoundary - Input;
            }

            return Input - DomainInterval.RightBoundary;
        }
    }
}
