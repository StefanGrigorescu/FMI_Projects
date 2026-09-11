using GeneticSharp;
using System.ComponentModel;

namespace MFIS
{
    [DisplayName("Elite")]
    internal sealed class CustomEliteSelection : SelectionBase
    {
        private readonly int _previousGenerationChromosomesNumber;

        private List<IChromosome> _previousGenerationChromosomes;

        public CustomEliteSelection()
            : this(1) {}

        public CustomEliteSelection(int previousGenerationChromosomesNumber)
            : base(2)
        {
            _previousGenerationChromosomesNumber = previousGenerationChromosomesNumber;
        }

        protected override IList<IChromosome> PerformSelectChromosomes(int number, Generation generation)
        {
            if (generation.Number == 1)
            {
                _previousGenerationChromosomes = new List<IChromosome>();
            }

            _previousGenerationChromosomes.AddRange(generation.Chromosomes);
            List<IChromosome> list = _previousGenerationChromosomes.OrderBy((IChromosome c) => c.Fitness).Take(number).ToList();
            _previousGenerationChromosomes = list.Take(_previousGenerationChromosomesNumber).ToList();

            var bestChromosome = list.First() as CustomChromosome;
            Console.WriteLine($"\nBest Chromosome:\n{bestChromosome}");
            return list;
        }
    }
}
