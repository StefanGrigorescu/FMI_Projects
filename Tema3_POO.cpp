//        ~ Grigorescu Stefan D.  grupa 131 ~
//   Tema 3 POO  (problema 16, indivizi, de pe primul fisier cu teme)

//   Mult indragitul joc s-a intors! Acum, indivizii nostri stiu sa isi faca arme, luptele sunt mai aprige,
//   sub decorul unui soare ce strabate lenes cerul pe deasupra locuintelor acestora
// 
//   Pentru a revedea notarile istoricilor despre evenimente, accesati documentul "Cronica.out" dupa incheierea rularii programului

#include <iostream>
#include <fstream>
#include <iomanip>
#include <windows.h>
#include <stack>

#define cout_std std::cout
#define cin_std std::cin
#define istream_std std::istream
#define ostream_std std::ostream

std::ofstream istoricii_noteaza("Cronica.out");

const double energie_init = 45, energie_max = 100, energie_1an = -6.9;
const int nmax = 30, varsta_apogeu = 8, varsta_max = 25, nrArme = 3;

#pragma region Functii auxiliare
void style(int P)
{
    HANDLE console_color;

    // Color of the console
    console_color = GetStdHandle(STD_OUTPUT_HANDLE);

    // P is color code according to your need.
    SetConsoleTextAttribute(console_color, P);
}
void allStyle()
{
    for (int i = 0; i < 50; i++)
    {
        style(i);
        cout_std << i << " sample text\n";
        style(7);
    }
}

unsigned int nr_cifre(int n)
{
    unsigned int cnt = 0;
    while (n)
    {
        cnt++;
        n /= 10;
    }
    return cnt;
}
void afis_spatii(int i)
{
    if (nr_cifre(i) == 1)
        cout_std << "   ";
    else if (nr_cifre(i) == 2)
        cout_std << "  ";
    else if (nr_cifre(i) == 3)
        cout_std << " ";
}
void afis_spatii2(int lungime)
{
    if (lungime == 1)
        cout_std << "   ";
    else if (lungime == 2)
        cout_std << "  ";
    else if (lungime == 3)
        cout_std << " ";
}
#pragma endregion

#pragma region Arme
class arma
{
protected:
    double dmginitial;
    double dmgPeAn;
    std::string display;

public:
    virtual double calculeazaDmg(int varsta) {
        return dmginitial;
    }
    virtual std::string get_display() const { return display; }
    virtual void egalAdrese(const arma* sursa)
    {
        dmginitial = sursa->dmginitial;
        dmgPeAn = sursa->dmgPeAn;
        display = sursa->display;
    }
    arma* operator =(const arma* sursa)
    {
        egalAdrese(sursa);
        return this;
    }
    virtual void egalObiecte(const arma& sursa)
    {
        dmginitial = sursa.dmginitial;
        dmgPeAn = sursa.dmgPeAn;
        display = sursa.display;
    }
    arma& operator =(const arma& sursa)
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
    sageti& operator=(const sageti& sursa)
    {
        dmgSageata = sursa.dmgSageata;
        nrSageti = sursa.nrSageti;
        return *this;
    }
    sageti* operator=(const sageti* sursa)
    {
        dmgSageata = sursa->dmgSageata;
        nrSageti = sursa->nrSageti;
        return this;
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

    arrowQlty& operator =(const arrowQlty& sursa)
    {
        qlty = sursa.qlty;
        return *this;
    }
};

template <int calitate>
class diferiteSageti : public sageti {
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
    diferiteSageti& operator=(const diferiteSageti& sursa)
    {
        nrSagetiExtra = sursa.nrSagetiExtra;
        dmgSageataExtra = sursa.dmgSageataExtra;
        return *this;
    }
};

class arc : public arma
{
    int displayStart;
    diferiteSageti<1> urmatoarea1;
    diferiteSageti<2> urmatoarea2;
    diferiteSageti<3> urmatoarea3;
    std::stack<arrowQlty> tolba;

public:
    arc() : arma(1, 0.1, "///}") {
        displayStart = 3;
    }
    void cresteTolba()
    {
            int aux = rand() % 3;
            arrowQlty calitate(aux);
            tolba.push(calitate);
    }
    void scadeTolba()
    {
        if (!tolba.empty())
            tolba.pop();
    }
    double calculeazaDmg(int varsta)
    {
        double dmg = 0;
        if (tolba.empty() == false)
        {
            const int calitate = tolba.top().get_qlty();
            sageti* sageata;
            switch (calitate)
            {
            case 1:
            {
                sageata = &urmatoarea1;
                sageata = dynamic_cast<diferiteSageti<1>*>(&urmatoarea1);
            }
            break;
            case 2:
            {
                sageata = &urmatoarea2;
                sageata = dynamic_cast<diferiteSageti<2>*>(&urmatoarea2);
            }
            break;
            default:
            {
                sageata = &urmatoarea3;
                sageata = dynamic_cast<diferiteSageti<3>*>(&urmatoarea3);
            }
            }
            if (sageata != NULL)
                dmg += (dmginitial + dmgPeAn * varsta + sageata->get_dmgSageata()) * sageata->get_nrSageti();
        }
        return dmg;
    }
    void actualizeaza_display()
    {
        if (tolba.empty())
            displayStart = 3;
        else
            switch (tolba.top().get_qlty())
            {
            case 1:
            {
                displayStart = 2;
            }
            break;
            case 2:
            {
                displayStart = 1;
            }
            break;
            default:
            {
                displayStart = 0;
            }
            }
    }
    std::string get_display() const
    {
        std::string cdisplay = display.substr(displayStart, display.npos);
        return cdisplay;
    }

    void egalObiecte(arc& sursa)
    {
        arma::egalObiecte(sursa);
        displayStart = sursa.displayStart;
        urmatoarea1 = sursa.urmatoarea1;
        urmatoarea2 = sursa.urmatoarea2;
        urmatoarea3 = sursa.urmatoarea3;

        std::stack<arrowQlty> oglinda;
        while (!sursa.tolba.empty())
        {
            oglinda.push(sursa.tolba.top());
            sursa.tolba.pop();
        }
        while (!oglinda.empty())
        {
            tolba.push(oglinda.top());
            sursa.tolba.push(oglinda.top());
            oglinda.pop();
        }
    }
    arc& operator =(arc& sursa)
    {
        egalObiecte(sursa);
        return *this;
    }
};

class sabie : public arma
{
public:
    sabie() : arma(6, 0.5, "!") {}
    double calculeazaDmg(int varsta) {
        return dmginitial + dmgPeAn * varsta;
    }

    void egalObiecte(const sabie& sursa)
    {
        arma::egalObiecte(sursa);
    }
    sabie& operator=(const sabie& sursa)
    {
        egalObiecte(sursa);
        return *this;
    }
};
class topor : public arma
{
    double coefGreutate;
public:
    topor() : arma(3, 1.2, "7") {
        coefGreutate = 1.3;
    }
    double calculeazaDmg(int varsta) {
        return coefGreutate * dmginitial + dmgPeAn * varsta;
    }

    void egalObiecte(const topor& sursa)
    {
        arma::egalObiecte(sursa);
        coefGreutate = sursa.coefGreutate;
    }
    topor& operator=(const topor& sursa)
    {
        egalObiecte(sursa);
        return *this;
    }
};
#pragma endregion

class individ
{
    int i;
    char tip;
    int varsta;
    double energie;
    unsigned char viu;

    int indiceArma;
    arma* armaSa;
    sabie arma1;
    topor arma2;
    arc arma3;

    static int inamic_mic;
    static int inamic_mare;

protected:

#pragma region Metode protected
    virtual void hraneste(double bonus = 0, double deficit = 0);
    void imbatraneste(individ* indivizi[]);
    void urmas(individ* tata, int poz, double coef_e = 1.33);
    virtual void inmulteste(individ* indivizi[]);
    void inamicii(individ* indivizi[], bool teama);
    virtual void moare(individ* indivizi[]);
    void efecte_atac(individ* indivizi[], int poz_inamic, double dmg, double coef_energie_atac);
    double calculeaza_dmg(int varsta, double energie, double coef1_atac, double coef2_atac);
    virtual void ataca(individ* indivizi[], int vma = 3, bool teama = 1, double c1a = 0.3, double c2a = 1.25, double cea = 0.2);

    template <class oArma>
    double extraDmg(oArma* armaSa) {
        return armaSa->calculeazaDmg(varsta);
    }
#pragma endregion 

public:

#pragma region Getteri & Setteri
    int get_i() const { return i; }
    char get_tip() const { return tip; }
    int get_varsta() const { return varsta; }
    double get_energie() const { return energie; }
    unsigned char get_viu() const { return viu; }
    arma* get_armaSa() const { return armaSa; }
    static int get_inamic_mic() { return inamic_mic; }
    static int get_inamic_mare() { return inamic_mare; }

    void set_i(int poz) { i = poz; }
    void set_tip(char specie) { tip = specie; }
    void set_varsta(int ani) { varsta = ani; }
    void set_energie(double fluctuatie)
    {
        energie = energie + fluctuatie;
        if (energie > energie_max)
            energie = energie_max;
    }
    void set_viu(unsigned char da_nu) { viu = da_nu; }
    static void set_inamic_mic(int ind) { inamic_mic = ind; }
    static void set_inamic_mare(int ind) { inamic_mare = ind; }
#pragma endregion

#pragma region Constructori si Destructori
    individ();
    individ(int poz, char specie);
    individ(individ& sursa);
    ~individ();
#pragma endregion

#pragma region Operatori
    friend istream_std& operator>>(istream_std&, individ&);
    friend ostream_std& operator<<(ostream_std&, const individ&);
    individ& operator =(individ&);
#pragma endregion

#pragma region Metode publice
    virtual std::istream& citire(std::istream& cin);
    virtual std::ostream& afisare(std::ostream& out) const;
    template <class oArma>
    void actualizare(individ* indivizi[]);
    bool esteviu();
    char gettip();
    static void initializ_inamici()
    {
        set_inamic_mic(-1);
        set_inamic_mare(-1);
    }
    int gasesteArma();    
#pragma endregion

};

int individ::inamic_mic;
int individ::inamic_mare;

#pragma region Operatori
int n_minus_i, nglobal;
std::istream& individ::citire(std::istream& cin)
{
    char specie;
    do
    {
        system("CLS");
        cout_std << "\n    Numar total de indivizi: " << nglobal << "\n    Numar de indivizi de creat: " << n_minus_i;
        cout_std << "\n\n       Alegeti specia individului curent ( introduceti 0, + sau ^ ):    ";
        try
        {
            std::string cspecie;
            cin >> cspecie;
            if (cspecie.size() != 1)
                throw '-';
            specie = cspecie[0];
        }
        catch (char)
        {
            specie = '-';
        }
    } while (specie != '0' and specie != '+' and specie != '^');
    set_tip(specie);
    return cin;
}

std::ostream& individ::afisare(std::ostream& cout) const
{
    cout << "\n    +--------------------------------------------------------------+\n    |";
    if (viu == '0' and i != -1)
    {
        style(4);
        cout << "mort    ";
        style(7);
        cout << "                                                      |";
    }
    else if (i == -1)
    {
        style(6);
        cout << "neocupat    ";
        style(7);
        cout << "                                                  |";
    }
    else
    {
        style(49);
        cout << "#" << i;
        style(7);
        cout << ":";
        if (i == 0)
            cout << "   ";
        else
            afis_spatii(i);
        cout << "tip ";
        if (tip == '0')
            style(12);
        else if (tip == '+')
            style(10);
        else
            style(3);
        cout << tip;
        style(7);
        cout << "   |   ";
        style(9);
        cout << varsta;
        style(7);
        if (varsta == 1)
            cout << " an ";
        else
            cout << " ani";
        if (varsta == 0)
            cout << "   ";
        else
            afis_spatii(varsta);
        cout << "|   ";
        cout << "energie ";
        if (energie >= 75)
            style(2);
        else if (energie >= 50)
            style(10);
        else if (energie >= 40)
            style(11);
        else if (energie >= 30)
            style(14);
        else if (energie >= 20)
            style(6);
        else if (energie >= 10)
            style(12);
        else
            style(4);
        cout << energie << "%";
        if (energie < 1)
            cout << "    ";
        else
            afis_spatii(int(energie));
        style(7);
        cout << " |   arma ";
        style(8);
        cout << get_armaSa()->get_display();
        style(7);
        afis_spatii2(get_armaSa()->get_display().length());
        cout << "  |";
    }
    cout << "\n    +--------------------------------------------------------------+\n";
    return cout;
}

istream_std& operator>>(istream_std& cin, individ& respectivul)
{
    respectivul.citire(cin);
    return cin;
}

ostream_std& operator<< (ostream_std& cout, const individ& dansul)
{
    dansul.afisare(cout);
    return cout;
}

individ& individ::operator=(individ& sursa)
{
    i = sursa.i;
    tip = sursa.tip;
    varsta = sursa.varsta;
    energie = sursa.energie;
    viu = sursa.viu;
    return *this;
}
#pragma endregion

#pragma region Construct & Destruct
individ::individ()  // individ gol - increatul
{
    i = -1;
    tip = '-';
    varsta = -1;
    energie = -1;
    viu = '0';

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
individ::individ(int poz, char specie): individ()  // individ complet, dar in afara vectorului de indivizi (un auxiliar practic)
{
    i = poz;
    tip = specie;
    varsta = 0;
    energie = energie_init;
    viu = '1';
}
individ::individ(individ& sursa)  // construcotrul de copiere; 
{
    i = sursa.i;
    tip = sursa.tip;
    varsta = sursa.varsta;
    energie = sursa.energie;
    viu = sursa.viu;

    indiceArma = sursa.indiceArma;
    armaSa = sursa.armaSa;
    arma1 = sursa.arma1;
    arma2 = sursa.arma2;
    arma3 = sursa.arma3;
}
individ::~individ()
{
    //cout_std << "Destructorul a distrus un obiect cu pozitia i = " << this->i << "\n";
}
#pragma endregion

#pragma region Metode & Functii
bool individ::esteviu()
{
    if (get_viu() == '1')
        return 1;
    return 0;
}

char individ::gettip()
{
    return tip;
}

void individ::hraneste(double bonus, double deficit)
{
    double fluctuatie = rand() % 10;
    fluctuatie = fluctuatie + 1 + bonus + deficit;
    set_energie(fluctuatie);
    if (get_energie() > energie_max)
    {
        fluctuatie = -1 * get_energie() + energie_max;
        set_energie(fluctuatie);
    }
}

void individ::urmas(individ* tata, int poz, double coef_e)
{
    const double coef_energ = coef_e;
    set_i(poz);
    set_tip(tata->get_tip());
    if (poz > tata->get_i())
        set_varsta(-1);
    else
        set_varsta(0);
    double fluctuatie = -1 * get_energie() + coef_energ * tata->get_energie();
    set_energie(fluctuatie);
    if (get_energie() > 0.75 * energie_max)
    {
        double fluctuatie = -1 * get_energie() + 0.75 * energie_max;
        set_energie(fluctuatie);
    }
    set_viu('1');
}

void individ::inmulteste(individ* indivizi[])
{
    const int varsta_min = 4;
    bool stop_breed = 0;
    if (get_varsta() < varsta_min)
        stop_breed = 1;
    if (i > 0)
        if (stop_breed == 0 and indivizi[i - 1]->esteviu() == 0)
        {
            stop_breed = 1;
            indivizi[i - 1]->urmas(indivizi[i], i - 1);
        }
    if (stop_breed == 0 and i < nmax - 1)
        if (indivizi[i + 1]->esteviu() == 0)
        {
            stop_breed = 1;
            indivizi[i + 1]->urmas(indivizi[i], i + 1);
        }
}

individ nowhere;
individ mort;
void PointToNowhere(individ* indivizi[], int len)
{
    for (int i = 0; i < len; i++)
        indivizi[i] = &nowhere;
}

void individ::moare(individ* indivizi[])
{
    indivizi[get_i()] = &mort;
    set_i(-1);
    set_tip('-');
    set_varsta(-1);
    set_energie(-1 * get_energie() - 1);
    set_viu('0');
}

void individ::imbatraneste(individ* indivizi[])
{
    set_varsta(get_varsta() + 1);
    set_energie(energie_1an);
    if (get_energie() <= 0 or get_varsta() > varsta_max)
        moare(indivizi);
}

void individ::inamicii(individ* indivizi[], bool teama)
{
    const int i = get_i();
    int inamicul_slab = get_inamic_mic();  //   = -1
    int inamicul_tare = get_inamic_mare();  //   = -1
    if (i > 0 and i < nmax - 1)
    {
        individ vecin_s, vecin_d;
        vecin_s = *indivizi[i - 1];
        vecin_d = *indivizi[i + 1];
        if (vecin_s.get_viu() == '1' and vecin_s.get_tip() != get_tip() and (vecin_s.get_energie() < get_energie() or teama == 0))
            inamicul_slab = i - 1;
        if (vecin_d.get_viu() == '1' and vecin_d.get_tip() != get_tip() and (vecin_d.get_energie() < get_energie() or teama == 0))
        {
            if (inamicul_slab == -1)
                inamicul_slab = i + 1;
            else if (vecin_s.get_energie() < vecin_d.get_energie())
                inamicul_tare = i + 1;
            else
            {
                inamicul_slab = i + 1;
                inamicul_tare = i - 1;
            }
        }
    }
    else if (i == 0)
    {
        individ vecin_d;
        vecin_d = *indivizi[i + 1];
        if (vecin_d.get_viu() == '1' and vecin_d.get_tip() != get_tip() and (vecin_d.get_energie() < get_energie() or teama == 0))
            inamicul_slab = i + 1;
    }
    else if (i == nmax - 1)
    {
        individ vecin_s;
        vecin_s = *indivizi[i - 1];
        if (vecin_s.get_viu() == '1' and vecin_s.get_tip() != get_tip() and (vecin_s.get_energie() < get_energie() or teama == 0))
            inamicul_slab = i - 1;
    }
    set_inamic_mic(inamicul_slab);
    set_inamic_mare(inamicul_tare);
}

void individ::efecte_atac(individ* indivizi[], int poz_inamic, double dmg, double coef_energie_atac)
{
    indivizi[poz_inamic]->set_energie(-1 * dmg);
    if (indivizi[poz_inamic]->get_energie() <= 0)
    {
        double fluctuatie = -1 * indivizi[poz_inamic]->get_energie();
        indivizi[poz_inamic]->set_energie(fluctuatie);
        indivizi[poz_inamic]->moare(indivizi);
    }
    double fluctuatie = -1 * coef_energie_atac * get_energie();
    set_energie(fluctuatie);
}

bool atacat = false;
double individ::calculeaza_dmg(int varsta, double energie, double coef1_atac, double coef2_atac)
{
    const int dif_max_varsta = 6;
    int dif_varsta = varsta_apogeu - varsta;
    if (dif_varsta < 0)
        dif_varsta = -1 * dif_varsta;
    int parametru_varsta = dif_max_varsta - dif_varsta;

    double dmg = coef1_atac * energie + coef2_atac * parametru_varsta + extraDmg(get_armaSa());
    atacat = true;
    istoricii_noteaza << " cu " << dmg << "% dmg, din care " << extraDmg(get_armaSa()) << "% de la arma sa  " << get_armaSa()->get_display();
    return dmg;
}

void individ::ataca(individ* indivizi[], int vma, bool teama, double c1a, double c2a, double cea)
{
    const int i = get_i(), varsta = get_varsta();
    const double energie = get_energie();

    const int varsta_min_atac = vma;
    const double coef1_atac = c1a, coef2_atac = c2a, coef_energie_atac = cea;

    if (varsta < varsta_min_atac)
        return;

    individ::inamicii(indivizi, teama);
    int inamicul_slab = individ::get_inamic_mic();
    int inamicul_tare = individ::get_inamic_mare();
    individ::initializ_inamici();

    if (inamicul_slab != -1)
    {
        istoricii_noteaza << "\n      Individul de pe pozitia " << inamicul_slab << " a fost atacat de vecinul de pe pozitia " << i;
        double dmg = calculeaza_dmg(varsta, energie, coef1_atac, coef2_atac);
        efecte_atac(indivizi, inamicul_slab, dmg, coef_energie_atac);
    }
    if (inamicul_tare != -1 and get_energie() > indivizi[inamicul_tare]->get_energie())
    {
        istoricii_noteaza << "\n      Individul de pe pozitia " << inamicul_tare << " a fost atacat de vecinul de pe pozitia " << i;
        double dmg = calculeaza_dmg(varsta, energie, coef1_atac, coef2_atac);
        efecte_atac(indivizi, inamicul_tare, dmg, coef_energie_atac);
    }
}

int individ::gasesteArma()
{
    if (armaSa == &arma1)
        return 1;
    else if (armaSa == &arma2)
        return 2;
    else if (armaSa == &arma3)
        return 3;

    return -1;
}

void setare_nr_indivizi(int& n)
{
    while (n < 1 or n>nmax)
    {
        system("CLS");
        cout_std << "\n    Alegeti numarul de indivizi (de la 1 la " << nmax << "):    ";
        try
        {
            std::string cn;
            cin_std >> cn;
            if (cn.size() > nr_cifre(nmax) or cn[0] == '0')
                throw '-';
            int cn2 = 0;
            for (unsigned int i = 0; i < cn.size(); i++)
                if (cn[i] < '0' or cn[i] > '9')
                    throw '-';
                else
                    cn2 = cn2 * 10 + cn[i] - '0';
            if (cn2 > nmax)
                throw '-';
            n = cn2;
        }
        catch (char)
        {
            n = -1;
        }
        cout_std << "\n";
    }
}

void mesaj_setare_tip_individ(int n, int i, individ* indivizi[], individ respectivul)
{
    system("CLS");
    cout_std << "\n    Numar total de indivizi: " << n << "\n    Numar de indivizi de creat: " << n - i << "\n   ";
    cout_std << "\n\n       Tipul ales pentru individul curent: ";
    switch (respectivul.gettip())
    {
    case '+':
    {
        style(10);
        cout_std << "+";
    }
    break;
    case '0':
    {
        style(12);
        cout_std << "0";
    }
    break;
    case '^':
    {
        style(3);
        cout_std << "^";
    }
    }
    style(7);
    cout_std << "\n       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
}

void mesaj_setare_pozitie_individ(individ* indivizi[])
{
    cout_std << "\n\n       Alegeti o pozitie neocupata pentru individul curent:  \n       ";
    for (int i = 0; i < nmax; i++)
        if (indivizi[i]->esteviu() == 0)
            cout_std << i << "  ";
        else
        {
            switch (indivizi[i]->gettip())
            {
            case '+':
            {
                style(10);
                cout_std << "+  ";
            }
            break;
            case '0':
            {
                style(12);
                cout_std << "0  ";
            }
            break;
            case '^':
            {
                style(3);
                cout_std << "^  ";
            }
            }
            style(7);
        }
    cout_std << "\n\n         Pozitia aleasa:    ";
}

int alesul_pt_meniu(int& ales)
{
    try
    {
        std::string c_ales;
        cin_std >> c_ales;
        if (c_ales.size() > nr_cifre(nmax - 1))
            throw '-';
        int c_ales2 = 0;
        for (unsigned int i = 0; i < c_ales.size(); i++)
            c_ales2 = c_ales2 * 10 + c_ales[i] - '0';
        if (c_ales2 > nmax - 1)
            throw '-';
        ales = c_ales2;
    }
    catch (char)
    {
        ales = -1;
    }
    return ales;
}

void mesaj_ales_neviu(int ales)
{
    cout_std << "   Individul din regiunea " << ales << " ";
    style(12);
    cout_std << "NU";
    style(7);
    cout_std << " este viu!\n";
}

void mesaj_orice_tasta()
{
    cout_std << "\n       Introducti orice caracter pentru a reveni la meniu: ";
    std::string orice;
    cin_std >> orice;
}

int Anul = 0;
void afisarea_de_la_case4(individ* indivizi[])
{
    system("CLS");
    style(6);

    cout_std << "      ";
    for (int i = 0; i < (Anul%52); i = i + 1)
        cout_std << " ";
    cout_std << "{@}\n        # ___________________";
    cout_std << "#   %  % #.   %   ## %  #   %    %#";
    cout_std << "\n        |  ||  Anul " << Anul;
    if (Anul == 0)
        cout_std << "   ";
    else
        afis_spatii(Anul);
    cout_std << "  || |   |  | |4v  |   || |  |   |,   ||";
    cout_std << "\n    .~~._____________~~._____________%%._____________~~.________.~~.\n     ||              ||              ||              ||          ||";
    style(7);
    for (int i = 0; i < nmax; i++)
        cout_std << *indivizi[i];
}
#pragma endregion

#pragma region Razboinic
class razboinic : public individ
{
    static unsigned int nr_razboinici;

    void efecte_inmultire(individ* indivizi[], int poz_tata, int poz_fiu, bool& stop_breed);
    void inmulteste(individ* indivizi[]);
    void moare(individ* indivizi[]);
    void ataca(individ* indivizi[]);

public:
    razboinic(int poz) : individ(poz, '0') {}
    razboinic() : individ() {}

    static void initializ_nr_razboinici();
    static void creste_nr_razboinici();
    static void scade_nr_razboinici();
    static unsigned int get_nr_razboinici() { return nr_razboinici; }
};

razboinic razboinici[nmax];
unsigned int razboinic::nr_razboinici;

void razboinic::initializ_nr_razboinici()
{
    nr_razboinici = 0;
}

void razboinic::creste_nr_razboinici()
{
    nr_razboinici++;
}

void razboinic::scade_nr_razboinici()
{
    nr_razboinici--;
}

void creare_razboinic(individ* indivizi[], int poz)
{
    razboinic aux(poz);
    razboinici[poz] = aux;
    indivizi[poz] = &razboinici[poz];
    razboinic::creste_nr_razboinici();
}

void razboinic::efecte_inmultire(individ* indivizi[], int poz_tata, int poz_fiu, bool& stop_breed)
{
    istoricii_noteaza << "\n      Razboinicul de pe pozitia " << poz_tata << " are un urmas pe pozitia ";
    stop_breed = 1;
    creare_razboinic(indivizi, poz_fiu);
    razboinici[poz_fiu].urmas(indivizi[poz_tata], poz_fiu);
    istoricii_noteaza << razboinici[poz_fiu].get_i();
}

void razboinic::inmulteste(individ* indivizi[])
{
    const int varsta_min = 4, indcrt = get_i();
    bool stop_breed = 0;
    if (get_varsta() < varsta_min)
        stop_breed = 1;
    if (indcrt > 0)
        if (stop_breed == 0 and indivizi[indcrt - 1]->esteviu() == 0)
            efecte_inmultire(indivizi, indcrt, indcrt - 1, stop_breed);
    if (stop_breed == 0 and indcrt < nmax - 1)
        if (indivizi[indcrt + 1]->esteviu() == 0)
            efecte_inmultire(indivizi, indcrt, indcrt + 1, stop_breed);
}

void razboinic::moare(individ* indivizi[])
{
    istoricii_noteaza << "\n      Razboinicul de pe pozitia " << get_i() << " a murit!";
    individ::moare(indivizi);
    scade_nr_razboinici();
}

void razboinic::ataca(individ* indivizi[])
{
    individ::ataca(indivizi, 2, true, 0.4, 1.25);
}
#pragma endregion

#pragma region Colonist
class colonist : public individ
{
    static unsigned int nr_colonisti;

    void hraneste();
    void efecte_inmultire(individ* indivizi[], int poz_tata, int poz_fiu, bool& stop_breed);
    void inmulteste(individ* indivizi[]);
    void moare(individ* indivizi[]);
    void ataca(individ* indivizi[]);

public:
    colonist(int poz) : individ(poz, '+') {}
    colonist() : individ() {}

    static void initializ_nr_colonisti();
    static void creste_nr_colonisti();
    static void scade_nr_colonisti();
    static unsigned int get_nr_colonisti() { return nr_colonisti; }
};

colonist colonisti[nmax];
unsigned int colonist::nr_colonisti;

void colonist::initializ_nr_colonisti()
{
    nr_colonisti = 0;
}

void colonist::creste_nr_colonisti()
{
    nr_colonisti++;
}

void colonist::scade_nr_colonisti()
{
    nr_colonisti--;
}

void colonist::hraneste()
{
    individ::hraneste(2.75);
}

void creare_colonist(individ* indivizi[], int poz)
{
    colonist aux(poz);
    colonisti[poz] = aux;
    indivizi[poz] = &colonisti[poz];
    colonist::creste_nr_colonisti();
}

void colonist::efecte_inmultire(individ* indivizi[], int poz_tata, int poz_fiu, bool& stop_breed)
{
    istoricii_noteaza << "\n      Colonistul de pe pozitia " << poz_tata << " are un urmas pe pozitia ";
    stop_breed = 1;
    creare_colonist(indivizi, poz_fiu);
    colonisti[poz_fiu].urmas(indivizi[poz_tata], poz_fiu);
    istoricii_noteaza << colonisti[poz_fiu].get_i();
}

void colonist::inmulteste(individ* indivizi[])
{
    const int varsta_min = 3, indcrt = get_i();
    bool stop_breed = 0;
    if (get_varsta() < varsta_min)
        stop_breed = 1;
    if (indcrt > 0)
        if (stop_breed == 0 and indivizi[indcrt - 1]->esteviu() == 0)
            efecte_inmultire(indivizi, indcrt, indcrt - 1, stop_breed);
    if (stop_breed == 0 and indcrt < nmax - 1)
        if (indivizi[indcrt + 1]->esteviu() == 0)
            efecte_inmultire(indivizi, indcrt, indcrt + 1, stop_breed);
}

void colonist::moare(individ* indivizi[])
{
    istoricii_noteaza << "\n      Colonistul de pe pozitia " << get_i() << " a murit!";
    individ::moare(indivizi);
    scade_nr_colonisti();
}

void colonist::ataca(individ* indivizi[])
{
    individ::ataca(indivizi, 3, true, 0.25);
}
#pragma endregion

#pragma region Nomad
class nomad : public individ
{
    static unsigned int nr_nomazi;

    void hraneste();
    void efecte_inmultire(individ* indivizi[], int poz_tata, int poz_fiu, bool& stop_breed);
    void inmulteste(individ* indivizi[]);
    void moare(individ* indivizi[]);
    void ataca(individ* indivizi[]);
    void efecte_prada(individ* indivizi[], int indice_prada, double coef_energie_prada = 0.05);

public:
    nomad(int poz) : individ(poz, '^') {}
    nomad() : individ() {}

    static void initializ_nr_nomazi();
    static void creste_nr_nomazi();
    static void scade_nr_nomazi();
    static unsigned int get_nr_nomazi() { return nr_nomazi; }
    void prada(individ* indivizi[]);
};

nomad nomazi[nmax];
unsigned int nomad::nr_nomazi;

void nomad::initializ_nr_nomazi()
{
    nr_nomazi = 0;
}

void nomad::creste_nr_nomazi()
{
    nr_nomazi++;
}

void nomad::scade_nr_nomazi()
{
    nr_nomazi--;
}

void nomad::hraneste()
{
    individ::hraneste(0, -1.90);
}

void creare_nomad(individ* indivizi[], int poz)
{
    nomad aux(poz);
    nomazi[poz] = aux;
    indivizi[poz] = &nomazi[poz];
    nomad::creste_nr_nomazi();
}

void nomad::efecte_inmultire(individ* indivizi[], int poz_tata, int poz_fiu, bool& stop_breed)
{
    istoricii_noteaza << "\n      Nomadul de pe pozitia " << poz_tata << " are un urmas pe pozitia ";
    stop_breed = 1;
    creare_nomad(indivizi, poz_fiu);
    nomazi[poz_fiu].urmas(indivizi[poz_tata], poz_fiu);
    istoricii_noteaza << nomazi[poz_fiu].get_i();
}

void nomad::inmulteste(individ* indivizi[])
{
    const int varsta_min = 4, indcrt = get_i();
    bool stop_breed = 0;
    if (get_varsta() < varsta_min)
        stop_breed = 1;
    if (indcrt > 0)
        if (stop_breed == 0 and indivizi[indcrt - 1]->esteviu() == 0)
            efecte_inmultire(indivizi, indcrt, indcrt - 1, stop_breed);
    if (stop_breed == 0 and indcrt < nmax - 1)
        if (indivizi[indcrt + 1]->esteviu() == 0)
            efecte_inmultire(indivizi, indcrt, indcrt + 1, stop_breed);
}

void nomad::moare(individ* indivizi[])
{
    istoricii_noteaza << "\n      Nomadul de pe pozitia " << get_i() << " a murit!";
    individ::moare(indivizi);
    scade_nr_nomazi();
}

void nomad::ataca(individ* indivizi[])
{
    individ::ataca(indivizi, 2, false, 0.3, 1.25, 0);
}

void nomad::efecte_prada(individ* indivizi[], int indice_prada, double coef_energie_prada)
{
    istoricii_noteaza << "\n      Nomadul de pe pozitia " << get_i() << " a pradat ";
    if (indivizi[indice_prada]->get_tip() == '+')
        coef_energie_prada = 0.07;
    double prada = coef_energie_prada * indivizi[indice_prada]->get_energie();
    istoricii_noteaza << prada << "% energie de la inamicul de pe pozitia " << indivizi[indice_prada]->get_i();
    set_energie(prada);
    efecte_atac(indivizi, indice_prada, prada, 0);
}

void nomad::prada(individ* indivizi[])
{
    double zi_buna = rand() % 10;
    if (zi_buna < 3.5)
        return;

    const int i = get_i(), varsta = get_varsta();
    const double energie = get_energie();

    const int varsta_min_prada = 2;

    if (varsta < varsta_min_prada)
        return;

    individ::inamicii(indivizi, 0);
    int inamicul_slab = individ::get_inamic_mic();
    int inamicul_tare = individ::get_inamic_mare();
    individ::initializ_inamici();

    if (inamicul_tare != -1)
        efecte_prada(indivizi, inamicul_tare);
    else if (inamicul_slab != -1)
        efecte_prada(indivizi, inamicul_slab);
}
#pragma endregion

template<class oArma> void individ::actualizare(individ* indivizi[])
{
    hraneste();
    imbatraneste(indivizi);
    if (get_viu() == '1')
    {
        inmulteste(indivizi);
        nomad* hotoman = dynamic_cast<nomad*>(this);
        if (hotoman != NULL)
            hotoman->prada(indivizi);
        ataca(indivizi);
    }
}
template<>
void individ::actualizare<arc>(individ* indivizi[])
{
    hraneste();
    imbatraneste(indivizi);
    if (get_viu() == '1')
    {
        inmulteste(indivizi);
        nomad* hotoman = dynamic_cast<nomad*>(this);
        if (hotoman != NULL)
            hotoman->prada(indivizi);
        ataca(indivizi);
        if (atacat == true)
        {
            arma3.scadeTolba();
            atacat = false;
        }

        if (varsta % 2 == 1)
            arma3.cresteTolba();

        arma3.actualizeaza_display();
    }
}
void actualizarea_de_la_case1(individ* indivizi[])
{
    Anul++;
    istoricii_noteaza << "\n    Anul " << Anul << ":";
    for (int i = 0; i < nmax; i++)
        if (indivizi[i]->esteviu() == 1)
        {
            switch (indivizi[i]->gasesteArma())
            {
            case 1:
            {
                indivizi[i]->actualizare<sabie>(indivizi);
            }
            break;
            case 2:
            {
                indivizi[i]->actualizare<topor>(indivizi);
            }
            break;
            case 3:
            {
                indivizi[i]->actualizare<arc>(indivizi);
            }
            break;
            default:
            {
                istoricii_noteaza << "\n    Un individ nu a putut fi identificat. Ce ciudat...";
            }
            }
        }
    system("CLS");
    for (int i = 0; i < nmax; i++)
        cout_std << *indivizi[i];
    istoricii_noteaza << "\n      | Numar razboinici:  " << razboinic::get_nr_razboinici();
    istoricii_noteaza << "\n      | Numar colonisti:  " << colonist::get_nr_colonisti();
    istoricii_noteaza << "\n      | Numar nomazi:  " << nomad::get_nr_nomazi();
    istoricii_noteaza << "\n";
}

void Meniu(individ* indivizi[])
{
    int n = -1;
    setare_nr_indivizi(n);

    for (int i = 0; i < n; i++)
    {
        system("CLS");
        cout_std << "\n    Numar total de indivizi: " << n << "\n    Numar de indivizi de creat: " << n - i;
        n_minus_i = n - i;
        nglobal = n;
        individ respectivul;
        cin_std >> respectivul;
        bool ok = 0;
        while (ok == 0)
        {
            int poz = -1;
            while (poz < 0 or poz > nmax - 1)
            {
                mesaj_setare_tip_individ(n, i, indivizi, respectivul);
                mesaj_setare_pozitie_individ(indivizi);
                try
                {
                    std::string cpoz;
                    cin_std >> cpoz;
                    if (cpoz.size() > nr_cifre(nmax - 1) or cpoz.size() > 1 and cpoz[0] == '0')
                        throw '-';
                    int cpoz2 = 0;
                    for (unsigned int i = 0; i < cpoz.size(); i++)
                    {
                        if (cpoz[i] < '0' or cpoz[i] > '9')
                            throw '-';
                        cpoz2 = cpoz2 * 10 + cpoz[i] - '0';
                    }
                    if (cpoz2 > nmax - 1)
                        throw '-';
                    poz = cpoz2;
                }
                catch (char)
                {
                    poz = -1;
                }
                cout_std << "\n";
            }
            if (indivizi[poz]->esteviu() == 0)
            {
                ok = 1;
                switch (respectivul.gettip())
                {
                case '0':
                {
                    creare_razboinic(indivizi, poz);
                }
                break;
                case '+':
                {
                    creare_colonist(indivizi, poz);
                }
                break;
                case '^':
                {
                    creare_nomad(indivizi, poz);
                }
                }
            }
        }
    }

    short int opt = -1;
    system("CLS");
    do
    {
        cout_std << "\n    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
        cout_std << "\n        Meniu de optiuni:\n    1) actualizare\n    2) esteviu\n    3) gettip\n    4) afisarea indivizilor\n    0) iesire\n\n    Optiunea aleasa:    ";
        bool corect = 1;
        try
        {
            std::string c_opt;
            cin_std >> c_opt;
            if (c_opt.size() != 1 or c_opt[0] < '0' and c_opt[0] > '4')
                throw '-';
            opt = c_opt[0] - '0';
        }
        catch (char)
        {
            corect = 0;
        }
        cout_std << "\n    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

        if (corect == 1)
        {
            switch (opt)
            {
            case 1:
            {
                //actualizare
                actualizarea_de_la_case1(indivizi);
            }
            break;
            case 2:
            {
                //esteviu
                int ales = -1;
                while (ales < 0 or ales >= nmax)
                {
                    system("CLS");
                    cout_std << "\n    Selectati un numar intre 0 si " << nmax - 1 << " pentru a afla daca este viu:    ";
                    ales = alesul_pt_meniu(ales);
                    cout_std << "\n\n    ";
                }
                if (indivizi[ales]->esteviu() == 0)
                    mesaj_ales_neviu(ales);
                else
                    cout_std << "   Individul din regiunea " << ales << " este viu!\n";
                mesaj_orice_tasta();
            }
            break;
            case 3:
            {
                //gettip
                int ales = -1;
                while (ales < 0 or ales >= nmax)
                {
                    system("CLS");
                    cout_std << "\n    Selectati un numar intre 0 si " << nmax - 1 << " pentru a afla tipul sau:    ";
                    ales = alesul_pt_meniu(ales);
                    cout_std << "\n\n    ";
                }
                if (indivizi[ales]->esteviu() == 0)
                    mesaj_ales_neviu(ales);
                else
                {
                    cout_std << "   Individul din regiunea " << ales << " este de tipul ";
                    if (indivizi[ales]->gettip() == '0')
                        style(12);
                    else if (indivizi[ales]->gettip() == '+')
                        style(10);
                    else
                        style(3);
                    cout_std << indivizi[ales]->gettip() << "\n";
                    style(7);
                }
                mesaj_orice_tasta();
            }
            break;
            case 4:
            {
                //afisarea_indivizilor: se realizeaza in while-ul de dupa switch
                //afisarea_de_la_case4(indivizi);
            }
            break;
            case 0:
            {
                //iesire
                system("CLS");
                style(9);
                cout_std << "\n\n      Simularea a luat sfarsit.\n      Niciun individ nu a fost ranit la realizarea programului. \n\n";
                style(7);
                return;
            }
            }

            int cont = -1;
            while (opt == 1 or opt == 4)
            {
                do
                {
                    afisarea_de_la_case4(indivizi);
                    cout_std << "\n\n    ";
                    style(49);
                    cout_std << "Selectati 1 pentru actualizarea indivizilor sau 2 pentru a reveni la meniu :    ";
                    style(7);
                    cout_std << "\n    ";
                    try
                    {
                        std::string c_cont;
                        cin_std >> c_cont;
                        if (c_cont.size() != 1 or c_cont[0] != '1' and c_cont[0] != '2')
                            throw '-';
                        cont = c_cont[0] - '0';
                    }
                    catch (char)
                    {
                        cont = -1;
                    }
                } while (cont == -1);
                if (cont == 1)
                {
                    actualizarea_de_la_case1(indivizi);
                    cont = -1;
                }
                else
                    break;
            }

            cout_std << "\n";
            if (cont)
            {
                opt = -1;
                system("CLS");
            }
        }
        else
            system("CLS");
    } while (opt < 1 or opt >4);

    style(9);
    cout_std << "\n   Simularea a luat sfarsit.\n   Niciun individ nu a fost ranit la realizarea programului. \n\n";
    style(7);
}

int main()
{
    srand(time(NULL));
    std::cout << std::fixed << std::setprecision(2) << std::left;
    istoricii_noteaza << std::fixed << std::setprecision(2) << std::left;

    individ* indivizi[nmax];
    PointToNowhere(indivizi, nmax);
    individ::initializ_inamici();
    mort.set_i(-2);

    razboinic::initializ_nr_razboinici();
    colonist::initializ_nr_colonisti();
    nomad::initializ_nr_nomazi();

    Meniu(indivizi);
    //allStyle();

    return 0;
}
