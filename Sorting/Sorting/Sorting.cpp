#include <iostream>
#include <cmath>
#include <cstdlib>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
using namespace std;
 
#define SIZE 1000

bool Sravn(int* A)
{
	for (int i = 1; i < SIZE; i++)
	{
		if (A[i - 1] > A[i])
		{
			return true;
		}
		else
		{
			return false;
			break;
		}
	}
}

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

void ParallelOddEvenSort(int arr[], int size, int thrs)
{
	omp_set_num_threads(thrs);

	for (int phase = 0; phase < size; phase++)
	{
		if (phase % 2 == 0)
		{
#pragma omp for
			for (int i = 1; i < size; i += 2)
				CompareExchange(arr[i - 1], arr[i]);
		}
		else
		{
#pragma omp for
			for (int i = 1; i < size - 1; i += 2)
				CompareExchange(arr[i], arr[i + 1]);
		}
	}
}

void QuickSort(int arr[], int lo, int hi) {
	if (lo < hi) {
		int i = lo, j = hi, x = arr[lo];
		while (i < j) {
			while (i < j && arr[j] >= x)
				--j;
			if (i < j)
				arr[i++] = arr[j];
			while (i < j && arr[i] < x)
				++i;
			if (i < j)
				arr[j--] = arr[i];
		}
		arr[i] = x;
		QuickSort(arr, lo, i - 1);
		QuickSort(arr, i + 1, hi);
	}
}

void QuickSortParallel(int* arr, int n, int thrs) {
	omp_set_num_threads(thrs);

	int i = 0, j = n - 1;
	int pivot = arr[n / 2];

	do {
		while (arr[i] < pivot) {
			i++;
		}
		while (arr[j] > pivot) {
			j--;
		}
		if (i <= j) {
			swap(arr[i], arr[j]);
			i++; j--;
		}
	} while (i <= j);

#pragma omp parallel shared(arr)
	{
		if (j > 0) {
			QuickSortParallel(arr, (j + 1), thrs);
		}
	}
#pragma omp parallel shared(arr)
	{
		if (n > i) {
			QuickSortParallel(arr + i, (n - i), thrs);
		}
	}
}

void ShellSort(int* arr, int size)
{
	int d = size / 2;

	while (d > 0)
	{
		for (int i = 0; i < size - d; i++)
		{
			int j = i;
			while (j >= 0 && arr[j] > arr[j + d])
			{
				swap(arr[j], arr[j + d]);
				j--;
			}
		}
		d = d / 2;
	}
}

void ShellSortParallel(int* arr, int size, int thrs)
{
	omp_set_num_threads(thrs);
#pragma omp parallel firstprivate(size)
	{
		int m = omp_get_thread_num();
		int step = size / 2;
		while (step > 0)
		{
#pragma omp parallel for
			for (int i = 0; i < (size - step); i++)
#pragma omp critical 
			{
				int j = i;
				while (j >= 0 && arr[j] > arr[j + step])
				{
					Swap(arr[j], arr[j + step]);
					j--;
				}
			}
			step = step / 2;
		}
	}
}

int main()
{
	double t, t1, t2;
	setlocale(LC_ALL, "");
	//cout << "У вас в системе"<< omp_get_num_procs() <<"потоков"<< endl << "\n\n";
	
	cout << "Сортировка пузырьком" << endl;
	//блок с сортировкой пузырьком
	int* a = new int[SIZE];
	for (int i = 0; i < SIZE; i++)
	{
		a[i] = SIZE - i;
	}
	t1 = omp_get_wtime();
	BubbleSort(a, SIZE);
	t2 = omp_get_wtime();
	t = t2 - t1;
	//PrintArray(a, SIZE);
	cout << "Время последовательного алгоритма Пузырька = " << t << endl;

	if (!Sravn(a))
	{
		cout << "Массив отсортирован правильно!" << endl;
	}
	else
	{
		cout << "Массив отсортирован неправильно!" << endl;
	}

	//блок  сортировкой чет-нечет
	for (int i = 0; i < SIZE; i++)
	{
		a[i] = SIZE - i;
	}
	t1 = omp_get_wtime();
	ParallelOddEvenSort(a, SIZE, 2);
	t2 = omp_get_wtime();
	t = t2 - t1;
	//PrintArray(a, SIZE);
	cout << "Время параллельного чет-нечетный метода(2 потока) = " << t << endl;

	if (!Sravn(a))
	{
		cout << "Массив отсортирован правильно!" << endl;
	}
	else
	{
		cout << "Массив отсортирован неправильно!" << endl;
	}

	for (int i = 0; i < SIZE; i++)
	{
		a[i] = SIZE - i;
	}
	t1 = omp_get_wtime();
	ParallelOddEvenSort(a, SIZE, 4);
	t2 = omp_get_wtime();
	t = t2 - t1;
	//PrintArray(a, SIZE);
	cout << "Время параллельного чет-нечетный метода(4 потока) = " << t << endl;

	if (!Sravn(a))
	{
		cout << "Массив отсортирован правильно!" << "\n\n";
	}
	else
	{
		cout << "Массив отсортирован неправильно!" << "\n\n";
	}

	cout << "Быстрая сортировка" << endl;
	//блок с быстрой сортировкой
	for (int i = 0; i < SIZE; i++)
	{
		a[i] = SIZE - i;
	}
	t1 = omp_get_wtime();
	QuickSort(a, 0, SIZE-1);
	t2 = omp_get_wtime();
	t = t2 - t1;
	//PrintArray(a, SIZE);
	cout << "Время последовательного алгоритма Быстрой сортировки = " << t << endl;

	if (!Sravn(a))
	{
		cout << "Массив отсортирован правильно!" << endl;
	}
	else
	{
		cout << "Массив отсортирован неправильно!" << endl;
	}

	for (int i = 0; i < SIZE; i++)
	{
		a[i] = SIZE - i;
	}
	t1 = omp_get_wtime();
	QuickSortParallel(a, SIZE, 2);
	t2 = omp_get_wtime();
	t = t2 - t1;
	//PrintArray(a, SIZE);
	cout << "Время параллельного алгоритма Быстрой сортировки(2 потока) = " << t << endl;

	if (!Sravn(a))
	{
		cout << "Массив отсортирован правильно!" << endl;
	}
	else
	{
		cout << "Массив отсортирован неправильно!" << endl;
	}

	for (int i = 0; i < SIZE; i++)
	{
		a[i] = SIZE - i;
	}
	t1 = omp_get_wtime();
	QuickSortParallel(a, SIZE, 4);
	t2 = omp_get_wtime();
	t = t2 - t1;
	//PrintArray(a, SIZE);
	cout << "Время параллельного алгоритма Быстрой сортировки(4 потока) = " << t << endl;

	if (!Sravn(a))
	{
		cout << "Массив отсортирован правильно!" << "\n\n";
	}
	else
	{
		cout << "Массив отсортирован неправильно!" << "\n\n";
	}

	cout << "Сортировка Шелла" << endl;
	//блок с сортировкой Шеллла
	for (int i = 0; i < SIZE; i++)
	{
		a[i] = SIZE - i;
	}
	t1 = omp_get_wtime();
	ShellSort(a, SIZE);
	t2 = omp_get_wtime();
	t = t2 - t1;
	//PrintArray(a, SIZE);
	cout << "Время последовательного алгоритма сортировки Шелла = " << t << endl;

	if (!Sravn(a))
	{
		cout << "Массив отсортирован правильно!" << endl;
	}
	else
	{
		cout << "Массив отсортирован неправильно!" << endl;
	}

	//блок с сортировкой Шеллла
	for (int i = 0; i < SIZE; i++)
	{
		a[i] = SIZE - i;
	}
	t1 = omp_get_wtime();
	ShellSortParallel(a, SIZE,2);
	t2 = omp_get_wtime();
	t = t2 - t1;
	//PrintArray(a, SIZE);
	cout << "Время параллельного алгоритма сортировки Шелла(2 потока) = " << t << endl;

	if (!Sravn(a))
	{
		cout << "Массив отсортирован правильно!" << endl;
	}
	else
	{
		cout << "Массив отсортирован неправильно!" << endl;
	}

	for (int i = 0; i < SIZE; i++)
	{
		a[i] = SIZE - i;
	}
	t1 = omp_get_wtime();
	ShellSortParallel(a, SIZE, 4);
	t2 = omp_get_wtime();
	t = t2 - t1;
	//PrintArray(a, SIZE);
	cout << "Время параллельного алгоритма сортировки Шелла(4 потока) = " << t << endl;

	if (!Sravn(a))
	{
		cout << "Массив отсортирован правильно!" << "\n\n";
	}
	else
	{
		cout << "Массив отсортирован неправильно!" << "\n\n";
	}

	delete[] a;

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