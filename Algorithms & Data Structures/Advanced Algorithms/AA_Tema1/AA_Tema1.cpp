#include <iostream>
#include <fstream>
#include <vector>

std::ifstream fin("date.in");
std::ofstream fout("date.out");

using std::vector;
using std::cout;


/// <summary>
///Suma maxima, dar care sa fie <=K, ce poate fi formata din elemente din S (numere intregi, pozitive, luate cel mult o singura data).
/// </summary>
/// <param name="xs">Vectorul de numere.</param>
/// <param name="k">Pragul maxim al sumei. </param>
/// <returns>Suma maxima. </returns>
int KnapsackA(vector<int> prices, int k)
{
	const int n = prices.size();
	const int sumLimit = (const int)(k + 1);
	int maxSum = 0;

	bool** sums = new bool*[n];
	for (int i = 0; i < n; i++)
		sums[i] = new bool[sumLimit];

	for (int i = 0; i < n; i++)
		for (int j = 1; j < sumLimit; j++)
			sums[i][j] = false;
	for (int i = 0; i < n; i++)
			sums[i][0] = true;

	for(int i = 1; i < n; i++)      // object prices index
		for (int partSum = 1; partSum < sumLimit; partSum++)  // partial sums
			if (sums[i - 1][partSum])
				sums[i][partSum] = true;
			else if(partSum >= prices[i] and sums[i - 1][partSum - prices[i]])
			{
				sums[i][partSum] = true;
				if (maxSum < partSum)
					maxSum = partSum;
			}

	return maxSum;
}

int KnapsackB(vector<int> prices, int k)
{
	int maxSum = 0;
	for (int i = 1; i < prices.size(); i++)
		if (maxSum + prices[i] <= k)
			maxSum += prices[i];
		else if (maxSum < prices[i] and prices[i] <=k)
			maxSum = prices[i];
	return maxSum;
}

int main()
{
	int k;
	fin >> k;
	vector<int> prices;
	prices.push_back(0);
	int price;
	while (fin >> price)
		prices.push_back(price);

	fout << KnapsackA(prices, k) << " ";
	fout << KnapsackB(prices, k);

	return 0;
}


