#pragma once
#include <iostream>


using namespace std;


/*
Namepace of definition domain.
*/
namespace domain
{
	class Domain
	{
	public:
		virtual bool contains(const double& x) = 0;

		virtual void show(ostream& out) = 0;
		friend ostream& operator<<(ostream& out, Domain& domain);
		friend ostream& operator<<(ostream& out, Domain*& domain);

	};

	ostream& operator<<(ostream& out, Domain& domain)
	{
		domain.show(out);
		return out;
	}	
	
	ostream& operator<<(ostream& out, Domain*& domain)
	{
		(*domain).show(out);
		return out;
	}


	class Interval : Domain
	{
	public:
		bool contains(const double& x);

		Interval(const double xMin, const double xMax, const bool closedLeft = false, const bool closedRight = false) :
			xMin(xMin), xMax(xMax), closedLeft(closedLeft), closedRight(closedRight) {};

		Interval(const Interval& source) :
			xMin(source.xMin), xMax(source.xMax), closedLeft(source.closedLeft), closedRight(source.closedRight) {};

		/*
		Initializes the definitionInterval once, then returns the reference to the static definitionInterval instance.
		*/
		static Interval* getDefinitionInterval(const double xMin, const double xMax, const bool closedLeft = false, const bool closedRight = false)
		{
			if (definitionInterval == NULL)
				definitionInterval = new Interval(xMin, xMax, closedLeft, closedRight);
			return definitionInterval;
		}

		void show(ostream& out);
		friend ostream& operator<<(ostream& out, Interval& I);
		friend ostream& operator<<(ostream& out, Interval*& I);

		const double xMin, xMax;
		const bool closedLeft, closedRight;

	private:
		static Interval* definitionInterval;

	};

	bool Interval::contains(const double& x)
	{
		if (xMin == xMax and (!closedLeft or !closedRight))  // Degenerate interval with an open boundary (void interval).
			return false;
		return ((xMin < x) || (xMin == x && closedLeft)) && ((x < xMax) || (x == xMax && closedRight));
	}

	ostream& operator<<(ostream& out, Interval& I)
	{
		I.show(out);
		return out;
	}

	ostream& operator<<(ostream& out, Interval*& I)
	{
		(*I).show(out);
		return out;
	}

	void Interval::show(ostream& out)
	{
		if (closedLeft)
			out << "[";
		else
			out << "(";

		out << xMin << ", " << xMax;

		if (closedRight)
			out << "]";
		else
			out << ")";

	}

}
