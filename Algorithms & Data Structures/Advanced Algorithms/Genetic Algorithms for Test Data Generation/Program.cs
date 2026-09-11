using GeneticSharp;
using MFIS;

// Create the fitness function
var fitness = new CustomFitness();

// Create the GA operators
var selection = new CustomEliteSelection();
var crossover = new UniformCrossover();
var mutation = new FlipBitMutation();   // better alternative than randomly regenerating the chromosome

// Create the GA parameters
var populationSize = 300;
var generations = 1000;

// Create the GA
var ga = new GeneticAlgorithm(
    new Population(populationSize, populationSize, new CustomChromosome()),
    fitness,
    selection,
    crossover,
    mutation);

ga.Termination = new GenerationNumberTermination(generations);

// Start the GA
ga.Start();
