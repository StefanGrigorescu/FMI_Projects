#pragma once
#include <fstream>


using namespace std;


/*
Namespace of input.
*/
namespace in
{

	class InputSingleton
	{
	public:

		/*
		Initializes the input once for the given pathIn.
		On subsequent calls, it returns a reference to the static InputSingleton instance.
		*/
		static InputSingleton* getInput(string pathIn)
		{
			if (input == NULL)
			{
				ifstream fin(pathIn);

				size_t generationSize;
				fin >> generationSize;

				double xMin, xMax;
				bool closedLeft, closedRight;
				fin >> xMin >> xMax >> closedLeft >> closedRight;

				int a, b, c;
				fin >> a >> b >> c;

				double precision;
				fin >> precision;

				double crossProb;
				fin >> crossProb;

				double mutationProb;
				fin >> mutationProb;

				size_t stepsCount;
				fin >> stepsCount;

				input = new InputSingleton(generationSize, xMin, xMax, closedLeft, closedRight, a, b, c, precision, crossProb, mutationProb, stepsCount);

			}

			return input;
		}

		const size_t generationSize;			// The size of a generation.
		const double xMin, xMax;					// The boundaries of the definition domain, as an interval.
		const bool closedLeft, closedRight;	// Determines whether the definition interval is closed at that end or not.
		const int a, b, c;								// The function coefficients.
		const double precision;						// Influences the density of chromosomes in a unit of the definition interval.
		const double crossProb;					// The crossover probability.
		const double mutationProb;				// The mutation probability.
		const size_t stepsCount;					// The number of steps the algorithm will run.

	private:
		InputSingleton(const size_t generationSize, const double xMin, const double xMax, const bool closedLeft, const bool closedRight, const int a, const int b, const int c, const double precision, const double crossProb, const double mutationProb, const size_t stepsCount) :
			generationSize(generationSize), xMin(xMin), xMax(xMax), closedLeft(closedLeft), closedRight(closedRight), a(a), b(b), c(c), precision(precision), crossProb(crossProb), mutationProb(mutationProb), stepsCount(stepsCount) {}

		static InputSingleton* input;				// Reference to the singleton instance of the InputUtility class.

	};

}
