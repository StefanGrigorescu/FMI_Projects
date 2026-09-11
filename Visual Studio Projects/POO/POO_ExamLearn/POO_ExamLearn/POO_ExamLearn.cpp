/*typeid(a).name();*/
//#include <iostream>
//using namespace std;
//
//class A
//{
//public:
//	A() {}
//	virtual void g() {}
//};
//
//class B: public A
//{
//public: B() {}
//	  void g() {}
//};
//
//int main()
//{
//	A* a = new B;
//	cout << typeid(a).name();
//}

/*awesome constructor&destructor order show*/
//#include <iostream>
//using namespace std;
//
//int id = 1;
//
//class Contor {
//	char c;
//	int ID;
//	static int contor;
//public:
//	void Print() {
//		cout << "obiect " << c << " id #" << ID << endl;
//	}
//	Contor(const char& ch) {
//		c = ch;
//		ID = id++;
//		++contor;
//		cout << "Constructor conversie: ";
//		Print();
//	}
//	Contor(const Contor& h) {
//		c = h.c;
//		ID = id++;
//		++contor;
//		cout << "Constructor copiere: ";
//		Print();
//	}
//	~Contor() {
//		--contor;
//		cout << "Destructor: ";
//		Print();
//	}
//	char get_c() { return c; }
//};
//int Contor::contor = 0;
//Contor f(Contor x, Contor y) {
//	cout << "Incepe functia f" << endl;
//	cout << "Se termina functia f" << endl;
//	
//	//return x;
//
//	char litera = x.get_c();
//	return litera;
//}
//void main() {
//	Contor o1('a'), o2('b');
//	cout << "Inainte de apelul lui f cu valoare de intoarcere" << endl;
//	Contor o3 = f(o1, o2);
//	cout << "Dupa apelul lui f" << endl;
//	cout << "Inainte de apelul lui f fara valoare de intoarcere" << endl;
//	f(o1, o2);
//	cout << "Dupa apelul lui f fara valoare de intoarcere" << endl;
//}

/*param makers (constructori gen) ordine*/
//#include <iostream>
//using namespace std;
//
//int paramMaker(string param)
//{
//	cout << "\n param: " << param;
//	return 1;
//}
//
//int fct(int a, int b)
//{
//	return 0;
//}
//
//int main()
//{
//	int we = fct(paramMaker("primu"), paramMaker("doilea"));
//
//	return 0;
//}

#include <iostream>
using namespace std;
class A
{
	int v[3];
public:
	A() {

		for (int i = 0; i < 3; i++)
			v[i] = i;
	}
	int& operator[] (int i) { return *(v + i); }
	int* getv() { return v; }
	A* operator-> ()
	{
		return this;
	}
};

class abstracta
{
	void gct() { cout << "stuffin"; }
protected:
	public:
	virtual void fct() /*{ cout << "clasa1"; } */= 0;
public:
	/*virtual*/ void stuffer() { cout << "stuffer1"; }
};

class abstracta2 : public abstracta
{
public:
	void fct() {
		cout << "idK";
		/*gct();*/ 
	}
	void stuffer() { cout << "stuffer2"; }
};

template <typename T, typename D = int>
class CV
{
public:
	void scriemi() { cout << "SMS"; 
	T a;
	D b;
	cout << "\n " << typeid(a).name() << " ";
	cout << "\n " << typeid(b).name();
	}
};

template<class A> void temple(A arg)
{
	cout << typeid(arg).name() << "\n";
}

string get_name()
{
	string s = "magic";
	return s;
}

const int* const w() {
	static int i;
	return &i;
}

int main()
{
	//A ob;
	//cout << ob[1];
	//cout << ob->getv()[2];
	//abstracta2 ab;
	//ab.fct();

	//abstracta* p = new abstracta2;
	//abstracta2 ab2;
	//abstracta* poi = &ab2;
	//
	//abstracta2* p2 = dynamic_cast<abstracta2*>(p);
	//p2->stuffer();
	////p->stuffer();
	//cout << typeid(*p).name() <<"\n";
	//
	//poi->stuffer();

	//CV<abstracta2> var;
	//var.scriemi();
	//abstracta amic ;
	//cout << " \n";
	//temple(amic);

	/*const string& p = get_name();
	std::cout << p;*/

	//abstracta* ob2 = new abstracta2;
	//ob2->fct();

	int* const t = new int(3);
	*t = 4;
	cout << *t;


	// VS
	int const* p = new int(3);  // pointer constant

}