#include <iostream>
#include <omp.h>
#include <time.h>
#include <math.h>
using namespace std;

#define SIZE 1000

void Band_Consistently(float** A, float** B, float** C)
{
	int BlocksNum = 4;
	int BlockSize = SIZE / BlocksNum;
	for (int i = 0; i < BlockSize; i++)
	{
		for (int j = 0; j < BlockSize; j++)
		{
			for (int k = 0; k < SIZE; k++)
			{
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

void Band_Parallel(float** A, float** B, float** C,int thrs)
{
	omp_set_num_threads(thrs);
#pragma omp parallel
	{
		int BlocksNum = 4;
		int BlockSize = SIZE / BlocksNum;
		for (int i = 0; i < BlockSize; i++)
		{
			for (int j = 0; j < BlockSize; j++)
			{
				for (int k = 0; k < SIZE; k++)
				{
					C[i][j] += A[i][k] * B[k][j];
				}
			}
		}
	}
}

void Block_Consistently(float** A, float** B, float** C)
{
	int ThreadNum = 4;
	int GridSize = ThreadNum / 2;
	int BlockSize = SIZE / GridSize;
	{
		int ThreadID = omp_get_thread_num();
		int RowIndex = ThreadID / GridSize;
		int ColIndex = ThreadID % GridSize;
		for (int iter = 0; iter < GridSize; iter++)
		{
			for (int i = RowIndex * BlockSize; i < (RowIndex + 1) * BlockSize; i++)
			{
				for (int j = ColIndex * BlockSize; j < (ColIndex + 1) * BlockSize; j++)
				{
					for (int k = iter * BlockSize; k < (iter + 1) * BlockSize; k++)
					{
						C[i][j] += A[i][k] * B[k][j];
					}
				}
			}

		}
	}
}

void Block_Parallel(float** A, float** B, float** C,int thrs)
{
	int ThreadNum = 4;
	int GridSize = ThreadNum / 2;
	int BlockSize = SIZE / GridSize;
	omp_set_num_threads(thrs);
#pragma omp parallel
	{
		int ThreadID = omp_get_thread_num();
		int RowIndex = ThreadID / GridSize;
		int ColIndex = ThreadID % GridSize;
		for (int iter = 0; iter < GridSize; iter++)
		{
			for (int i = RowIndex * BlockSize; i < (RowIndex + 1) * BlockSize; i++)
			{
				for (int j = ColIndex * BlockSize; j < (ColIndex + 1) * BlockSize; j++)
				{
					for (int k = iter * BlockSize; k < (iter + 1) * BlockSize; k++)
					{
						C[i][j] += A[i][k] * B[k][j];
					}
				}
			}
		
		}
	}
}

void Multiplication_Consistently(float** A, float** B, float** C)
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			C[i][j] = 0.0;
			for (int k = 0; k < SIZE; k++)
			{
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}

}

void Multiplication_Parallel(float** A, float** B, float** C,int thrs)
{
	omp_set_num_threads(thrs); //колличество использованных потоков
#pragma omp parallel
	{
		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
			{
				C[i][j] = 0.0;
				for (int k = 0; k < SIZE; k++)
				{
					C[i][j] += A[i][k] * B[k][j];
				}
			}
		}
	}
}

void Band(int q)
{
	int i, j, k;
	float t1, t2, t3, t4, t5, t6;

	float** a, ** b, ** c;
	a = new float* [SIZE];
	for (i = 0; i < SIZE; i++)
	{
		a[i] = new float[SIZE];
	}

	b = new float* [SIZE];
	for (i = 0; i < SIZE; i++)
	{
		b[i] = new float[SIZE];
	}

	c = new float* [SIZE];
	for (i = 0; i < SIZE; i++)
	{
		c[i] = new float[SIZE];
	}

	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			a[i][j] = b[i][j] = 1;
		}
	}

	if (q == 1)
	{
		t1 = omp_get_wtime();
		Band_Consistently(a, b, c);
		t2 = omp_get_wtime();
		cout << "Время ленточного счёта (последовательный метод) = " << t2 - t1 << "\n\n";
	}
	if (q == 2)
	{
		t3 = omp_get_wtime();
		Band_Parallel(a, b, c, 8);
		t4 = omp_get_wtime();
		cout << "Время ленточного счёта (параллельный метод,8 потоков) = " << t4 - t3 << "\n\n";

		t5 = omp_get_wtime();
		Band_Parallel(a, b, c, 16);
		t6 = omp_get_wtime();
		cout << "Время ленточного счёта (параллельный метод,16 потоков) = " << t6 - t5 << "\n\n";
	}
}

void Block(int q)
{
	int i, j, k;
	float t1, t2, t3, t4, t5, t6;

	float** a, ** b, ** c;
	a = new float* [SIZE];
	for (i = 0; i < SIZE; i++)
	{
		a[i] = new float[SIZE];
	}

	b = new float* [SIZE];
	for (i = 0; i < SIZE; i++)
	{
		b[i] = new float[SIZE];
	}

	c = new float* [SIZE];
	for (i = 0; i < SIZE; i++)
	{
		c[i] = new float[SIZE];
	}

	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			a[i][j] = b[i][j] = 1;
		}
	}

	if (q == 1)
	{
		t1 = omp_get_wtime();
		Block_Consistently(a, b, c);
		t2 = omp_get_wtime();
		cout << "Время блочного счёта (последовательный метод) = " << t2 - t1 << "\n\n";
	}
	if (q == 2)
	{
		t3 = omp_get_wtime();
		Block_Parallel(a, b, c, 8);
		t4 = omp_get_wtime();
		cout << "Время блочного счёта (параллельный метод,8 потоков) = " << t4 - t3 << "\n\n";

		t5 = omp_get_wtime();
		Block_Parallel(a, b, c, 16);
		t6 = omp_get_wtime();
		cout << "Время блочного счёта (параллельный метод,16 потоков) = " << t6 - t5 << "\n\n";
	}
}

void Multiplication(int q)
{
	int i, j;
	float t1, t2, t3, t4, t5, t6;

	float** a, ** b, ** c;
	a = new float* [SIZE];
	for (i = 0; i < SIZE; i++)
	{
		a[i] = new float[SIZE];
	}

	b = new float* [SIZE];
	for (i = 0; i < SIZE; i++)
	{
		b[i] = new float[SIZE];
	}

	c = new float* [SIZE];
	for (i = 0; i < SIZE; i++)
	{
		c[i] = new float[SIZE];
	}

	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			a[i][j] = b[i][j] = 1;
		}
	}

	if (q == 1)
	{
		t1 = omp_get_wtime();
		Multiplication_Consistently(a, b, c);
		t2 = omp_get_wtime();
		cout << "Время обычного перемножение (последовательный метод) = " << t2 - t1 << "\n\n";
	}
	if (q == 2)
	{
		t3 = omp_get_wtime();
		Multiplication_Parallel(a, b, c,8);
		t4 = omp_get_wtime();
		cout << "Время обычного перемножение (параллельный метод,8 потоков) = " << t4 - t3 << "\n\n";

		t5 = omp_get_wtime();
		Multiplication_Parallel(a, b, c, 16);
		t6 = omp_get_wtime();
		cout << "Время обычного перемножение (параллельный метод,16 потоков) = " << t6 - t5 << "\n\n";
	}
}


void switcher()
{
	int choser;
	cout << "Выберите интересующий вариант:\n" <<
		"1 - Обычное перемножение (последовательный метод)\n" <<
		"2 - Обычное перемножение (параллельный метод)\n" <<
		"3 - Ленточный счёт (последовательный метод)\n" <<
		"4 - Ленточный счёт (параллельный метод)\n" <<
		"5 - Блочный счёт (последовательный метод)\n" <<
		"6 - Блочный счёт (параллельный метод)\n" <<
		"\nДля отмены выберите любую другую цифру!\n";
	cin >> choser;

	system("cls");

	switch (choser)
	{
	case(1):
	{
		Multiplication(1);
		switcher();
	}
	case(2):
	{
		Multiplication(2);
		switcher();
	}
	case(3):
	{
		Band(1);
		switcher();
	}
	case(4):
	{
		Band(2);
		switcher();
	}
	case(5):
	{
		Block(1);
		switcher();
	}
	case(6):
	{
		Block(2);
		switcher();
	}
	default:
		break;
	}
}

int main()
{
	setlocale(LC_ALL, "Rus");

    //cout << "В системе " << omp_get_num_procs() << " потоков" << endl;

	switcher();

	system("pause");
	return 0;
}