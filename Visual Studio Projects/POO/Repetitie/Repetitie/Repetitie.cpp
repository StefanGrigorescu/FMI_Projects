#include <iostream>

using namespace std;


template<typename T> std::string getType(T var)
{
    return typeid(var).name();
}
const std::string aliniat = "\n    ";
#define getName(x) #x
#define present(x) std::cout << aliniat << getName(x) << " (" << getType(x) << "): "


class tata
{
    int nr;
    bool da;
    string nume;
    int tip;

    virtual void foo3();

protected:
    virtual void citim(std::istream& in);
    virtual void scriem(ostream& out);

public:
    tata(int nr = 1, bool da = 0, std::string nume = "gica", int tip = 1) : nr(nr), da(da), nume(nume), tip(tip) {};

    friend std::istream& operator >>(std::istream& in, tata& tatal);
    friend ostream& operator<<(ostream& out, tata& tatal);

    /* virtual function:
    the base class creates a virtual table for function implementations references;
    while upcasting, the base class pointer searches the virtual table for a virtual function's latest available implementation */
    virtual void foo1(); 
    void foo2();  

};

void tata::citim(std::istream& in)
{
    std::cout << aliniat << "tata.citim: ";
    present(nr);
    in >> nr;
    present(da);
    in >> da;
    present(nume);
    in >> nume;
    present(tip);
    in >> tip;
}

std::istream& operator >>(std::istream& in, tata& tatal)
{
    std::cout << aliniat << "tata.operator>>: ";
    tatal.citim(in);
    return in;
}

void tata::scriem(std::ostream& out)
{
    std::cout << aliniat << "tata.scriem: ";
    present(nr);
    out << nr;
    present(da);
    out << da;
    present(nume);
    out << nume;
    present(tip);
    out << tip;
}

ostream& operator<<(ostream& out, tata& tatal)
{
    std::cout << aliniat << "tata.operator<<: ";
    tatal.scriem(out);
    return out;
}

void tata::foo1()
{
    std::cout << aliniat << "tata.foo1";
}
void tata::foo2()
{
    std::cout << aliniat << "tata.foo2";
}
void tata::foo3()
{
    std::cout << aliniat << "tata.foo3";
}

class fiu : virtual public tata
{
    int idk;
public:
    fiu(int idk = 13) : idk(idk) {}

    void citim(std::istream& in);
    virtual void scriem(ostream& out);

    void foo1();
    void foo2();
};

void fiu::citim(std::istream& in)
{
    cout << aliniat << "fiu.citim: ";
    tata::citim(in);

    present(idk);
    in >> idk;
}
void fiu::scriem(ostream& out)
{
    cout << aliniat << "fiu.scriem: ";
    tata::scriem(out);

    present(idk);
    out << idk;
}

void fiu::foo1()
{
    std::cout << aliniat << "fiu.foo1";
}
void fiu::foo2()
{
    std::cout << aliniat << "fiu.foo2";
}

int main()
{
    fiu el;
    cin >> el;
    cout << el;

    el.tata::foo1();
    el.tata::foo2();

    el.foo1();
    el.foo2();


    tata* ea = new fiu();
    cin >> *ea;
    cout << *ea;

    ea->tata::foo1();
    ea->tata::foo2();

    ea->foo1();
    ea->foo2();
    //ea->foo3();   // foo3() is still inaccessible for derived class instance, disregarding the pointer type

    return 0;
}


