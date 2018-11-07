#include <vector>
#include <algorithm>
#include <set>
#include <ctime>
#include <iterator>
#include <iostream>
#include <experimental/filesystem>

using namespace std;



typedef vector <pair<int, pair<int, int>>> graphE;
typedef vector <pair<int, pair<int, int>>> eVec;
typedef int** matrix_t;

int dsu_get(int v, vector<int> &p)
{
	return (v == p[v]) ? v : (p[v] = dsu_get(p[v], p));
}

void dsu_unite(int a, int b, vector<int> &p)
{
	a = dsu_get(a, p);
	b = dsu_get(b, p);

	if (a != b)
		p[a] = b;
}

graphE getGraphE(matrix_t &matrix, const int n)
{
	graphE g;
	for (int i = 0; i < n; i++)
		for (int j = i; j < n; j++)
		{
			if(matrix[i][j] == 0)
				continue;

			g.emplace_back(matrix[i][j], make_pair(i, j));
		}

	return g;
}

matrix_t getMatrix(const vector <pair<int, pair<int, int>>> &graph, int n)
{
	int** matrix = new int*[n];
	for (int i = 0; i < n; i++)
	{
		matrix[i] = new int[n];
		memset(matrix[i], 0, n);
	}

	for (const pair<int, pair<int, int>> p : graph)
		matrix[p.second.first][p.second.second] = p.first;

	return matrix;
}

void displayMatrix(int** matrix, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			cout << matrix[i][j] << "\t";

		cout << endl;
	}
}

matrix_t initMatrix(int n)
{
	matrix_t matrix = new int*[n];
	for (int i = 0; i < n; i++)
	{
		matrix[i] = new int[n];
		memset(matrix[i], 0, n);
	}

	return  matrix;
}

void setMatrix(matrix_t &matrix, int n)
{
	
	for (int i = 0; i < n; i++)
	{
		for (int j = i; j < n; j++)
		{
			matrix[i][j] = rand() % 1337;
			if (i == j)
				matrix[i][j] = 0;

			matrix[j][i] = matrix[i][j];
		}
	}
}

eVec krusk(graphE g, int n, int &total)
{
	
	int cost = 0;
	eVec res;
	vector<int> p;
	sort(g.begin(), g.end());

	p.resize(n);

	for (int i = 0; i < n; ++i)
		p[i] = i;
	for (size_t i = 0; i < g.size(); ++i)
	{
		int a = g[i].second.first;
		int b = g[i].second.second;
		int l = g[i].first;

		if (a == b)
			continue;


		if (dsu_get(a, p) != dsu_get(b, p))
		{
			cost += l;
			res.push_back(g[i]);
			dsu_unite(a, b, p);
		}
	}
	total = cost;
	return res;
}


void dfs(const int v, vector<bool> &visited, int** matrix, const int n, vector<int> &result)
{
	visited[v] = true;
	result.push_back(v);
	for (int i = 0; i < n; i++)
		if (!visited[i] && matrix[v][i] != 0)
			dfs(i, visited, matrix, n, result);
}

graphE getCompE(int** matrix, const vector<int>& component)
{
	graphE result;
	for (size_t i = 0; i < component.size(); i++)
		for (size_t j = i; j < component.size(); j++)
		{
			if (matrix[i][j] == 0)
				continue;

			result.push_back(make_pair(matrix[i][j], make_pair(i, j)));
		}

	return result;
}

vector<graphE> findComp(int** matrix, int n)
{
	vector<graphE> result;

	vector<int> comp;

	vector<bool> visited(n, false);
	
	for(int i = 0; i < n; i++)
	{
		if(!visited[i])
		{
			comp.clear();
			dfs(i, visited, matrix, n, comp);
			result.push_back(getCompE(matrix, comp));
		}
	}

	return result;
}



int main()
{
	cout << "Cource MST <created by shockbyte>" << endl;
	srand(static_cast<int>(time(nullptr)));

	int n;
	n = 246 + rand() % 300;
	
	matrix_t matrix = initMatrix(n);
	setMatrix(matrix, n);

	cout << "vertices count->" << n << endl;
	cout << "analysis..." << endl;

	vector<graphE> gList = findComp(matrix, n);
	if(gList.size() > 1 )
	{
		cout << "disconnected graph!" << endl;
		cout << "connected components ->" << gList.size() << endl;
	}
	else
		cout << "connected graph!" << endl;

	for(size_t i = 0; i < gList.size(); i++)
	{
		int len = 0;
		cout << "edges [" << i << "]" << endl;
		eVec v = krusk(gList[i], n, len);
		for(pair<int, pair<int, int>> p : v)
			cout << "(" << p.second.first << ", " << p.second.second << ") [" << p.first << "]" << endl;

		cout << "Total lenght->" << len << endl << endl;  
	}
	return 0;
}