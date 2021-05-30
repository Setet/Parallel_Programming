#include <iostream>
#include <omp.h>
#include <time.h>
#include <math.h>
using namespace std;

#define SIZE 1000

void Initialization(float** A, float** B, float** C, float** N)
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			A[i][j] = B[i][j] = 1;
			C[i][j] = N[i][j] = 0;
		}
	}
}

//блок сравнения двух матриц
bool Сomparison(float** A, float** B)
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (A[i][j] != B[i][j])
			{
				return false;
			}
		}
	}
	return true;
}

int main()
{
	setlocale(LC_ALL, "Rus");

	//объявляем матрицы
	//A и B-две начальные матрицы
	//C-для блочного+ленточного
	//N-для простого умножения

	float** A, ** B, ** C, ** N;
	A = new float* [SIZE];
	for (int i = 0; i < SIZE; i++)
	{
		A[i] = new float[SIZE];
	}

	B = new float* [SIZE];
	for (int i = 0; i < SIZE; i++)
	{
		B[i] = new float[SIZE];
	}

	C = new float* [SIZE];
	for (int i = 0; i < SIZE; i++)
	{
		C[i] = new float[SIZE];
	}

	N = new float* [SIZE];
	for (int i = 0; i < SIZE; i++)
	{
		N[i] = new float[SIZE];
	}

	//записываем в них значения
	Initialization(A, B, C, N);

	double t1, t2, consistent, band;

	t1 = omp_get_wtime();
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			for (int k = 0; k < SIZE; k++)
			{
				N[i][j] += A[i][k] * B[k][j];
			}
		}
	}
	t2 = omp_get_wtime();

	consistent = t2 - t1;

	int kolwo_procces, gor, vert;
	cout << "Введите кол-во потоков(желательно,не больше " << omp_get_num_procs() << " штук)" << endl << ">: ";
	cin >> kolwo_procces;
	cout << "Деление по горизонтали" << endl << ">: ";
	cin >> gor;
	cout << "Деление по вертикали" << endl << ">: ";
	cin >> vert;

	int j, i, g, k;
	t1 = omp_get_wtime();
#pragma omp parallel num_threads(kolwo_procces) shared(A,B,C) private (i,j,g,k)
	{
		g = omp_get_thread_num();
		for (i = g; i < SIZE; i += kolwo_procces)
		{
			for (j = 0; j < SIZE; j++)
			{
				for (k = 0; k < SIZE; k++)
				{
					C[i][j] += A[i][k] * B[k][j];
				}
			}
		}
	}
	t2 = omp_get_wtime();

	band = t2 - t1;

	//сравниваем ленточный счёт с нормальным
	if (!Сomparison(C, N))
	{
		cout << "Ошибка!!!" << endl;
	}

	//вывод времени последовательного и ленточного метода
	cout << "Время Последовательного счёта =" << consistent << endl;
	cout << "Время Ленточного счёта = " << band << endl;

	//обнуляем матрицу C после ленточного умножения
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			C[i][j] = 0;
		}
	}

	double block;
	t1 = omp_get_wtime();
#pragma omp parallel num_threads( kolwo_procces) shared(A, B, C)
	{

		int razm = omp_get_num_threads();
		int tid1 = omp_get_thread_num() / vert;
		int tid2 = omp_get_thread_num() % vert;
		if (SIZE / gor * tid1 != SIZE / gor * gor - SIZE / gor * gor / gor)
		{
			for (int i = tid1 * (SIZE / gor); i < tid1 * (SIZE / gor) + (SIZE / gor); i++)
			{
				if (SIZE / vert * tid2 != SIZE / vert * vert - SIZE / vert * vert / vert)
				{
					for (int j = tid2 * (SIZE / vert); j < tid2 * (SIZE / vert) + (SIZE / vert); j++)
					{
						for (int k = 0; k < SIZE; k++)
						{
							C[i][j] = C[i][j] + (A[i][k] * B[k][j]);

						}
					}
				}
				else
				{
					for (int j = tid2 * (SIZE / vert); j < SIZE; j++)
					{
						for (int k = 0; k < SIZE; k++)
						{
							C[i][j] = C[i][j] + (A[i][k] * B[k][j]);

						}
					}
				}

			}
		}
		else
		{
			for (int i = tid1 * (SIZE / gor); i < SIZE; i++)
			{
				if (SIZE / vert * tid2 != SIZE / vert * vert - SIZE / vert * vert / vert)
				{
					for (int j = tid2 * (SIZE / vert); j < tid2 * (SIZE / vert) + (SIZE / vert); j++)
					{
						for (int k = 0; k < SIZE; k++)
						{
							C[i][j] = C[i][j] + (A[i][k] * B[k][j]);
						}
					}
				}
				else
				{
					for (int j = tid2 * (SIZE / vert); j < SIZE; j++)
					{

						for (int k = 0; k < SIZE; k++)
						{
							C[i][j] = C[i][j] + (A[i][k] * B[k][j]);
						}
					}
				}

			}
		}
	}
	t2 = omp_get_wtime();

	if (!Сomparison(C, N))
	{
		cout << "Ошибка!!!" << endl;
	}

	block = t2 - t1;
	cout << "Время Блочного = " << block << "\n\n";

	int choser;

	cout << "Запустить ещё раз?\n" <<
		"1 - Да\n" <<
		"2 - Нет\n" <<
		">: ";
	cin >> choser;

	switch (choser)
	{
	case(1):
	{
		system("cls");
		return main();
	}
	case(2):
	{
		system("pause");
		return 0;
	}
	}
}