#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <stack>
#include <set>
#include <map>
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <cmath>
#include <cstring>
//#include <utility>

std::ifstream f("graful.in");
std::ofstream g("graful.out");
//std::ifstream f("barbar.in");
//std::ofstream g("barbar.out");


int start = 0, contor;        // contorul il vom folosi in mai multe probleme;  start va aparea eventual ca nod de start
const int nodgol_int = 0;  

///  in special pentru problema ciclu Hamiltonian:
const int nrvfMax = 18;
const int configMax = (1 << nrvfMax) +	5;          // o configuratie este o reprezentare binara in care bitii setati indica nodurile utilizate intr-un drum
const int inf = (const int)( nrvfMax * 1e6 + 1);  // costul maxim al unui arc este de 10^6, iar un ciclu Hamiltonian va avea exact nrvf arce (si nu vom tine evidenta unor drumuri mai lungi)
std::vector<int>* noduriIncidente;

///  pentru problema lui paftenie barbarul:
char** temnita;

#define tmp_moneda template <typename moneda>

template <typename moneda = int>
class graf;
template <typename moneda = int>
class subgraf;
template <typename moneda = int>
class vecin;
template <typename moneda = int>
class muchie;

#pragma region vecin
template <typename moneda = int>
std::ostream& operator<< (std::ostream& out, const vecin<moneda>& vecinul);


template <typename moneda>
class vecin
{
public:
	int index;                 // eticheta sa
	moneda cost;           // costul muchiei catre acest vecin
	int capacitate;
	int flux;
	vecin* vecinInvers;  // pointer catre muchia cu sens opus (pentru grafuri cu arce inverse, din graful rezidual)

	vecin(int idx = 0, moneda costul = 0, int capacitatea = 0, int fluxul = 0) : index(idx), cost(costul), capacitate(capacitatea), flux(fluxul), vecinInvers(NULL) {}
	vecin(const muchie<moneda> &sursa) : index(sursa.getvf2()), cost(sursa.getcost()), capacitate(sursa.getcap()), flux(sursa.getflux()), vecinInvers(NULL) {}

	int getidx() const { return index; }
	moneda getcost() const { return cost; }
	int getcap() const { return capacitate; }
	int getflux() const { return flux; }

	friend std::ostream& operator<< <moneda>(std::ostream& out, const vecin<moneda>& vecinul);
};
template <typename moneda>
std::ostream& operator<< (std::ostream& out, const vecin<moneda>& vecinul)
{
	out << vecinul.index << " (" << vecinul.cost << " $) (" << vecinul.capacitate << " cap, " << vecinul.flux << " flux) | ";
	return out;
}
#pragma endregion

#pragma region muchie
// declar prin antet operatorii ce ii vor fi prieteni: 
tmp_moneda
std::ostream& operator<<(std::ostream& out, const muchie<moneda>& m);
tmp_moneda
std::ofstream& operator<<(std::ofstream& out, const muchie<moneda>& m);


template <typename moneda>
class muchie
{
public:
	int vf1;
	int vf2;
	moneda cost;
	int capacitate;
	int flux;

	muchie(int varf1 = 0, int varf2 = 0, moneda costul = 0, int capacitatea = 0, int fluxul = 0) : vf1(varf1), vf2(varf2), cost(costul), capacitate(capacitatea), flux(fluxul) {}
	muchie(const muchie < moneda >& sursa) : vf1(sursa.getvf1()), vf2(sursa.getvf2()), cost(sursa.getcost()), capacitate(sursa.getcap()), flux(sursa.getflux()) {}
	muchie(int vf1_param, const vecin<moneda> &sursa): vf1(vf1_param), vf2(sursa.getidx()), cost(sursa.getcost()), capacitate(sursa.getcap()), flux(sursa.getflux()) {}

	void setAll(int varf1, int varf2, moneda costul = 0, int capacitatea = 0, int fluxul = 0)
	{
		vf1 = varf1;
		vf2 = varf2;
		cost = costul;
		capacitate = capacitatea;
		flux = fluxul;
	}
	int getvf1() const { return vf1; }
	int getvf2() const { return vf2; }
	moneda getcost() const { return cost; }
	int getcap() const { return capacitate; }
	int getflux() const { return flux; }

	bool operator == (const muchie<moneda>& m) const
	{
		return (vf1 == m.getvf1() and vf2 == m.getvf2() and cost == m.cost);
	}
	bool operator < (const muchie<moneda>& m) const
	{
		if (cost < m.getcost())
			return true;
		if (cost == m.getcost())
		{
			if (vf1 < m.getvf1())
				return true;
			if (vf1 == m.getvf1() and vf2 < m.getvf2())
				return true;
		}
		return false;
	}
	bool operator > (const muchie<moneda>& m) const
	{
		if (cost > m.getcost())
			return true;
		if (cost == m.getcost())
		{
			if (vf1 > m.getvf1())
				return true;
			if (vf1 == m.getvf1() and vf2 > m.getvf2())
				return true;
		}
		return false;
	}
	muchie<moneda> operator=(const muchie<moneda>& sursa)
	{
		vf1 = sursa.vf1;
		vf2 = sursa.vf2;
		cost = sursa.cost;
		return *this;
	}

	friend std::ostream& operator<< <moneda>(std::ostream& out, const muchie<moneda>& m);		// aici specific ca operatorii mai sus declarati sunt prieteni

	friend std::ofstream& operator<< <moneda>(std::ofstream& out, const muchie<moneda>& m);
};

tmp_moneda			// si aici specific implementarea operatorilor prieten
std::ostream& operator<< (std::ostream& out, const muchie<moneda>& m)
{
	out << m.vf1 << " " << m.vf2 << " " << m.cost;
	return out;
}
tmp_moneda
std::ofstream& operator<< (std::ofstream& out, const muchie<moneda>& m)
{
	out << m.vf1 << " " << m.vf2;
	return out;
}

namespace std
{
	template<typename moneda>
	struct hash< muchie<moneda> >
	{
		// operator() pentru cast de la hash < muchie<moneda> >  la unsigned int (hashcode-ul asociat inputului)
		unsigned int operator() (const muchie< moneda >& m) const
		{
			const int nrprim = 31;

			unsigned int hashcode = hash< int >()(m.getvf1());
			hashcode = hashcode * nrprim + hash< int >()(m.getvf2());
			hashcode = hashcode * nrprim + hash< moneda >()(m.getcost());
			return hashcode;
		}
	};

	template<typename data = int >
	ostream& operator<<(ostream& out, const set< data >& s)
	{
		out << "\n { ";
		for (auto it = s.begin(); it != s.end(); it++)
			out << *it << " | ";
		out << "}";
		return out;
	}
	template<typename data = int >
	ofstream& operator<<(ofstream& out, const set< data >& s)
	{
		out << "\n { ";
		for (auto it = s.begin(); it != s.end(); it++)
			out << *it << " | ";
		out << "}";
		return out;
	}
}
#pragma endregion

namespace treap
{
#define tmp_kt_pt template<typename kt = int, typename pt = int>

	template<typename key_type = int, typename priority_type = int>
	struct nod;

	nod<int, long long int>* R, * nodgol;

	template<typename key_type, typename priority_type>
	struct nod
	{
		key_type key;
		priority_type priority;
		nod* left;
		nod* right;

		nod()
		{
			left = nodgol;
			right = nodgol;
			key = 404;
			priority = -404;
		}
		nod(key_type cheie, priority_type prioritate, nod* stang, nod* drept) : key(cheie), priority(prioritate), left(stang), right(drept) {}
		nod* operator = (const nod*& sursa)
		{
			key = sursa->key;
			priority = sursa->priority;
			left = sursa->left;
			right = sursa->right;
			return this;
		}
		void afisare()
		{
			std::cout << "  { " << key << " : " << priority << " }";
		}
	};


	tmp_kt_pt
		nod<kt, pt>* cauta(nod<kt, pt>* crt, kt cheie_cautata)
	{
		if (crt == nodgol)
			return nodgol;

		if (cheie_cautata == crt->key)
			return crt;
		if (cheie_cautata < crt->key)
			return cauta(crt->left, cheie_cautata);
		return cauta(crt->right, cheie_cautata);
	}

	tmp_kt_pt
		void rotire_st(nod<kt, pt>*& crt)
	{
		nod<kt, pt>* stangul = crt->left;
		crt->left = stangul->right;
		stangul->right = crt;
		crt = stangul;
	}

	tmp_kt_pt
		void rotire_dr(nod<kt, pt>*& crt)
	{
		nod<kt, pt>* dreptul = crt->right;
		crt->right = dreptul->left;
		dreptul->left = crt;
		crt = dreptul;
	}

	tmp_kt_pt
		void balans(nod<kt, pt>*& crt)
	{
		if (crt->left->priority > crt->priority)
			rotire_st(crt);
		else if (crt->right->priority > crt->priority)
			rotire_dr(crt);
	}

	tmp_kt_pt
		void balans_invers(nod<kt, pt>*& crt)
	{
		if (crt->left->priority < crt->priority)
			rotire_st(crt);
		else if (crt->right->priority < crt->priority)
			rotire_dr(crt);
	}

	tmp_kt_pt
		void inserare(nod<kt, pt>*& crt, kt cheie, pt prioritate, bool inverseaza_prioritate = false)
	{
		if (crt == nodgol)
		{
			crt = new nod<kt, pt>(cheie, prioritate, nodgol, nodgol);
			return;
		}
		if (cheie < crt->key)
			inserare(crt->left, cheie, prioritate, inverseaza_prioritate);
		else if (cheie > crt->key)
			inserare(crt->right, cheie, prioritate, inverseaza_prioritate);

		if (!inverseaza_prioritate)
			balans(crt);
		else
			balans_invers(crt);
	}

	tmp_kt_pt
		void stergere(nod<kt, pt>*& crt, kt cheie, bool inverseaza_prioritate = false)
	{
		if (crt == nodgol)
			return;

		if (cheie < crt->key)
			stergere(crt->left, cheie, inverseaza_prioritate);
		else if (cheie > crt->key)
			stergere(crt->right, cheie, inverseaza_prioritate);
		else
		{
			if (crt->left == nodgol and crt->right == nodgol)
			{
				delete crt;
				crt = nodgol;
			}
			else
			{
				if (!inverseaza_prioritate)
				{
					if (crt->left->priority > crt->right->priority)
						rotire_st(crt);
					else
						rotire_dr(crt);
				}
				else
				{
					if (crt->left->priority < crt->right->priority)
						rotire_st(crt);
					else
						rotire_dr(crt);
				}
				stergere(crt, cheie, inverseaza_prioritate);
			}
		}
	}

	tmp_kt_pt
		void split(nod<kt, pt>*& root, nod<kt, pt>*& root_left, nod<kt, pt>*& root_right, kt cheie, bool inverseaza_prioritate = false)
	{
		const pt infinit = std::numeric_limits<pt>::max();
		inserare(root, cheie, infinit, inverseaza_prioritate);
		root_left = root->left;
		root_right = root->right;
		delete root;
		root = nodgol;
	}

	tmp_kt_pt
		void join(nod<kt, pt>*& root, nod<kt, pt>*& root_left, nod<kt, pt>*& root_right, kt cheie, bool inverseaza_prioritate = false)
	{
		root = new nod<kt, pt>(cheie, 0, root_left, root_right);
		stergere(root, root->key, inverseaza_prioritate);
	}

	tmp_kt_pt
		void afis_elem_sortate(nod<kt, pt>* crt)
	{
		if (crt == NULL)
			return;
		if (crt->left != nodgol)
			afis_elem_sortate(crt->left);
		crt->afisare();
		if (crt->right != nodgol)
			afis_elem_sortate(crt->right);
	}

	tmp_kt_pt
		void succesor(const nod<kt, pt>* crt, kt cheie, kt& result = -1, char poz_relativa = 'x', int& countdown = 2)
	{
		if (countdown == 2)		//   coboram sa cautam cheia / urcam inapoi sa luam primul nod care a coborat pe fiul stang
		{
			if (crt->key != cheie)
			{
				if (crt->left != nodgol and cheie < crt->key)
				{
					poz_relativa = 's';
					succesor(crt->left, cheie, result, poz_relativa, countdown);
				}
				else if (crt->right != nodgol and crt->key < cheie)
				{
					poz_relativa = 'd';
					succesor(crt->right, cheie, result, poz_relativa, countdown);
				}

				if (countdown == 2 and poz_relativa == 's')
				{
					countdown = 0;
					result = crt->key;
				}
			}
			else if (crt->right != nodgol)	  //  daca exista subarborele drept, continuam cautarea acolo; 
			{											  //  daca nu, ne intoarcem din functiile recursive
				countdown = 1;
				succesor(crt->right, cheie, result, poz_relativa, countdown);
			}
		}
		else if (countdown == 1)	// dupa ce am gasit cheia, cautam cel mai stang nod din subarborele drept
		{
			if (crt->left == nodgol)
			{
				countdown = 0;
				result = crt->key;
			}
			else
				succesor(crt->left, cheie, result, poz_relativa, countdown);
		}
	}

	tmp_kt_pt
		void predecesor(const nod<kt, pt>* crt, kt cheie, kt& result = -1, char poz_relativa = 'x', int& countdown = 2)
	{
		if (countdown == 2)		//   coboram sa cautam cheia / urcam inapoi sa luam primul nod care a coborat pe fiul drept
		{
			if (crt->key != cheie)
			{
				if (crt->left != nodgol and cheie < crt->key)
				{
					poz_relativa = 's';
					predecesor(crt->left, cheie, result, poz_relativa, countdown);
				}
				else if (crt->right != nodgol and crt->key < cheie)
				{
					poz_relativa = 'd';
					predecesor(crt->right, cheie, result, poz_relativa, countdown);
				}

				if (countdown == 2 and poz_relativa == 'd')
				{
					countdown = 0;
					result = crt->key;
				}
			}
			else if (crt->left != nodgol)	  //  daca exista subarborele stang, continuam cautarea acolo; 
			{											  //  daca nu, ne intoarcem din functiile recursive
				countdown = 1;
				predecesor(crt->left, cheie, result, poz_relativa, countdown);
			}
		}
		else if (countdown == 1)	// dupa ce am gasit cheia, cautam cel mai drept nod din subarborele stang
		{
			if (crt->right == nodgol)
			{
				countdown = 0;
				result = crt->key;
			}
			else
				predecesor(crt->right, cheie, result, poz_relativa, countdown);
		}
	}

	tmp_kt_pt
		void dezaloca(nod<kt, pt>*& crt)		//  sterge tot subarborele din nodul dat ca parametru, inclusiv pe acesta
	{
		if (crt == nodgol)
			return;

		if (crt->left != nodgol)
			dezaloca(crt->left);
		if (crt->right != nodgol)
			dezaloca(crt->right);

		delete crt;
	}
}

typedef std::stack< std::pair <int, int >> stackpair;
typedef std::vector< std::pair< int, int >> vpair;
typedef std::unordered_set< int > multime;
typedef std::vector< std::vector < int >> vector_vectori;
#define vector_vecini std::vector < vecin<moneda>>
#define vmuchii std::vector< muchie< moneda>>

template <typename moneda>
class graf
{
protected:
	const bool orientat;
	const bool areCosturi;
	const bool areListaMuchii;
	const bool areListeIndecsiMuchii;
	const bool areMatPonderi;
	const bool areCapacitati;
	const bool adaptareCuplaj;   // daca vrem sa construim o retea de transport dintr-un cuplaj

	int nrvf, nrmuchii;
	vector_vecini* vecini;

	muchie<moneda>* lista_muchii;
	int size_lista_muchii;
	bool lista_muchii_sortata;
	std::vector<int>* listeIndecsiMuchii;

	vector_vecini* veciniGrafRezidual;
	int nrvfSt, nrvfDr;

	void initializariGenerale(vector_vecini* vecini_param);
	void initializariMuchii(int x, int y, moneda c = 0, int capacitatea = 0, int &idxListaMuchii = 0);
	void DFS(int nod, bool* viz, int* ordineVizita, int& idxOrdineVizita);
	void biconexe(int nod, int tata, bool* viz, multime* wayback, stackpair& muchiiviz, vector_vectori& comp_biconexe);
	void tareconexe(int nod, int& freeorder, int* order, int* leastbackorder, bool* pestiva, std::stack<int>& noduriviz, vector_vectori& comp_tareconexe);
	void mCrits(int nod, int tata, bool* viz, multime* wayback, stackpair& muchiiviz, vector_vectori& connections, vector_vectori& solutia);
	void DFS_SortareTopologica(int nod, bool* viz, int*& finished, int& idxfinished);
	void bellmanFordPartial(long long int*& costul, std::map<int, moneda>& actualizariCost, bool& areCicluNegativ = false, int nod_sursa = 1);
	int calculeazaCostDrum(int config, int ultimulNod, moneda costDrum[configMax][nrvfMax], moneda costArc[nrvfMax][nrvfMax], int nod_start = 1);
	bool drumuriBFS(int **&capacitati, int **&fluxuri, int *&arboreBFS, int *&tata, bool *&vizitat, const int dim, const int nod_destinatie, const int nod_sursa = 1);

	graf(int nrvf_param);		// doar pt declararea unor subgrafuri
public:
	graf(bool orientat_param = true, bool areCosturi_param = true, bool areListaMuchii_param = true, bool areListeIndecsiMuchii_param = false, bool areMatPonderi_param = false, bool areCapacitati_param = false, bool adaptareCuplaj_param = false, int nrvf_param = -1, int nrmuchii_param = -1, vector_vecini *vecini_param = NULL);

	int get_nrvf() { return nrvf; }
	int get_nrvfSt() { return nrvfSt; }
	int get_nrvfDr() { return nrvfDr; }
	void copiaza_listeAdiacenta(vector_vectori& solution);
	void verifvecini(int nod_start = 1);
	void verifveciniGrafRezidual(int nod_start = 1);

	void BFS(int *&dist, int start = 1);
	int* cadruDFS();
	vector_vectori cadru_biconexe();
	vector_vectori cadru_tareconexe(bool &statusIsError = false);
	vector_vectori criticalConnections(int n, vector_vectori& connections);
	void cadruSortareTopologica(int*& finished, bool &statusIsError = false);

	void kruskal(muchie<moneda>*& muchiiAPM, moneda& cost_apm = 0, bool &statusIsError = false);
	void prim(muchie<moneda>*& muchiiAPM, moneda& cost_apm = 0, bool &statusIsError = false);
	void dijkstra(long long int*& costul, int nod = 1);
	void bellmanFord(long long int*& costul, bool& areCicluNegativ = false, int nod_sursa = 1);
	void TarjanTopologicBellmanFord(long long int*& costul, bool& areCicluNegativ = false, int nod_sursa = 1);

	void royFloyd(long long int**& costuri, bool &statusIsError = false);
	void cicluEulerian(std::vector<int>& ciclul, bool &esteEulerian = true, bool& statusIsError = false, int nod = 1);
	int cicluHamiltonian(bool& esteHamiltonian, bool& statusIsError, int nod_start = 1);
	int edmondsKarp(const int nod_destinatie, const int nod_sursa = 1, bool actualizeazaFluxuri = false);
	vpair muchiileCuFlux();

	vpair conectareGraf(int& extraTotal, int& extraMaxim);
	int distantaSiguraPaftenie(char**& temnita, const int nrLin, const int nrCol, std::vector<int>& idxDragoni, const int idIntrare, const int idIesire);

	~graf()
	{
		delete[]vecini;
		if (lista_muchii)
			delete[] lista_muchii;
		if (listeIndecsiMuchii)
			delete[] listeIndecsiMuchii;
		if (veciniGrafRezidual)
			delete[]veciniGrafRezidual;
	}
};

#pragma region graf & auxiliare
tmp_moneda
void graf<moneda>::initializariGenerale(vector_vecini* vecini_param)
{
	vecini = new vector_vecini[nrvf + 1];

	if (vecini_param)     // daca avem un vector de vecini sursa de unde sa incarcam datele
		for (int i = 0; i <= nrvf; i++)
			for (size_t j = 0; j < vecini_param[i].size(); j++)
				vecini[i].push_back(vecini_param[i][j]);

	if (areListaMuchii)
	{
		size_lista_muchii = nrmuchii;
		lista_muchii = new muchie<moneda>[size_lista_muchii];
	}
	else
	{
		size_lista_muchii = 0;
		lista_muchii = NULL;
	}
	lista_muchii_sortata = false;

	if (areListeIndecsiMuchii)
		listeIndecsiMuchii = new std::vector<int>[nrvf + 1];
	else
		listeIndecsiMuchii = NULL;

	if (areCapacitati)
		veciniGrafRezidual = new vector_vecini[nrvf + 1];
	else
		veciniGrafRezidual = NULL;
}

tmp_moneda
void graf<moneda>::initializariMuchii(int x, int y, moneda c, int capacitatea, int &idxListaMuchii)
{
	vecin<moneda> aux(y, c, capacitatea);
	vecini[x].push_back(aux);
	if (!orientat)
	{
		vecin<moneda> aux(x, c, capacitatea);
		vecini[y].push_back(aux);
	}

	if (veciniGrafRezidual)   //  <=>  areCapacitati
	{
		vecin<moneda> auxInvers(x, c, 0);
		auxInvers.vecinInvers = &(vecini[x].back());                              // referinta catre muchia directa asociata
		veciniGrafRezidual[y].push_back(auxInvers);

		vecini[x].back().vecinInvers = &(veciniGrafRezidual[y].back());  // referinta catre muchia indirecta asociata
	}

	if (areListaMuchii)
	{
		lista_muchii[idxListaMuchii].setAll(x, y, c, capacitatea);
		if (areListeIndecsiMuchii)
		{
			listeIndecsiMuchii[x].push_back(idxListaMuchii);
			if (!orientat)
				listeIndecsiMuchii[y].push_back(idxListaMuchii);
		}
		idxListaMuchii++;
	}
}

tmp_moneda
graf<moneda>::graf(bool orientat_param, bool areCosturi_param, bool areListaMuchii_param, bool areListeIndecsiMuchii_param, bool areMatPonderi_param, bool areCapacitati_param, bool adaptareCuplaj_param, int nrvf_param, int nrmuchii_param, vector_vecini* vecini_param) : orientat(orientat_param), areCosturi(areCosturi_param), areListaMuchii(areListaMuchii_param), areListeIndecsiMuchii(areListeIndecsiMuchii_param), areMatPonderi(areMatPonderi_param), areCapacitati(areCapacitati_param), adaptareCuplaj(adaptareCuplaj_param), nrvf(nrvf_param), nrmuchii(nrmuchii_param)
{
	if (adaptareCuplaj)
	{                                               // setarile necesare pentru acest tip de graf:     orientat = true,   areCapacitati = true,     adaptareCuplaj = true.
		f >> nrvfSt >> nrvfDr >> nrmuchii;
		nrvf = nrvfSt + nrvfDr + 2;   // adaugam un nod sursa (nodul 0) si un nod destinatie (nodul nrvf);

		initializariGenerale(vecini_param);

		int idxListaMuchii = 0;
		for (int i = 0; i < nrmuchii; i++)
		{
			int x, y;
			moneda c = 0;
			int capacitatea = 1;

			f >> x >> y;
			y += nrvfSt;
			if (areCosturi)
				f >> c;

			initializariMuchii(x, y, c, capacitatea, idxListaMuchii);
		}
		for (int i = 1; i <= nrvfSt; i++)
			initializariMuchii(0, i, 0, 1, idxListaMuchii);
		for (int i = nrvfSt + 1; i < nrvf; i++)
			initializariMuchii(i, nrvf, 0, 1, idxListaMuchii);
	}
	else
	{
		nrvfSt = 0;
		nrvfDr = 0;

		if (!areMatPonderi)
		{
			if (nrvf < 0)
				f >> nrvf;
			if (nrmuchii < 0)
				f >> nrmuchii;
			if (start)	// daca start este nenul, inseamna ca avem de citit un nod de start...
				f >> start;

			initializariGenerale(vecini_param);

			int idxListaMuchii = 0;
			for (int i = 0; i < nrmuchii; i++)
			{
				int x, y;
				moneda c = 0;
				int capacitatea = 0;

				f >> x >> y;
				if (areCosturi)
					f >> c;
				if (areCapacitati)
					f >> capacitatea;

				initializariMuchii(x, y, c, capacitatea, idxListaMuchii);
			}
		}
		else
		{
			f >> nrvf;
			nrmuchii = nrvf * nrvf;

			initializariGenerale(vecini_param);

			int idxListaMuchii = 0;
			for (int i = 1; i <= nrvf; i++)
				for (int j = 1; j <= nrvf; j++)
				{
					// momentan, vom considera doar cazul in care existenta matricei ponderilor implica inexistenta notiunilor de flux si capacitate
					moneda cost;
					f >> cost;
					
					initializariMuchii(i, j, cost, 0, idxListaMuchii);
				}
		}
	}
}
tmp_moneda
graf<moneda>::graf(int nrvf_param) : orientat(true), areCosturi(true), areListaMuchii(false), areListeIndecsiMuchii(false), areMatPonderi(false), areCapacitati(false), adaptareCuplaj(false), nrvfSt(0), nrvfDr(0)
{
	nrvf = nrvf_param;
	nrmuchii = 0;		// se va modifica direct in constructorul clasei subgraf
	initializariGenerale();
}

tmp_moneda
void graf<moneda>::verifvecini(int nod_start)
{
	for (int i = nod_start; i <= nrvf; i++)
	{
		std::cout << "\n  vecinii lui " << i << " :  ";
		for (unsigned int j = 0; j < vecini[i].size(); j++)
			std::cout << vecini[i][j];
	}
	std::cout << "\n";
}

tmp_moneda
void graf<moneda>::verifveciniGrafRezidual(int nod_start)
{
	for (int i = nod_start; i <= nrvf; i++)
	{
		std::cout << "\n  vecinii din graful rezidual ai lui " << i << " :  ";
		for (unsigned int j = 0; j < veciniGrafRezidual[i].size(); j++)
			std::cout << veciniGrafRezidual[i][j];
	}
	std::cout << "\n";
}

tmp_moneda
void graf<moneda>::BFS(int *&dist, int start)
{
	if(!dist)                                    // daca parametrul dist este un pointer catre NULL (nu este setat deja pt BFS)
		dist = new int[nrvf + 1]{ 0 };	// initializam distantele cu 0 (le decrementam ulterior)
	std::queue <int> qBFS;				// coada pt BFS
	qBFS.push(start);
	dist[start] = 1;

	while (!qBFS.empty())
	{
		const int nod = qBFS.front();
		for (unsigned int i = 0; i < vecini[nod].size(); i++)
		{
			const int nod_urm = vecini[nod][i].index;
			if (dist[nod_urm] == 0)
			{
				qBFS.push(nod_urm);
				dist[nod_urm] = dist[nod] + 1;
			}
		}

		qBFS.pop();
	}

	for (int i = 0; i <= nrvf; i++)
		dist[i]--;
}

tmp_moneda
void graf<moneda>::DFS(int nod, bool* viz, int* ordineVizita, int& idxOrdineVizita)
{
	viz[nod] = true;
	*(ordineVizita + idxOrdineVizita) = nod;
	for (unsigned int i = 0; i < vecini[nod].size(); i++)
	{
		const int nod_urm = vecini[nod][i].index;
		if (!viz[nod_urm])
		{
			idxOrdineVizita++;
			DFS(nod_urm, viz, ordineVizita, idxOrdineVizita);
		}
	}
}
tmp_moneda
int* graf<moneda>::cadruDFS()
{
	contor = 0;
	bool* viz = new bool[nrvf + 1]{ 0 };
	int* ordineVizita = new int[nrvf + 1]{ 0 };
	int idxOrdineVizita = 0;

	for (int i = 1; i <= nrvf; i++)
		if (!viz[i])
		{
			contor++;
			idxOrdineVizita++;
			DFS(i, viz, ordineVizita, idxOrdineVizita);
		}

	delete[]viz;
	return ordineVizita;
}

tmp_moneda
void graf<moneda>::biconexe(int nod, int tata, bool* viz, multime* wayback, stackpair& muchiiviz, vector_vectori& comp_biconexe)
{
	viz[nod] = true;

	for (unsigned int i = 0; i < vecini[nod].size(); i++)
	{
		const int nod_urm = vecini[nod][i].index;

		if (viz[nod_urm])
		{
			if (nod_urm != tata and nod_urm != nod)
			{
				wayback->insert(nod_urm);
				for (unsigned int i = 0; i < vecini[nod_urm].size(); i++)
					if (vecini[nod_urm][i].index == nod)
					{
						vecini[nod_urm][i].index = nod_urm;
						break;
					}
			}
		}
		else
		{
			muchiiviz.push({ nod, nod_urm });
			multime* wayback_fiu = new multime;
			biconexe(nod_urm, nod, viz, wayback_fiu, muchiiviz, comp_biconexe);

			wayback_fiu->erase(nod);
			if (wayback_fiu->size() == 0)
			{
				contor++;
				std::vector< int > aux;
				comp_biconexe.push_back(aux);
				while (muchiiviz.top().first != nod)
				{
					comp_biconexe.back().push_back(muchiiviz.top().second);
					muchiiviz.pop();
				}
				comp_biconexe.back().push_back(muchiiviz.top().second);
				comp_biconexe.back().push_back(muchiiviz.top().first);
				muchiiviz.pop();
			}
			else
				wayback->insert(wayback_fiu->begin(), wayback_fiu->end());

			delete wayback_fiu;
		}
	}
}
tmp_moneda
vector_vectori graf<moneda>::cadru_biconexe()
{
	contor = 0;
	vector_vectori comp_biconexe;		// solutia, de forma unui vector cu alti vectori ce reprezinta componentele biconexe
	bool* viz = new bool[nrvf + 1]{ 0 };	// nodurile vizitate deja
	stackpair muchiiviz;							// stiva de muchii vizitate
	multime* setgol = new multime;		// un set "wayback" pe care il pasez fiului pentru a-mi returna caile de intoarcere disponibile
	biconexe(1, -1, viz, setgol, muchiiviz, comp_biconexe);

	delete setgol;
	delete[]viz;
	return comp_biconexe;
}

tmp_moneda
void graf<moneda>::tareconexe(int nod, int& freeorder, int* order, int* leastbackorder, bool* pestiva, std::stack<int>& noduriviz, vector_vectori& comp_tareconexe)
{
	order[nod] = freeorder;					// ordinul lui nod este primul ordin disponibil
	leastbackorder[nod] = freeorder;	// cel mai mic ordin al unui nod din urma accesibil este setat ca ordinul nodului crt
	freeorder++;
	noduriviz.push(nod);
	pestiva[nod] = true;

	for (unsigned int i = 0; i < vecini[nod].size(); i++)
	{
		const int nod_urm = vecini[nod][i].index;
		if (order[nod_urm] == 0)			// daca e nevizitat
		{
			tareconexe(nod_urm, freeorder, order, leastbackorder, pestiva, noduriviz, comp_tareconexe);
			leastbackorder[nod] = std::min(leastbackorder[nod], leastbackorder[nod_urm]);
		}
		else if (pestiva[nod_urm])		// vizitat, dar inca pe stiva
			leastbackorder[nod] = std::min(leastbackorder[nod], order[nod_urm]);		// comparam cu order[nod_urm] si nu cu leastbackorder[nod_urm] 
	}																															// deoarece in leastbackorder tinem cont de nodul cel mai din urma
																																// in care ne putem intoarce folosind o muchie de intoarcere din nod, nu un lant intreg
	if (leastbackorder[nod] == order[nod])   //  daca nodul nu se poate intoarce mai sus de el
	{
		contor++;		// am mai gasit o comp conexa
		std::vector< int > aux;
		comp_tareconexe.push_back(aux);

		int varf = noduriviz.top();
		while (varf != nod)
		{
			noduriviz.pop();
			pestiva[varf] = false;
			comp_tareconexe.back().push_back(varf);

			varf = noduriviz.top();
		}
		noduriviz.pop();
		pestiva[varf] = false;
		comp_tareconexe.back().push_back(varf);
	}
}
tmp_moneda
vector_vectori graf<moneda>::cadru_tareconexe(bool &statusIsError)
{
	vector_vectori comp_tareconexe;				// solutia, de forma unui vector cu alti vectori ce reprezinta componentele tareconexe

	if (!orientat)
	{
		std::cout << "\n   Graful dat trebuie sa fie orientat pentru a rezolva aceasta problema";
		g << "\n   Graful dat trebuie sa fie orientat pentru a rezolva aceasta problema";
		return comp_tareconexe;
	}
	std::vector< int > idxZeroRamaneGol;
	comp_tareconexe.push_back(idxZeroRamaneGol);

	contor = 0;													// nr componente conexe
	std::stack<int> noduriviz;								// stiva de noduri vizitate
	bool* pestiva = new bool[nrvf + 1]{ 0 };
	int* order = new int[nrvf + 1]{ 0 };				// ordinea intrarii pe stiva a nodurilor
	int freeorder = 1;											// nr de ordine disponibil; se va incrementa pe masura ce un nod primeste nr de ordine (in array-ul order)
	int* leastbackorder = new int[nrvf + 1]{ 0 };		// cel mai mic ordin accesibil din urma, de pe stiva

	for (int i = 1; i <= nrvf; i++)
		if (order[i] == 0)		//  nod nevizitat 
			tareconexe(i, freeorder, order, leastbackorder, pestiva, noduriviz, comp_tareconexe);

	delete[] leastbackorder;
	delete[] order;
	delete[] pestiva;

	return comp_tareconexe;
}

tmp_moneda
void graf<moneda>::copiaza_listeAdiacenta(vector_vectori& solution)
{
	// Obs: ^ asta face doar o copie a listelor de adiacenta din graf
	// e un design incurcat pt a adapta antetul functiei cerute pe leetcode, dar eu am preferat sa folosesc connections pe post de g.vecini ca sa reutilizez algoritmul de la ex precedent

	std::vector< int > aux;
	solution.push_back(aux);
	for (int i = 1; i <= nrvf; i++)
	{
		std::vector< int > aux;
		solution.push_back(aux);
		for (unsigned int j = 0; j < vecini[i].size(); j++)
			solution.back().push_back(vecini[i][j].index);
	}
}
tmp_moneda
void graf<moneda>::mCrits(int nod, int tata, bool* viz, multime* wayback, stackpair& muchiiviz, vector_vectori& connections, vector_vectori& solutia)
{
	viz[nod] = true;
	for (unsigned int i = 0; i < connections[nod].size(); i++)
	{
		const int nod_urm = connections[nod][i];
		if (viz[nod_urm])
		{
			if (nod_urm != tata and nod_urm != nod)
			{
				wayback->insert(nod_urm);
				for (unsigned int i = 0; i < connections[nod_urm].size(); i++)
					if (connections[nod_urm][i] == nod)
					{
						connections[nod_urm][i] = nod_urm;
						break;
					}
			}
		}
		else
		{
			muchiiviz.push({ nod, nod_urm });
			multime* wayback_fiu = new multime;
			mCrits(nod_urm, nod, viz, wayback_fiu, muchiiviz, connections, solutia);

			if (wayback_fiu->size() == 0)
			{
				std::vector< int > aux;
				solutia.push_back(aux);
				while (muchiiviz.top().first != nod)
					muchiiviz.pop();

				solutia.back().push_back(muchiiviz.top().first);
				solutia.back().push_back(muchiiviz.top().second);
				muchiiviz.pop();
			}
			else
			{
				wayback_fiu->erase(nod);
				wayback->insert(wayback_fiu->begin(), wayback_fiu->end());
			}

			delete wayback_fiu;
		}
	}
}
tmp_moneda
std::vector< std::vector< int > > graf<moneda>::criticalConnections(int n, vector_vectori& connections)
{
	vector_vectori solutia;		// solutia, de forma unui vector cu alti vectori ce reprezinta muchiile critice
	bool* viz = new bool[nrvf + 1]{ 0 };	// nodurile vizitate deja
	stackpair muchiiviz;							// stiva de muchii vizitate
	multime* setgol = new multime;		// un set "wayback" pe care il pasez fiului pentru a-mi returna caile de intoarcere disponibile
	mCrits(1, -1, viz, setgol, muchiiviz, connections, solutia);

	delete setgol;
	delete[]viz;
	return solutia;
}

tmp_moneda
void graf<moneda>::DFS_SortareTopologica(int nod, bool* viz, int*& finished, int& idxfinished)
{
	viz[nod] = true;
	for (unsigned int i = 0; i < vecini[nod].size(); i++)
	{
		const int nod_urm = vecini[nod][i].index;
		if (!viz[nod_urm])
			DFS_SortareTopologica(nod_urm, viz, finished, idxfinished);
	}
	finished[idxfinished] = nod;
	idxfinished++;
}
tmp_moneda
void graf<moneda>::cadruSortareTopologica(int*& finished, bool &statusIsError)
{
	if (!orientat)
	{
		std::cout << "\n   Graful dat trebuie sa fie orientat pentru a rezolva aceasta problema";
		g << "\n   Graful dat trebuie sa fie orientat pentru a rezolva aceasta problema";
		statusIsError = true;
		return;
	}

	finished = new int[nrvf + 1]{ 0 };
	bool* viz = new bool[nrvf + 1]{ 0 };
	int idxfinished = 1;
	for (int i = 1; i <= nrvf; i++)
		if (!viz[i])
			DFS_SortareTopologica(i, viz, finished, idxfinished);

	delete[]viz;
}

bool havelHakimi()
{
	int nrnod, sumgrade = 0;
	f >> nrnod;
	std::map< int, int, std::greater< int > > grad;		//   grad  :  nr_noduri_cu_acel_grad  | sortat descresc

	grad.insert(std::pair< int, int >(0, 0));
	for (int i = 1; i <= nrnod; i++)
	{
		int gradaux;		// gradul nodului curent
		f >> gradaux;
		if (gradaux >= nrnod)
		{
			std::cout << "\n grad prea mare introdus! ";
			return false;
		}
		sumgrade += gradaux;

		if (grad.find(gradaux) == grad.end())		// daca nu avem inca o clasa de noduri pentru gradul curent
		{
			grad.insert(std::pair< int, int >(gradaux, 1));		// facem una, ce retine contorul pt un nod
			std::cout << "\n clasa " << gradaux << " a fost creata si contine " << grad[gradaux] << " nod(uri)";
		}
		else
		{
			grad[gradaux]++;		// else, crestem contorul
			std::cout << "\n clasa " << gradaux << " contine acum " << grad[gradaux] << " nod(uri)";
		}
	}
	if (sumgrade % 2 != 0)
	{
		std::cout << "\n suma gradelor este impara! ";
		return false;
	}

	while (grad[0] < nrnod)
	{
		int maxgr = grad.begin()->first;	// cel mai mare grad existent
		if (maxgr == 0)
			break;
		std::cout << "\n\n | incepere iteratie pentru legare nod din clasa " << maxgr;
		std::map< int, int >::iterator lastgr = grad.begin();
		// ultimul grad care va avea noduri pe care le legam acum de un nod din maxgr
		int contor = maxgr;			// nr noduri ce trb legate de acest nod in cauza
		grad[0] ++;						// rezolvam un nod din maxgr
		grad[maxgr]--;

		bool intrat_in_grad0 = false;
		while (contor and lastgr != grad.end())
		{
			if (lastgr->first == 0)
				intrat_in_grad0 = true;

			if (contor - lastgr->second >= 0)
			{
				contor -= lastgr->second;		// voi lega nodul de nodurile din lastgr
				lastgr++;
			}
			else
				break;
		}
		if (intrat_in_grad0)
		{
			std::cout << "\n nu am avut suficiente noduri ramase pt a lega nodul curent";
			return false;
		}
		std::cout << "\n clasa cea mai mica la care ne-am oprit: " << lastgr->first;

		if (contor)  // daca avem o clasa din care trb sa "mutam" doar o parte din valori
		{
			// pornim din dreapta si "mutam" valorile intr-o clasa mai mica
			if (grad.find(lastgr->first - 1) == grad.end())
				grad.insert(std::pair< int, int >(lastgr->first - 1, contor));
			else
				grad[lastgr->first - 1] += contor;
			// in contor au ramas o parte din nodurile ce trb "mutate" din lastgr
			lastgr->second -= contor;

			std::cout << "\n am legat nodul de " << contor << " nod( uri ) din clasa " << lastgr->first;
		}

		int auxleft = 0, auxright = 0;
		for (auto it = grad.begin(); it != lastgr; it++)
		{
			auxright = it->second;		// salvam nr noduri din clasa curenta
			it->second = auxleft;		// luam nr de noduri din stanga
			auxleft = auxright;			// val din auxright se salveaza in auxleft pt pasul urm
			std::cout << "\n am legat nodul de " << auxleft << " nod(uri) din clasa " << it->first;

			if (grad.find(it->first - 1) == grad.end())
			{
				// daca nu avem clasa coresp etichetei curente minus 1, o cream si ii dam direct auxleft
				grad.insert(std::pair< int, int >(it->first - 1, auxleft));
				auxleft = 0;
				it++;		 // sarim clasa noua
			}
		}
		lastgr->second += auxleft;	// am scos deja din lastgr, acum ii si aducem (eventual) cv din stanga

		while (grad[maxgr] == 0)		// daca nu mai avem noduri in maxgr dupa ce rezolvam unul
		{
			std::cout << "\n clasa " << maxgr << " a fost epuizata";
			grad.erase(maxgr);			// nu ne va mai trebui clasa cu eticheta "maxgr"
			maxgr = grad.begin()->first;
		}
	}
	return true;
}

template <typename moneda = int>
int tataMare(int nod, int tata[])
{
	// implementam un algoritm "tataMare()" care imi trece recursiv prin tati si la intoarcere imi seteaza peste tot cel mai batran nod ca fiind tata direct

	if (tata[nod] == nodgol_int)
		return nod;                                                      // l-am gasit pe tataMare
	tata[nod] = tataMare<moneda>(tata[nod], tata);  // toate nodurile de pe drum il vor primi ca tata pe tataMare
	return tata[nod];                                                 // dau inapoi in recursie informatia pe care am primit-o eu despre cine e tataMare in arborele acesta
}

void kruskal_paduri()
{
	int nrvf, nrcmds;
	f >> nrvf >> nrcmds;

	int* tata = new int[nrvf + 1]{ 0 };					// 0 va fi echivalent cu self, deoarece nu exista nodul 0
	int* h_subarbore = new int[nrvf + 1]{ 0 };		// ne va interesa in general doar inaltimea pt nodurile returnate de tataMare
	for (int i = 0; i < nrcmds; i++)
	{
		int cmd, x, y;
		f >> cmd >> x >> y;

		if (cmd == 1)
		{
			const int tataMare1 = tataMare(x, tata);
			const int tataMare2 = tataMare(y, tata);

			if (tataMare1 != tataMare2)
			{
				const int h1 = h_subarbore[tataMare1];
				const int h2 = h_subarbore[tataMare2];
				if (h1 == h2)
				{
					tata[tataMare2] = tataMare1;
					h_subarbore[tataMare1]++;
				}
				else if (h1 > h2)
					tata[tataMare2] = tataMare1;
				else
					tata[tataMare1] = tataMare2;
			}
		}
		else
		{
			const int tataMare1 = tataMare(x, tata);
			const int tataMare2 = tataMare(y, tata);

			if (tataMare1 != tataMare2)
				g << "NU\n";
			else
				g << "DA\n";
		}
	}
	delete[] h_subarbore;
	delete[] tata;
}

tmp_moneda
void graf<moneda>::kruskal(muchie<moneda>*& muchiiAPM, moneda& cost_apm, bool &statusIsError)
{
	if (!areCosturi)
	{
		std::cout << "\n   Muchiile din graful dat trebuie sa  pentru a rezolva aceasta problema";
		g << "\n   Muchiile din graful dat trebuie sa  pentru a rezolva aceasta problema";
		statusIsError = true;
		return;
	}

	if (!lista_muchii_sortata)
	{
		std::sort(lista_muchii, lista_muchii + size_lista_muchii);
		lista_muchii_sortata = true;
	}
	const int nrMuchiiAPM = nrvf - 1;
	muchiiAPM = new muchie<moneda>[nrMuchiiAPM + 1];
	contor = 0;												// pt muchiile aflate curent in APM

	//construim un vector de tati in care, pt eficienta, tatal fiecarui nod va fi initializat cu zero.
	int* tata = new int[nrvf + 1]{ 0 };
	int* h_subarbore = new int[nrvf + 1]{ 0 }; // ne va interesa in general doar inaltimea pt nodurile returnate de tataMare

	for (int i = 0; i < size_lista_muchii; i++)
	{
		const int tataMare1 = tataMare(lista_muchii[i].vf1, tata);
		const int tataMare2 = tataMare(lista_muchii[i].vf2, tata);
		if (tataMare1 != tataMare2)
		{
			// avem o muchie buna
			contor++;
			muchiiAPM[contor] = lista_muchii[i];
			cost_apm += muchiiAPM[contor].cost;

			if (contor == nrMuchiiAPM)		// am gasit toate muchiile necesare
				break;

			const int h1 = h_subarbore[tataMare1];
			const int h2 = h_subarbore[tataMare2];
			if (h1 == h2)
			{
				tata[tataMare2] = tataMare1;
				h_subarbore[tataMare1]++;
			}
			else if (h1 > h2)
				tata[tataMare2] = tataMare1;
			else
				tata[tataMare1] = tataMare2;
		}
	}
	delete[]tata;
	delete[]h_subarbore;
}

tmp_moneda
void graf<moneda>::prim(muchie<moneda>*& muchiiAPM, moneda& cost_apm, bool &statusIsError)
{
	if (!areCosturi)
	{
		std::cout << "\n   Muchiile din graful dat trebuie sa  pentru a rezolva aceasta problema";
		g << "\n   Muchiile din graful dat trebuie sa  pentru a rezolva aceasta problema";
		statusIsError = true;
		return;
	}

	typedef muchie< moneda > muchie_m;

	const int nrMuchiiAPM = nrvf - 1;
	muchiiAPM = new muchie_m[nrMuchiiAPM + 1];
	cost_apm = 0;
	contor = 0;																// pt muchiile aflate curent in APM

	if (!orientat)
	{
		std::priority_queue< muchie_m, vmuchii, std::greater< muchie_m > > muchii_de_verif;		// folosim pq pt a accesa minimul in O(1)

		bool* noduri_selectate = new bool[nrvf + 1]{ 0 };		// aici vrem doar verificare de true in O(1)
		int nr_noduri_selectate = 1;
		int nod = 1;
		noduri_selectate[nod] = true;

		while (contor < nrMuchiiAPM)
		{
			for (unsigned int i = 0; i < vecini[nod].size(); i++)
			{
				muchie_m aux(nod, vecini[nod][i].index, vecini[nod][i].cost);
				if (!noduri_selectate[aux.vf2])		// daca aux.vf2 nu este printre nodurile selectate deja
					muchii_de_verif.push(aux);				// luam pt verificare muchia adiacenta lui nod
			}

			while (!muchii_de_verif.empty() and noduri_selectate[muchii_de_verif.top().vf2])		// cat timp muchia de cost minim disponibila duce la un nod deja selectat
				muchii_de_verif.pop();																									// scoatem muchia din set

			if (muchii_de_verif.empty())
			{
				std::cout << "\n Graful nu este conex!";
				statusIsError = true;
				break;
			}

			const muchie_m muchia = muchii_de_verif.top();					// muchia pe care o vom selecta
			muchii_de_verif.pop();															// scoatem muchia din verificare
			contor++;																				// crestem nr muchiilor din APM
			muchiiAPM[contor] = muchia;
			cost_apm += muchia.cost;

			nod = muchia.vf2;																	// nod = primul nod neselectat
			noduri_selectate[nod] = true;												// selectam si noul nod
			nr_noduri_selectate++;														// si il numaram la noduri selectate
		}
		delete[]noduri_selectate;
	}
	else
	{
		int nod = 1;
		long long int* costul = new long long int[nrvf + 1];
		for (int i = 1; i <= nrvf; i++)
			costul[i] = LLONG_MAX - 1 - (rand() % RAND_MAX);
		costul[nod] = 0;
		int* tata = new int[nrvf + 1]{ 0 };

		using treap::R;
		using treap::nodgol;
		R = nodgol = new treap::nod<int, long long int >(0, LLONG_MAX, NULL, NULL);
		for (int cheie = 1; cheie <= nrvf; cheie++)
			treap::inserare(R, cheie, costul[cheie], true);

		bool* vizitat = new bool[nrvf + 1]{ 0 };
		vizitat[nod] = true;
		while (R != nodgol)
		{
			nod = R->key;									// nodul cu costul actual minim
			if (!vizitat[nod])
				break;
			treap::stergere(R, R->key, true);		// stergem din treap acest nod selectat
			for (unsigned int i = 0; i < vecini[nod].size(); i++)
			{
				const vecin<moneda> vecinul = vecini[nod][i];
				const long long int cost_pretendent = vecinul.cost;
				if (cost_pretendent < costul[vecinul.index])										// am gasit o muchie de cost mai mic catre vecinul.index
				{
					vizitat[vecinul.index] = true;
					costul[vecinul.index] = cost_pretendent;
					tata[vecinul.index] = nod;
					treap::stergere(R, vecinul.index, true);										// stergem varianta veche a nodului din treap
					treap::inserare(R, vecinul.index, costul[vecinul.index], true);		// si il inseram iarasi, updatat cu noul cost
				}
			}
		}

		for (int i = 1; i <= nrvf; i++)
		{
			if (!vizitat[i])
			{
				std::cout << "\n Graful nu este conex!";
				statusIsError = true;
				treap::dezaloca(R);		// eliberam memoria
				delete nodgol;
				delete[] vizitat;
				delete[] tata;
				delete[]costul;
				return;
			}
			if (tata[i])
			{
				muchie<moneda> aux(tata[i], i, costul[i]);											// muchie<moneda> aux ( nodul din care il accesez pe i in APM,  i  , costul muchiei catre i  );
				contor++;
				muchiiAPM[contor] = aux;
				cost_apm += costul[i];
			}
		}

		treap::dezaloca(R);		// eliberam memoria
		delete nodgol;
		delete[] vizitat;
		delete[] tata;
		delete[]costul;
	}
}

tmp_moneda
void graf<moneda>::dijkstra(long long int*& costul, int nod)
{
	if (!areCosturi)
	{
		std::cout << "\n   Muchiile din graful dat trebuie sa  pentru a rezolva aceasta problema";
		g << "\n   Muchiile din graful dat trebuie sa  pentru a rezolva aceasta problema";
		return;
	}

	typedef muchie< moneda > muchie_m;

	bool* vizitat = new bool[nrvf + 1]{ 0 };

	if (!orientat)
	{
		costul = new long long int[nrvf + 1]{ 0 };																			// aici vrem inserare si accesare in O(1) si cam atat
		std::priority_queue< muchie_m, vmuchii, std::greater< muchie_m > > muchii_de_verif;	// folosim pq pt a accesa minimul in O(1)
		bool* noduri_selectate = new bool[nrvf + 1]{ 0 };																// aici vrem doar verificare de true in O(1)
		noduri_selectate[nod] = true;
		contor = 1;												// avem un nod selectat

		while (contor < nrvf)
		{
			for (unsigned int i = 0; i < vecini[nod].size(); i++)
			{
				muchie_m aux(nod, vecini[nod][i].index, vecini[nod][i].cost);
				if (!noduri_selectate[aux.vf2])			// daca aux.vf2 nu este printre nodurile selectate deja
					muchii_de_verif.push(aux);			// luam pt verificare muchia adiacenta lui nod
			}

			while (!muchii_de_verif.empty() and noduri_selectate[muchii_de_verif.top().vf2])		// cat timp muchia de cost minim disponibila duce la un nod deja selectat
				muchii_de_verif.pop();																								// scoatem muchia din set

			if (muchii_de_verif.empty())
			{
				std::cout << "\n Graful nu este conex!";
				break;
			}

			const muchie_m muchia = muchii_de_verif.top();						// muchia pe care o vom selecta
			costul[muchia.vf2] = costul[muchia.vf1] + muchia.cost;				// salvam muchia in solutie
			nod = muchia.vf2;																		// nod = primul nod neselectat
			muchii_de_verif.pop();																// scoatem muchia din verificare
			noduri_selectate[nod] = true;													// selectam si noul nod
			contor++;																					// si il numaram la noduri selectate
		}
		delete[] noduri_selectate;
	}
	else
	{
		costul = new long long int[nrvf + 1];
		for (int i = 1; i <= nrvf; i++)
			costul[i] = LLONG_MAX - 1 - (rand() % RAND_MAX);
		costul[nod] = 0;
		//int* tata = new int[nrvf + 1]{ 0 };

		using treap::R;
		using treap::nodgol;
		R = nodgol = new treap::nod<int, long long int >(0, LLONG_MAX, NULL, NULL);
		for (int cheie = 1; cheie <= nrvf; cheie++)
			treap::inserare(R, cheie, costul[cheie], true);

		vizitat[nod] = true;
		while (R != nodgol)
		{
			nod = R->key;									// nodul cu costul actual minim
			if (!vizitat[nod])
				break;
			treap::stergere(R, R->key, true);		// stergem din treap acest nod selectat
			for (unsigned int i = 0; i < vecini[nod].size(); i++)
			{
				const vecin<moneda> vecinul = vecini[nod][i];
				const long long int cost_pretendent = costul[nod] + vecinul.cost;
				if (cost_pretendent < costul[vecinul.index])
				{
					vizitat[vecinul.index] = true;
					costul[vecinul.index] = cost_pretendent;
					//tata[vecinul.index] = nod;
					treap::stergere(R, vecinul.index, true);										// stergem varianta veche a nodului din treap
					treap::inserare(R, vecinul.index, costul[vecinul.index], true);		// si il inseram iarasi, updatat cu noul cost
				}
			}
		}

		treap::dezaloca(R);		// eliberam memoria
		delete nodgol;
		//delete[] tata;
	}

	for (int i = 1; i <= nrvf; i++)
		if (!vizitat[i])
			costul[i] = 0;

	delete[] vizitat;
}

tmp_moneda
void graf<moneda>::bellmanFord(long long int*& costul, bool& areCicluNegativ, int nod_sursa)
{
	const long long int maximul = 922337203685477580;
	costul = new long long int[nrvf + 1];
	for (int i = 1; i <= nrvf; i++)
		costul[i] = maximul;
	costul[nod_sursa] = 0;

	std::queue<int> qnodes;
	qnodes.push(nod_sursa);
	bool* inCoada = new bool[nrvf + 1]{ 0 };
	inCoada[nod_sursa] = true;
	int* nrRelaxari = new int[nrvf + 1]{ 0 };

	while (!qnodes.empty())
	{
		const int nod = qnodes.front();
		qnodes.pop();
		inCoada[nod] = false;
		for (unsigned int j = 0; j < vecini[nod].size(); j++)
		{
			const int vecinul = vecini[nod][j].index;
			const moneda cost_arc = vecini[nod][j].cost;

			if (costul[nod] < maximul and costul[vecinul] > costul[nod] + cost_arc)
			{
				nrRelaxari[vecinul]++;
				if (vecinul == nod_sursa or nrRelaxari[vecinul] >= nrvf)
				{
					g << "Ciclu negativ!";
					delete[]nrRelaxari;
					delete[] inCoada;
					delete[] costul;
					return;
				}
				if (!inCoada[vecinul])
				{
					qnodes.push(vecinul);
					inCoada[vecinul] = true;
				}
				costul[vecinul] = costul[nod] + cost_arc;
			}
		}
	}

	delete[]nrRelaxari;
	delete[] inCoada;
}

tmp_moneda
void graf<moneda>::bellmanFordPartial(long long int*& costul, std::map<int, moneda>& actualizariCost, bool& areCicluNegativ, int nod_sursa)
{
	const long long int maximul = 922337203685477580;
	costul = new long long int[nrvf + 1];
	for (int i = 1; i <= nrvf; i++)
		costul[i] = maximul;
	costul[nod_sursa] = 0;

	std::queue<int> qnodes;
	qnodes.push(nod_sursa);
	bool* inCoada = new bool[nrvf + 1]{ 0 };
	inCoada[nod_sursa] = true;
	int* nrRelaxari = new int[nrvf + 1]{ 0 };

	while (!qnodes.empty())
	{
		const int nod = qnodes.front();
		qnodes.pop();
		inCoada[nod] = false;
		for (unsigned int j = 0; j < vecini[nod].size(); j++)
		{
			const int vecinul = vecini[nod][j].index;
			const moneda cost_arc = vecini[nod][j].cost;

			if (costul[nod] < maximul and costul[vecinul] > costul[nod] + cost_arc)
			{
				nrRelaxari[vecinul]++;
				if (vecinul == nod_sursa or nrRelaxari[vecinul] >= nrvf)
				{
					areCicluNegativ = true;
					g << "Ciclu negativ!";
					std::cout << "Ciclu negativ!\n";
					delete[]nrRelaxari;
					delete[] inCoada;
					delete[] costul;
					return;
				}
				if (!inCoada[vecinul])
				{
					qnodes.push(vecinul);
					inCoada[vecinul] = true;
				}
				costul[vecinul] = costul[nod] + cost_arc;

				if (actualizariCost.find(vecinul) != actualizariCost.end())   // daca am inregistrat deja o actualizare, acum o modificam doar
					actualizariCost[vecinul] = costul[vecinul];
				else
					actualizariCost.insert(std::pair< int, moneda >(vecinul, costul[vecinul]));
			}
		}
	}

	delete[]nrRelaxari;
	delete[] inCoada;
}

tmp_moneda
void graf<moneda>::TarjanTopologicBellmanFord(long long int*& costul, bool& areCicluNegativ, int nod_sursa)
{
	if (!areCosturi or !orientat)
	{
		std::cout << "\n   Muchiile din graful dat trebuie sa aiba cost si graful trebuie sa fie orientat pentru a rezolva aceasta problema astfel!";
		g << "\n   Muchiile din graful dat trebuie sa aiba cost si graful trebuie sa fie orientat pentru a rezolva aceasta problema astfel!";
		return;
	}

	typedef muchie< moneda > muchie_m;
	// pt grafuri orientate se numesc arce, dar vom folosi tot termenul "muchii" pt consistenta

	bool statusIsError = false;
	vector_vectori vCTC = cadru_tareconexe(statusIsError);	     // vector de vectori ce reprezinta nodurile componentelor tare conexe (CTC)
	int* careCTC = new int[nrvf + 1];                       // array ce ne spune despre fiecare nod in care CTC este situat
	const size_t nrCTC = vCTC.size() - 1;				 // CTC cu indexul 0 nu se ia in considerare, este goala

	for (size_t i = 1; i <= nrCTC; i++)                       // i reprezinta CTC
		for (size_t j = 0; j < vCTC[i].size(); j++)        // al j-lea nod din CTC curenta
			careCTC[vCTC[i][j]] = i;                          // o sa indexam CTC-urile tot de la 1, cum avem mai toate grafurile indexate in program							

	// supergraful va avea CTC drept noduri si "muchii virtuale" drept muchii (provin din muchiile nodurilor din CTC diferite)
	vmuchii muchiiSupergraf;                                         // muchiile virtuale din supergraf
	vmuchii* muchiiInterCTC = new vmuchii[nrCTC + 1]; // muchiile reale din graful initial, ale nodurilor din CTC diferite
	vmuchii* muchiiCTC = new vmuchii[nrCTC + 1];          // muchiile dintre nodurile aflate in aceeasi CTC

	for (size_t i = 1; i <= nrCTC; i++)                                // i va fi index pentru CTC
	{
		for (size_t j = 0; j < vCTC[i].size(); j++)
		{
			const int nod = vCTC[i][j];                                            // al j-lea nod din CTC i 
			for (size_t k = 0; k < vecini[nod].size(); k++)                 // vecinii acestui nod
			{
				const vecin<moneda> vecinul = vecini[nod][k];
				const int CTCvecinului = careCTC[vecinul.index];      // CTC a acestui vecin
				if (CTCvecinului == i)	                                                 // daca nod si vecinul sunt in aceeasi CTC
				{
					muchie_m aux(nod, vecinul.index, vecinul.cost);
					(muchiiCTC[i]).push_back(aux);                             // adaugam muchia la muchiile CTC i
				}
				else                                                                           // daca nod si vecinul sunt din CTC diferite
				{
					muchie_m aux(i, CTCvecinului, vecinul.cost);
					muchiiSupergraf.push_back(aux);                         // adaugam muchia la muchiile dintre CTC-urile lui nod si vecinul

					muchie_m punteInterCTC(nod, vecinul.index, vecinul.cost);
					muchiiInterCTC[i].push_back(punteInterCTC);     // retinem vecinii din alte CTC-uri pentru fiecare nod al grafului initial
				}
			}
		}
	}
	// putem pregati de acum array-ul de costuri
	const long long int maximul = 922337203685477580;
	costul = new long long int[nrvf + 1];
	for (int i = 1; i <= nrvf; i++)
		costul[i] = maximul;
	costul[nod_sursa] = 0;

	//for (size_t i = 1; i <= nrCTC; i++)
	//{
	//	std::cout << "CTC " << i << ": ";
	//	for (size_t j = 0; j < vCTC[i].size(); j++)
	//		std::cout << vCTC[i][j] << "  ";
	//	std::cout << "\n";
	//}
	//std::cout << "\n--------1--------\n";
	//for (int i = 1; i <= nrCTC; i++)
	//{
	//	std::cout << "CTC " << i << ": ";
	//	for (int j = 0; j < muchiiCTC[i].size(); j++)
	//		std::cout << muchiiCTC[i][j] << " | ";
	//	std::cout << " }\n";
	//}
	//std::cout << "\n--------2--------\n";
	//std::cout << "supergraf: ";
	//for (int j = 0; j < muchiiSupergraf.size(); j++)
	//	std::cout << muchiiSupergraf[j] << " | ";
	//std::cout << " }\n";
	//std::cout << "\n--------3--------\n";
	// vom construi un supergraf (mai exact multigraf) cu CTC pe post de noduri 
	// si CTC-urile ce contin doar muchiile din acelasi CTC (vor memora toate nodurile pt a pastra indexarea, dar cele fara muchii nu conteaza)
	subgraf<moneda> supergraf(nrCTC + 1, muchiiSupergraf);

	int* vOrdineTopologica = new int[nrCTC + 1]{ 0 };          // intai sortam topologic supergraful
	supergraf.cadruSortareTopologica(vOrdineTopologica, statusIsError);

	bool vizitatCTC_nod_sursa = false;
	int nod_sursa_local;
	for (size_t i = nrCTC; i >= 1; i--)                    // parcurgem vectorul sortat topologic invers pt a incepe cu CTC-urile fara restrictii topologice
	{
		nod_sursa_local = 0;
		const int idxCTC = vOrdineTopologica[i];
		if (idxCTC == careCTC[nod_sursa])
		{
			vizitatCTC_nod_sursa = true;
			nod_sursa_local = nod_sursa;                // pana nu se seteaza acest bool, nu vom prelucra CTC-urile in cauza (inaccesibile din nod_sursa)
		}
		if (vizitatCTC_nod_sursa)
		{
			subgraf<moneda> CTC_crt(nrvf + 1, muchiiCTC[idxCTC]);

			long long int* costul_CTC_crt = NULL;  // folosim alt array de cost pentru a nu se suprascrie mereu cu LLONG_MAX
			std::map<int, moneda> actualizariCost;
			CTC_crt.bellmanFordPartial(costul_CTC_crt, actualizariCost, areCicluNegativ, nod_sursa_local);   // folosim acelasi bool
			if (costul_CTC_crt)
			{
				if (!areCicluNegativ)
				{
					for (auto it = actualizariCost.begin(); it != actualizariCost.end(); it++)
					{
						const int nod_actualizat = it->first;
						const moneda cost_actualizat = it->second;
						if (costul[nod_actualizat] > cost_actualizat)
							costul[nod_actualizat] = cost_actualizat;
					}

					const int nrPunti = muchiiInterCTC[idxCTC].size();
					for (int j = 0; j < nrPunti; j++)
					{
						const muchie<moneda> punte(muchiiInterCTC[idxCTC][j]);    // puntea este o muchie a grafului initial dintre doua vf aflate in CTC diferite
						const int idxCTC_vecin = careCTC[punte.vf2];                       // indexul CTC invecinat
						const long long int costNou = costul[punte.vf1] + punte.cost;
						muchie<moneda> muchieNoua(0, punte.vf2, costNou);             // pregatim o noua sursa in 0 pentru CTC invecinat si ii adaugam o muchie spre punte.vf2 cu costul = costul catre nodul din CTC anterior + punte.cost 
						(muchiiCTC[idxCTC_vecin]).push_back(muchieNoua);			   // marcam noua muchie in muchiiCTC
					}
					// acum suntem pregatiti sa prelucram urmatoarea CTC data de sortarea topologica

					delete[]costul_CTC_crt;
				}

				if (areCicluNegativ)
				{
					delete[] vOrdineTopologica;
					delete[] muchiiCTC;
					delete[] muchiiInterCTC;
					delete[] careCTC;
					return;
				}
			}
		}
	}

	delete[] vOrdineTopologica;
	delete[] muchiiCTC;
	delete[] muchiiInterCTC;
	delete[] careCTC;
}

tmp_moneda
void setToMax(moneda& inf)
{
	inf = std::numeric_limits<moneda>::max();

	int aux;
	long int aux1;
	short int aux2;

	unsigned int aux3;
	unsigned short int aux4;
	unsigned long int aux5;

	if (typeid(inf) == typeid(aux) or typeid(inf) == typeid(aux1) or typeid(inf) == typeid(aux2))
	{
		static_cast<long long int>(inf);
		inf = LLONG_MAX;
	}
	else if (typeid(inf) == typeid(aux3) or typeid(inf) == typeid(aux4) or typeid(inf) == typeid(aux5))
	{
		static_cast<unsigned long long int>(inf);
		inf = ULLONG_MAX;
	}
	std::cout << "\n inf a fost setat la valoarea de " << inf;
}
tmp_moneda
void graf<moneda>::royFloyd(long long int**& costuri, bool &statusIsError)
{
	if (!orientat or !areCosturi)
	{
		std::cout << "\n   Muchiile din graful dat trebuie sa aiba cost si graful trebuie sa fie orientat pentru a rezolva aceasta problema astfel!";
		g << "\n   Muchiile din graful dat trebuie sa aiba cost si graful trebuie sa fie orientat pentru a rezolva aceasta problema astfel!";
		statusIsError = true;
		return;
	}

	costuri = new long long int* [nrvf + 1];

	for (int i = 1; i <= nrvf; i++)
	{
		costuri[i] = new long long int[nrvf + 1];
		for (size_t j = 0; j < vecini[i].size(); j++)
			costuri[i][j + 1] = vecini[i][j].cost;
		costuri[i][i] = 0;
	}

	for (int k = 1; k <= nrvf; k++)
		for (int i = 1; i <= nrvf; i++)
			for (int j = 1; j <= nrvf; j++)
				if (i != j and ((costuri[i][j] == 0 or costuri[i][j] > costuri[i][k] + costuri[k][j]) and costuri[i][k] != 0 and costuri[k][j] != 0))
				{
					costuri[i][j] = costuri[i][k] + costuri[k][j];
					//std::cout << "costuri["<<i<<"]["<<j<<"] a fost actualizat la " << costuri[i][j] << "\n";
				}
}

tmp_moneda
void graf<moneda>::cicluEulerian(std::vector<int>& ciclul, bool& esteEulerian, bool& statusIsError, int nod)
{
	if (!listeIndecsiMuchii)
	{
		std::cout << "Graful trebuie sa aiba un multiset de muchii pentru a apela aceasta metoda!";
		g << "Graful trebuie sa aiba un multiset de muchii pentru a apela aceasta metoda!";
		statusIsError = true;
		return;
	}

	bool* muchieParcursa = new bool[nrmuchii] {0};  // ca sa luam muchiile o singura data
	std::vector<int> stivaNoduri;  
	stivaNoduri.push_back(nod);                         
	int nrMuchiiParcurse = 0;

	for (int i = 1; i <= nrvf; i++)
		if (listeIndecsiMuchii[i].size() % 2 != 0)         // daca un nod are gradul impar, graful nu poate avea un ciclu Eulerian
		{
			esteEulerian = false;
			return;
		}

	while(!stivaNoduri.empty())
	{
		nod = stivaNoduri.back();
		if (listeIndecsiMuchii[nod].size())                  // daca mai avem muchii de verificat din nodul curent
		{
			const int idxMuchie = listeIndecsiMuchii[nod].back();
			listeIndecsiMuchii[nod].pop_back();
			if (!muchieParcursa[idxMuchie])                // daca muchia curenta nu este selectata 
			{
				int vecin = lista_muchii[idxMuchie].vf2;
				if (vecin == nod)                                     // in cazul in care nodurile incidente acestei muchii sunt distincte si vf2 coincide cu nodul curent, vecin va fi vf1 (e ok si daca nodurile incidente coincid, merge tot pe cazul 2)
					vecin = lista_muchii[idxMuchie].vf1;

				muchieParcursa[idxMuchie] = true;
				stivaNoduri.push_back(vecin);
				nrMuchiiParcurse++;
			}
		}
		else                                                               // daca nodul curent nu mai are muchii de verificat, il scoatem de pe stiva si revenim la un nod anterior
		{
			ciclul.push_back(nod);
			stivaNoduri.pop_back();
		}
	}

	if (nrMuchiiParcurse < nrmuchii)
		esteEulerian = false;
	delete[]muchieParcursa;
}

tmp_moneda
int graf<moneda>::calculeazaCostDrum(int config, int ultimulNod, moneda costDrum[configMax][nrvfMax], moneda costArc[nrvfMax][nrvfMax], int nod_start)
{
	if (costDrum[config][ultimulNod] == -1)   // daca nu a fost calculat costul inca pentru acest drum, se va calcula acum recursiv:
	{
		// noi practic ne vom da mereu cu un pas in spate pentru a calcula costul drumului dat de config ce se termina in ultimulNod
		// in functie de costul drumului dat de config & ultimulNod (configuratia fara ultimulNod) si costul arcului ce duce la ultimul nod din noua configuratie
		// (aici vine recursia pana la cazul de baza, unde costDrum[1<<nod_start][nod_start] nu mai este -1, ci 0

		costDrum[config][ultimulNod] = inf;    

		for (size_t i = 0; i < noduriIncidente[ultimulNod].size(); i++)
		{
			const int nodIncCrt = noduriIncidente[ultimulNod][i];
			if (config & (1 << nodIncCrt))   // daca nodIncCrt face parte din configuratie
			{
				if (nodIncCrt == nod_start and config != ((1 << (ultimulNod)) + (1 << (nod_start))))
				{/* omitem acest nodIncCrt, deoarece nodul de start nu poate fi intalnit decat intr-o configuratie in care este singur cu inca un nod pentru care este incident*/}
				else
					costDrum[config][ultimulNod] = std::min(costDrum[config][ultimulNod], calculeazaCostDrum(config ^ (1 << ultimulNod), nodIncCrt, costDrum, costArc, nod_start) + costArc[nodIncCrt][ultimulNod] );
					// cum mentionam mai sus, aici este calculat recursiv drumul, in functie de o config fara ultimulNod si al carei drum corespunzator se incheie in nodIncCrt
			}
		}
	}

	return costDrum[config][ultimulNod];
}

tmp_moneda
int graf<moneda>::cicluHamiltonian(bool& esteHamiltonian, bool& statusIsError, int nod_start)
{
	if (!orientat or !areCosturi)
	{
		std::cout << "\n   Muchiile din graful dat trebuie sa aiba cost si graful trebuie sa fie orientat pentru a rezolva aceasta problema astfel!";
		g << "\n   Muchiile din graful dat trebuie sa aiba cost si graful trebuie sa fie orientat pentru a rezolva aceasta problema astfel!";
		statusIsError = true;
		return -1;
	}

	noduriIncidente = new std::vector<int>[nrvfMax];   // pt fiecare nod retinem un vector cu nodurile ce au arc spre el
	moneda costDrum [configMax][nrvfMax];               // costurile drumurilor decrise de configuratia respectiva si care se incheie in varful respectiv
	moneda costArc[nrvfMax][nrvfMax];

	int costH = inf;		                                             // costul ciclului hamiltonian
	for (int i = 0; i < nrvf; i++)
	{
		for (int j = 0; j < nrvf; j++)
			costArc[i][j] = inf;                                    // setez costurile tuturor arcelor din fiecare nod i la infinit
		
		for (size_t j = 0; j < vecini[i].size(); j++)
		{
			const int idxVecin = vecini[i][j].index;
			noduriIncidente[idxVecin].push_back(i);  // adaug nodul i ca nod incident (originea unui arc orientat incident) pentru vecinul sau 
			costArc[i][idxVecin] = vecini[i][j].cost;    // actualizez costul tuturor arcelor (date ca input) din fiecare nod i
		}
	}

	memset(costDrum, -1, sizeof(costDrum));        // initializez costurile tuturor drumurilor cu -1 (memset este mai rapid decat iterarea prin matrice pt fiecare elem)
	costDrum[1 << nod_start][nod_start] = 0;        // marchez cu zero costul drumului ce are setat in configuratie doar nodul de start si are ca ultim nod respectivul nod de start

	for (size_t i = 0; i < noduriIncidente[nod_start].size(); i++)
	{
		// cautam nodul incident al nodului de start pentru care
		// suma dintre costArc[nodIncCrt][nod_start] si costul drumului ce trece prin toate nodurile o singura data si se incheie in nodIncCrt este minima:
		const int nodIncCrt = noduriIncidente[nod_start][i];   
		costH = std::min(costH, calculeazaCostDrum((1 << nrvf) - 1, nodIncCrt, costDrum, costArc, nod_start) + costArc[nodIncCrt][nod_start]);
	}

	delete[] noduriIncidente;
	if (costH == inf)
		esteHamiltonian = false;
	return costH;
}

tmp_moneda
bool graf<moneda>::drumuriBFS(int **&capacitati, int **&fluxuri, int *&arboreBFS, int *&tata, bool *&vizitat, const int dim, const int nod_destinatie, const int nod_sursa)
{
	memset(vizitat, 0, sizeof(bool)*dim );  // setam toate nodurile ca nevizitate
	vizitat[nod_sursa] = true;                              // vizitam nodul sursa
	arboreBFS[0] = 1;                                          // in arboreBFS[0] tinem contorul nodurilor vizitate
	arboreBFS[1] = nod_sursa;                            // salvam primul nod vizitat
	
	for (int i = 1; i <= arboreBFS[0]; i++)
	{
		int nod = arboreBFS[i];      
		if (nod != nod_destinatie)                  // nu vom pleca nicaieri mai departe de nod_destinatie, acolo se incheie orice drum
		{
			for (size_t j = 0; j < vecini[nod].size(); j++)
			{
				int vecinul = vecini[nod][j].index;
				if (capacitati[nod][vecinul] != fluxuri[nod][vecinul] and !vizitat[vecinul])  // daca muchia este nesaturata si vecinul este nevizitat
				{
					vizitat[vecinul] = true;
					arboreBFS[++arboreBFS[0]] = vecinul;   // incrementam contorul din arboreBFS si stocam nodul vizitat
					tata[vecinul] = nod;                                // retinem tatal vecinului in cadrul acestui arboreBFS
				}
			}
			for (size_t j = 0; j < veciniGrafRezidual[nod].size(); j++)  // repetam si pentru muchiile inverse
			{
				int vecinul = veciniGrafRezidual[nod][j].index;
				if (capacitati[nod][vecinul] != fluxuri[nod][vecinul] and !vizitat[vecinul])  // daca muchia este nesaturata si vecinul este nevizitat
				{
					vizitat[vecinul] = true;
					arboreBFS[++arboreBFS[0]] = vecinul;  // incrementam contorul din arboreBFS si stocam nodul vizitat
					tata[vecinul] = nod;                                // retinem tatal vecinului in cadrul acestui arboreBFS
				}
			}
		}
	}

	return vizitat[nod_destinatie];
}

tmp_moneda
int graf<moneda>::edmondsKarp(const int nod_destinatie, const int nod_sursa, bool actualizeazaFluxuri)
{
	const int dim = (const int) (nrvf + 1);

	int** capacitati = new int* [dim];                  // capacitatile muchiilor (pentru muchiile inverse fara dublura in graful initial va ramane 0)
	for (int i = 0; i < dim; i++)
		capacitati[i] = new int[dim] {0};

	int** fluxuri = new int* [dim];                       // fluxurile muchiilor 
	for (int i = 0; i < dim; i++)
		fluxuri[i] = new int[dim] {0};
	
	int* arboreBFS = new int[dim] {0};
	int* tata = new int[dim] {0};                          // tatii in arboreleBFS
	bool* vizitat = new bool[dim] {0};
	
	for(int i = 0; i <= nrvf; i++)
		for (size_t j = 0; j < vecini[i].size(); j++)
			capacitati[i][vecini[i][j].index] = vecini[i][j].capacitate;  // actualizam capacitatile pt muchiile prezente in graful direct (adica cel initial)
	
	int fluxGraf = 0;     // rezultatul functiei
	while (drumuriBFS(capacitati, fluxuri, arboreBFS, tata, vizitat, dim, nod_destinatie, nod_sursa))       // cat timp avem un drum nesaturat pana in nodul_destinatie
	{
		for (size_t i = 0; i < veciniGrafRezidual[nod_destinatie].size(); i++)
		{
			int nod = veciniGrafRezidual[nod_destinatie][i].index;
			if (fluxuri[nod][nod_destinatie] != capacitati[nod][nod_destinatie] and vizitat[nod])              // luam pe rand toate drumurile nesaturate pe care le-am generat acum cu drumBFS(...) 
			{
				int fluxAdaos = INT_MAX;      // fluxul ce va putea fi adaugat pe toate muchiile drumului curent
				tata[nod_destinatie] = nod;

				nod = nod_destinatie;
				while (nod != nod_sursa)
				{
					fluxAdaos = std::min(fluxAdaos, capacitati[tata[nod]][nod] - fluxuri[tata[nod]][nod]);  // acest flux va fi cel mai mare ce incape in fiecare muchie de pe drum
					nod = tata[nod];                                                                                                             // urcam inapoi pe drumul din arborele BFS, spre nod_sursa
				}

				if (fluxAdaos)                           // daca fluxul este nenul (negativ nu este, deoarece capacitati[n1][n2] >= fluxuri[n1][n2], oricare ar fi n1,2)
				{
					nod = nod_destinatie;
					while (nod != nod_sursa)       // actualizam fluxul pt toate muchiile de pe drum si inversele lor
					{
						const int vf1 = tata[nod], vf2 = nod;
						fluxuri[vf1][vf2] += fluxAdaos;         // muchia de pe drum primeste flux
						fluxuri[vf2][vf1] -= fluxAdaos;         // muchia ei inversa returneaza acest flux

						nod = tata[nod];
					}
					fluxGraf += fluxAdaos;
				}
			}
		}
	}

	if (actualizeazaFluxuri)
	{
		for (int i = 0; i <= nrvf; i++)
		{
			for (size_t j = 0; j < vecini[i].size(); j++)
			{
				const int vecinul = vecini[i][j].index;
				vecini[i][j].flux = fluxuri[i][vecinul];
			}
			for (size_t j = 0; j < veciniGrafRezidual[i].size(); j++)
			{
				const int vecinul = veciniGrafRezidual[i][j].index;
				veciniGrafRezidual[i][j].flux = fluxuri[vecinul][i];
			}
		}
	}

	if (vizitat)
		delete[] vizitat;
	if (tata)
		delete[] tata;
	if (arboreBFS)
		delete[] arboreBFS;

	for (int i = 0; i < dim; i++)
	{
		if (fluxuri[i])
			delete[] fluxuri[i];
		if (capacitati[i])
			delete[] capacitati[i];
	}
	if (fluxuri)
		delete[] fluxuri;
	if (capacitati)
		delete[] capacitati;

	return fluxGraf;
}

tmp_moneda
 vpair graf<moneda>::muchiileCuFlux()
{
	// returneaza un vector de perechi de int ce reprezinta muchiile cu flux din reteaua de transport
	vpair solutia;
	for(int i = 1; i <= nrvfSt; i++)
		for(size_t j = 0; j < vecini[i].size(); j++)
			if (vecini[i][j].flux)				
				solutia.push_back(std::pair<int, int>(i, vecini[i][j].index));
	return solutia;
}

tmp_moneda
vpair graf<moneda>::conectareGraf(int& extraTotal, int& extraMaxim)
{
	// extraTotal repr nr total de muchii extra ce trb adaugate pt ca graful sa fie conex
	// extraMaxim repr cel mai mare nr de muchii extra incidente aceluaiasi nod

	vpair muchiileExtra;         // vectorul solutie
	if (nrvf == 2)
	{// daca am doua noduri si stiu ca graful nu este conex, inseamna ca ele sunt izolate si doar le leg unul cu altul
		extraTotal = 1;
		extraMaxim = 1;
		std::pair<int, int> aux(1, 2);
		muchiileExtra.push_back(aux);
		return muchiileExtra;
	}

	contor = 0;                       // nr componente conexe
	bool* viz = new bool[nrvf + 1]{ 0 };
	int* ordineVizita = new int[nrvf + 1]{ 0 };
	int idxOrdineVizita = 0;   //  (indexam de la 1)

	vpair capCoada;                // vector de perechi de int ce repr nodul cu care am inceput parcurgerea DFS a unei componente conexe si cel cu care am incheiat
	std::vector<int> noduriIzolate;
	std::vector<int> legaturiDisponibile;                     // legaturi disponibile pt nodurile izolate
	for (int i = 1; i <= nrvf; i++)
		if (!viz[i])
		{
			contor++;
			idxOrdineVizita++;
			int idxOrdineVizitaVechi = idxOrdineVizita; // vom retine punctul de ordine de unde incepe aceasta comp conexa
			DFS(i, viz, ordineVizita, idxOrdineVizita);
			int cap = i, coada = *(ordineVizita + idxOrdineVizita);

			if (cap == coada)                                           // daca avem un singur nod in componenta conexa
				noduriIzolate.push_back(cap);                 // retinem nodul izolat
			else
			{
				std::pair<int, int> legaturile(cap, coada);  // primul, respectiv ultimul, nod parcurs din aceasta comp conexa
				capCoada.push_back(legaturile);              //    acestea vor lega comp conexe (care nu sunt noduri izolate) intre ele
			}
			
			for (int ord = idxOrdineVizitaVechi + 1; ord < idxOrdineVizita; ord++)  
			{// referitor la nodurile acestei comp conexe care nu sunt nici cap nici coada:
				const int legatura = ordineVizita[ord];
				if (noduriIzolate.size())                                                    // daca am noduri izolate care au nevoie de legatura
				{
					std::pair<int, int> aux(noduriIzolate.back(), legatura);  // o folosesc acum sa le conectez
					muchiileExtra.push_back(aux);                                     
					noduriIzolate.pop_back();                                             // si le scot din nodurile izolate 
				}
				else                                                                                   // daca nu am acum noduri izolate pt legatura aceasta
					legaturiDisponibile.push_back(legatura);                      // o pastrez pt mai tz
			}
		}

	int legatura = -1;
	if (capCoada.size())                                                           // daca am cel putin o comp conexa (care nu e nod izolat)
	{
		legaturiDisponibile.push_back(capCoada[0].first);        // capul primei comp conexe este o legatura disponibila pt nodurile izolate
		legatura = capCoada[0].second;
		for (size_t i = 1; i < capCoada.size(); i++)                       // voi obtine un lant de componente conexe:
		{
			std::pair<int, int> aux(legatura, capCoada[i].first);   // legam coada perechii precedente de capul celei curente
			muchiileExtra.push_back(aux);                                // stochez muchia extra
			legatura = capCoada[i].second;                                 // retin coada perechii curente ca fiind urmatoarea legatura
		}
		legaturiDisponibile.push_back(legatura);                     // coada ultimei comp conexe este o legatura disponibila pt nodurile izolate
	}

	while (noduriIzolate.size() and legaturiDisponibile.size())  // voi lega nodurile izolate de legaturile disponibile cat pot
	{
		std::pair<int, int> aux(noduriIzolate.back(), legaturiDisponibile.back());
		muchiileExtra.push_back(aux);
		noduriIzolate.pop_back();
		legaturiDisponibile.pop_back();
	}
	
	extraTotal = contor - 1;   // informatia asta reiese imediat
	extraMaxim = 1;               // <- asta depinde 
	if (noduriIzolate.size())   // daca au mai ramas noduri izolate
	{
		extraMaxim = 2;   // voi inlantui nodurile izolate intre 
		if (legatura != -1)   // si pe primul il voi lega de comp conexa mare, daca ea exista
		{
			std::pair<int, int> aux(noduriIzolate.back(), legatura);
			muchiileExtra.push_back(aux);
	        // nu dau pop din noduriIzolate pt ca o voi face oricum in afara ifului
		}
		legatura = noduriIzolate.back();  // nodul de unde incep lantul de noduri izolate
		noduriIzolate.pop_back();           // cum am salvat nodul in variabila legatura, nu il mai retin in noduriIzolate

		while (noduriIzolate.size())
		{
			std::pair<int, int> aux(noduriIzolate.back(), legatura);
			muchiileExtra.push_back(aux);

			legatura = noduriIzolate.back();
			noduriIzolate.pop_back();
		}
	}
	
	delete[] ordineVizita;
	delete[]viz;
	return muchiileExtra;
}

// ia linia si coloana dintr-o matrice si returneaza locul intr-o proiectie unidimensionala (array) 
int getId(const int lin, const int col, const int nrLin)
{
	return lin * nrLin + col;
}

// ia locul intr-o proiectie unidimensionala(array) si seteaza coordonatele de linie si coloana intr-o matrice cu nrLin linii
void getCoords(int& lin, int& col, const int id, const int nrLin)
{
	lin = id / nrLin;
	col = id % nrLin;
}

void transformaVecinDragon(char**& temnita, int nrLin, int nrCol, int lin, int col, std::vector<int> &viitoriiDragoni, bool &incheieCautarile)
{
	if (lin >= 0 and lin < nrLin and col >= 0 and col < nrCol)
	{
		if (temnita[lin][col] == 'I' or temnita[lin][col] == 'O')
			incheieCautarile = true;           // daca intrarea sau iesirea devin dragoni, incheiem cautarea distantei maxime
		else if (temnita[lin][col] == '.' or temnita[lin][col] == '*')
		{
			viitoriiDragoni.push_back(getId(lin, col, nrLin));
			temnita[lin][col] = 'D';
		}
	}
}

tmp_moneda
int graf<moneda>::distantaSiguraPaftenie(char **&temnita, const int nrLin, const int nrCol, std::vector<int> &idxDragoni, const int idIntrare, const int idIesire)
{
	int* dist = new int[nrvf + 1] {0};
	
	BFS(dist, idIntrare);                   // verificam daca nodul de iesire este accesibil
	if (dist[idIesire] < 0)
	{
		delete[] dist;
		return -1;
	}

	int distantaMax = 0;
	while (true)
	{
		memset(dist, 0, sizeof(int)*(nrvf+1));
		for (size_t i = 0; i < idxDragoni.size(); i++)
			dist[idxDragoni[i]] = -1;       // in felul asta, va fi inaccesibil parcurgerii BFS, distanta fiind practic +infinit      

		BFS(dist, idIntrare);
		if (dist[idIesire] >= 0)
			distantaMax++;
		else
			break;

		std::vector<int> viitoriiDragoni; // transform celulele non-dragoni din jurul dragonilor in dragoni 
		for (size_t i = 0; i < idxDragoni.size(); i++)
		{
			const int id = idxDragoni[i];   
			int lin, col;
			getCoords(lin, col, id, nrLin);

			bool incheieCautarile = false;
			transformaVecinDragon(temnita, nrLin, nrCol, lin - 1, col, viitoriiDragoni, incheieCautarile);
			transformaVecinDragon(temnita, nrLin, nrCol, lin + 1, col, viitoriiDragoni, incheieCautarile);
			transformaVecinDragon(temnita, nrLin, nrCol, lin, col - 1, viitoriiDragoni, incheieCautarile);
			transformaVecinDragon(temnita, nrLin, nrCol, lin, col + 1, viitoriiDragoni, incheieCautarile);
			if (incheieCautarile)
			{
				delete[] dist;
				return distantaMax;
			}
		}
		idxDragoni = viitoriiDragoni;   // viitorii dragoni devin oficial dragoni 
	}

	delete[] dist;
	return distantaMax;
}
#pragma endregion

template <typename moneda>
class subgraf : public graf<moneda>
{
public:
	subgraf(int nrvf_param, vmuchii vector_muchii) : graf<moneda>(nrvf_param)
	{
		// Obs: indexarea nodurilor incepe de la 1 (nodul 0 este ignorat)
		// initializarea membrilor:
		graf<moneda>::nrmuchii = vector_muchii.size();
		const int nrmuchii = graf<moneda>::nrmuchii, nrvf = graf<moneda>::nrvf;

		for (int i = 0; i < nrmuchii; i++)
		{
			const int nod = vector_muchii[i].vf1;
			vecin<moneda> aux(vector_muchii[i].vf2, vector_muchii[i].cost);		// vecin pt nod, cu indexul si costul din vector_muchii[i]
			graf<moneda>::vecini[nod].push_back(aux);
		}

		/*std::cout << "vecinii lui 0 sunt: ";
		for (int j = 0; j < graf<moneda>::vecini[0].size(); j++)
			std::cout << graf<moneda>::vecini[0][j] << " | ";
		std::cout << " \n";
		graf<moneda>::verifvecini();
		std::cout << "\n";*/
	}
};


int main()
{
	start = 0;		// daca start e setat la zero, nu se va mai citi un nod de start

	///		/// verif vecini
	/*graf<> graful(false, false, false);
	graful.verifvecini();*/

	///		/// BFS
	/*start = 1;
	graf<> graful(true, false, false);
	int* dist = NULL;
	graful.BFS(dist, start);
	if (dist)
	{
		for (int i = 1; i <= graful.get_nrvf(); i++)
			g << dist[i] << " ";
		delete[]dist;
	}*/

	///		/// DFS & comp conexe
	/*graf<> graful(false, false, false);
	int* ordineVizita = graful.cadruDFS();
	g << contor;
	if (ordineVizita)
	{
		for (int i = 1; i <= graful.get_nrvf(); i++)
			std::cout << ordineVizita[i] << " ";
		delete[]ordineVizita;
	}*/

	///		/// comp biconexe
	/*graf<> graful(false, false, false);
	vector_vectori comp_biconexe = graful.cadru_biconexe();
	g << contor << "\n";
	for (unsigned int i = 0; i < comp_biconexe.size(); i++)
	{
		for (unsigned int j = 0; j < comp_biconexe[i].size(); j++) {
			g << comp_biconexe[i][j] << " ";
		}
		g << "\n";
	}*/

	///		/// muchii critice
	/*graf<> graful(false, false, false);
	vector_vectori* connections = new vector_vectori;
	graful.copiaza_listeAdiacenta(*connections);
	vector_vectori vector_afisare = graful.criticalConnections(graful.get_nrvf(), *connections);
	for (int i = 0; i < vector_afisare.size(); i++)
		std::cout << vector_afisare[i][0] << " " << vector_afisare[i][1] << "\n";
	delete connections;*/

	///		/// comp tare conexe
	/*graf<> graful(true, false, false);
	bool statusIsError = false;
	vector_vectori comp_tareconexe = graful.cadru_tareconexe(statusIsError);
	g << contor << "\n";
	for (unsigned int i = 1; i < comp_tareconexe.size(); i++)
	{
		for (unsigned int j = 0; j < comp_tareconexe[i].size(); j++)
			g << comp_tareconexe[i][j] << " ";
		g << "\n";
	}*/

	///		/// havel hakimi 
	/*std::cout << "\n\n Status final Havel Hakimi: " << havelHakimi() << "\n\n";
	*/
	
	///		/// sortare topologica
	/*graf<> graful(true, false, false);
	int* v_ordineTopologica = NULL;
	statusIsError = false;
	graful.cadruSortareTopologica(v_ordineTopologica, statusIsError);
	if (v_ordineTopologica)
	{
		for (int i = graful.get_nrvf(); i >= 1; i--)
			g << v_ordineTopologica[i] << " ";
		delete[]v_ordineTopologica;
	}*/

	///		/// paduri dijuncte 
	/*kruskal_paduri();
	*/

	///		/// kruskal
	/*graf<> graful(false, true, true);
	muchie<>* muchiiAPM = NULL;
	int cost_apm = 0;
	bool statusIsError = false;
	graful.kruskal(muchiiAPM, cost_apm, statusIsError);
	if (muchiiAPM)
	{
		const size_t nrMuchiiAPM = graful.get_nrvf() - 1;
		g << cost_apm << "\n" << nrMuchiiAPM;
		for (unsigned int i = 1; i <= nrMuchiiAPM; i++)
		{
			g << "\n" ;
			g << muchiiAPM[i];
		}

		delete[]muchiiAPM;
	}*/

	///		/// prim
	/*graf<> graful(false, true, false);
	muchie<>* muchiiAPM = NULL;
	int cost_apm = 0;
	bool statusIsError = false;
	graful.prim(muchiiAPM, cost_apm, statusIsError);
	if (muchiiAPM)
	{
		const size_t nrMuchiiAPM = graful.get_nrvf() - 1;
		g << cost_apm << "\n" << nrMuchiiAPM;
		for (unsigned int i = 1; i <= nrMuchiiAPM; i++)
		{
			g << "\n" ;
			g << muchiiAPM[i];
		}

		delete[]muchiiAPM;
	}*/

	///		/// dijkstra
	/*graf<> graful(true, true, false);
	long long int* costul = NULL;
	graful.dijkstra(costul);
	if (costul)
	{
		for (int i = 2; i <= graful.get_nrvf(); i++)
			g << costul[i] << " ";
		delete [] costul;
	}*/

	///		/// bellman ford
	/*graf<> graful(true, true, false);
	long long int* costul = NULL;
	bool areCicluNegativ = false;
	graful.bellmanFord(costul, areCicluNegativ);
	if (costul)
	{
		if (!areCicluNegativ)
			for (int i = 2; i <= graful.get_nrvf(); i++)
				g << costul[i] << " ";

		delete[]costul;
	}*/

	///	/// tarjan -> sortare topologica -> bellman ford 
	/*graf<> graful(true, true, false);
	long long int* costul = NULL;
	bool areCicluNegativ = false;
	graful.TarjanTopologicBellmanFord(costul, areCicluNegativ);
	if (costul)
	{
		if (!areCicluNegativ)
			for (int i = 2; i <= graful.get_nrvf(); i++)
				g << costul[i] << " ";

		delete[]costul;
	}*/

	///	/// roy-floyd
	/*graf<> graful(true, true, false, false, true);
	long long int** costuri = NULL;
	bool statusIsError = false;
	graful.royFloyd(costuri, statusIsError);
	if (costuri)
	{
		for (int i = 1; i <= graful.get_nrvf(); i++)
		{
			for (int j = 1; j <= graful.get_nrvf(); j++)
				g << costuri[i][j] << " ";
			g << "\n";
			if(costuri[i])
				delete [] costuri[i];
		}
		delete[] costuri;
	}*/

	///	/// diametru arbore 
	/*int nrvf;
	f >> nrvf;
	graf<> graful(false, false, false, false, false, false, false, nrvf, nrvf-1);
	start = 1 +  rand()%graful.get_nrvf();		// incepem cautarea dintr-un nod oarecare; este important sa setam acest start DUPA ce am initializat graful, altfel se va citi incorect (vezi conditie citire nod start in constructor)
	int* dist;
	graful.BFS(dist, start);
	if (dist)
	{
		int diametrul = -1;
		for (int i = 1; i <= graful.get_nrvf(); i++)	// cautam cel mai indepartat nod din BFS
			if (dist[i] > diametrul)
			{
				diametrul = dist[i];		// actualizam diametrul
				start = i;		// actualizam radacina
			}
		delete[]dist;

		graful.BFS(dist, start);		// mai parcurgem odata cu un BFS din radacina
		for (int i = 1; i <= graful.get_nrvf(); i++)	// cautam cel mai indepartat nod din BFS fata de radacina gasita
			if (dist[i] > diametrul)
				diametrul = dist[i];		// actualizam diametrul

		if (dist)
		{
			delete[]dist;
			diametrul++;		// diametrul numara nodurile din lantul maxim, in timp ce un BFS numara muchiile din lant (cu 1 mai putine decat nodurile)
			g << diametrul;
		}
	}*/

	///	/// ciclu eulerian
	/*graf<> graful(false, false, true, true);
	std::vector<int> ciclul;	// aici vom avea nodurile ciclului Eulerian
	bool esteEulerian = true, statusIsError = false;
	graful.cicluEulerian(ciclul, esteEulerian, statusIsError);
	if (!esteEulerian)
		g << "-1";
	else
	{
		for (size_t i = 0; i < ciclul.size() - 1; i++)   // parcurgem ciclul, fara sa mai afisam iar nodul de start
			g << ciclul[i] << " ";
	}*/

	///	 /// ciclu hamiltonian
	/*graf<> graful(true, true, false);
	bool esteHamiltonian = true, statusIsError = false;
	int costMinCicluH = graful.cicluHamiltonian(esteHamiltonian, statusIsError);
	if (!esteHamiltonian)
		g << "Nu exista solutie\n";
	else
		g << costMinCicluH << '\n';*/

	///		/// flux maxim
	/*graf<> graful(true, false, false, false, false, true);
	//graful.verifvecini();
	//graful.verifveciniGrafRezidual();
	g << graful.edmondsKarp(graful.get_nrvf());*/

	///		/// cuplaj maxim
	/*graf<> graful(true, false, false, false, false, true, true);
	g << graful.edmondsKarp(graful.get_nrvf(), 0, true) << "\n";
	vpair cuplaj = graful.muchiileCuFlux();
	for (size_t i = 0; i < cuplaj.size(); i++)
		g << cuplaj[i].first << " " << cuplaj[i].second - graful.get_nrvfSt() << "\n";*/

	///		/// conexidad 
	/*graf<int> graful(false, false, false);
	int extraTotal, extraMaxim;
	vpair muchiileExtra = graful.conectareGraf(extraTotal, extraMaxim);
	g << extraMaxim << "\n" << extraTotal << "\n";
	for (size_t i = 0; i < muchiileExtra.size(); i++)
		g << muchiileExtra[i].first << " " << muchiileExtra[i].second << "\n";*/

	///		/// paftenie barbarul
	/*int nrLin, nrCol;
	f >> nrLin >> nrCol;

	temnita = new char* [nrLin];    // matricea temnitei, cu liniile si coloanele indexate de la 0
	for (int i = 0; i < nrLin; i++)
		temnita[i] = new char[nrCol];

	int nrvfGraf = nrLin * nrCol, nrmuchiiGraf = 0;
	std::vector< vecin< int >>* veciniGraf = new std::vector< vecin< int >>[nrvfGraf + 1];  // fiecare celula din matrice cu camera goala, celula de intrare si cea de iesire vor fi cate un nod intr-un graf
	std::vector<int> idxDragoni;          // id-urile la care gasim dragoni 
	int idIntrare, idIesire;              

	for(int i = 0; i < nrLin; i++ )           // initializam matricea temnita, vectorul de vecini (cu id-uri) pt graful celulelor accesibile, id-urile speciale (pt intrarea si iesirea din temnita)
		for (int j = 0; j < nrCol; j++)
		{
			char val;
			f >> val;
			temnita[i][j] = val;
			int id = getId(i, j, nrLin);      // pozitie in secventa liniara de mem

			if (val == '.' or val == 'I' or val == 'O')
			{
				if (i > 0 and (temnita[i - 1][j] == '.' or temnita[i - 1][j] == 'I' or temnita[i - 1][j] == 'O'))   // daca am celula accesibila deasupra
				{                                                                                                                                        // leg celula crt de cea de deasupra (si reciproc)
					nrmuchiiGraf++;
					int idSus = getId(i - 1, j, nrLin);
					veciniGraf[id].push_back(idSus);
					veciniGraf[idSus].push_back(id);
				}
				if (j > 0 and (temnita[i][j - 1] == '.' or temnita[i][j - 1] == 'I' or temnita[i][j - 1] == 'O'))    // daca am celula accesibila la stanga
				{                                                                                                                                          // leg celula crt de cea din stanga (si reciproc)
					nrmuchiiGraf++;
					int idStanga = getId(i, j - 1, nrLin);
					veciniGraf[id].push_back(idStanga);
					veciniGraf[idStanga].push_back(id);
				}

				if(val == 'I')
					idIntrare = id;               // pozitie intrare in secventa liniara de mem
				if(val == 'O')
					idIesire = id;                 // pozitie iesire in secventa liniara de mem
			}
			else if(val == 'D')
				idxDragoni.push_back(id); // pozitie dragon in secventa liniara de mem
		}

	graf<int> graful(false, false, false, false, false, false, false, nrvfGraf, 0, veciniGraf);
	g << graful.distantaSiguraPaftenie(temnita, nrLin, nrCol, idxDragoni, idIntrare, idIesire);
	
	for (int i = 0; i < nrLin; i++)
		delete[] temnita[i];
	delete[] temnita;*/

	return 0;
}