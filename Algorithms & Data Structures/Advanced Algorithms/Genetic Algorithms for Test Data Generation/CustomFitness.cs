using GeneticSharp;
using MFIS.Transitions;

namespace MFIS
{
    internal class CustomFitness : IFitness
    {
        public double Evaluate(IChromosome c)
        {
            if (c is null)
            {
                return 1 - double.MinValue;
            }

            CustomChromosome chromosome = c as CustomChromosome;
            if (chromosome is null)
            {
                return 1 - double.MinValue;
            }

            List<ITransition> transitions = chromosome.Transitions;

            int? firstFailedTransitionIdx = GetFirstFailedTransitionIndex(transitions);
            
            if(firstFailedTransitionIdx is null)
            {
                // No failed transition. Chromosome has the best fitness score possible.
                return 0;
            }

            ITransition firstFailedTransition = transitions[firstFailedTransitionIdx.Value];

            int approachLevel = transitions.Count - (firstFailedTransitionIdx.Value + 1);   // the + 1 is because the idx starts from 0
            double normalizedObj = firstFailedTransition.ObjectiveFunctionNormalized();

            return approachLevel + normalizedObj;
        }

        /// <summary>
        /// Returns the index of the first failed transition or null if there is no transition failed in the list.
        /// </summary>
        /// <param name="transitions"></param>
        /// <returns></returns>
        private int? GetFirstFailedTransitionIndex(List<ITransition> transitions)
        {
            for(int i = 0; i < transitions.Count; i++)
            {
                if (!transitions[i].PreconditionsPassed())
                {
                    return i;
                }
            }

            return null;
        }
    }
}
