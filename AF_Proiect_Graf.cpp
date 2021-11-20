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

std::ifstream f("graful.in");
std::ofstream g("graful.out");
//std::ifstream f("bellmanford.in");
//std::ofstream g("bellmanford.out");


int start = 0, contor;   // contorul il vom folosi in mai multe probleme;  start va aparea eventual ca nod de start
const int nodgol_int = 0;

#define tmp_moneda template <typename moneda>

template <typename moneda = int>
class vecin
{
public:
	int index;  // eticheta sa
	int cost;	 // costul muchiei catre acest vecin

	vecin(int idx = 0, moneda costul = 0) : index(idx), cost(costul) {}
	tmp_moneda
	friend std::ostream& operator<<(std::ostream& out, const vecin<moneda>& vecinul);
};
template <typename moneda = int>
std::ostream& operator<<(std::ostream& out, const vecin<moneda>& vecinul)
{
	out << vecinul.index << " " << vecinul.cost;
	return out;
}

#pragma region muchie
// declar prin antet clasa si doi operatori ce ii vor fi prieteni: 
template <typename moneda = int>
class muchie;

template<typename moneda>
std::ostream& operator<<(std::ostream& out, const muchie<moneda>& m);
template<typename moneda>
std::ofstream& operator<<(std::ofstream& out, const muchie<moneda>& m);

template <typename moneda>
class muchie
{
public:
	int vf1;
	int vf2;
	moneda cost;

	muchie(int varf1 = 0, int varf2 = 0, int costul = 0) : vf1(varf1), vf2(varf2), cost(costul) {}
	muchie(const muchie < moneda >& sursa) : vf1 (sursa.getvf1() ), vf2(sursa.getvf2()), cost(sursa.getcost()) {}


	void setAll(int varf1, int varf2, int costul)
	{
		vf1 = varf1;
		vf2 = varf2;
		cost = costul;
	}
	int getvf1() const { return vf1; }
	int getvf2() const { return vf2; }
	moneda getcost() const
	{
		return cost;
	}

	bool operator == (const muchie& m) const
	{
		return (vf1 == m.getvf1() and vf2 == m.getvf2() and cost == m.cost);
	}
	bool operator < (const muchie& m) const
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
	bool operator > (const muchie& m) const
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

	friend std::ostream& operator<< <>(std::ostream& out, const muchie<moneda>& m);		// aici specific ca operatorii mai sus declarati sunt prieteni
	friend std::ofstream& operator<< <>(std::ofstream& out, const muchie<moneda>& m);
};

template<typename moneda>			// si aici specific implementarea celor doi operatori
std::ostream& operator<<(std::ostream& out, const muchie<moneda>& m)
{
	out << m.vf1 << " " << m.vf2 << " " << m.cost;
	return out;
}
template<typename moneda>
std::ofstream& operator<<(std::ofstream& out, const muchie<moneda>& m)
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

	nod<int, long long int>* R, *nodgol;

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
				if (! inverseaza_prioritate )
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

typedef std::stack< std::pair <int, int > > stackpair;
typedef std::unordered_set< int > multime;
typedef std::vector< std::vector < int > > vector_vectori;
typedef std::vector < vecin<int> > vector_vecini;

template <typename moneda = int>
class graf
{
protected:
	const bool orientat;
	const bool areCosturi;
	const bool areListaMuchii;

	int nrvf, nrmuchii;
	vector_vecini* vecini;

	muchie<moneda>* lista_muchii;
	int size_lista_muchii;
	bool lista_muchii_sortata;

	void DFS(int nod, bool* viz);
	void biconexe(int nod, int tata, bool* viz, multime* wayback, stackpair& muchiiviz, vector_vectori& comp_biconexe);
	void tareconexe(int nod, int& freeorder, int* order, int* leastbackorder, bool* pestiva, std::stack<int>& noduriviz, vector_vectori& comp_tareconexe);
	void mCrits(int nod, int tata, bool* viz, multime* wayback, stackpair& muchiiviz, vector_vectori& connections, vector_vectori& solutia);
	void DFS_SortareTopologica(int nod, bool* viz, int*& finished, int& idxfinished);

	graf(int nrvf_param);		// doar pt declararea unor subgrafuri
public:
	graf(bool orientat_param = true, bool areCosturi_param = true, bool areListaMuchii_param = true);

	int get_nrvf() { return nrvf; }
	void copiaza_listeAdiacenta(vector_vectori& solution);
	void verifvecini();

	void BFS();
	void cadruDFS();
	void cadru_biconexe();
	vector_vectori cadru_tareconexe();
	vector_vectori criticalConnections(int n, vector_vectori& connections);
	void cadruSortareTopologica(int*& finished, bool statusIsError = false);

	void kruskal();
	void prim();
	void dijkstra(int nod = 1);
	void bellmanFord(int nod_sursa = 1);
	void TarjanTopologicBellmanFord(int nod_sursa = 1);

	~graf()
	{
		delete[]vecini;
		if (lista_muchii)
			delete[] lista_muchii;
	}
};

template <typename moneda = int>
class subgraf;


#pragma region graf & auxiliare
tmp_moneda
graf<moneda>::graf(bool orientat_param, bool areCosturi_param, bool areListaMuchii_param) : orientat(orientat_param), areCosturi(areCosturi_param), areListaMuchii(areListaMuchii_param)
{
	f >> nrvf >> nrmuchii;
	if (start)	// daca start este nenul, inseamna ca avem de citit un nod de start...
		f >> start;

	vecini = new vector_vecini[nrvf + 1];
	if (areListaMuchii)
	{
		//if (orientat)
		size_lista_muchii = nrmuchii;
		//else
		// 	size_lista_muchii = 2 * nrmuchii;

		lista_muchii = new muchie<moneda>[size_lista_muchii];
	}
	else
	{
		size_lista_muchii = 0;
		lista_muchii = NULL;
	}
	lista_muchii_sortata = false;

	int idxListaMuchii = 0;
	for (int i = 0; i < nrmuchii; i++)
	{
		int x, y, c = 0;
		f >> x >> y;
		if (areCosturi)
			f >> c;

		vecin<moneda> aux(y, c);
		vecini[x].push_back(aux);
		if (!orientat)
		{
			vecin<moneda> aux(x, c);
			vecini[y].push_back(aux);
		}

		if (areListaMuchii)
		{
			lista_muchii[idxListaMuchii].setAll(x, y, c);
			idxListaMuchii++;
			//if (!orientat)
			//{
			//	lista_muchii[idxListaMuchii].setAll(y, x, c);
			//	idxListaMuchii++;
			//}
		}
	}
}
tmp_moneda
graf<moneda>::graf(int nrvf_param) : orientat(true), areCosturi(true), areListaMuchii(false), lista_muchii(NULL), size_lista_muchii(0), lista_muchii_sortata(false)
{
	nrvf = nrvf_param;
	nrmuchii = 0;		// se va modifica direct in constructorul clasei subgraf
	vecini = new vector_vecini[nrvf + 1];
}

tmp_moneda
void graf<moneda>::verifvecini()
{
	for (int i = 1; i <= nrvf; i++)
	{
		std::cout << "\n  vecinii lui " << i << " :  ";
		for (unsigned int j = 0; j < vecini[i].size(); j++)
			std::cout << vecini[i][j].index << " ";
	}
}

tmp_moneda
void graf<moneda>::BFS()
{
	int* dist = new int[nrvf + 1]{ 0 };	// initializam distantele cu 0 (le decrementam ulterior)
	std::queue <int> qBFS;					// coada pt BFS
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

	for (int i = 1; i <= nrvf; i++)
		g << dist[i] - 1 << " ";

	delete[] dist;
}

tmp_moneda
void graf<moneda>::DFS(int nod, bool* viz)
{
	viz[nod] = true;
	for (unsigned int i = 0; i < vecini[nod].size(); i++)
	{
		const int nod_urm = vecini[nod][i].index;
		if (!viz[nod_urm])
			DFS(nod_urm, viz);
	}
}
tmp_moneda
void graf<moneda>::cadruDFS()
{
	contor = 0;
	bool* viz = new bool[nrvf + 1]{ 0 };
	for (int i = 1; i <= nrvf; i++)
		if (!viz[i])
		{
			contor++;
			DFS(i, viz);
		}
	g << contor;
	delete[]viz;
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
void graf<moneda>::cadru_biconexe()
{
	contor = 0;
	vector_vectori comp_biconexe;		// solutia, de forma unui vector cu alti vectori ce reprezinta componentele biconexe
	bool* viz = new bool[nrvf + 1]{ 0 };	// nodurile vizitate deja
	stackpair muchiiviz;							// stiva de muchii vizitate
	multime* setgol = new multime;		// un set "wayback" pe care il pasez fiului pentru a-mi returna caile de intoarcere disponibile
	biconexe(1, -1, viz, setgol, muchiiviz, comp_biconexe);
	delete setgol;
	delete[]viz;

	g << contor << "\n";
	for (unsigned int i = 0; i < comp_biconexe.size(); i++)
	{
		for (unsigned int j = 0; j < comp_biconexe[i].size(); j++) {
			g << comp_biconexe[i][j] << " ";
		}
		g << "\n";
	}
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
vector_vectori graf<moneda>::cadru_tareconexe()
{
	vector_vectori comp_tareconexe;				// solutia, de forma unui vector cu alti vectori ce reprezinta componentele tareconexe

	if (!orientat)
	{
		std::cout << "\n   Graful dat trebuie sa fie orientat pentru a rezolva aceasta problema";
		g << "\n   Graful dat trebuie sa fie orientat pentru a rezolva aceasta problema";
		return comp_tareconexe;
	}

	contor = 0;													// nr componente conexe
	std::stack<int> noduriviz;								// stiva de noduri vizitate
	bool* pestiva = new bool[nrvf + 1]{ 0 };
	int* order = new int[nrvf + 1]{ 0 };				// ordinea intrarii pe stiva a nodurilor
	int freeorder = 1;											// nr de ordine disponibil; se va incrementa pe masura ce un nod primeste nr de ordine (in array-ul order)
	int* leastbackorder = new int[nrvf + 1]{ 0 };		// cel mai mic ordin accesibil din urma, de pe stiva

	for (int i = 1; i <= nrvf; i++)
		if (order[i] == 0)		//  nod nevizitat 
			tareconexe(i, freeorder, order, leastbackorder, pestiva, noduriviz, comp_tareconexe);

	g << contor << "\n";
	for (unsigned int i = 0; i < comp_tareconexe.size(); i++)
	{
		for (unsigned int j = 0; j < comp_tareconexe[i].size(); j++)
			g << comp_tareconexe[i][j] << " ";
		g << "\n";
	}

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
void graf<moneda>::cadruSortareTopologica(int*& finished, bool statusIsError)
{
	if (!orientat)
	{
		std::cout << "\n   Graful dat trebuie sa fie orientat pentru a rezolva aceasta problema";
		g << "\n   Graful dat trebuie sa fie orientat pentru a rezolva aceasta problema";
		statusIsError = true;
		return;
	}

	bool* viz = new bool[nrvf + 1]{ 0 };
	int idxfinished = 1;
	for (int i = 1; i <= nrvf; i++)
		if (!viz[i])
			DFS_SortareTopologica(i, viz, finished, idxfinished);

	for (int i = nrvf; i >= 1; i--)
		g << finished[i] << " ";
	delete[]viz;
}

template <typename moneda = int>
int tataMare(int nod, int tata[])
{
	// implementam un algoritm "tataMare()" care imi trece recursiv prin tati si la intoarcere imi seteaza peste tot cel mai batran nod ca fiind tata direct

	if (tata[nod] == nodgol_int)
		return nod;			// l-am gasit pe tataMare
	tata[nod] = tataMare<moneda>(tata[nod], tata);		// toate nodurile de pe drum il vor primi ca tata pe tataMare
	return tata[nod];		// dau inapoi in recursie informatia pe care am primit-o eu despre cine e tataMare in arborele acesta
}
tmp_moneda
void graf<moneda>::kruskal()
{
	if (!areCosturi)
	{
		std::cout << "\n   Muchiile din graful dat trebuie sa  pentru a rezolva aceasta problema";
		g << "\n   Muchiile din graful dat trebuie sa  pentru a rezolva aceasta problema";
		return;
	}
	
	if (! lista_muchii_sortata)
	{
		std::sort(lista_muchii, lista_muchii + size_lista_muchii);
		lista_muchii_sortata = true;
	}
	std::vector<int> indecsii_muchii;
	int cost_apm = 0;   

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
			indecsii_muchii.push_back(i);
			cost_apm += lista_muchii[i].cost;

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

	g << cost_apm << "\n" << nrvf - 1;	// cost si nr muchii APM 
	for (unsigned int i = 0; i < indecsii_muchii.size(); i++)
		g << "\n" << lista_muchii[ indecsii_muchii[i] ];
}

tmp_moneda
void graf<moneda>::prim()
{
	if (!areCosturi)
	{
		std::cout << "\n   Muchiile din graful dat trebuie sa  pentru a rezolva aceasta problema";
		g << "\n   Muchiile din graful dat trebuie sa  pentru a rezolva aceasta problema";
		return;
	}

	typedef muchie< moneda > muchie;
	typedef std::vector< muchie > vmuchii;

	std::priority_queue< muchie, vmuchii, std::greater< muchie > > muchii_de_verif;		// folosim pq pt a accesa minimul in O(1)
	vmuchii solutia;																											// aici vrem inserare in O(1) si cam atat
	bool* noduri_selectate = new bool[nrvf + 1] { 0 };														// aici vrem doar verificare de true in O(1)
	int nod = 1;
	noduri_selectate[ nod ] = true;
	contor = 1;										// avem un nod selectat
	int cost_apm = 0;

	while (contor < nrvf)
	{
		for (unsigned int i = 0; i < vecini[nod].size(); i++)
		{
			muchie aux(nod, vecini[nod][i].index, vecini[nod][i].cost);
			if ( ! noduri_selectate[ aux.vf2 ] )		// daca aux.vf2 nu este printre nodurile selectate deja
				muchii_de_verif.push(aux);				// luam pt verificare muchia adiacenta lui nod
		}

		while (!muchii_de_verif.empty() and noduri_selectate[ muchii_de_verif.top().vf2 ] )		// cat timp muchia de cost minim disponibila duce la un nod deja selectat
			muchii_de_verif.pop();																									// scoatem muchia din set

		if (muchii_de_verif.empty())
		{
			std::cout << "\n Graful nu este conex!";
			break;
		}

		const muchie muchia = muchii_de_verif.top();		// muchia pe care o vom selecta
		nod = muchia.vf2;																	// nod = primul nod neselectat
		solutia.push_back(muchia);													// salvam muchia in solutie
		cost_apm += muchia.cost;
		muchii_de_verif.pop();															// scoatem muchia din verificare
		noduri_selectate[ nod ] = true;												// selectam si noul nod
		contor++;																				// si il numaram la noduri selectate
	}

	g << cost_apm << "\n" << --contor << "\n";
	for (unsigned int i = 0; i < solutia.size(); i++)
		g << solutia[i] << "\n";
}

tmp_moneda
void graf<moneda>::dijkstra(int nod)
{
	if (!areCosturi)
	{
		std::cout << "\n   Muchiile din graful dat trebuie sa  pentru a rezolva aceasta problema";
		g << "\n   Muchiile din graful dat trebuie sa  pentru a rezolva aceasta problema";
		return;
	}

	typedef muchie< moneda > muchie;
	typedef std::vector< muchie > vmuchii;

	long long int* costul;
	bool* vizitat = new bool[nrvf + 1]{ 0 };
	contor = 1;										// avem un nod selectat
	
	if (!orientat)
	{
		costul = new long long int[nrvf + 1]{ 0 };																	// aici vrem inserare si accesare in O(1) si cam atat
		std::priority_queue< muchie, vmuchii, std::greater< muchie > > muchii_de_verif;		// folosim pq pt a accesa minimul in O(1)
		bool* noduri_selectate = new bool[nrvf + 1]{ 0 };														// aici vrem doar verificare de true in O(1)
		noduri_selectate[nod] = true;

		while (contor < nrvf)
		{
			for (unsigned int i = 0; i < vecini[nod].size(); i++)
			{
				muchie aux(nod, vecini[nod][i].index, vecini[nod][i].cost);
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

			const muchie muchia = muchii_de_verif.top();							// muchia pe care o vom selecta
			costul[muchia.vf2] = costul[muchia.vf1] + muchia.cost;				// salvam muchia in solutie
			nod = muchia.vf2;																		// nod = primul nod neselectat
			muchii_de_verif.pop();																// scoatem muchia din verificare
			noduri_selectate[nod] = true;													// selectam si noul nod
			contor++;																					// si il numaram la noduri selectate
		}
	}
	else
	{
		costul = new long long int[nrvf + 1];
		for (int i = 1; i <= nrvf; i++)
			costul[i] = LLONG_MAX - 1 - (rand()%RAND_MAX);
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
				const vecin vecinul = vecini[nod][i];
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

	for (int i = 2; i <= nrvf; i++)
	{
		if(vizitat[i])
			g << costul[i] << " ";
		else 
			g << "0 ";
	}
	delete[] vizitat;
	delete[]costul;
}

tmp_moneda
void graf<moneda>::bellmanFord(int nod_sursa)
{
	const long long int maximul = 922337203685477580;
	long long int *costul = new long long int[nrvf + 1];
	for (int i = 1; i <= nrvf; i++)
		costul[i] = maximul;
	costul[nod_sursa] = 0;
	std::queue<int> qnodes;
	qnodes.push(nod_sursa);
	bool* inCoada = new bool[nrvf + 1]{0}; 
	inCoada[nod_sursa] = true;
	int* nrRelaxari = new int[nrvf + 1]{ 0 };

	while(!qnodes.empty())
	{
		const int nod = qnodes.front();
		qnodes.pop();
		inCoada[nod] = false;
		for (unsigned int j = 0; j < vecini[nod].size(); j++)
		{
			const int vecinul = vecini[nod][j].index;
			const int cost_arc = vecini[nod][j].cost;

			if ( costul[nod] < maximul and costul[vecinul] > costul[nod] + cost_arc)
			{
				nrRelaxari[vecinul]++;
				if(vecinul == nod_sursa or nrRelaxari[vecinul] >= nrvf)
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

	for (int i = 2; i <= nrvf; i++)
		g << costul[i] << " ";
	delete[]nrRelaxari;
	delete[] inCoada;
	delete[] costul;
}

tmp_moneda
void graf<moneda>::TarjanTopologicBellmanFord(int nod_sursa)
{
	if (!areCosturi or !orientat)
	{
		std::cout << "\n   Muchiile din graful dat trebuie sa aiba cost si graful trebuie sa fie orientat pentru a rezolva aceasta problema astfel!";
		g << "\n   Muchiile din graful dat trebuie sa aiba cost si graful trebuie sa fie orientat pentru a rezolva aceasta problema astfel!";
		return;
	}

	typedef muchie< moneda > muchie;
	typedef std::vector< muchie > vmuchii;		// pt grafuri orientate se numesc arce, dar vom folosi tot termenul "muchii" pt consistenta

	// Obs: vom folosi echivalent termenii de subgraf si componenta tare conexa (ctc)
	vector_vectori subgrafuri = cadru_tareconexe();		// vector de vectori ce reprezinta nodurile componentelor tare conexe (ctc)
	int* careSubgraf = new int[nrvf + 1];							// array ce ne spune despre fiecare nod in care ctc este situat
	const size_t nr_subgrafuri = subgrafuri.size();			// nr ctc

	for (size_t i = 0; i < nr_subgrafuri; i++)						// i reprezinta ctc
		for (size_t j = 0; j < subgrafuri[i].size(); j++)			// al j-lea nod din ctc curenta
			careSubgraf[subgrafuri[i][j]] = i + 1;					// o sa indexam ctc-urile tot de la 1, cum avem mai toate grafurile indexate in program							
	
	// supergraful va avea ctc drept noduri si "muchii virtuale" drept muchii (provin din muchiile nodurilor din ctc diferite)
	vmuchii muchii_supergraf;															// muchiile virtuale din supergraf
	vmuchii* muchiiInterCTC = new vmuchii[nr_subgrafuri +1];			// muchiile reale din graful initial, ale nodurilor din ctc diferite
	vmuchii *muchii_subgrafuri = new vmuchii[nr_subgrafuri + 1];		// muchiile dintre nodurile aflate in aceeasi ctc

	for (size_t i = 1; i <= nr_subgrafuri; i++)										// i va fi index pentru muchii_supergraf si muchii_subgrafuri
	{
		const int idx_subgrafuri = i - 1;												// in vectorul subgrafuri, vectorii sunt indexati incepand de la 0
		for (size_t j = 0; j < subgrafuri[idx_subgrafuri].size(); j++)						
		{
			const int nod = subgrafuri[idx_subgrafuri][j];						// al j-lea nod din ctc i 
			for (size_t k = 0; k < vecini[nod].size(); k++)							// vecinii acestui nod
			{
				const vecin<moneda> vecinul = vecini[nod][k];
				const int sugbraf_vecinul = careSubgraf[vecinul.index];	// ctc a acestui vecin
				if (sugbraf_vecinul == i)													// daca nod si vecin sunt in aceeasi ctc
				{
					muchie aux(nod, vecinul.index, vecinul.cost);
					(muchii_subgrafuri[i]).push_back(aux);						// adaugam muchia la muchiile ctc i
				}
				else																					// daca nod si vecin sunt din alte ctc
				{
					muchie aux(i, sugbraf_vecinul, vecinul.cost);		
					muchii_supergraf.push_back(aux);								// adaugam muchia la muchiile dintre ctc-urile lui nod si vecin
					
					muchie punteInterCTC(nod, vecinul.index, vecinul.cost);
					muchiiInterCTC[i].push_back(punteInterCTC);			// retinem vecinii din alte ctc-uri pentru fiecare nod al grafului initial
				}
			}
		}
	}

	// vom construi un supergraf cu ctc pe post de noduri 
	// si subgrafurile ce contin doar nodurile si muchiile din acelasi ctc
	subgraf<moneda> supergraf(nr_subgrafuri, muchii_supergraf);
	
	int* v_ordineTopologica = new int[supergraf.get_nrvf() + 1] { 0 };	// intai sortam topologic supergraful
	supergraf.cadruSortareTopologica(v_ordineTopologica);
	for (int i = supergraf.get_nrvf(); i >= 1; i--)
	{
		const int idx_subgraf = v_ordineTopologica[i];
		const int nr_noduri_subgraf = subgrafuri[idx_subgraf - 1].size();		// folosim idx-1 aici pt ca array-ul subgrafuri este indexat de la zero 
		subgraf<moneda> subgraf_crt(nr_noduri_subgraf, muchii_subgrafuri[idx_subgraf]);
		// la constructorul celui de-al doilea subgraf, in ultimul for, ultima instructiune genereaza o eroare de scriere
		// trebuie sa repar acolo
		subgraf_crt.bellmanFord();
	}
		

	// inca in lucru
	delete[]v_ordineTopologica;
	delete []muchiiInterCTC;
	delete []muchii_subgrafuri;
	delete []careSubgraf;
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
#pragma endregion

template <typename moneda>
class subgraf : public graf<moneda>
{
	typedef std::vector< muchie<moneda> > vmuchii;
public:
	subgraf(int nrvf_param, vmuchii vector_muchii): graf<moneda>(nrvf_param)
	{
		// Obs: indexarea nodurilor incepe de la zero in supergraf si subgrafurile pt ctc
		// initializarea membrilor:
		graf<moneda>::nrmuchii = vector_muchii.size();
		const int nrmuchii = graf<moneda>::nrmuchii, nrvf = graf<moneda>::nrvf;

		for (int i = 0; i < nrmuchii; i++)
		{
			vecin<moneda> aux(vector_muchii[i].vf2, vector_muchii[i].cost);		// vecin pt nodul i, cu indexul si costul din vector_muchii[i]
			std::cout << vector_muchii[i] << " } ";
			graf<moneda>::vecini[i].push_back(aux);
		}
		std::cout << " |\n";
		/*for (int i = 0; i < nrvf; i++)
		{
			for (size_t j = 0; j < graf<moneda>::vecini[i].size(); j++)
				std::cout << graf<moneda>::vecini[i][j] << " | ";
			std::cout << "\n";
		}*/
	}
};

int main()
{
	start = 0;		// daca start e setat la zero, nu se va mai citi un nod de start
	
	//graf<> graful(false, false, false);
	//graful.verifvecini();

	//graf<> graful(false, false, false);
	//graful.cadru_biconexe();

	//	//// muchii critice:
	//graf<> graful(false, false, false);
	//vector_vectori* connections = new vector_vectori;
	//graful.copiaza_listeAdiacenta(*connections);
	//vector_vectori vector_afisare = graful.criticalConnections(graful.get_nrvf(), *connections);
	//for (int i = 0; i < vector_afisare.size(); i++)
	//	std::cout << vector_afisare[i][0] << " " << vector_afisare[i][1] << "\n";
	//delete connections;

	//graf<> graful(true, false, false);
	//graful.cadru_tareconexe();

	//std::cout << "\n\n Status final Havel Hakimi: " << havelHakimi() << "\n\n";

	//graf<> graful(true, false, false);
	//bool statusIsError = false;
	//int* v_ordineTopologica =  new int[ graful.get_nrvf() + 1]{ 0 };
	//graful.cadruSortareTopologica(v_ordineTopologica, statusIsError);
	//if(!statusIsError)
	//	for (int i = graful.get_nrvf(); i >= 1; i--)
	//		std::cout << v_ordineTopologica[i] << " ";
	//delete[]v_ordineTopologica;

	//graf<> graful(false, true, true);
	//graful.kruskal();

	//kruskal_paduri();

	//graf<> graful(false, true, false);
	//graful.prim();

	//graf<> graful(true, true, false);
	//graful.dijkstra();

	//graf<> graful(true, true, true);
	//graful.bellmanFord();

	graf<> graful(true, true, false);
	graful.TarjanTopologicBellmanFord();

	return 0;
}
