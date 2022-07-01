#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <fstream>

using namespace std;


const double inf = 2e6 + 2;

namespace line
{
    class Line
    {
    public:
        Line(double a, double b, double c) : a(a), b(b), c(c) {}
        Line() : a(0), b(0), c(0) {}

        double getXBound() const { return (double)  - 1 * c / a; }
        double getYBound() const { return (double) - 1 * c / b; }

        double getA() const { return a; }
        double getB() const { return b; }
        double getC() const { return c; }

        friend istream& operator>>(istream& in, Line& line);
        friend ostream& operator<<(ostream& out, const Line& line);

        friend ifstream& operator >> (ifstream& in, Line& line);
        friend ofstream& operator<<(ofstream& out, const Line& line);

        void setA(double val) { a = val; }
        void setB(double val) { b = val; }
        void setC(double val) { c = val; }

    private:
        double a, b, c;

    };

    istream& operator>>(istream& in, Line& line)
    {
        in >> line.a >> line.b >> line.c;
        return in;
    }

    ostream& operator<<(ostream& out, const Line& line)
    {
        out << "(" << line.a << ")x + (" << line.b << ")y + (" << line.c << ") <= 0";
        return out;
    }

    ifstream& operator>>(ifstream& fin, Line& line)
    {
        fin >> line.a >> line.b >> line.c;
        return fin;
    }

    ofstream& operator<<(ofstream& fout, const Line& line)
    {
        fout << "(" << line.a << ")x + (" << line.b << ")y + (" << line.c << ") <= 0";
        return fout;
    }

    typedef bool (*comparator)(Line&, Line&);   // For callback to a bool comparator that takes two arguments of type Line&.
    class comparators
    {
    public:
        static bool cmpHorizontalPos(Line& line1, Line& line2)
        {
            return line1.getYBound() < line2.getYBound();
        }
        static bool cmpHorizontalNeg(Line& line1, Line& line2)
        {
            return line1.getYBound() < line2.getYBound();
        }
        static bool cmpVerticalPos(Line& line1, Line& line2)
        {
            return line1.getXBound() < line2.getXBound();
        }
        static bool cmpVerticalNeg(Line& line1, Line& line2)
        {
            return line1.getXBound() < line2.getXBound();
        }
    };


    int binarySearchLowerIdx(vector<Line>& lines, Line& target, comparator cmp)
    {
        if (!lines.size() || cmp(target, lines[0]))
            return -1;
        if (cmp(lines[lines.size() - 1], target))
            return lines.size() - 1;

        int power2 = 1;
        while (power2 < lines.size())
            power2 <<= 1;

        int lowerIdx = 0;
        while (power2)
        {
            if (lowerIdx + power2 < lines.size() && cmp(lines[lowerIdx + power2], target))
                lowerIdx += power2;
            power2 >>= 1;
        }

        return lowerIdx;
    }


    vector<Line>::iterator searchAndInsert(vector<Line>& lines, Line& target, comparator cmp)
    {
        int pos = 1 + binarySearchLowerIdx(lines, target, cmp);
        if (pos == lines.size())
        {
            lines.push_back(target);
            return lines.begin() + pos;
        }
        return lines.insert(lines.begin() + pos, target);
    }
}


namespace point
{
    class Point
    {
    public:
        Point(double x = 0, double y = 0) : x(x), y(y) {}
        Point(const Point& source) : x(source.getX()), y(source.getY()) {}
        Point(const line::Line& source) : x(source.getXBound()), y(source.getYBound()) {}

        bool belongsToSegment(const Point& firstBound, const Point& secondBound) const;
        double getDistance(const Point& other) const;

        Point& operator=(const Point& source)
        {
            setPoint(source.getX(), source.getY());
            return *this;
        }

        double getX() const { return x; }
        double getY() const { return y; }

        friend istream& operator>>(istream& in, Point& point);
        friend ostream& operator<<(ostream& out, const Point& point);

        friend ifstream& operator>>(ifstream& fin, Point& point);
        friend ofstream& operator<<(ofstream& fout, const Point& point);

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

    ifstream& operator>>(ifstream& fin, Point& point)
    {
        double x, y;
        fin >> x >> y;
        point.setPoint(x, y);

        return fin;
    }

    ofstream& operator<<(ofstream& fout, const Point& point)
    {
        fout << "(" << point.getX() << ", " << point.getY() << ")";
        return fout;
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


    typedef bool (*comparator)(Point&, Point&);   // For callback to a bool comparator that takes two arguments of type Point&.
    class comparators
    {
    public:
        static bool cmpHorizontalPos(Point& p1, Point& p2)
        {
            return p1.getY() < p2.getY();
        }
        static bool cmpHorizontalNeg(Point& p1, Point& p2)
        {
            return p1.getY() < p2.getY();
        }
        static bool cmpVerticalPos(Point& p1, Point& p2)
        {
            return p1.getX() < p2.getX();
        }
        static bool cmpVerticalNeg(Point& p1, Point& p2)
        {
            return p1.getX() < p2.getX();
        }
    };
}


typedef bool (*comparatorPointLine)(point::Point&, line::Line&);   // For callback to a bool comparator that takes two arguments, first of type Point& and second of type Line&.
typedef bool (*comparatorLinePoint)(line::Line&, point::Point&);   // For callback to a bool comparator that takes two arguments, first of type Line& and second of type Point&.
class comparators
{
public:
    static bool cmpHorizontalPos(point::Point& p, line::Line& line)
    {
        return p.getY() < line.getYBound();
    }
    static bool cmpHorizontalNeg(point::Point& p, line::Line& line)
    {
        return p.getY() < line.getYBound();
    }
    static bool cmpVerticalPos(point::Point& p, line::Line& line)
    {
        return p.getX() < line.getXBound();
    }
    static bool cmpVerticalNeg(point::Point& p, line::Line& line)
    {
        return p.getX() < line.getXBound();
    }

    static bool cmpHorizontalPos(line::Line& line, point::Point& p)
    {
        return line.getYBound() < p.getY();
    }
    static bool cmpHorizontalNeg(line::Line& line, point::Point& p)
    {
        return line.getYBound() < p.getY();
    }
    static bool cmpVerticalPos(line::Line& line, point::Point& p)
    {
        return line.getXBound() < p.getX();
    }
    static bool cmpVerticalNeg(line::Line& line, point::Point& p)
    {
        return line.getXBound() < p.getX();
    }
};


int binarySearchLowerIdx(vector<line::Line>& lines, point::Point& targetPoint, comparatorPointLine cmp1, comparatorLinePoint cmp2)
{
    if (!lines.size() || cmp1(targetPoint, lines[0]))
        return -1;
    if (cmp2(lines[lines.size() - 1], targetPoint))
        return lines.size() - 1;

    int power2 = 1;
    while (power2 < lines.size())
        power2 <<= 1;

    int lowerIdx = 0;
    while (power2)
    {
        if (lowerIdx + power2 < lines.size() && cmp2(lines[lowerIdx + power2], targetPoint))
            lowerIdx += power2;
        power2 >>= 1;
    }

    return lowerIdx;
}

int binarySearchUpperIdx(vector<line::Line>& lines, point::Point& targetPoint, comparatorPointLine cmp1, comparatorLinePoint cmp2)
{
    if (!lines.size() || cmp1(targetPoint, lines[0]))
        return 0;
    if (cmp2(lines[lines.size() - 1], targetPoint))
        return lines.size();

    int power2 = 1;
    while (power2 < lines.size())
        power2 <<= 1;

    int upperIdx = lines.size() - 1;
    while (power2)
    {
        if (upperIdx - power2 >= 0 && cmp1(targetPoint, lines[upperIdx - power2]))
            upperIdx -= power2;
        power2 >>= 1;
    }

    return upperIdx;
}

bool inInterval(const int target, int lowerBound, int upperBound)
{
    return target >= lowerBound && target <= upperBound;
}

double getYBound(const int idxLines, vector<line::Line>& lines, bool isLowerBound)
{
    if (inInterval(idxLines, 0, lines.size() - 1))
        return lines[idxLines].getYBound();

    if (isLowerBound)
        return lines[lines.size() - 1].getYBound();      // For a lower bound, the nearest yBound is the one with the greatest value, which is the last in the sorted lines vector.

    return lines[0].getYBound();                             // For an upper bound, the nearest yBound is the one with the lowest value, which is the first in the sorted lines vector.
}

double getXBound(const int idxLines, vector<line::Line>& lines, bool isLowerBound)
{
    if (inInterval(idxLines, 0, lines.size() - 1))
        return lines[idxLines].getXBound();

    if (isLowerBound)
        return lines[lines.size() - 1].getXBound();      // For a lower bound, the nearest xBound is the one with the greatest value, which is the last in the sorted lines vector.

    return lines[0].getXBound();                             // For an upper bound, the nearest xBound is the one with the lowest value, which is the first in the sorted lines vector.
}

//void print(vector<line::Line> lines)
//{
//    cout << " ++++++++++++++++++ \n";
//    for (int i = 0; i < lines.size(); i++)
//        cout << lines[i] << "   /   ";
//    cout << " \n\n";
//}


int main()
{
    using namespace line;
    using namespace point;

    //ifstream cin("date.in");
    //ofstream cout("date.out");

    vector<Line> vHorizontalPos;
    vector<Line> vHorizontalNeg;
    vector<Line> vVerticalPos;
    vector<Line> vVerticalNeg;

    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        Line line;
        cin >> line;

        // horizontal
        if (!line.getA())   
        {
            if (line.getB() < 0)
            {
                searchAndInsert(vHorizontalPos, line, line::comparators::cmpHorizontalPos);
                continue;
            }
            searchAndInsert(vHorizontalNeg, line, line::comparators::cmpHorizontalNeg);
            continue;
        }

        // vertical
        if (line.getA() < 0)
        {
            searchAndInsert(vVerticalPos, line, line::comparators::cmpVerticalPos);
            continue;
        }
        searchAndInsert(vVerticalNeg, line, line::comparators::cmpVerticalNeg); 

    }

    int m;
    cin >> m;

    if (!vHorizontalPos.size() || !vHorizontalNeg.size() || !vVerticalPos.size() || !vVerticalNeg.size())   // Not enough half-planes of each type to be able to obtain an interesting rectangle.
    {
        for (int i = 0; i < m; i++)
            cout << "NO\n";
        return 0;
    }

    for (int i = 0; i < m; i++)
    {
        Point crtPoint;
        cin >> crtPoint;

        const int idxHorizontalPos = ::binarySearchLowerIdx(vHorizontalPos, crtPoint, ::comparators::cmpHorizontalPos, ::comparators::cmpHorizontalPos);
        if (idxHorizontalPos < 0)
        {
            cout << "NO\n";
            continue;
        }
        const int idxHorizontalNeg = ::binarySearchUpperIdx(vHorizontalNeg, crtPoint, ::comparators::cmpHorizontalNeg, ::comparators::cmpHorizontalNeg);
        if (idxHorizontalNeg > (int)vHorizontalNeg.size() - 1)
        {
            cout << "NO\n";
            continue;
        }
        const int idxVerticalPos = ::binarySearchLowerIdx(vVerticalPos, crtPoint, ::comparators::cmpVerticalPos, ::comparators::cmpVerticalPos);
        if (idxVerticalPos < 0)
        {
            cout << "NO\n";
            continue;
        }
        const int idxVerticalNeg = ::binarySearchUpperIdx(vVerticalNeg, crtPoint, ::comparators::cmpVerticalNeg, ::comparators::cmpVerticalNeg);
        if (idxVerticalNeg > (int)vVerticalNeg.size() - 1)
        {
            cout << "NO\n";
            continue;
        }

        const double upperY = getYBound(idxHorizontalNeg, vHorizontalNeg, false);
        if(crtPoint.getY() == upperY)
        {
            cout << "NO\n";
            continue;
        }

        const double lowerY = getYBound(idxHorizontalPos, vHorizontalPos, true);
        if (crtPoint.getY() == lowerY)
        {
            cout << "NO\n";
            continue;
        }

        if (upperY < lowerY)
        {
            cout << "NO\n";
            continue;
        }

        const double upperX = getXBound(idxVerticalNeg, vVerticalNeg, false);
        if (crtPoint.getX() == upperX)
        {
            cout << "NO\n";
            continue;
        }

        const double lowerX = getXBound(idxVerticalPos, vVerticalPos, true);
        if (crtPoint.getX() == lowerX)
        {
            cout << "NO\n";
            continue;
        }

        if (upperX < lowerX)
        {
            cout << "NO\n";
            continue;
        }

        cout << "YES\n";
        cout << fixed << setprecision(6) << (const double) ((upperY - lowerY) * (upperX - lowerX)) << "\n";
    }

    return 0;
}
