#include <iostream>
#include <omp.h>
#include <time.h>
using namespace std;

#define SIZE 10

void main()
{

	setlocale(LC_ALL, "Russian");
	const int n = SIZE;

	//Матрица n1 x m1
	int matrix1[n][n];
	//Матрица n2 x m2
	int matrix2[n][n];

	//Генерируем  матрицы для умножения
	int l = 1;;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			matrix1[i][j] = l;
		}
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			matrix2[i][j] = l;
		}
	}

	double t1Linear = omp_get_wtime();

	int result[n][n];

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			result[i][j] = 0;
			for (int k = 0; k < n; k++)
			{
				result[i][j] += (matrix1[i][k] * matrix2[k][j]);
			}
		}
	}


	double t2Linear = omp_get_wtime();
	cout << t2Linear - t1Linear << " - Время последовательного";

	//Устанавливаем число потоков
	//int threadsNum = 8;
	//omp_set_num_threads(threadsNum);
	int i, j, k;
	double t1 = omp_get_wtime();

#pragma omp parallel for shared(matrix1, matrix2, resultParallel) private(i, j, k)

	int resultParallel[n][n];

	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			resultParallel[i][j] = 0;
			for (k = 0; k < n; k++)
			{
				resultParallel[i][j] += (matrix1[i][k] * matrix2[k][j]);
			}
		}
	}
	double t2 = omp_get_wtime();
	cout << endl << t2 - t1 << " - время параллельного\n";

	t1 = omp_get_wtime();
	int threads1Num = 2;
	int resultlentochka[n][n];

	omp_set_num_threads(threads1Num);
#pragma omp parallel shared(matrix1,matrix2,resultlentochka,m) private(i,j,k)
	{


		int numThread = omp_get_thread_num();
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < n; j++)
			{
				resultlentochka[i + (numThread * n)][j] = 0;
				for (k = 0; k < n; k++)
				{
					resultlentochka[i + (numThread * n)][j] += (matrix1[i + (numThread * n)][k] * matrix2[k][j]);
				}
			}

		}
	}
	t2 = omp_get_wtime();
	cout << endl << t2 - t1 << " - время ленточного(паралельно)\n";


	//Проверка на совпадение значений
	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			if (resultlentochka[i][j] != result[i][j])
			{
				cout << "Значение не совпадает" << endl;
				break;
			}

		}
	}

	cout << "Матрица которая ленточная" << endl;
	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			cout << resultlentochka[i][j] << " ";
		}
		cout << endl;
	}

	cout << "Матрица которая последовательная" << endl;
	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			cout << result[i][j] << " ";
		}
		cout << endl;
	}

	system("pause");
}