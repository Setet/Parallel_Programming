#include <iostream>
#include <cmath>
#include <cstdlib>
#include <stdlib.h>
#include <omp.h>

using namespace std;

double func(double x)
{
	return pow(x, 2) + sin(x);
}

/// <summary>
/// Численное интегрирование по формуле Симпсона. Последовательный метод
/// </summary>
/// <param name="a - Нижняя граница интегрирования"></param>
/// <param name="b - Верхняя граница интегрирования"></param>
/// <param name="n - Колличество отрезков"></param>
/// <returns></returns>
double SimpsonParableIntegral(double a, double b, double n)
{
	double h = (b - a) / n; // вычисляем шаг - h
	double sum = 0;     // сумма, результат вычисления интеграла.
	double x0 = a;      // правая граница подотрезка отрезка [a, b]
	double x1 = a + h;  // левая граница подотрезка отрезка [a, b]

	for (int i = 0; i < n; i++) // в цикле применяем формулу Симпсона
	{
		sum += func(x0) + 4 * func(x0 + h / 2) + func(x1);   //для каждого подотрезка, и складываем все полученные значения в общую сумму.
		x0 += h;    // сдвигаем левую и
		x1 += h;    // правую границу
	}

	return (h / 6) * sum;	// возвращаем сумму умноженную на (h/6)(по формуле), т.к. (h/6) общий множитель который можно вынести за скобки.
}

double SPIParallel(double a, double b, int n, int thrs)
{
	double delta = (b - a) / 10;
	double integral = 0.;

	omp_set_num_threads(thrs); //колличество использованных потоков
#pragma omp parallel for reduction(+: integral) //прагма дает инструкции компилятору
	for (int i = 0; i < 10; i++) integral += SimpsonParableIntegral(a + i * delta, a + (i + 1) * delta, n);// , i, div);

	return integral;
}

void simpChoser(int i)
{
	double a, b, n;
	cout << "Введите a\n";
	cin >> a;
	cout << "Введите b\n";
	cin >> b;
	cout << "Введите n\n";
	cin >> n;

	double ans = 0;
	double t1 = omp_get_wtime();
	double t2 = omp_get_wtime();

	if (i == 1)
	{
		ans = SimpsonParableIntegral(a, b, n);
		t1 = omp_get_wtime() - t1;

		cout << "Время = " << t1 << endl;
		cout << "Ответ = " << ans << endl;
	}
	if (i == 2)
	{
		ans = SPIParallel(a, b, n, 8);
		t1 = omp_get_wtime() - t1;

		cout << "Время = " << t1 << endl;
		cout << "Ответ = " << ans << endl;
		cout << "Потоков = 8\n\n";

		ans = SPIParallel(a, b, n, 16);
		t2 = omp_get_wtime() - t2;

		cout << "Время = " << t2 << endl;
		cout << "Ответ = " << ans << endl;
		cout << "Потоков = 16\n\n";
	}
}

void switcherOfMyHeart()
{
	int choser;
	cout << "\nВыберите интересующий вариант:\n" <<
		"1 - Численное интегрирование по формуле Симпсона (последовательный метод)\n" <<
		"2 - Численное интегрирование по формуле Симпсона (параллельный метод)\n" <<
		"\nДля отмены выберите любую другую цифру!\n";
	cin >> choser;

	system("cls");

	switch (choser)
	{
	case(1):
	{
		simpChoser(1);
		switcherOfMyHeart();
	}
	case(2):
	{
		simpChoser(2);
		switcherOfMyHeart();
	}
	default:
		break;
	}
}

int main()
{
	setlocale(LC_ALL, "");

	switcherOfMyHeart();

	return 0;
}