#include <cmath>
#include "Chromosome.h"



namespace chrmsm
{
	size_t Chromosome::genesCount;
	double Chromosome::precision;

	int Chromosome::a;
	int Chromosome::b;
	int Chromosome::c;
	domain::Interval* Chromosome::I;

	size_t Chromosome::crtPopSize;
	size_t Chromosome::generationSize;
	unsigned int Chromosome::idMax;
	vector<Chromosome> Chromosome::vPopulation;

	unsigned int Chromosome::eliteIdx;
	double Chromosome::eliteFitness;
	double Chromosome::totalFitness;

	double Chromosome::crossProb;
	double Chromosome::mutationProb;

}

namespace domain
{
	Interval* Interval::definitionInterval;
}

namespace in
{
	InputSingleton* InputSingleton::input;
}

string pathIn = "Evolution.in";
in::InputSingleton input = *(chrmsm::Chromosome::init(pathIn));


int main()
{
	double eliteFitness = chrmsm::Chromosome::run(input.stepsCount);




}

