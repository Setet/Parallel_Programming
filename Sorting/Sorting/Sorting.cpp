#include <iostream>
#include <cmath>
#include <cstdlib>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
using namespace std;
 
#define SIZE 100

void PrintArray(int arr[], int size)
{
	for (int i = 0; i < size; i++)
	{
		cout << arr[i] << " ";
	}
}

bool Сomparison(int* A, int* B)
{
	for (int i = 0; i < SIZE; i++)
	{
		if (A[i] != B[i])
		{
			return false;
		}
	}
	return true;
}

void Swap(int& a, int& b)
{
	int t = a;
	a = b;
	b = t;
}

int partition(int arr[], int low, int high)
{
	int pivot = arr[high]; // pivot
	int i = (low - 1); // Index of smaller element
	for (int j = low; j <= high - 1; j++)
	{
		// If current element is smaller than or
		// equal to pivot
		if (arr[j] <= pivot)
		{
			i++; // increment index of smaller element
			Swap(arr[i], arr[j]);
		}
	}
	Swap(arr[i + 1], arr[high]);
	return (i + 1);
}

void CompareExchange(int& a, int& b)
{
	if (a > b)
	{
		int t = a;
		a = b;
		b = t;
	}
}

void InsertSort(int arr[], int i, int length, int half) {
	for (int f = half + i; f < length; f = f + half)
	{
		int j = f;
		while (j > i && arr[j - half] > arr[j])
		{
			Swap(arr[j], arr[j - half]);
			j = j - half;
		}
	}
}

void BubbleSort(int arr[], int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 1; j < size; j++)
		{
			CompareExchange(arr[j - 1], arr[j]);
		}
	}
}

void QuickSortParallel(int arr[], int low, int high)
{
	if (low < high)
	{
		int pi = partition(arr, low, high);


#pragma omp task first private(arr,low,pi)
		{
			QuickSortParallel(arr, low, pi - 1);
		}

		//#pragma omp task firstprivate(arr, high,pi)
		{
			QuickSortParallel(arr, pi + 1, high);
		}
	}
}

void ShellSortParallel(int array[], int length)
{
	int h;
	int i = 0;
	for (h = length / 2; h > 0; h = h / 2)
	{
#pragma omp parallel for shared( array, length, h, i)  default(none)
		for (i = 0; i < h; i++)
		{
			InsertSort(array, i, length, h);
		}
	}
}

int main()
{
	double t, t1, t2;
    setlocale(LC_ALL, "");
	//cout << "У вас в системе"<< omp_get_num_procs() <<"потоков"<< endl << "\n\n";

	//блок с сортировкой пузырьком
	int* a = new int[SIZE];
	for (int i = 0; i < SIZE; i++)
	{
		a[i] = SIZE - i;
	}
	t1= omp_get_wtime();
	BubbleSort(a, SIZE);
	t2 = omp_get_wtime();
	t = t2 - t1;
	//PrintArray(a, SIZE);
	cout << "Время последовательного алгоритма Пузырька = " << t << endl;

	bool sorted = true;
	for (int i = 1; i < SIZE; i++)
	{
		if (a[i - 1] > a[i])
		{
			sorted = false;
			break;
		}
	}
	if (sorted == true)
	{
		cout << "Массив отсортирован правильно!" << endl;
	}
	else
	{
		cout << "Массив отсортирован неправильно!" << endl;
	}


	//блок с быстрой сортировкой
	int* b = new int[SIZE];
	for (int i = 0; i < SIZE; i++)
	{
		b[i] = SIZE - i;
	}
	t1 = omp_get_wtime();
	QuickSortParallel(b, 0, SIZE - 1);
	t2 = omp_get_wtime();
	t = t2 - t1;
	//PrintArray(a, SIZE);
	cout << "Время параллельного алгоритма Быстрой сортировки = " << t << endl;

	if (!Сomparison(a, b))
	{
		cout << "Массив отсортирован неправильно!" << endl;
	}
	else
	{
		cout << "Массив отсортирован правильно!" << endl;
	}

	//блок с сортировкой Шеллла
	int* c = new int[SIZE];
	for (int i = 0; i < SIZE; i++)
	{
		c[i] = SIZE - i;
	}
	t1 = omp_get_wtime();
	ShellSortParallel(c, SIZE);
	t2 = omp_get_wtime();
	t = t2 - t1;
	//PrintArray(a, SIZE);
	cout << "Время параллельного алгоритма сортировки Шелла = " << t << endl;

	if (!Сomparison(b, c))
	{
		cout << "Массив отсортирован неправильно!\n" << endl;
	}
	else
	{
		cout << "Массив отсортирован правильно!\n" << endl;
	}

	delete[] a;
	delete[] b;
	delete[] c;

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
		return 0;
	}
	}
}