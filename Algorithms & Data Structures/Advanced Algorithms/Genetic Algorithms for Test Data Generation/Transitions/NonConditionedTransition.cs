namespace MFIS.Transitions
{
    internal class NonConditionedTransition : ITransition
    {
        public Label Label { get; init; }

        public bool PreconditionsPassed() => true;
        public double ObjectiveFunctionNormalized() => 0;
    }
}
