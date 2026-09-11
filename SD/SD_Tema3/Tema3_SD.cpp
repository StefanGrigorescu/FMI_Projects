#include <iostream>
#include <fstream>

std::ifstream f("Treap.in");
//std::ofstream g("Treap.out");

struct nod
{
	int key;
	int priority;
	nod* left;
	nod* right;

	nod() 
	{
		left = nodgol;
		right = nodgol;
		key = 404;
		priority = -404;
	}
	nod(int cheie, int prioritate, nod* stang, nod* drept)
	{
		key = cheie;
		priority = prioritate;
		left = stang;
		right = drept;
	}
	nod* operator = (nod* sursa)
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
} *R, *nodgol;

nod* cauta(nod* crt, int cheie_cautata)
{
	if (crt == nodgol)
		return nodgol;

	if (cheie_cautata == crt->key)
		return crt;
	if (cheie_cautata < crt->key)
		return cauta(crt->left, cheie_cautata);
	return cauta(crt->right , cheie_cautata);
}

void rotire_st(nod* &crt)
{
	nod* stangul = crt->left;
	crt->left = stangul->right;
	stangul->right = crt;
	crt = stangul;
}

void rotire_dr(nod* &crt)
{
	nod* dreptul = crt->right;
	crt->right = dreptul->left;
	dreptul->left = crt;
	crt = dreptul;
}

void balans(nod*& crt)
{
	if (crt->left->priority > crt->priority)
		rotire_st(crt);
	else if (crt->right->priority > crt->priority)
		rotire_dr(crt);
}

void inserare(nod*& crt, int cheie, int prioritate)
{
	if (crt == nodgol)
	{
		crt = new nod(cheie, prioritate, nodgol, nodgol);
		return;
	}
	if (cheie < crt->key)
		inserare(crt->left, cheie, prioritate);
	else if ( cheie > crt->key)
		inserare(crt->right, cheie, prioritate);

	balans(crt);
}

void stergere(nod*& crt, int cheie)
{
	if (crt == nodgol)
		return;

	if (cheie < crt->key)
		stergere(crt->left, cheie);
	else if (cheie > crt->key)
		stergere(crt->right, cheie);
	else
	{
		if (crt->left == nodgol and crt->right == nodgol)
		{
			delete crt;
			crt = nodgol;
		}
		else
		{
			if (crt->left->priority > crt->right->priority)
				rotire_st(crt);
			else
				rotire_dr(crt);
			stergere(crt, cheie);
		}
	}
}

const int infinit = 2e9;
void split(nod* &root, nod* &root_left, nod* &root_right, int cheie)
{
	inserare(root, cheie, infinit);
	root_left = root->left;
	root_right = root->right;
	delete root;
	root = nodgol;
}

void join(nod*& root, nod*& root_left, nod*& root_right, int cheie)
{
	root = new nod(cheie, 0, root_left, root_right);
	stergere(root, root->key);
}

void afis_elem_sortate(nod* crt)
{
	if (crt == NULL)
		return;
	if (crt->left != nodgol)
		afis_elem_sortate(crt->left);
	crt->afisare();
	if (crt->right != nodgol)
		afis_elem_sortate(crt->right);
}

void succesor(const nod* crt, int cheie, char poz_relativa, int &result, int &countdown)
{
	if (countdown == 2)		//   coboram sa cautam cheia / urcam inapoi sa luam primul nod care a coborat pe fiul stang
	{
		if (crt->key != cheie)
		{
			if (crt->left != nodgol and cheie < crt->key)
			{
				poz_relativa = 's';
				succesor(crt->left, cheie, poz_relativa, result, countdown);
			}
			else if (crt->right != nodgol and crt->key < cheie)
			{
				poz_relativa = 'd';
				succesor(crt->right, cheie, poz_relativa, result, countdown);
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
			succesor(crt->right, cheie, poz_relativa, result, countdown);
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
			succesor(crt->left, cheie, poz_relativa, result, countdown);
	}
}

void predecesor(const nod* crt, int cheie, char poz_relativa, int& result, int& countdown)
{
	if (countdown == 2)		//   coboram sa cautam cheia / urcam inapoi sa luam primul nod care a coborat pe fiul drept
	{
		if (crt->key != cheie)
		{
			if (crt->left != nodgol and cheie < crt->key)
			{
				poz_relativa = 's';
				predecesor(crt->left, cheie, poz_relativa, result, countdown);
			}
			else if(crt->right != nodgol and crt->key < cheie)
			{
				poz_relativa = 'd';
				predecesor(crt->right, cheie, poz_relativa, result, countdown);
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
			predecesor(crt->left, cheie, poz_relativa, result, countdown);
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
			predecesor(crt->right, cheie, poz_relativa, result, countdown);
	}
}

int main()
{
	R = nodgol = new nod(0, 0, NULL, NULL);

	int nr_init_noduri;
	f >> nr_init_noduri;

	for (int i = 0; i < nr_init_noduri; i++)
	{
		int cheie, prioritate;
		f >> cheie >> prioritate;

		inserare(R, cheie, prioritate);
	}

	//stergere(R, 6);
	//std::cout << "\n";
	//afis_elem_sortate(R);

	//int cheia = 13, succesorul = -1, countdown = 2;
	//if(cauta(R, cheia) != nodgol)
	//	succesor(R, cheia, 'x', succesorul, countdown);
	//std::cout << "\n\n      succesorul lui " << cheia << " este " << succesorul << "\n\n";

	//int cheia = 17, predecesorul = -1, countdown = 2;
	//if (cauta(R, cheia) != nodgol)
	//	predecesor(R, cheia, 'x', predecesorul, countdown);
	//std::cout << "\n\n      predecesorul lui " << cheia << " este " << predecesorul << "\n\n";

	return 0;
}