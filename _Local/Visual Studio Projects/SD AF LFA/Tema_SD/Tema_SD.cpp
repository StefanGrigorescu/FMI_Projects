#include <iostream>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <iomanip>

#define cout_std std::cout
#define  cin_std std::cin

//using namespace std;

std::ifstream f("DeSortat.txt");
std::ofstream g("iesire.txt");

/*void Citire(int &n, int &maxim)
{
    //cout_std<<"Cititi nr de numere de sortat: ";
    //cin_std>>n;

    f >> n;
    n--;
    //cout_std<<"\nCititi numerele: \n";
    int* v = new int[n + 1];
    maxim = -1;
    for (int i = 0; i <= n; i++)
    {
        //cin_std >> v[i];

        f >> v[i];
        if (maxim < v[i])
            maxim = v[i];
    }
}*/

void schimba(int& a, int& b)
{
    int aux = a;
    a = b;
    b = aux;
}

void intamplare(int v[],  int v0[], int vcpp[], int n, int& maxim, int maj, short int tip)
{

    for (int i = 0; i <= n; i++)
        switch (tip)
        {
            case 0:
            {
                // random
                v[i] = v0[i] = vcpp[i] = rand()% (maj+1);
                if (maxim < v[i])
                    maxim = v[i];
            }
            break;
            case 1:
            {
                // sortat cresc
                v[i] = vcpp[i];
            }
            break;
            case 2:
            {
                // sortat descresc 
                if (i == 0)
                    std::sort(vcpp, vcpp + n + 1, std::greater<int>());
                v[i] = v0[i] = vcpp[i];
            }
            break;
            case 3:
            {
                // aproape sortat 
                v[i] = v0[i] = vcpp[i];

                if (i % 3 == 0 and i % 2 != 0)
                {
                    schimba(vcpp[i], vcpp[i - 1]);
                    schimba(v[i], v[i - 1]);
                    schimba(v0[i], v0[i - 1]);
                }
                if (i % 13 == 0 and i % 3 != 0)
                {
                    schimba(vcpp[i], vcpp[i - 1]);
                    schimba(v[i], v[i - 1]);
                    schimba(v0[i], v0[i - 1]);
                }
                if(i%23 == 0 and i%3 != 0)
                {
                    schimba(vcpp[i], vcpp[i - 1]);
                    schimba(v[i], v[i - 1]);
                    schimba(v0[i], v0[i - 1]);
                }
                if(i%42 == 0 and i%5 != 0)
                {
                    schimba(vcpp[i], vcpp[i - 1]);
                    schimba(v[i], v[i - 1]);
                    schimba(v0[i], v0[i - 1]);
                }
            }
            break;
        }
}

void Afisare(int v[], int n)
{
    for (int i = 0; i <= n; i++)
        cout_std << v[i] << " "; 
}

bool AllRight(int v[], int vcpp[], int n)
{
    for (int i = 0; i <= n; i++)
        if (v[i] != vcpp[i])
            return false;
    return true;
}

void Restaurare(int v[], int v0[], int n)
{
    for (int i = 0; i <= n; i++)
        v[i] = v0[i];
}

void salvare(int v[], int n)
{
    for (int i = 0; i <= n; i++)
        g << v[i] << " ";

}

void BubbleSort(int v[],  int s, int d)
{
    for (int i = s; i <= d; i++)
        for (int j = 0; j <= d - i - 1 ; j++)
            if (v[j] > v[j+1])
                schimba(v[j],v[j+1]);
}

void CountSort(int v[], int n, int maxim)
{
    int *fr = new int[maxim+1];
    for (int i=0; i <=maxim; i++)
        fr[i] = 0;
    for (int i=0; i<=n; i++)
        fr[v[i]]++;
    int k = 0;
    for (int i = 0; i <=maxim; i++)
        for (int j = 1; j <= fr[i]; j++)
        {
            v[k] = i;
            k++;
        }
    delete []fr;
    // Afisarea s-a efectuat deja, nu mai apelam functia Afisare()
}

void RadixSort(int v[], int ordin, int s, int d)
{
    if (d - s + 1 >= 2)
    {
        int crt = s, capat=d, doi = 1 << ordin;
        while (crt <= capat)
        {
            if ((v[crt] & doi) > 0)
            {
                schimba(v[crt], v[capat]);
                capat--;
                crt--;
            }
            crt++;
        }
        if (ordin > 0)
        {
            RadixSort(v, ordin - 1, s, capat);
            RadixSort(v, ordin - 1, capat + 1, d);
        }
    }
}

void RadixSort_LSD(int v[], int ordin, int ordinmax, int n)
{
    int* setat = new int[n + 2];
    int* nesetat = new int[n + 2];
    int l0 = 0, l1 = 0, doi = 1 << ordin;
    for (int i = 0; i <= n; i++)
        if ((v[i] & doi) == 0)
        {
            l0++;
            nesetat[l0] = v[i];
        }
        else
        {
            l1++;
            setat[l1] = v[i];
        }
    for (int k0 = 1; k0 <= l0; k0++)
        v[k0 - 1] = nesetat[k0];
    for (int k1 = 1; k1 <= l1; k1++)
        v[l0 - 1 + k1] = setat[k1];
    delete[]setat;
    delete[]nesetat;
    if (ordin <= ordinmax)
        RadixSort_LSD(v, ordin + 1, ordinmax, n);
}

void Interclasare(int v[], int w[], int s, int d)
{
    int m = (s + d) / 2;
    int i1 = s;
    int i2 = m + 1;
    int k = 0;
    while (i1 <= m and i2 <= d)
    {
        if(v[i1]<=v[i2])
        {
            w[k] = v[i1];
            i1++;
        }
        else
        {
            w[k] = v[i2];
            i2++;
        }
        k++;
    }
    while (i1 <= m)
    {
        w[k] = v[i1];
        i1++;
        k++;
    }
    while (i2 <= d)
    {
        w[k] = v[i2];
        i2++;
        k++;
    }
    for (int i = s; i <= d; i++)
        v[i] = w[i - s];
}

void MergeSort(int v[], int w[], int s, int d)
{
    if (d - s +1 <= 2)
    {
        if (v[s] > v[d])
            schimba(v[s], v[d]);
    }
    else
    {
        int m = (s + d) / 2;
        MergeSort(v, w, s, m);
        MergeSort(v, w, m + 1, d);
        Interclasare(v, w, s, d);
    }
}

void invarte(int v[], int s, int d)
{
    for (int i = s; i <= (s + d) / 2; i++)
        schimba(v[i], v[d - i]);
}

int mediana(int v[], int s, int d, int mod)
{
    if (d - s + 1 < 5)
    {
        BubbleSort(v, s, d);
        return v[(s + d) / 2];
    }
    int *med = new int[d - s + 1];
    int l = 0;
    int med5[5];
    for (int i = s; i + 4 <= d; i += 5)
    {    
        for (int j = 0; j <= 4; j++)
            med5[j] = v[i + j];
        BubbleSort(med5, 0, 4);
        med[l] = med5[2];
        l++;
    }
    l--;

    int rezultat;
    if (mod == 3)
    {
        int* w = new int[l + 1];
        MergeSort(med, w, 0, l);
        delete[]w;

        rezultat = med[l/2];
    }
    else
        rezultat = mediana(med, 0, l, mod);
    
    delete[]med;
    return rezultat;
}

void QuickSort(int v[], int s, int d, int mod)      // p, pivotul vesel  
{
    if (d - s + 1 <= 1)
        return;
    
    int p = v[rand() % (d-s+1) + s];
    if(mod == 1 or mod == 3)
        p = mediana(v, s, d, mod);
    
    //cout_std << "p este " << p<<"\n";
    int i = s;
    int j = d;

    bool st_ord = 1, dr_ord = 1;
    bool st_desc = 1, dr_desc = 1;
    while (i <= j)
    {
        while (i <= j and v[i] < p)
        {
            if (i > s)
            {
                if (v[i] < v[i - 1])
                    st_ord = 0;
                else if (v[i] > v[i - 1])
                    st_desc = 0;
            }
            i++;
        }

        while (j >= i and v[j] > p)
        {
            if (j < d)
            {
                if (v[j] > v[j + 1])
                    dr_ord = 0;
                else if (v[j] < v[j + 1])
                    dr_desc = 0;
            }
            j--;
        }
            
        if (i < j)
        {
            schimba(v[j], v[i]);
            if (i > s)
            {
                if (v[i] < v[i - 1])
                    st_ord = 0;
                else if (v[i] > v[i - 1])
                    st_desc = 0;
            }
            if (j < d)
            {
                if (v[j] > v[j + 1])
                    dr_ord = 0;
                else if (v[j] < v[j + 1])
                    dr_desc = 0;
            }
            i++;
            j--;
        }

        else if (i == j)
        {
            if (v[i] <= p)
                i++;
            else
                j--;
        }
    }
    if (st_desc == 1)
        invarte(v, s, j);
    else if(st_ord == 0)
        QuickSort(v, s, j, mod);     // v[j] este ultimul elem mai mic sau egal cu pivotul
    
    if (dr_desc == 1)
        invarte(v, j + 1, d);
    else if(dr_ord == 0)
        QuickSort(v, j+1, d, mod);
}

/*void Meniu(int v[], int n, int maxim)
{
    short int opt;
    do
    {
        cout_std << "\nSelectati cerinta:\n1)Bubble Sort\n2)Count Sort\n3)Radix Sort\n4)Merge Sort\n5) Quick Sort\n\nAlegeti optiunea (numar de la 1 la 5): ";
        cin_std >> opt;
        switch (opt)
        {
        case 1:
        {
            //BubbleSort();
            BubbleSort(v, 0,n);
            Afisare(v, n);
        }
        break;
        case 2:
        {
            //CountSort();
            CountSort(v, n, maxim);
        }
        break;
        case 3:
        {
            //RadixSort();
            int ordin = -1, cmaxim = maxim;
            while (cmaxim)
            {
                cmaxim = cmaxim >> 1;
                ordin++;
            }
            if (ordin != -1)
                RadixSort(v, ordin, 0, n);
            Afisare(v, n);
        }
        break;
        case 4:
        {
            //MergeSort();
            int *w = new int[n+1];
            MergeSort(v, w, 0, n);
            delete []w;
            Afisare(v, n);
        }
        break;
        case 5:
        {
            //QuickSort();
            QuickSort(v, 0, n);
            Afisare(v, n);
        }
        break;
        }
    } while(opt < 1 or opt > 5);
}*/

void verif(int v[], int vcpp[], int n)
{
    cout_std << "\n"; Afisare(vcpp, n); cout_std << "\n"; Afisare(v, n); cout_std << "\n";
}

void Meniu(int v[], int v0[], int vcpp[], int n, int maxim, short int tip)
{
    using namespace std::chrono;
    std::cout << std::fixed << std::setprecision(9) << std::left;
    
#pragma region C++ VanillaSort
    auto start_vanilla = high_resolution_clock::now();
    std::sort(vcpp, vcpp + n+1);
    auto stop_vanilla = high_resolution_clock::now();
    duration<float> tspent_vanilla = stop_vanilla - start_vanilla;
    cout_std << "     Sortarea C++ default a sortat in timpul de " << tspent_vanilla.count()<< " secunde\n     ";
    //verif(v, vcpp, n);
#pragma endregion 

#pragma region BubbleSort
    if (n <= 50000)
    {
        
        auto start_bubble = high_resolution_clock::now();
        BubbleSort(v, 0, n);
        auto stop_bubble = high_resolution_clock::now();
        std::chrono::duration<float> tspent_bubble = stop_bubble - start_bubble;
        if (AllRight(v, vcpp, n) == 1)
            cout_std << "BubbleSort a sortat corect in timpul de " << tspent_bubble.count() << " secunde\n     ";
        //verif(v, vcpp, n);
        if (tip != 1)
            Restaurare(v, v0, n);
    }
    else
        cout_std << "BubbleSort nu a fost apelat; este prea incet pt dimensiunea datelor de intrare\n     ";
#pragma endregion

#pragma region CountSort
    auto start_count = high_resolution_clock::now();
    CountSort(v, n, maxim);
    auto stop_count = high_resolution_clock::now();
    std::chrono::duration<float> tspent_count = stop_count - start_count;
    if (AllRight(v, vcpp, n) == 1)
        cout_std << "CountSort a sortat corect in timpul de " << tspent_count.count() << " secunde\n     ";
    //verif(v, vcpp, n);
    if (tip != 1)
        Restaurare(v, v0, n);
#pragma endregion

#pragma region RadixSort_MSD
    auto start_radix = high_resolution_clock::now();
    int ordin = -1, cmaxim = maxim;
    while (cmaxim)
    {
        cmaxim = cmaxim >> 1;
        ordin++;
    }
    if (ordin != -1)
        RadixSort(v, ordin, 0, n);
    auto stop_radix = high_resolution_clock::now();
    std::chrono::duration<float> tspent_radix = stop_radix - start_radix;
    if (AllRight(v, vcpp, n) == 1)
        cout_std << "RadixSort_MSD a sortat corect in timpul de " << tspent_radix.count() << " secunde\n     ";
    //verif(v, vcpp, n);
    if (tip != 1)
        Restaurare(v, v0, n);
#pragma endregion

#pragma region RadixSort_LSD
    if (n <= 10000000)
    {
        auto start_radixL = high_resolution_clock::now();
        if (ordin != -1)
            RadixSort_LSD(v, 0, ordin, n);  // <- parametrii v, ordin, ordinmax, n 
        auto stop_radixL = high_resolution_clock::now();
        std::chrono::duration<float> tspent_radixL = stop_radixL - start_radixL;
        if (AllRight(v, vcpp, n) == 1)
            cout_std << "RadixSort_LSD a sortat corect in timpul de " << tspent_radixL.count() << " secunde\n     ";
        //verif(v, vcpp, n);
        if (tip != 1)
            Restaurare(v, v0, n);
    }
    else
        cout_std << "RadixSort_LSD nu a fost apelat; Dimensiunea datelor de intrare este prea mare\n     ";
#pragma endregion

#pragma region MergeSort

    auto start_merge = high_resolution_clock::now();
    int* w = new int[n + 1];
    MergeSort(v, w, 0, n);
    delete[]w;
    auto stop_merge = high_resolution_clock::now();
    std::chrono::duration<float> tspent_merge = stop_merge - start_merge;
    if (AllRight(v, vcpp, n) == 1)
        cout_std << "MergeSort a sortat corect in timpul de " << tspent_merge.count() << " secunde\n     ";
    //verif(v, vcpp, n);
    if (tip != 1)
        Restaurare(v, v0, n);
#pragma endregion

#pragma region QuickSorts

    int mod = 1; // modul intai de alegere a pivotului
    if (n <= 10000000)
    {
        //QuickSort cu pivotul mediana medianelor 
        auto start_quickmm = high_resolution_clock::now();
        QuickSort(v, 0, n, mod);
        auto stop_quickmm = high_resolution_clock::now();
        std::chrono::duration<float> tspent_quickmm = stop_quickmm - start_quickmm;
        if (AllRight(v, vcpp, n) == 1)
            cout_std << "QuickSort cu pivotul mediana medianelor a sortat corect in timpul de " << tspent_quickmm.count() << " secunde\n     ";
        //verif(v, vcpp, n);
        if (tip != 1)
            Restaurare(v, v0, n);
    }
    else
        cout_std << "QuickSort cu mediana medianelor nu a fost apelat; Dimensiunea datelor de intrare este prea mare\n     ";

    if (n <= 10000000)
    {
        //QuickSort cu pivotul random
        mod++; // modul al doilea de alegere a pivotului
        auto start_quickr = high_resolution_clock::now();
        QuickSort(v, 0, n, mod);
        auto stop_quickr = high_resolution_clock::now();
        std::chrono::duration<float> tspent_quickr = stop_quickr - start_quickr;
        if (AllRight(v, vcpp, n) == 1)
            cout_std << "QuickSort cu pivotul random a sortat corect in timpul de " << tspent_quickr.count() << " secunde\n     ";
        //verif(v, vcpp, n);
        if (tip != 1)
            Restaurare(v, v0, n);
    }
    else
        cout_std << "QuickSort cu pivot random nu a fost apelat; Dimensiunea datelor de intrare este prea mare\n     ";
    
    if (n <= 10000000)
    {
        //QuickSort cu pivotul mediana de 5
        mod++; // modul al treilea de alegere a pivotului
        auto start_quickm5 = high_resolution_clock::now();
        QuickSort(v, 0, n, mod);
        auto stop_quickm5 = high_resolution_clock::now();
        std::chrono::duration<float> tspent_quickm5 = stop_quickm5 - start_quickm5;
        if (AllRight(v, vcpp, n) == 1)
            cout_std << "QuickSort cu pivotul mediana de 5 a sortat corect in timpul de " << tspent_quickm5.count() << " secunde\n      ";
        //verif(v, vcpp, n);
    }
    else
        cout_std << "QuickSort cu pivot mediana de 5 nu a fost apelat; Dimensiunea datelor de intrare este prea mare\n     ";
#pragma endregion

    cout_std << "\n";
}

int main()
{
    srand(time(NULL));

    int n, maxim = -1, maj;
    f >> n>>maj;
    n--;
    int* v = new int[n + 1];
    int* v0 = new int[n + 1];
    int* vcpp = new int[n + 1];
    for (short int tip = 0; tip <= 3; tip++)
    {
        intamplare(v, v0, vcpp, n, maxim, maj, tip);

        if (tip == 0)
            cout_std << "\nDimensiune date intrare = " << n + 1 << ";   Val maxima = " << maxim << "\nPentru alti parametrii, modificati fisierul de intrare ''De sortat.txt''\n\n     ";

        switch (tip)
        {
        case 0:
            cout_std << "\nTest 0) Pt elemente in ordine aleatorie:\n";
            break;
        case 1:
            cout_std << "\nTest 1) Pt elemente sortate crescator:\n";
            break;
        case 2:
            cout_std << "\nTest 2) Pt elemente sortate descrescator:\n";
            break;
        case 3:
            cout_std << "\nTest 3) Pt elemente aproape sortate (crescator):\n";
            break;
        }
        
        Meniu(v, v0, vcpp, n, maxim, tip);
    }
    delete[]v;
    delete[]v0;
    delete[]vcpp;
    return 0;
}

