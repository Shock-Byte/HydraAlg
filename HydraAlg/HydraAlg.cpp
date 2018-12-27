#include <vector>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <regex>
#include <Windows.h>
#include <conio.h>
#include "resource.h"
using namespace std;

//O(MlogN + M + N)

#define MAX(a, b, c) (((a) >= (b) && (a) >= (c)) ? (a) : (((b) >= (a) && (b) >= (c)) ? (b) : (c)))

typedef pair<int, pair<int, int>> eComp;
typedef vector <eComp> graphE;
typedef int** matrix_t;

vector<string> split(const string& input, const string& delim)
{
	regex re(delim);
	sregex_token_iterator
		first{ input.begin(), input.end(), re, -1 },
		last;
	return { first, last };
}

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


void displayMatrix(int** matrix, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			cout << matrix[i][j] << "\t";

		cout << endl;
	}

	cout << endl;
}

matrix_t initMatrix(int n)
{
	matrix_t matrix = new int*[n];
	for (int i = 0; i < n; i++)
	{
		matrix[i] = new int[n];
		memset(matrix[i], 0, n * sizeof(int));
	}

	return  matrix;
}

void setMatrix(matrix_t &matrix, int n)
{
	
	for (int i = 0; i < n; i++)
	{
		for (int j = i; j < n; j++)
		{
			int lucky = ((rand() % 10) % 2 == 0);
			if (!lucky)
				matrix[i][j] = 0;
			else
				matrix[i][j] = ((rand() % 2000) + 1);

			if (i == j)
				matrix[i][j] = 0;

			matrix[j][i] = matrix[i][j];
		}
	}
}

void setMatrix(const graphE& graph, matrix_t& matrix)
{
	for(eComp e: graph)
	{
		matrix[e.second.first][e.second.second] = e.first;
	
		matrix[e.second.second][e.second.first] = e.first;
	}
}

graphE krusk(graphE g, int n, int &total)
{
	
	int cost = 0;
	graphE res;
	vector<int> p(n);

	sort(g.begin(), g.end());

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
			if (matrix[component[i]][component[j]] == 0)
				continue;

			result.push_back(make_pair(matrix[component[i]][component[j]], make_pair(component[i], component[j])));
		}

	return result;
}

vector<graphE> findComp(int** matrix, int n)
{
	vector<graphE> result;


	vector<bool> visited(n, false);
	
	for(int i = 0; i < n; i++)
	{
		if(!visited[i])
		{
			vector<int> comp;
			dfs(i, visited, matrix, n, comp);
			result.push_back(getCompE(matrix, comp));
		}
	}

	return result;
}

void showLogo()
{
	HMODULE hModule = GetModuleHandle(NULL);
	HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(IDR_HTML1), RT_HTML);
	HGLOBAL hMemory = LoadResource(hModule, hResource);
	DWORD dwSize = SizeofResource(hModule, hResource);
	LPVOID lpAddress = LockResource(hMemory);


	for(DWORD i  = 0; i < dwSize; i++)
		cout << static_cast<char*>(lpAddress)[i];

}

int main()
{
	SetConsoleTitle(L"LOL KEK KURSACH!!!");
	srand(static_cast<int>(time(nullptr)));

	showLogo();

	cout << endl << "Cource MST <created by shockbyte>" << endl;

	matrix_t matrix;

	string data = "XXX";
	int n = 0;

	graphE buff;
	vector<string> sBuf;
	cout << "Do you want run stress test? Y/N: ";
	int yn = _getch();
	switch(yn)
	{
	case 'y':
	case 'Y':
		n = 1337;
		matrix = initMatrix(n);
		setMatrix(matrix, n);
		break;

	case 'n':
	case 'N':
		cout << endl << endl;

		cout << "Set epmty line to continue.." << endl;
		while (1)
		{
			cout << "input [length, firstV, secondV]->";

			getline(cin, data);
			if (data.empty())
				break;

			sBuf = split(data, " ");
			if (sBuf.size() != 3)
			{
				cout << "ERROR!" << endl;
				exit(0);
			}

			static int l = atoi(sBuf[0].c_str());
			static int a = atoi(sBuf[1].c_str());
			static int b = atoi(sBuf[2].c_str());

			buff.push_back(make_pair(l, make_pair(a, b)));
			n = MAX(n, a, b);
		}
		if (n == 0)
		{
			cout << "ERROR!" << endl;
			exit(0);
		}
		++n;

		matrix = initMatrix(n);
		setMatrix(buff, matrix);
		buff.clear();

		cout << "GRAPH MATRIX" << endl;
		displayMatrix(matrix, n);

		cout << endl << "vertices count->" << n << endl;
		cout << "analysis..." << endl;
		break;

	default:
		cout << endl << "ERROR!";
		_getch();
		exit(0);
	}
	cout << endl << "vertices count->" << n << endl;
	cout << "analysis..." << endl;
	DWORD t1 = clock();
	vector<graphE> gList = findComp(matrix, n);
	DWORD t2 = clock();

	cout << "Time to find components->" << t2 - t1 << endl;
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
		t1 = clock();
		graphE v = krusk(gList[i], n, len);
		t2 = clock();
		cout << "time to find MST->" << t2 - t1 << endl;
		cout << "graph edges <" << v.size() << ">" << endl;
		if(yn == 'N')
			for (pair<int, pair<int, int>> p : v)
				cout << "(" << p.second.first << ", " << p.second.second << ") [" << p.first << "]" << endl;

		cout << "Total lenght->" << len << endl << endl;
	}
	system("pause");
	return 0;
}