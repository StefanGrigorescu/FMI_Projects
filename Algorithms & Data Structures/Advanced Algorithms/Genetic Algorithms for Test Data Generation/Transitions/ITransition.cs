namespace MFIS.Transitions
{
    internal interface ITransition
    {
        public bool PreconditionsPassed();
        /// <summary>
        ///
        /// </summary>
        /// <returns>The projection of the ObjectiveFunction result from Interval's max span to the [0,1) interval</returns>
        public double ObjectiveFunctionNormalized();
    }
}
