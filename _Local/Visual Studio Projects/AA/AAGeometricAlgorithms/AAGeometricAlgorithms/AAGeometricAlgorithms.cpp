#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <cmath>


enum class Orientation: int { right = -1, touch = 0, left = 1 };


std::ostream& operator<<(std::ostream& out, const Orientation& val)
{
	switch (val)
	{
	case Orientation::left:
		return out << "left";
	case Orientation::touch:
		return out << "touch";
	default:
		return out << "right";
	}
}


class Point
{
public:
	Point(double x = 0, double y = 0) : x(x), y(y) {}
	Point(const Point& source) : x(source.getX()), y(source.getY()) {}

	bool belongsToSegment(const Point& firstBound, const Point& secondBound) const;
	double getDistance(const Point& other) const;

	Point& operator=(const Point& source) 
	{
		setPoint(source.getX(), source.getY());
		return *this; 
	}

	double getX() const { return x; }
	double getY() const { return y; }

	friend std::istream& operator>>(std::istream& in, Point& point);
	friend std::ostream& operator<<(std::ostream& out, const Point& point);

	friend bool operator ==(const Point& p0, const Point& p1);

	friend bool operator <(const Point& p0, const Point& p1);

private:
	void setPoint(double x, double y);

	const double x;
	const double y;
};

bool Point::belongsToSegment(const Point& firstBound, const Point& secondBound) const
{
	return (firstBound.getDistance(secondBound) == (firstBound.getDistance(*this) + getDistance(secondBound)));
}

double Point::getDistance(const Point& other) const
{
	double distance = sqrt(std::pow(x - other.getX(), 2) + std::pow(y - other.getY(), 2));
	return distance;
}

std::istream& operator>>(std::istream& in, Point& point)
{
	double x, y;
	in >> x >> y;
	point.setPoint(x, y);

	return in;
}

std::ostream& operator<<(std::ostream& out, const Point& point)
{
	out << "(" << point.getX() << ", " << point.getY() << ")";
	return out;
}

bool operator ==(const Point& p0, const Point& p1)
{
	return p0.x == p1.x && p0.y == p1.y;
}

bool operator <(const Point& p0, const Point& p1)
{
	return p0.x < p1.x || (p0.x == p1.x && p0.y < p1.y);
}

void Point::setPoint(double x, double y)
{
	double* changeableX = const_cast<double*> (&(this->x));
	*changeableX = x;
	double* changeableY = const_cast<double*>(&(this->y));
	*changeableY = y;
}


template<typename T>
int compare(T obj0, T obj1)
{
	if (obj0 < obj1)
		return -1;
	if (obj0 == obj1)
		return 0;
	return 1;
}

int compare(double obj0, double obj1)
{
	if (obj0 < obj1)
		return -1;
	if (obj0 == obj1)
		return 0;
	return 1;
}

int compare(const Point& p0, const Point& p1)
{
	std::cout << " CALLED compare for points here !!!! " << "\n";
	if (p0 < p1)
		return -1;
	if (p0 == p1)
		return 0;
	return 1;
}


class Cover
{
public:
	static std::vector<Point> getCover(std::vector<Point> points);

	static Orientation getRotation(const Point& p, const Point& q, const Point& r);

	static double getDet(const Point& p, const Point& q, const Point& r);

};

std::vector<Point> Cover::getCover(std::vector<Point> points)
{
	std::vector<Point> cover;

	std::sort(points.begin(), points.end());
	size_t len = points.size();
	if (len < 3)
		return points;

	cover.push_back(points[0]);
	cover.push_back(points[1]);

	for (size_t i = 2; i < len; i++)
	{
		Point r = points[i];

		while (true)
		{
			if (cover.size() < 2)	
				break;

			Point q = cover[cover.size() - 1];
			Point p = cover[cover.size() - 2];

			if (getRotation(p, q, r) == Orientation::left)
				break;
			
			cover.pop_back();
		}

		cover.push_back(r);
	}

	size_t lowerCoverLen = cover.size();
	for (int i = len - 1; i >= 0; i--)
	{
		Point r = points[i];

		while (true)
		{
			if (cover.size() < lowerCoverLen + 1)
				break;

			Point q = cover[cover.size() - 1];
			Point p = cover[cover.size() - 2];

			if (getRotation(p, q, r) == Orientation::left)
				break;

			cover.pop_back();
		}

		if( !(r==cover[cover.size()-1]))
			cover.push_back(r);
	}

	if(cover.size() > 2)
		cover.pop_back();	

	return cover;
}

Orientation Cover::getRotation(const Point& p, const Point& q, const Point& r)
{
	double det = getDet(p, q, r);
	if (det < 0)
		return Orientation(-1);
	if (det > 0)
		return Orientation(1);
	return Orientation(0);

}

double Cover::getDet(const Point& p, const Point& q, const Point& r)
{
	double det = 0.5 * ((p.getX() - r.getX()) * (q.getY() - r.getY()) - (q.getX() - r.getX()) * (p.getY() - r.getY()));
	return det;
}


template<typename T>
T nextElem(std::vector<T> v, int idx)
{
	return v[(idx + 1) % v.size()];
}


std::vector<Point> readAndSortPoints(int n, int& idxRight)
{
	std::vector<Point> points;

	int idxLeft;
	double xMin = 1e9 + 1, xMax = -1e9 - 1, yMin = 1e9 + 1, yMax = -1e9 - 1;
	for (int i = 0; i < n; i++)
	{
		Point point;
		std::cin >> point;
		points.push_back(point);
		if (xMin > points[i].getX() || (xMin == points[i].getX() &&  yMin > points[i].getY()))
		{
			xMin = points[i].getX();
			yMin = points[i].getY();
			idxLeft = i;
		}
		if (xMax < points[i].getX() || (xMax == points[i].getX() && yMax < points[i].getY()) )
		{
			xMax = points[i].getX();
			yMax = points[i].getY();
			idxRight = i;
		}
	}
	std::vector<Point> pointsSorted;
	for (size_t i = idxLeft; i < points.size(); i++)
		pointsSorted.push_back(points[i]);

	for (size_t i = 0; i < idxLeft; i++)
		pointsSorted.push_back(points[i]);

	idxRight = (idxRight + points.size() - idxLeft) % points.size();
	return pointsSorted;
}


class PivotPointComparator
{
public:
	PivotPointComparator(const Point*& pivot) : pivot(pivot) {}

	// Returns 1 if the other point is compared as greater, relatively to the xOrientation, than the pivot on X axis, -1 if less, 0 if equal.
	int compareOnX(const Point& other, int xOrientation = 1)
	{
		return compare(other.getX() * xOrientation, pivot->getX() * xOrientation);
	}

	// Returns 1 if the other point is compared as greater, relatively to the yOrientation, than the pivot on Y axis, -1 if less, 0 if equal.
	int compareOnY(const Point& other, int yOrientation = 1)
	{
		return compare(other.getY() * yOrientation, pivot->getY() * yOrientation);
	}

private:
	const Point* pivot;
};


// It is supposed that the given vector was sorted before calling this binarySearch method.
// idxLeft and idxRight must be natural numbers.
// the interval is closed left and right.
int binarySearch(std::vector<Point> points, int idxLeft, int idxRight, const Point*& targetPoint, int xOrientation = 1)
{
	PivotPointComparator cmp(targetPoint);

	if (cmp.compareOnX(points[idxLeft], xOrientation) == 1  ||  cmp.compareOnX(points[idxRight], xOrientation) == -1)
		return -1;

	int power2 = 1;
	while (power2 < idxRight - idxLeft + 1)
		power2 <<= 1;

	while (power2)
	{
		int crtIdx = idxLeft + power2;
		if (idxRight < crtIdx)
		{
			power2 >>= 1;
			continue;
		}			

		switch (cmp.compareOnX(points[crtIdx], xOrientation))
		{
		case 1:
			idxRight = crtIdx;
		break;
		case -1:
			idxLeft = crtIdx;
		}
		power2 >>= 1;
	}

	while (idxLeft < idxRight  &&  cmp.compareOnX(points[idxLeft], xOrientation) != 0)
		idxLeft++;
	while (idxLeft < idxRight  &&  cmp.compareOnX(points[idxRight], xOrientation) != 0)
		idxRight--;

	int yOrientation = 1;
	if (points[idxRight].getY() < points[idxLeft].getY())
		yOrientation *= -1;

	if (cmp.compareOnY(points[idxLeft], yOrientation) == 1)
		return idxLeft;	
	if (cmp.compareOnY(points[idxRight], yOrientation) == -1)
		return idxRight;

	power2 = 1;
	while (power2 < idxRight - idxLeft + 1)
		power2 <<= 1;

	while (power2)
	{
		int crtIdx = idxLeft + power2;
		if (idxRight < crtIdx)
		{
			power2 >>= 1;
			continue;
		}

		switch (cmp.compareOnY(points[crtIdx], yOrientation))
		{
		case 1:
		{
			idxRight = crtIdx;
		}
		break;
		case -1:
		{
			idxLeft = crtIdx;
			if (targetPoint->belongsToSegment(points[idxLeft], nextElem(points, idxLeft)))
				return idxLeft;
		}
		}
		
		power2 >>= 1;
	}

	return idxLeft;
}


int main()
{
	int n;
	std::cin >> n;
	int inflexionPoint;
	std::vector<Point> points = readAndSortPoints(n, inflexionPoint);

	int m;
	std::cin >> m;
	for (int i = 0; i < m; i++)
	{
		Point crtCheck;
		std::cin >> crtCheck;

		//bool done = false;
		//for (size_t i = 0; i < points.size(); i++)
		//{
		//	Orientation rotation = Cover::getRotation(points[i], nextElem(points, i), crtCheck);

		//	if (rotation == Orientation::right)
		//	{
		//		std::cout << "OUTSIDE";
		//		done = true;
		//		break;
		//	}
		//	else if (rotation == Orientation::touch  &&  crtCheck.belongsToSegment(points[i], nextElem(points, i)))
		//	{
		//		std::cout << "BOUNDARY";
		//		done = true;
		//		break;
		//	}
		//}
		//if (!done)
		//	std::cout << "INSIDE";
		//std::cout << "\n";

		if (crtCheck.belongsToSegment(points[points.size() - 1], points[0]))
		{
			std::cout << "BOUNDARY\n";
			continue;
		}

		const Point* targetPoint = &crtCheck;
		int lowerBoundIdx = binarySearch(points, 0, inflexionPoint, targetPoint);
		if (lowerBoundIdx < 0)
		{
			std::cout << "OUTSIDE\n";
			continue;
		}
		if (crtCheck.belongsToSegment(points[lowerBoundIdx], nextElem(points, lowerBoundIdx)))
		{
			std::cout << "BOUNDARY\n";
			continue;
		}
		if (Cover::getRotation(points[lowerBoundIdx], nextElem(points, lowerBoundIdx), crtCheck) != Orientation::left)
		{
			std::cout << "OUTSIDE\n";
			continue;
		}

		lowerBoundIdx = binarySearch(points, inflexionPoint, points.size() - 1, targetPoint, -1);
		if (lowerBoundIdx < 0)
		{
			std::cout << "OUTSIDE\n";
			continue;
		}
		if (crtCheck.belongsToSegment(points[lowerBoundIdx], nextElem(points, lowerBoundIdx)))
		{
			std::cout << "BOUNDARY\n";
			continue;
		}
		if (Cover::getRotation(points[lowerBoundIdx], nextElem(points, lowerBoundIdx), crtCheck) != Orientation::left)
		{
			std::cout << "OUTSIDE\n";
			continue;
		}
		std::cout << "INSIDE\n";
	}

	return 0;
}
