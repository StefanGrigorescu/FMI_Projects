#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <cmath>


enum class Orientation : int { right = -1, touch = 0, left = 1 };


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

		if (!(r == cover[cover.size() - 1]))
			cover.push_back(r);
	}

	if (cover.size() > 2)
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


std::vector<Point> readPoints(int n, int& allXMaxIdx, int& allYMaxIdx)
{
	std::vector<Point> points;

	double allXMax = -1e9 - 1, allYMax = -1e9 - 1;
	for (int i = 0; i < n; i++)
	{
		Point point;
		std::cin >> point;
		points.push_back(point);

		if (allXMax < points[i].getX())
		{
			allXMax = points[i].getX();
			allXMaxIdx = i;
		}
		if (allYMax < points[i].getY())
		{
			allYMax = points[i].getY();
			allYMaxIdx = i;
		}
	}

	return points;
}


int previousIdx(int idx, int len)
{
	if (idx > 0)
		return idx - 1;
	return len - 1;
}


int main()
{
	int n;
	std::cin >> n;
	int allXMaxIdx, allYMaxIdx;
	std::vector<Point> points = readPoints(n, allXMaxIdx, allYMaxIdx);
	const size_t len = points.size();

	size_t i;
	int crtIdx = allXMaxIdx + 1;
	for (i = 0; i < len; i++)
	{
		crtIdx %= len;
		if (points[previousIdx(crtIdx, len)].getX() < points[crtIdx].getX())
			break;

		crtIdx++;
	}

	bool printYes = true;
	for (; i < len; i++)
	{
		crtIdx %= len;
		if (points[previousIdx(crtIdx, len)].getX() > points[crtIdx].getX())
		{
			std::cout << "NO\n";
			printYes = false;
			break;
		}

		crtIdx++;
	}
	if(printYes)
		std::cout << "YES\n";

	crtIdx = allYMaxIdx + 1;
	for (i = 0; i < len; i++)
	{
		crtIdx %= len;
		if (points[previousIdx(crtIdx, len)].getY() < points[crtIdx].getY())
			break;

		crtIdx++;
	}

	for (; i < len; i++)
	{
		crtIdx %= len;
		if (points[previousIdx(crtIdx, len)].getY() > points[crtIdx].getY())
		{
			std::cout << "NO\n";
			return 0;
		}

		crtIdx++;
	}
	std::cout << "YES\n";

	return 0;
}
