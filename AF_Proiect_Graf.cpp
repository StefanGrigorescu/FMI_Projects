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
//#include <utility>

std::ifstream f("graful.in");
std::ofstream g("graful.out");
//std::ifstream f("bellmanford.in");
//std::ofstream g("bellmanford.out");


int start = 0, contor;   // contorul il vom folosi in mai multe probleme;  start va aparea eventual ca nod de start
const int nodgol_int = 0;

#define tmp_moneda template <typename moneda>

template <typename moneda = int>
class graf;
template <typename moneda = int>
class subgraf;

#pragma region vecin
template <typename moneda = int>
class vecin;
template <typename moneda = int>
std::ostream& operator<< (std::ostream& out, const vecin<moneda>& vecinul);

template <typename moneda>
class vecin
{
public:
	int index;  // eticheta sa
	moneda cost;	 // costul muchiei catre acest vecin

	vecin(int idx = 0, moneda costul = 0) : index(idx), cost(costul) {}
	friend std::ostream& operator<< <moneda>(std::ostream& out, const vecin<moneda>& vecinul);
};
template <typename moneda>
std::ostream& operator<< (std::ostream& out, const vecin<moneda>& vecinul)
{
	out << vecinul.index << " " << vecinul.cost;
	return out;
}
#pragma endregion

#pragma region muchie
// declar prin antet clasa si operatorii ce ii vor fi prieteni: 
template <typename moneda = int>
class muchie;

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

	muchie(int varf1 = 0, int varf2 = 0, moneda costul = 0) : vf1(varf1), vf2(varf2), cost(costul) {}
	muchie(const muchie < moneda >& sursa) : vf1 (sursa.getvf1() ), vf2(sursa.getvf2()), cost(sursa.getcost()) {}


	void setAll(int varf1, int varf2, moneda costul)
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

	bool operator == (const muchie<moneda> &m) const
	{
		return (vf1 == m.getvf1() and vf2 == m.getvf2() and cost == m.cost);
	}
	bool operator < (const muchie<moneda> &m) const
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
	bool operator > (const muchie<moneda> &m) const
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

	friend std::ostream &operator<< <moneda>(std::ostream &out, const muchie<moneda>& m);		// aici specific ca operatorii mai sus declarati sunt prieteni
	
	friend std::ofstream &operator<< <moneda>(std::ofstream &out, const muchie<moneda>& m);
};

tmp_moneda			// si aici specific implementarea operatorilor prieten
std::ostream& operator<< (std::ostream &out, const muchie<moneda> &m)
{
	out << m.vf1 << " " << m.vf2 << " " << m.cost;
	return out;
}
tmp_moneda
std::ofstream& operator<< (std::ofstream &out, const muchie<moneda> &m)
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
#define vector_vecini std::vector < vecin<moneda> >
#define vmuchii std::vector< muchie< moneda> >

template <typename moneda>
class graf
{
protected:
	const bool orientat;
	const bool areCosturi;
	const bool areListaMuchii;
	bool areMatPonderi;

	int nrvf, nrmuchii;
	vector_vecini* vecini;

	muchie<moneda>* lista_muchii;
	int size_lista_muchii;
	bool lista_muchii_sortata;

	void DFS(int nod, bool* viz, int* ordineVizita, int& idxOrdineVizita);
	void biconexe(int nod, int tata, bool* viz, multime* wayback, stackpair& muchiiviz, vector_vectori& comp_biconexe);
	void tareconexe(int nod, int& freeorder, int* order, int* leastbackorder, bool* pestiva, std::stack<int>& noduriviz, vector_vectori& comp_tareconexe);
	void mCrits(int nod, int tata, bool* viz, multime* wayback, stackpair& muchiiviz, vector_vectori& connections, vector_vectori& solutia);
	void DFS_SortareTopologica(int nod, bool* viz, int*& finished, int& idxfinished);
	//void bellmanFordPartial(long long int*& costul, std::map<int, moneda> &actualizariCost, bool& areCicluNegativ = false, int nod_sursa = 1);

	graf(int nrvf_param);		// doar pt declararea unor subgrafuri
public:
	graf(bool orientat_param = true, bool areCosturi_param = true, bool areListaMuchii_param = true, bool areMatPonderi = false, int nrvf_param = -1, int nrmuchii_param = -1);

	int get_nrvf() { return nrvf; }
	void copiaza_listeAdiacenta(vector_vectori& solution);
	void verifvecini();

	int* BFS();
	int* cadruDFS();
	vector_vectori cadru_biconexe();
	vector_vectori cadru_tareconexe(bool statusIsError = false);
	vector_vectori criticalConnections(int n, vector_vectori& connections);
	void cadruSortareTopologica(int*& finished, bool statusIsError = false);

	void kruskal(muchie<moneda> *&muchiiAPM, moneda &cost_apm = 0, bool statusIsError = false);
	void prim(muchie<moneda> *&muchiiAPM, moneda &cost_apm = 0, bool statusIsError = false);
	void dijkstra(long long int *&costul, int nod = 1);
	void bellmanFord(long long int *&costul, bool& areCicluNegativ = false, int nod_sursa = 1);
	void TarjanTopologicBellmanFord(long long int*& costul, bool& areCicluNegativ = false, int nod_sursa = 1);
	
	void royFloyd(long long int**&costuri, bool statusIsError = false);

	~graf()
	{
		delete[]vecini;
		if (lista_muchii)
			delete[] lista_muchii;
	}
};

#pragma region graf & auxiliare
tmp_moneda
graf<moneda>::graf(bool orientat_param, bool areCosturi_param, bool areListaMuchii_param, bool areMatPonderi_param, int nrvf_param, int nrmuchii_param) : orientat(orientat_param), areCosturi(areCosturi_param), areListaMuchii(areListaMuchii_param), areMatPonderi(areMatPonderi_param), nrvf(nrvf_param), nrmuchii(nrmuchii_param)
{
	if (!areMatPonderi)
	{
		if(nrvf < 0)
			f >> nrvf;
		if(nrmuchii < 0)
			f >> nrmuchii;
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
	else
	{
		f >> nrvf;
		nrmuchii = nrvf * nrvf;

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

		int idxListaMuchii = 0;

		vecini = new vector_vecini[nrvf + 1];
		for (int i = 1; i <= nrvf; i++)
			for (int j = 1; j <= nrvf; j++)
			{
				moneda cost;
				f >> cost;
				vecin<moneda> aux(j, cost);
				vecini[i].push_back(aux);

				if (areListaMuchii)
				{
					lista_muchii[idxListaMuchii].setAll(i, j, cost);
					idxListaMuchii++;
				}
			}
	}
}
tmp_moneda
graf<moneda>::graf(int nrvf_param) : orientat(true), areCosturi(true), areListaMuchii(false), areMatPonderi(false), lista_muchii(NULL), size_lista_muchii(0), lista_muchii_sortata(false)
{
	nrvf = nrvf_param;
	nrmuchii = 0;		// se va modifica direct in constructorul clasei subgraf
	//vecini = new vector_vecini[nrvf + 1];
	vecini = NULL;	// vom folosi un map cu liste de adiacenta ca sa ne ocupam de mai putine noduri intr-un subgraf
}

tmp_moneda
void graf<moneda>::verifvecini()
{
	for (int i = 1; i <= nrvf; i++)
	{
		std::cout << "\n  vecinii lui " << i << " :  ";
		for (unsigned int j = 0; j < vecini[i].size(); j++)
			std::cout << vecini[i][j].index << " (" << vecini[i][j].cost << " $) | ";
	}
}

tmp_moneda
int* graf<moneda>::BFS()
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
		dist[i]--;

	return dist;
}

tmp_moneda
void graf<moneda>::DFS(int nod, bool* viz, int* ordineVizita, int &idxOrdineVizita)
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
vector_vectori graf<moneda>::cadru_tareconexe(bool statusIsError)
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
void graf<moneda>::cadruSortareTopologica(int*& finished, bool statusIsError)
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
void graf<moneda>::kruskal(muchie<moneda> *&muchiiAPM, moneda &cost_apm, bool statusIsError)
{
	if (!areCosturi)
	{
		std::cout << "\n   Muchiile din graful dat trebuie sa  pentru a rezolva aceasta problema";
		g << "\n   Muchiile din graful dat trebuie sa  pentru a rezolva aceasta problema";
		statusIsError = true;
		return;
	}
	
	if (! lista_muchii_sortata)
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
			muchiiAPM[ contor ] = lista_muchii[i];
			cost_apm += muchiiAPM[ contor ].cost;

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
void graf<moneda>::prim(muchie<moneda>*& muchiiAPM, moneda &cost_apm, bool statusIsError)
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
void graf<moneda>::dijkstra(long long int *&costul, int nod)
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
void graf<moneda>::bellmanFord(long long int *&costul, bool& areCicluNegativ, int nod_sursa)
{
	const long long int maximul = 922337203685477580;
	costul = new long long int[nrvf + 1];
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
			const moneda cost_arc = vecini[nod][j].cost;

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

	delete[]nrRelaxari;
	delete[] inCoada;
}

tmp_moneda
void setToMax(moneda &inf)
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
void graf<moneda>::royFloyd(long long int**& costuri, bool statusIsError)
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
				if ( i != j  and ((costuri[i][j] == 0 or costuri[i][j] > costuri[i][k] + costuri[k][j]) and costuri[i][k] != 0 and costuri[k][j] != 0))
				{
					costuri[i][j] = costuri[i][k] + costuri[k][j];
					//std::cout << "costuri["<<i<<"]["<<j<<"] a fost actualizat la " << costuri[i][j] << "\n";
				}
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
	std::map<int, vector_vecini > listeVecini;
public:

	subgraf(const int &nrvf_param, const std::vector<int> &vector_noduri,const vmuchii &vector_muchii): graf<moneda>(nrvf_param)
	{
		// Obs: indexarea nodurilor incepe de la 1 (nodul 0 este ignorat)
		// initializarea membrilor:
		graf<moneda>::nrmuchii = vector_muchii.size();
		const int nrmuchii = graf<moneda>::nrmuchii, nrvf = graf<moneda>::nrvf;
		
		for (int i = 0; i < nrmuchii; i++)
		{
			const int nod = vector_muchii[i].vf1;
			vecin<moneda> aux( vector_muchii[i].vf2, vector_muchii[i].cost );		// vecin pt nod, cu indexul si costul din vector_muchii[i]
			//graf<moneda>::vecini[nod].push_back(aux);
			if (listeVecini.find(nod) == listeVecini.end())
			{
				vector_vecini listaVecini;
				listeVecini.insert(std::pair<int, vector_vecini>( nod, listaVecini ));
			}
			listeVecini[nod].push_back(aux);
		}
		//for (auto i = listeVecini.begin; i != listeVecini.end(); i++)
		//{
		//	std::cout << " vecini lui i : ";
		//	for (int j = 0; j <= listeVecini[i].size; j++)
		//		std::cout << listeVecini[i][j].index << " (" << listeVecini[i][j].cost << ")  | ";
		//	std::cout << "\n";
		//}

		/*std::cout << "vecinii lui 0 sunt: ";
		for (int j = 0; j < graf<moneda>::vecini[0].size(); j++)
			std::cout << graf<moneda>::vecini[0][j] << " | ";
		std::cout << " \n";
		graf<moneda>::verifvecini();
		std::cout << "\n";*/
	}

	void bellmanFordPartial(std::map<int, moneda>& actualizariCost, bool& areCicluNegativ = false, int nod_sursa = 1);
};
tmp_moneda
void subgraf<moneda>::bellmanFordPartial(/*long long int*& costul, */std::map<int, moneda>& actualizariCost, bool& areCicluNegativ, int nod_sursa)
{
	const long long int maximul = 922337203685477580;
	//costul = new long long int[nrvf + 1];
	//for (int i = 1; i <= nrvf; i++)
		//costul[i] = maximul;
	actualizariCost.insert(std::pair<int, moneda>(nod_sursa, 0));
	//costul[nod_sursa] = 0;

	std::queue<int> qnodes;
	qnodes.push(nod_sursa);
	//bool* inCoada = new bool[nrvf + 1]{ 0 };
	std::map<int, bool> inCoada;
	inCoada.insert(std::pair<int, bool>(nod_sursa, true));
	inCoada[nod_sursa] = true;
	//int* nrRelaxari = new int[nrvf + 1]{ 0 };
	std::map<int, int> nrRelaxari;

	while (!qnodes.empty())
	{
		const int nod = qnodes.front();
		qnodes.pop();
		inCoada[nod] = false;
		for (size_t j = 0; j < listeVecini[nod].size(); j++)
			//unsigned int j = 0; j < vecini[nod].size(); j++)
		{
			const int vecinul = listeVecini[nod][j].index;
			const moneda cost_arc = listeVecini[nod][j].cost;

			if (actualizariCost.find(vecinul) == actualizariCost.end())
				actualizariCost.insert(std::pair<int, moneda>(vecinul, maximul));
			int costNod = actualizariCost[nod], costVecinul = actualizariCost[vecinul];

			if (costNod < maximul and costVecinul > costNod + cost_arc)
			{
				if (nrRelaxari.find(vecinul) == nrRelaxari.end())
					nrRelaxari.insert(std::pair<int, int>(vecinul, 1));
				else
					nrRelaxari[vecinul]++;
				if (vecinul == nod_sursa or nrRelaxari[vecinul] >= graf<moneda>::nrvf)
				{
					areCicluNegativ = true;
					g << "Ciclu negativ!";
					std::cout << "Ciclu negativ!\n";
					//delete[]nrRelaxari;
					//delete[] inCoada;
					//delete[] costul;
					return;
				}
				if (inCoada.find(vecinul) == inCoada.end())
				{
					qnodes.push(vecinul);
					inCoada.insert(std::pair<int, bool>(vecinul, true));
					//inCoada[vecinul] = true;
				}
				else if (!inCoada[vecinul])
				{
					qnodes.push(vecinul);
					inCoada[vecinul] = true;
				}
				actualizariCost[vecinul] = costNod + cost_arc;
				//costul[vecinul] = costul[nod] + cost_arc;

				//if (actualizariCost.find(vecinul) != actualizariCost.end())   // daca am inregistrat deja o actualizare, acum o modificam doar
				//	actualizariCost[vecinul] = costul[vecinul];
				//else
				//	actualizariCost.insert(std::pair< int, moneda >(vecinul, costul[vecinul]));
			}
		}
	}

	//delete[]nrRelaxari;
	//delete[] inCoada;
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

	vector_vectori vCTC = cadru_tareconexe();	     // vector de vectori ce reprezinta nodurile componentelor tare conexe (CTC)
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

	std::vector<int> vfSupergraf;
	for (size_t i = 0; i <= nrCTC; i++)
		vfSupergraf.push_back(i);
	
	subgraf<moneda> supergraf(nrCTC + 1, vfSupergraf, muchiiSupergraf);

	int* vOrdineTopologica = new int[nrCTC + 1]{ 0 };          // intai sortam topologic supergraful
	supergraf.cadruSortareTopologica(vOrdineTopologica);

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
			const int nrVfCTC = 1 + vCTC[idxCTC].size();
			subgraf<moneda> CTC_crt(nrVfCTC, vCTC[idxCTC], muchiiCTC[idxCTC]);

			//long long int* costul_CTC_crt = NULL;  // folosim alt array de cost pentru a nu se suprascrie mereu cu LLONG_MAX
			std::map<int, moneda> actualizariCost;
			CTC_crt.bellmanFordPartial(/*costul_CTC_crt, */actualizariCost, areCicluNegativ, nod_sursa_local);   // folosim acelasi bool
			//if (costul_CTC_crt)
			//{
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

				//delete[]costul_CTC_crt;
			}

			if (areCicluNegativ)
			{
				delete[] vOrdineTopologica;
				delete[] muchiiCTC;
				delete[] muchiiInterCTC;
				delete[] careCTC;
				return;
			}
			//}
		}
	}

	delete[] vOrdineTopologica;
	delete[] muchiiCTC;
	delete[] muchiiInterCTC;
	delete[] careCTC;
}


int main()
{
	start = 0;		// daca start e setat la zero, nu se va mai citi un nod de start

	///		/// verif vecini
	/*graf<> graful(false, false, false);
	graful.verifvecini();*/

	///		/// BFS
	/*start = 1;
	graf<> graful(true, false, false);
	int* dist = graful.BFS();
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
	vector_vectori comp_tareconexe = graful.cadru_tareconexe();
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
	graful.cadruSortareTopologica(v_ordineTopologica);
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
	graful.kruskal(muchiiAPM, cost_apm);
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
	graful.prim(muchiiAPM, cost_apm);
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
	graf<> graful(true, true, false);
	long long int* costul = NULL;
	bool areCicluNegativ = false;
	graful.TarjanTopologicBellmanFord(costul, areCicluNegativ);
	if (costul)
	{
		if (!areCicluNegativ)
			for (int i = 2; i <= graful.get_nrvf(); i++)
				g << costul[i] << " ";

		delete[]costul;
	}

	///	/// roy-floyd
	/*graf<> graful(true, true, false, true);
	long long int** costuri = NULL;
	graful.royFloyd(costuri);
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
	graf<> graful(false, false, false, false, nrvf, nrvf-1);
	start = 1 +  rand()%graful.get_nrvf();		// incepem cautarea dintr-un nod oarecare; este important sa setam acest start DUPA ce am initializat graful, altfel se va citi incorect (vezi conditie citire nod start in constructor)
	int* dist = graful.BFS();
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

		dist = graful.BFS();		// mai parcurgem odata cu un BFS din radacina 
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

	return 0;
}
