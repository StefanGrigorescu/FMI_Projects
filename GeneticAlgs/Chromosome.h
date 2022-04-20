#pragma once
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <algorithm>
#include <random>

#include "Domain.h"
#include "InputSingleton.h"


using namespace in;
using namespace domain;


/*
Namespace of Chromosome.
*/
namespace chrmsm
{

	string pathOut = "Evolution.out";
	ofstream fout(pathOut);

	class Chromosome
	{
	public:
		Chromosome(const Chromosome& source)
		{
			id = source.getId();
			abscissa = source.getAbscissa();
			genesArr = source.getGenesArr();
			selectionInterval = source.getSelectionInterval();

		};

		/*
		Initializes the input once for the given pathIn and calls the initStatics method.
		On subsequent calls, it returns a reference to the read input.
		Returns a reference to the InputSingleton instance.
		*/
		static InputSingleton* init(string pathIn);

		static double run(size_t stepsCount);

		static vector<Chromosome> getNextGeneration();

		/*
		Calculates the probability of selecting a chromosome based on its fitness and the totalFitness.
		*/
		double getSelectionProbability(unsigned int idx);

		/*
		Calculates the fitness of the chromosome depending on its abscissa, the function coefficients and the domain boundaries.
		*/
		double getFitness(unsigned int idx);

		size_t getId() const { return id; }

		/*
		Returns a reference to a copy of the bool array pointed by the genesArr data member.
		*/
		bool* getGenesArr() const 
		{
			bool* newGenesArr = new bool[genesCount];
			for (size_t i = 0; i < genesCount; i++)
				newGenesArr[i] = genesArr[i];

			return newGenesArr;
		}

		size_t getGenesCount() const { return genesCount; }

		double getAbscissa() const { return abscissa; }

		/*
		Returns a reference to a copy of the Interval pointed by the selectionInterval data member.
		*/
		Interval* getSelectionInterval() const 
		{
			if (selectionInterval == NULL)
				return NULL;

			return new Interval(*selectionInterval); 
		}

		friend ostream& operator<<(ostream& out, vector<Chromosome>& chromosomes);

		friend ostream& operator<<(ostream& out, Chromosome& chromosome);

	private:
		Chromosome()
		{
			this->id = idMax++;

			genesArr = new bool[genesCount];
			for (size_t i = 0; i < genesCount; i++)
				genesArr[i] = rand() % 2;	// 0 or 1

			selectionInterval = NULL;

			initAbscissa();

		};

		/*
		Initializes abscissa according to genesArr, the precision and the lower bound of the interval.
		*/
		void initAbscissa();

		/*
		Initializes the static data members of the Chromosome class.
		*/
		static void initStatics(InputSingleton input);

		/*
		Calculates the eliteFitness, the totalFitness and the eliteIdx.
		*/
		static void calculateFitnesses();

		static void setSelectionIntervals();

		friend void crossover(Chromosome& c1, Chromosome& c2);

		friend void crossover(Chromosome& c1, Chromosome& c2, Chromosome& c3);

		friend void mutationAll(vector<Chromosome>& vNextGeneration);

		/*
		Has a chance of applying a mutation to any chromosome but the elite chromosome.
		*/
		void mutation(unsigned int idx);

		void show(ostream& out, unsigned int idx);

		/*
		Retrieves the idx of the chromosome in vPopulation by its selectionInterval and found is set to true.
		If no selectionInterval contains the selectionRand parameter, one bound of the array is returned (0 or crtPopSize-1) and found is set to false.
		*/
		static unsigned int getIdx(double selectionRand, bool& found);

		/*
		Increments the static idMax and assigns its value to the current chromosome's id.
		*/
		void setNextId();


		unsigned int id;											// Unique identifier of the chromosome among all the generations.
		double abscissa;										// The chromosome value as a an abscissa (base 10).
		bool* genesArr;										// The binary array representing the genes of the chromosome.
		Interval* selectionInterval;						// The selection interval corresponding to the given chromosome.
		static size_t genesCount;							// The total number of genes, determining the genesArr's size.
		static double precision;							// Influences the density of chromosomes in a unit of the definition interval.

		static int a;												// The function coefficients.
		static int b;												// The function coefficients.
		static int c;												// The function coefficients.
		static Interval* I;									// The function definition domain, as an interval.

		static size_t crtPopSize;							// The size of the current population.
		static size_t generationSize;					// The size of a generation.
		static unsigned int idMax;						// Used for initializing the unique id of the chromosomes.
		static vector<Chromosome> vPopulation;	// Vector containing the current chromosomes population.

		static unsigned int eliteIdx;						// The index of the elite chromosome of the generation.
		static double eliteFitness;						// The best fitness of all the chromosomes of all generations until now.
		static double totalFitness;						// The total fitness of all the chromosomes of the current generation.
		
		static double crossProb;							// The crossover probability.
		static double mutationProb;						// The mutation probability.

	};


	InputSingleton* Chromosome::init(string pathIn)
	{
		InputSingleton* input = InputSingleton::getInput(pathIn);

		// If the definition interval is NULL, it is because it was never initialized and the initStatics method is called.
		if (I == NULL)
			initStatics(*input);
		else
			cout << "The class was already initialized once.";
		return input;
	}

	double Chromosome::run(size_t stepsCount)
	{
		for (size_t i = 0; i < stepsCount; i++)
		{
			getNextGeneration();

			fout << "\n\n eliteFitness = " << eliteFitness << " \n";
			cout << "\n\n eliteFitness = " << eliteFitness << " \n";
		}

		return eliteFitness;
	}

	/*
	Crossovers the chromosomes in vNextGeneration whose indexes are in vCrossoverIdxs.
	*/
	void crossoverAll(vector<Chromosome>& vNextGeneration, vector<unsigned int>& vCrossoverIdxs)
	{
		auto rng = std::default_random_engine{};
		std::shuffle(std::begin(vCrossoverIdxs), std::end(vCrossoverIdxs), rng);

		while (vCrossoverIdxs.size() > 3)
		{
			size_t lastIdx = vCrossoverIdxs.size() - 1;

			// Crossover the chromosomes with the last two indexes in vCrossoverIdx.
			crossover(vNextGeneration[vCrossoverIdxs[lastIdx]], vNextGeneration[vCrossoverIdxs[lastIdx - 1]]);

			vCrossoverIdxs.pop_back();
			vCrossoverIdxs.pop_back();
		}

		if (vCrossoverIdxs.size() == 3)
		{
			size_t lastIdx = vCrossoverIdxs.size() - 1;

			// Crossover the chromosomes with the last three indexes in vCrossoverIdx.
			crossover(vNextGeneration[vCrossoverIdxs[lastIdx]], vNextGeneration[vCrossoverIdxs[lastIdx - 1]], vNextGeneration[vCrossoverIdxs[lastIdx - 2]]);
		}
		else if (vCrossoverIdxs.size() == 2)
		{
			size_t lastIdx = vCrossoverIdxs.size() - 1;

			// Crossover the chromosomes with the last two indexes in vCrossoverIdx.
			crossover(vNextGeneration[vCrossoverIdxs[lastIdx]], vNextGeneration[vCrossoverIdxs[lastIdx - 1]]);
		}

		fout << "Population after crossover: \n";
		for (size_t i = 0; i < vNextGeneration.size(); i++)
			fout << vNextGeneration[i] << " \n";

	}

	/*
	Calculates a random mutation probability for every chromosome in vNextGeneration and applies mutations if necessary.
	*/
	void mutationAll(vector<Chromosome>& vNextGeneration)
	{
		for(size_t idx = 0; idx < vNextGeneration.size(); idx++)
		{
			double mutationChance = ((double)rand() / (double)RAND_MAX);
			if (mutationChance < Chromosome::mutationProb)
				vNextGeneration[idx].mutation(idx);
		}

		fout << "Population after mutations: \n";
		for (auto chromosome : vNextGeneration)
			fout << chromosome << " \n";

	}

	vector<Chromosome> Chromosome::getNextGeneration()
	{
		fout << "Current population max value: " << eliteFitness << " \n";
		fout << "Current population average value: " << totalFitness / crtPopSize << " \n";

		setSelectionIntervals();

		vector<Chromosome> vNextGeneration;
		vNextGeneration.reserve(generationSize);
		vNextGeneration.push_back(vPopulation[eliteIdx]);

		vector<unsigned int> vCrossoverIdxs;		// The indexs of the chromosomes in vNextGeneration to participate to the crossover.

		for (size_t i = 1; i < generationSize; i++)
		{
			double selectionRand = ((double)rand() / (double)RAND_MAX);
			bool found;
			unsigned int idx = getIdx(selectionRand, found);
			if (!found)
				continue;
			
			vNextGeneration.push_back(vPopulation[idx]);
			size_t lastIdx = vNextGeneration.size() - 1;
			vNextGeneration[lastIdx].setNextId();			// get a new id for the candidate chromosome
			fout << "Selected candidate for next generation:   " << vNextGeneration[lastIdx] << " \n";

			double crossoverChance = ((double)rand() / (double)RAND_MAX);
			if (crossoverChance < crossProb)
			{
				vCrossoverIdxs.push_back(lastIdx);
				fout << "Selected for crossover:   " << vNextGeneration[lastIdx] << " \n";
			}
		}

		crossoverAll(vNextGeneration, vCrossoverIdxs);
		mutationAll(vNextGeneration);
		fout << " \n -------------------------------- \n\n";

		vPopulation = vNextGeneration;
		crtPopSize = vNextGeneration.size();
		calculateFitnesses();
		return vNextGeneration;
	}

	double Chromosome::getSelectionProbability(unsigned int idx)
	{
		if (idx == eliteIdx)	// The elite chromosome does not participate to any selection protocol.
			return 0;
		return getFitness(idx) / (totalFitness - eliteFitness);
	}

	double Chromosome::getFitness(unsigned int idx)
	{
		try
		{
			const double x = abscissa;

			if (!I->contains(x))
				throw '0';

			double fitness = a * x * x + b * x + c;
			if (fitness < 0)
				throw '1';

			return fitness;
		}
		catch (char e)
		{
			cout << idx << ": x = " << abscissa << ", ";// << e;
			if (e == '0')
				cout << "The x given for the current individual does not fall within the given definition domain." << " \n";
			else if(e == '1')
				cout << "The function coefficients were not initialized correctly, because the function should be strictly positive on the given definition domain." << " \n";

			return 0;
		}
	}

	ostream& operator<<(ostream& out, vector<Chromosome>& chromosomes)
	{
		for (size_t i = 0; i < chromosomes.size(); i++)
			out << chromosomes[i] << "\n";
		out << "\n";

		return out;
	}

	ostream& operator<<(ostream& out, Chromosome& chromosome)
	{
		chromosome.show( out, chromosome.id );
		return out;
	}

	void Chromosome::initAbscissa()
	{

		double x = 0;
		int power2 = 1;
		// Transform the number represented by genesArr from base 2 to base 10.
		for (int i = genesCount - 1; i >= 0; i--)
		{
			if (genesArr[i])
				x += power2;
			power2 <<= 1;
		}


		int num = (1 << genesCount);
		x /= num;
		x *= (I->xMax - I->xMin);
		x += I->xMin;

		abscissa = x;

	}

	void Chromosome::initStatics(InputSingleton input)
	{
		srand((unsigned)time(NULL));
		precision = input.precision;
		//	2^genesCount	=	(b-a)*10^precision    <=>    genesCount = log2( (xMax-xMin)*10^precision )
		double aux = log2((input.xMax - input.xMin) * pow(10, input.precision));
		genesCount = aux + 1;	// genesCount might have been rounded down by the double to size_t conversion

		a = input.a;
		b = input.b;
		c = input.c;
		I = domain::Interval::getDefinitionInterval(input.xMin, input.xMax, input.closedLeft, input.closedRight);
		generationSize = input.generationSize;
		crossProb = input.crossProb;
		mutationProb = input.mutationProb;
		crtPopSize = generationSize + rand() % generationSize + 1;		// The initial population has up to 2 * generationSize chromosomes.
		idMax = 0;

		vPopulation = vector<Chromosome>();
		vPopulation.reserve(crtPopSize);
		for (size_t i = 0; i < crtPopSize; i++)
			vPopulation.push_back(Chromosome());

		calculateFitnesses();

	}

	void Chromosome::calculateFitnesses()
	{
		eliteFitness = -1 * numeric_limits<double>::infinity();
		totalFitness = 0;

		for (size_t i = 0; i < crtPopSize; i++)
		{
			double crtFitness = vPopulation[i].getFitness(i);
			if (eliteFitness < crtFitness)
			{
				eliteFitness = crtFitness;
				eliteIdx = i;
			}
			totalFitness += crtFitness;
		}

	}

	void Chromosome::setSelectionIntervals()
	{
		if (crtPopSize)
		{
			double selectionProb = vPopulation[0].getSelectionProbability(0);
			vPopulation[0].selectionInterval = new Interval(0, 0 + selectionProb, true, false);
			fout << "0:   selectionProbability = " << selectionProb << ";   selectionInterval = " << vPopulation[0].selectionInterval << " \n";

			for (size_t i = 1; i < crtPopSize; i++)
			{
				selectionProb = vPopulation[i].getSelectionProbability(i);
				double lowerBound = vPopulation[i - 1].selectionInterval->xMax;
				vPopulation[i].selectionInterval = new Interval(lowerBound, lowerBound + selectionProb, true, false);

				fout << i << ":   selectionProbability = " << selectionProb << ";   selectionInterval = " << vPopulation[i].selectionInterval << " \n";
			}
		}

	}

	void crossover(Chromosome& c1, Chromosome& c2)
	{
		unsigned int breakpoint = rand() % Chromosome::genesCount;
		fout << "Crossover with breakpoint = " << breakpoint << " between:     " << c1 << "     and     " << c2 << " \n";
		for (size_t i = breakpoint; i < Chromosome::genesCount; i++)
			swap(c1.genesArr[i], c2.genesArr[i]);

		fout << "    Resulting      " << c1 << "     and     " << c2;

		c1.initAbscissa();
		c2.initAbscissa();
	}

	void crossover(Chromosome& c1, Chromosome& c2, Chromosome& c3)
	{
		unsigned int breakpoint = rand() % Chromosome::genesCount;
		fout << "Crossover with breakpoint = " << breakpoint << " between:     " << c1 << "     and     " << c2 << "     and     " << c3 << " \n";
		for (size_t i = breakpoint; i < Chromosome::genesCount; i++)
		{
			swap(c1.genesArr[i], c2.genesArr[i]);
			swap(c2.genesArr[i], c3.genesArr[i]);
		}

		fout << "    Resulting      " << c1 << "     and     " << c2 << "     and     " << c3;

		c1.initAbscissa();
		c2.initAbscissa();
		c3.initAbscissa();
	}

	void Chromosome::mutation(unsigned int idx)
	{
		if (idx != eliteIdx)
		{
			unsigned int mutationIdx = rand() % genesCount;
			genesArr[mutationIdx] = ~(genesArr[mutationIdx]);

			initAbscissa();
		}

	}

	void Chromosome::show(ostream& out, unsigned int idx)
	{
		out << idx << ": f(" << abscissa << ") = " << getFitness(idx);
	}

	unsigned int Chromosome::getIdx(double selectionRand, bool& found)
	{
		found = true;

		unsigned int power2 = 1;
		while (power2 < crtPopSize)
			power2 <<= 1;

		unsigned int idx = 0;
		if (idx < crtPopSize && vPopulation[idx].selectionInterval->contains(selectionRand))
			return idx;

		// Find the index of the last interval to be entirely lower than selectionRand.
		while (power2)
		{
			if (idx + power2 < crtPopSize && vPopulation[idx + power2].selectionInterval->xMax <= selectionRand)
				idx += power2;
			power2 >>= 1;
		}

		// The case when selectionRand = xMax of the current interval (which is right closed) and the following interval does not contain xMax.
		if (vPopulation[idx].selectionInterval->contains(selectionRand))		
			return idx;

		if (idx + 1 < crtPopSize)
		{
			idx += 1;

			if (!vPopulation[idx].selectionInterval->contains(selectionRand))
			{
				cout << "The selection interval was not found.\n";
				found = false;
			}
		}

		return idx;
	}
	
	void Chromosome::setNextId()
	{
		id = idMax++;
	}

}
