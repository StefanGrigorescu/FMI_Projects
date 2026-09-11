#include <iostream>
#include <cmath>


enum class Circle { INSIDE = 1, BOUNDARY = 0, OUTSIDE = -1 };


void print(double value, std::string name)
{
	std::cout << name << " = " << value << " \n";
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


double getDet(double a11, double a12, double a21, double a22)
{
	return a11 * a22 - a21 * a12;
}

double getDet(double a11, double a12, double a13, double a21, double a22, double a23, double a31, double a32, double a33)
{
	return a11 * getDet(a22, a23, a32, a33) - a12 * getDet(a21, a23, a31, a33) + a13 * getDet(a21, a22, a31, a32);
}

double getDet4(double a11, double a12, double a21, double a22, double a31, double a32, double a41, double a42)
{
	//std::string
	//	a0 = "a11", a1 = "a12", a2 = "a13",
	//	b0 = "a21", b1 = "a22", b2 = "a23",
	//	c0 = "a31", c1 = "a32", c2 = "a33",
	//	d0 = "a41", d1 = "a42", d2 = "a43";

	//std::cout << "-1.0 * (" << a2 << "-" << d2 << ") * (" << b1 << "-" << d1 << ") * (" << c0 << "-" << d0 <<
	//	") + (" << a1 << "-" << d1 << ") * (" << b2 << "-" << d2 << ") * (" << c0 << "-" << d0 <<
	//	") + (" << a2 << "-" << d2 << ") * (" << b0 << "-" << d0 << ") * (" << c1 << "-" << d1 <<
	//	") - (" << a0 << "-" << d0 << ") * (" << b2 << "-" << d2 << ") * (" << c1 << "-" << d1 <<
	//	") - (" << a1 << "-" << d1 << ") * (" << b0 << "-" << d0 << ") * (" << c2 << "-" << d2 <<
	//	") + (" << a0 << "-" << d0 << ") * (" << b1 << "-" << d1 << ") * (" << c2 << "-" << d2 << ")";

	double a13 = std::pow(a11, 2) + std::pow(a12, 2);
	double a23 = std::pow(a21, 2) + std::pow(a22, 2);
	double a33 = std::pow(a31, 2) + std::pow(a32, 2);
	double a43 = std::pow(a41, 2) + std::pow(a42, 2);

	return -1.0 * (a13 - a43) * (a22 - a42) * (a31 - a41) 
		+ (a12 - a42) * (a23 - a43) * (a31 - a41) 
		+ (a13 - a43) * (a21 - a41) * (a32 - a42) 
		- (a11 - a41) * (a23 - a43) * (a32 - a42) 
		- (a12 - a42) * (a21 - a41) * (a33 - a43) 
		+ (a11 - a41) * (a22 - a42) * (a33 - a43);
}


double getDet(const Point& p, const Point& q, const Point& r)
{
	return 0.5 * ((p.getX() - r.getX()) * (q.getY() - r.getY()) - (q.getX() - r.getX()) * (p.getY() - r.getY()));

}

//double getDet(const Point& p, const Point& q, const Point& r, const Point& s)
//{
//	double a = getDet(p, q, r);
//
//	double coordP = std::pow(p.getX(), 2) + std::pow(p.getY(), 2);
//	double coordQ = std::pow(q.getX(), 2) + std::pow(q.getY(), 2);
//	double coordR = std::pow(r.getX(), 2) + std::pow(r.getY(), 2);
//	double coordS = std::pow(s.getX(), 2) + std::pow(s.getY(), 2);
//
//	Point* aux1 = new Point(p.getY(), coordP);
//	Point* aux2 = new Point(q.getY(), coordQ);
//	Point* aux3 = new Point(r.getY(), coordR);
//	double b = getDet(*aux1, *aux2, *aux3);
//
//	aux1 = new Point(p.getX(), coordP);
//	aux2 = new Point(q.getX(), coordQ);
//	aux3 = new Point(r.getX(), coordR);
//	double c = getDet(*aux1, *aux2, *aux3);
//
//	double d = getDet(p.getX(), p.getY(), coordP, q.getX(), q.getY(), coordQ, r.getX(), r.getY(), coordR);
//
//	double aux = std::sqrt(std::pow(b, 2) + std::pow(c, 2) - 4 * a * d);
//
//	double det =  -1.0 * a * (std::pow(s.getX() + b / (2 * a), 2) + std::pow(s.getY() - c / (2 * a), 2) - std::pow(aux, 2));
//	return det;
//}

int main()
{
	Point* a = new Point , * b = new Point, * c = new Point, * d = new Point;
	std::cin >> *a;
	std::cin >> *b;
	std::cin >> *c;

	int m;
	std::cin >> m;
	for (int i = 0; i < m; i++)
	{
		std::cin >> *d;
		
		double detAbcd = getDet4(a->getX(), a->getY(), b->getX(), b->getY(), c->getX(), c->getY(), d->getX(), d->getY());

		if (detAbcd < 0)
		{
			std::cout << "OUTSIDE\n";
			continue;
		}
		if (detAbcd > 0)
		{
			std::cout << "INSIDE\n";
			continue;
		}
		std::cout << "BOUNDARY\n";
	}

	delete a;
	delete b;
	delete c;
	delete d;
	return 0;
}

// Iau o dreapta orizontala care trece prin punctul de verificat.
// Daca intersecteaza un segment (nu in varful lui), schimb boolul isIn = !isIn.
// Daca intalnesc un varf de segment, verific rotatia razei fata de segmentul anterior punctului si cel posterior.
// Daca sunt egale, nu se schimba isIn.
// Numar cate puncte sunt deasupra si cate dedesubtul razei. Consider ca punctele de pe raza sunt 

// poligon x monoton l orice dreapta verticala iau trece prin cel mult 3 (sau 2?) puncte ale poligonului.
// deci ideea e ca gasesc punctul de y maxim, gen yi, apoi parcurg ciclul y(i+1) etc., y1, y2 etc. pana la yi inapoi. 
// si ideea e ca voi avea doar doua intervale de monotonie: odata scade y pana intr-un punct, apoi creste si atat, nu se mai schimba iar monotonia

// primesc un poligon si trb sa intorc 2 stringuri: pt x monotonie si pt y monotonie


// ex3: 
// cerc circ.
// mediatoare
// panta mediatoarei este -1 pe panta unei drepte (pt ca e perpendiculara)
// fac 2 mediatoare, obtin intersectia si obrin centrul cercului
// apoi orice pct iau, verific daca distanta de la el la centru este mai mica decat raza sau nu
// 
// sau, versiunea cu determinanti: vezi formula pe teams pe halta la facultate intr-un ss
// 
// 

// deci muchiile legale sunt daca sunt in afara cercului 
// 
// bd e legala daca a e in afara lui bdc si daca c e in afara lui bda; daca gasesc unul din astea doua puncte in interiorul cercului,
// => bd e ilegala
// 
// pt muchii ilegale/legale trb sa vad 
// cum e d fata de cercul circ al lui a b c
// cum e c fata de cercul circ al lui a b d
// sau sa vad cu triunghiul bdc cum vine a-ul
