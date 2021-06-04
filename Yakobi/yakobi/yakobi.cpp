#include <iostream>
#include <omp.h>
#include <random>
#include<cmath>
#include <ctime>
using namespace std;

int main()
{
	srand(time(0));
	/*заполение массивов*/
	const int MAX = 500;
	const int NMAX = 501;
	setlocale(0, "RUS");
	float a[MAX][NMAX], c[MAX], d[MAX], pred[MAX];
	if (MAX == 2)
	{
		for (int i = 0; i < MAX; i++)
		{
			for (int j = 0; j < NMAX; j++)
			{
				cout << "Введите " << i << " " << j << " элемент" << endl;
				cin >> a[i][j];
			}
		}
	}
	else
	{
		for (int i = 0; i < MAX; i++)
		{
			d[i] = 0;
			for (int j = 0; j < NMAX; j++)
			{
				a[i][j] = rand() % 5 + 1;
			}
			a[i][i] = rand() % 5 + 10000;
		}
		for (int i = 0; i < MAX; i++)
		{
			c[i] = rand() % 5;
			pred[i] = c[i];
		}
		for (int i = 0; i < MAX; i++)
		{
			int sum = 0;
			for (int j = 0; j < MAX; j++)
			{
				sum = sum + a[i][j] * c[j];
			}
			a[i][NMAX - 1] = sum;
		}
		for (int i = 0; i < MAX; i++)
		{
			c[i] = a[i][NMAX - 1];
		}
	}

	/*последовательный алгоритм*/
	for (int i = 0; i < MAX; i++)
	{
		float rez = a[i][i];
		for (int j = 0; j < NMAX; j++)
		{
			a[i][j] = a[i][j] / rez;
		}
	}
	float eps = 1e-6;
	cout << "Eps = " << eps << endl;
	double t1 = omp_get_wtime();
	bool boo = 0;
	double max;
	while (!boo)
	{
		float rez[MAX];
		for (int i = 0; i < MAX; i++)
		{
			float sum = 0;
			for (int j = 0; j < MAX; j++)
			{
				if (i != j)
				{
					sum = sum + a[i][j] * c[j];
				}
			}
			rez[i] = -sum + a[i][NMAX - 1];
		}
		for (int i = 0; i < MAX; i++)
		{
			d[i] = abs(c[i] - rez[i]);
		}
		max = d[0];
		for (int i = 0; i < MAX; i++)
		{
			if (max < abs(d[i]))
				max = abs(d[i]);
		}

		if (max < eps)
			boo = 1;

		for (int i = 0; i < MAX; i++)
		{
			c[i] = rez[i];
		}
	}
	double t2 = omp_get_wtime();
	double obch = t2 - t1;
	cout << "Время последовательного : " << obch << endl;

	bool prow = 1;
	for (int i = 0; i < MAX; i++)
	{
		float g = abs(pred[i] - c[i]);
		if (g > eps)
			prow = 0;
	}
	if (prow)
	{
		cout << "Всё хорошо" << endl;
	}
	else
	{
		cout << "Ошибка" << endl;
	}

	/*парралельный алгоритм*/
	int kolwo_procces = 2;

	for (int i = 0; i < MAX; i++)
	{
		c[i] = a[i][NMAX - 1];
	}
	t1 = omp_get_wtime();
	boo = false;
	while (!boo)
	{
		float rez[MAX];
#pragma omp parallel num_threads(kolwo_procces) 
		{

			for (int i = omp_get_thread_num(); i < MAX; i = i + kolwo_procces)
			{
				float sum = 0;
				for (int j = 0; j < MAX; j++)
				{
					if (i != j)
					{
						sum = sum + a[i][j] * c[j];
					}
				}
				rez[i] = -sum + a[i][NMAX - 1];
			}
		}
		for (int i = 0; i < MAX; i++)
		{
			d[i] = abs(c[i] - rez[i]);
		}
		max = d[0];
		for (int i = 0; i < MAX; i++)
		{
			if (max < abs(d[i]))
				max = abs(d[i]);
		}
		if (max < eps)
			boo = 1;

		for (int i = 0; i < MAX; i++)
		{
			c[i] = rez[i];
		}
	}
	t2 = omp_get_wtime();
	double par = t2 - t1;
	cout << "\nВремя паралельного(2 потока) : " << par << endl;

	prow = 1;
	for (int i = 0; i < MAX; i++)
	{
		if (abs(pred[i] - c[i]) >= eps)
			prow = 0;
	}
	if (prow)
	{
		cout << "Всё хорошо" << endl;
	}
	else
	{
		cout << "Ошибка" << endl;
	}

	kolwo_procces = 4;

	for (int i = 0; i < MAX; i++)
	{
		c[i] = a[i][NMAX - 1];
	}
	t1 = omp_get_wtime();
	boo = false;
	while (!boo)
	{
		float rez[MAX];
#pragma omp parallel num_threads(kolwo_procces) 
		{

			for (int i = omp_get_thread_num(); i < MAX; i = i + kolwo_procces)
			{
				float sum = 0;
				for (int j = 0; j < MAX; j++)
				{
					if (i != j)
					{
						sum = sum + a[i][j] * c[j];
					}
				}
				rez[i] = -sum + a[i][NMAX - 1];
			}
		}
		for (int i = 0; i < MAX; i++)
		{
			d[i] = abs(c[i] - rez[i]);
		}
		max = d[0];
		for (int i = 0; i < MAX; i++)
		{
			if (max < abs(d[i]))
				max = abs(d[i]);
		}
		if (max < eps)
			boo = 1;

		for (int i = 0; i < MAX; i++)
		{
			c[i] = rez[i];
		}
	}
	t2 = omp_get_wtime();
	par = t2 - t1;
	cout << "\nВремя паралельного(4 потока) : " << par << endl;

	prow = 1;
	for (int i = 0; i < MAX; i++)
	{
		if (abs(pred[i] - c[i]) >= eps)
			prow = 0;
	}
	if (prow)
	{
		cout << "Всё хорошо" << endl;
	}
	else
	{
		cout << "Ошибка" << endl;
	}
}