#include <vector>//интерфейс для вектора
#include <algorithm>//Набор стандартных алгов (сортировка например)
#include <ctime>//Тупо для рандома
#include <iostream>//Выводить что-то надо

using namespace std;//стд ))))000)



typedef vector <pair<int, pair<int, int>>> graphE;//набор ребер (вес - вершина 1 - вершина 2)
typedef int** matrix_t;//Матрица для графа

//Получение элемента из мн-ва
int dsu_get(int v, vector<int> &p)
{
	return (v == p[v]) ? v : (p[v] = dsu_get(p[v], p));
}

//Объединение мн-в
void dsu_unite(int a, int b, vector<int> &p)
{
	a = dsu_get(a, p);
	b = dsu_get(b, p);

	if (a != b)
		p[a] = b;
}

//Просто вывод матрицы на экран
void displayMatrix(int** matrix, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			cout << matrix[i][j] << "\t";

		cout << endl;
	}
}

//Инициализация матрицы
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

//Заполнение матрицы случайными значениями
void setMatrix(matrix_t &matrix, int n)
{
	
	for (int i = 0; i < n; i++)
	{
		for (int j = i/*чтобы была симметрия*/; j < n; j++)
		{
			matrix[i][j] = rand() % 1337;
			if (i == j)
				matrix[i][j] = 0;/*без петель*/

			matrix[j][i] = matrix[i][j];
		}
	}
}

//Сам алг крускала
graphE krusk(graphE g, int n, int &total)
{
	
	int cost = 0;//Общая длина
	graphE res;
	vector<int> p;
	sort(g.begin(), g.end());//Сортируем ребра

	p.resize(n);

	for (int i = 0; i < n; ++i)//Заполняем вершины
		p[i] = i;

	for (size_t i = 0; i < g.size(); ++i)
	{
		int a = g[i].second.first;
		int b = g[i].second.second;
		int l = g[i].first;

		if (a == b)//Пропускаем петли (мало ли пастить будут)
			continue;


		if (dsu_get(a, p) != dsu_get(b, p))//Если по вершинам не ходили
		{
			cost += l;//Плюсуем путь
			res.push_back(g[i]);//Добавляем ребро
			dsu_unite(a, b, p);//Объенияем мн-ва
		}
	}
	total = cost;//Возвращаем цену пути
	return res;//Возвращаем путь
}

//Это тупа дфс, обход в глубину на поиск компонент связанности
void dfs(const int v, vector<bool> &visited, int** matrix, const int n, vector<int> &result)
{
	visited[v] = true;
	result.push_back(v);
	for (int i = 0; i < n; i++)
		if (!visited[i] && matrix[v][i] != 0)
			dfs(i, visited, matrix, n, result);
}

//Тут получаем саму компоненту в виде ребер
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

//А тут сами компоненты
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


//это же муйн
int main()
{
	cout << "Cource MST <created by shockbyte>" << endl;
	srand(static_cast<int>(time(nullptr)));

	int n;
	n = 246 + rand() % 300;//Тупа задаем граф на 246+ вершинах это 30+к ребер

	//Заполняем матрицу взвешанности
	matrix_t matrix = initMatrix(n);
	setMatrix(matrix, n);

	cout << "vertices count->" << n << endl;
	cout << "analysis..." << endl;

	//Получаем компоненты вязанности и применяем алгоритм крускала
	vector<graphE> gList = findComp(matrix, n);
	if(gList.size() > 1 )
	{
		cout << "disconnected graph!" << endl;
		cout << "connected components ->" << gList.size() << endl;
	}
	else
		cout << "connected graph!" << endl;

	for(size_t i = 0; i < gList.size(); i++)//По каждой из компонент проходимся
	{
		int len = 0;
		graphE v = krusk(gList[i], n, len);//Получаем мин остовное дерево
		cout << "graph edges <" << v.size() << ">" << endl;
		for(pair<int, pair<int, int>> p : v)//Выводим ребра
			cout << "(" << p.second.first << ", " << p.second.second << ") [" << p.first << "]" << endl;

		cout << "Total lenght->" << len << endl << endl;  //Выводим всю длину
	}
	return 0;//ВЫХОД!
}