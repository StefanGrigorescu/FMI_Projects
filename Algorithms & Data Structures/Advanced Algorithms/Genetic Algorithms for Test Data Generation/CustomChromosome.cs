using GeneticSharp;
using MFIS.Transitions;
using System;

namespace MFIS
{
    internal class CustomChromosome : ChromosomeBase, IBinaryChromosome
    {
        private const int GeneCount = 4;
        private const double BitMutationProbability = 0.2;
        public List<ITransition> Transitions { get; init; }

        private readonly Random random = new();

        public CustomChromosome() : base(GeneCount)
        {
            CreateGenes();

            // idle
            // accelerate (a > 0)
            // accelerate (a = 0)
            // accelerate (a > 0)
            // accelerate (a < 0)
            // stop

            Transitions = new()
            {
                TransitionsFactory.Accelerating(GetGene(0)),
                TransitionsFactory.Maintaining(GetGene(1)),
                TransitionsFactory.Accelerating(GetGene(2)),
                TransitionsFactory.Decelerating(GetGene(3)),
                TransitionsFactory.Stopping()
            };
        }

        public override IChromosome CreateNew()
        {
            return new CustomChromosome();
        }

        public override Gene GenerateGene(int geneIndex)
        {
            var geneValue = RandomizationProvider.Current.GetDouble(Interval.MinValue, Interval.MaxValue);
            return new Gene(geneValue);
        }

        public double GetAccParameter(int functionCallIndex)
        {
            var geneValue = (double)GetGene(functionCallIndex).Value;
            return geneValue;
        }

        public void SetAccParameter(int functionCallIndex, double value)
        {
            ReplaceGene(functionCallIndex, new Gene(value));
        }

        public void FlipGene(int index)
        {
            Gene gene = GetGene(index);

            double mutatedDouble = MutateDouble((double)gene.Value);
            if(mutatedDouble < Interval.MinValue)
            {
                mutatedDouble = Interval.MinValue;
            }
            if(mutatedDouble > Interval.MaxValue)
            {
                mutatedDouble = Interval.MaxValue;
            }

            ReplaceGene(index, new Gene(mutatedDouble));
        }

        private double MutateDouble(double value)
        {
            return -1 * value;

            /*byte[] bytes = BitConverter.GetBytes(value);

            for (int i = 0; i < bytes.Length * 8; i++)
            {
                if (random.NextDouble() > BitMutationProbability)
                {
                    continue;
                }

                int byteIndex = i / 8;
                int bitIndex = i % 8;
                byte mask = (byte)(1 << bitIndex);
                bytes[byteIndex] ^= mask;
            }

            return BitConverter.ToDouble(bytes, 0);*/
        }


        public override string ToString()
        {
            string genesArray = string.Join(
                ", ",
                Enumerable
                    .Range(0, Length)
                    .Select(i => GetGene(i).Value));
            
            return $"   Genes Array: [{genesArray}]\n   Fitness: {Fitness}";
        }
    }
}
