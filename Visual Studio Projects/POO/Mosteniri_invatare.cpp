#include <iostream>
#include <stack>

//class masina
//{
//    int nr_inmat;
//    int km;
//
//public:
//    virtual int get_km() { return km; }
//    int get_nr_inmat() { return nr_inmat; }
//
//    masina() { nr_inmat = 420; km = 69; }
//    masina(int param_km) { km = param_km; nr_inmat = 24; /* always initialize stuff */ }
//};
//class submasina : public masina
//{
//    int gloante_pe_teava;
//    int cartuse_rezerva;
//    int dimensiune_cartus;
//
//public:
//    submasina(int gpt = 26, int cr = 3, int dc = 26): masina() 
//    {
//        gloante_pe_teava = gpt;
//        cartuse_rezerva = cr;
//        dimensiune_cartus = dc;
//    }
//    void trage_gloante(int nr_gloante);
//};
//void submasina::trage_gloante(int nr_gloante)
//{
//    std::cout << "   ";
//    for (int i = 0; i < nr_gloante; i++)
//    {
//        if (gloante_pe_teava == 0 and cartuse_rezerva != 0)
//        {
//            gloante_pe_teava = dimensiune_cartus;
//            cartuse_rezerva--;
//            std::cout << " Chk-ch-chk!";
//        }
//        if (gloante_pe_teava > 0)
//        {
//            std::cout << " Trr";
//            gloante_pe_teava--;
//        }
//        else
//        {
//            std::cout << "    Baga-mi-as! Nu mai am gloante!";
//            return;
//        }
//    }
//    std::cout << "\n";
//}

int varsta = 0, nrArme = 3;

class arma
{
protected:
    double dmginitial;
    double dmgPeAn;
    std::string display;

public:
    virtual double calculeazaDmg() {
        return dmginitial;
    }
    virtual void egalAdrese(const arma* sursa)
    {
        dmginitial = sursa->dmginitial;
        dmgPeAn = sursa->dmgPeAn;
        display = sursa->display;
    }
    arma* operator =(const arma* sursa) 
    {
        egalAdrese(sursa);
    }
    virtual void egalObiecte(const arma& sursa)
    {
        dmginitial = sursa.dmginitial;
        dmgPeAn = sursa.dmgPeAn;
        display = sursa.display;
    }
    arma& operator =(const arma &sursa) 
    {
        egalObiecte(sursa);
        return *this;
    }
    arma() {
        display = "X";

        dmginitial = -1;
        dmgPeAn = -0.1;
    }
    arma(double dmginit, double dmgAn, std::string cdisplay)
    {
        dmginitial = dmginit;
        dmgPeAn = dmgAn;
        display = cdisplay;
    }
};

class sageti
{
    double dmgSageata;
    int nrSageti;

protected:
    void set_nrSageti(int val) {
        nrSageti = val;
    }
    void set_dmgSageata(double val) {
        dmgSageata = val;
    }

public:
    int get_nrSageti() const {
        return nrSageti;
    }
    double get_dmgSageata() const {
        return dmgSageata;
    }
    sageti() {
        dmgSageata = 0.1;
        nrSageti = 3;
    }
};

class arrowQlty {
    int qlty;
public:
    arrowQlty(int val) {
        qlty = val % 3 + 1;
    }
    int get_qlty() const { return qlty; }
};

template <int calitate>
class diferiteSageti : public sageti{
    int nrSagetiExtra;
    double dmgSageataExtra;

public:
    diferiteSageti()
    {
        nrSagetiExtra = 0;
        dmgSageataExtra = 0;

        arrowQlty cond(calitate);
        switch (cond.get_qlty())
        {
        case 1:
        {
            nrSagetiExtra = rand() % 6 + 1;
            set_nrSageti(get_nrSageti() + nrSagetiExtra);
        }
        break;
        case 2:
        {
            dmgSageataExtra = (rand() % 2) / 10 + 0.07;
            set_dmgSageata(get_dmgSageata() + dmgSageataExtra);
        }
        break;
        case 3:
        {
            nrSagetiExtra = rand() % 2 + 2;
            set_nrSageti(get_nrSageti() + nrSagetiExtra);

            dmgSageataExtra = (rand() % 15) / 100 + 0.09;
            set_dmgSageata(get_dmgSageata() + dmgSageataExtra);
        }
        }
    }
};

class arc: public arma
{
    int displayStart;
    diferiteSageti<1> urmatoarea1;
    diferiteSageti<2> urmatoarea2;
    diferiteSageti<3> urmatoarea3;
    std::stack<arrowQlty> tolba;

public:
    arc(): arma(1, 0.1, "///}") {
        displayStart = 0;
    }
    void cresteTolba()
    {
        if (varsta % 2 == 0)
        {
           int aux = rand() % 3;
           arrowQlty calitate(aux);
           tolba.push(calitate);
        }
    }
    double calculeazaDmg() {
        std::cout << "\n suntem in calculeazaDmg pt arc";
        double dmg = 0;
        if (tolba.empty() == false)
        {
            const int calitate = tolba.top().get_qlty();
            //std::cout << "\n calitate = " << calitate;
            sageti* sageata;
            if (calitate == 1)
            {
                //std::cout << "\ncalitate1 ";
                sageata = &urmatoarea1;
                sageata = dynamic_cast<diferiteSageti<1>*>(&urmatoarea1);
                displayStart = 2;
            }
            else if (calitate == 2)
            {
                //std::cout << "\ncalitate2 ";
                sageata = &urmatoarea2;
                sageata = dynamic_cast<diferiteSageti<2>*>(&urmatoarea2);
                displayStart = 1;
            }
            else
            {
                //std::cout << "\ncalitate3 ";
                sageata = &urmatoarea3;
                sageata = dynamic_cast<diferiteSageti<3>*>(&urmatoarea3);
                displayStart = 0;
            }
            if (sageata != NULL)
            {
                //std::cout << "\n sageata nu este NULL, hai ca e bun";
                dmg += (dmginitial + dmgPeAn * varsta + sageata->get_dmgSageata()) * sageata->get_nrSageti();
            }
        }
        return dmg;
    }
    std::string get_display() const
    {
        std::string cdisplay = display.substr(displayStart, display.npos);
        return cdisplay;
    }
};

class sabie: public arma
{
public:
    sabie(): arma(1, 0.1, "!") {}
    double calculeazaDmg() {
        std::cout << "\n suntem in calculeazaDmg pt sabie";
        return dmginitial + dmgPeAn * varsta;
    }
};
class topor: public arma
{
    double coefGreutate;
public:
    topor(): arma(0.5, 1.2, "7") {
        coefGreutate = 1.3;
    }
    double calculeazaDmg() {
        std::cout << "\n suntem in calculeazaDmg pt topor";
        return coefGreutate*dmginitial + dmgPeAn * varsta;
    }
};

class individ
{
    int varsta;
    double dmg;
    int indiceArma;
    arma* armaSa;
    sabie arma1;
    topor arma2;
    arc arma3;

    template <class oArma>
    double extraDmg(oArma armaSa) {
        return armaSa->calculeazaDmg();
    }
public:
    individ() {
        varsta = 0;
        dmg = 0;
        indiceArma = rand() % nrArme + 1;
        switch (indiceArma)
        {
        case 1:
        {
            armaSa = dynamic_cast<sabie*>(&arma1);
        }
        break;
        case 2: 
        {
            armaSa = dynamic_cast<topor*>(&arma2);
        }
        break;
        default:
        {
            armaSa = dynamic_cast<arc*>(&arma3);
        }
        }
    }
    arma* get_armaSa() const { return armaSa; }
    void actualizare() {
        varsta++;
        dmg += extraDmg(armaSa);
    }
    void afisare(){
        std::cout << "\nindividul ales are acum dmg = " << dmg;
    }
};

class baza
{
    int data1;
    int data2; 
    bool data3;
    std::string data4;


public:
    baza() 
    {
        data1 = citeste(data1, "varsta");
        data2 = citeste(data2, "salariu");
        data3 = citeste(data3, "daca e strain (0/1)");
        data4 = citeste(data4, "nume");
    }
    template <typename d>
    d citeste(d &data, std::string numele)
    {
        std::cout << "\n     Cititi " << numele << ": ";
        std::cin >> data;
        return data;
    }
};

int main()
{
    //submasina thomson(26, 1);
    ////thomson.trage_gloante(69);
    //masina* p = &thomson;
    //std::cout << p << " ";
    //submasina *sm = dynamic_cast<submasina*>(p);
    //std::cout << sm <<"   " << p << "\n";
    //thomson.trage_gloante(420);
    //std::cout << "\n";

    srand(time(NULL));

 /*   individ Mateus;
    for (int i = 0; i < 5; i++)
    {
        Mateus.afisare();
        Mateus.actualizare();
    }
    Mateus.afisare();*/

    individ individ1;
    //individ1.get_armaSa()->calculeazaDmg(); 
    baza mare;

    return 0;
}

