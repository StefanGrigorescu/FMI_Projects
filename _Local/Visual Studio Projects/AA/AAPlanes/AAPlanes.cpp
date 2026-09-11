#include <iostream>

using namespace std;

const double inf = 2e6 + 2;

class Line
{
public:
    Line(double a, double b, double c) : a(a), b(b), c(c) 
    {
        if (!validAB(a, b))
            cout << "Line args constructor: Warning: a * b does not equal 0!\n";
    }
    Line() : a(0), b(0), c(0) {}

    double getA() const { return a; }
    double getB() const { return b; }
    double getC() const { return c; }

    friend istream& operator>>(istream& in, Line& line);
    friend ostream& operator<<(ostream& out, const Line& line);

    void setA(double val) 
    {
        if (validAB(val, b))
            a = val;
        else
            cout << "SetA: Parameter value is not valid (a * b must equal 0).\n";
    }
    void setB(double val) 
    { 
        if (validAB(a, val))
            b = val;
        else
            cout << "SetB: Parameter value is not valid (a * b must equal 0).\n";        
    }    
    void setC(double val) { c = val; }

private:
    static bool validAB(double a, double b);

    double a, b, c;

};


istream& operator>>(istream& in, Line& line)
{
    in >> line.a >> line.b >> line.c;
    if (!Line::validAB(line.a, line.b))
        cout << "operator>>(istream&, Line&): Warning: a * b does not equal 0!\n";
    return in;
}

ostream& operator<<(ostream& out, const Line& line)
{
    out << line.a << "x + (" << line.b << ")y + " << line.c;
    return out;
}

bool Line::validAB(double a, double b) { return a * b == 0; }


namespace horizontal
{
    class Positive
    {
    public:
        static bool update(Line* line)
        {
            double yBoundary = -1 * line->getC() / (line->getB());
            if (Positive::yBoundary > yBoundary)
                return false;

            Positive::yBoundary = yBoundary;
            setLine(line);
            return true;
        }

        static void setLine(Line* line)
        {
            if (Positive::line)
                delete Positive::line;
            Positive::line = line;
        }

        static double getYBoundary()
        {
            return yBoundary;
        }

    private:
        static Line* line;
        static double yBoundary;
    };

    class Negative
    {
    public:
        static bool update(Line* line)
        {
            double yBoundary = -1 * line->getC() / (line->getB());
            if (Negative::yBoundary < yBoundary)
                return false;

            Negative::yBoundary = yBoundary;
            setLine(line);
            return true;
        }

        static void setLine(Line* line)
        {
            if (Negative::line)
                delete Negative::line;
            Negative::line = line;
        }

        static double getYBoundary()
        {
            return yBoundary;
        }

    private:
        static Line* line;
        static double yBoundary;
    };

    bool update(Line* line)
    {
        if (line->getB() < 0)
            return Positive::update(line);
        else
            return Negative::update(line);
    }

    bool intersectedBoundaries()
    {
        return Negative::getYBoundary() >= Positive::getYBoundary();
    }

    bool equalBoundaries()
    {
        if (Negative::getYBoundary() == inf || Positive::getYBoundary() == -1 * inf)
            return true;
        return Negative::getYBoundary() == Positive::getYBoundary();
    }

    void freeMem()
    {
        Positive::setLine(NULL);
        Negative::setLine(NULL);
    }
}

Line* horizontal::Negative::line = NULL;
double horizontal::Negative::yBoundary = inf;
Line* horizontal::Positive::line = NULL;
double horizontal::Positive::yBoundary = -1 * inf;


namespace vertical
{
    class Positive
    {
    public:
        static bool update(Line* line)
        {
            double xBoundary = -1 * line->getC() / (line->getA());
            if (Positive::xBoundary > xBoundary)
                return false;

            Positive::xBoundary = xBoundary;
            setLine(line);
            return true;
        }

        static void setLine(Line* line)
        {
            if (Positive::line)
                delete Positive::line;
            Positive::line = line;
        }

        static double getXBoundary()
        {
            return xBoundary;
        }

    private:
        static Line* line;
        static double xBoundary;
    };

    class Negative
    {
    public:
        static bool update(Line* line)
        {
            double xBoundary = -1 * line->getC() / (line->getA());
            if (Negative::xBoundary < xBoundary)
                return false;

            Negative::xBoundary = xBoundary;
            setLine(line);
            return true;
        }

        static void setLine(Line* line)
        {
            if (Negative::line)
                delete Negative::line;
            Negative::line = line;
        }

        static double getXBoundary()
        {
            return xBoundary;
        }

    private:
        static Line* line;
        static double xBoundary;
    };

    bool update(Line* line)
    {
        if (line->getA() < 0)
            return Positive::update(line);
        else
            return Negative::update(line);
    }

    bool intersectedBoundaries()
    {
        return Negative::getXBoundary() >= Positive::getXBoundary();
    }

    bool equalBoundaries()
    {
        if (Negative::getXBoundary() == inf || Positive::getXBoundary() == -1 * inf)
            return true;
        return Negative::getXBoundary() == Positive::getXBoundary();
    }

    void freeMem()
    {
        Positive::setLine(NULL);
        Negative::setLine(NULL);
    }
}

Line* vertical::Negative::line = NULL;
double vertical::Negative::xBoundary = inf;
Line* vertical::Positive::line = NULL;
double vertical::Positive::xBoundary = -1 * inf;


int main()
{
    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        Line* line = new Line();
        cin >> *line;
        bool updated = false;
        
        if (!line->getA())
            updated = horizontal::update(line);
        else if (!line->getB())
            updated = vertical::update(line);
        
        if (!updated)
            delete line;
    }

    if (!horizontal::intersectedBoundaries() || !vertical::intersectedBoundaries())
        cout << "VOID";
    else if (horizontal::equalBoundaries() || vertical::equalBoundaries())
        cout << "UNBOUNDED";
    else
        cout << "BOUNDED";

    horizontal::freeMem();
    vertical::freeMem();
    return 0;
}
