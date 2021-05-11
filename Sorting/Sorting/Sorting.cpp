#include <iostream>
#include <cmath>
#include <cstdlib>
#include <stdlib.h>
#include <omp.h>
#include <ctime>
using namespace std;
 
#define SIZE 100

void Bubble_Sort_Consistently(int* randomDigits)
{
    for (int i = 0; i < SIZE - 1; i++)
    {
        for (int j = 0; j < SIZE - i - 1; j++)
        {
            if (randomDigits[j] > randomDigits[j + 1])
            {
                
                int temp = randomDigits[j];
                randomDigits[j] = randomDigits[j + 1];
                randomDigits[j + 1] = temp;
            }
        }
    }

    cout << "Уже отсортированный массив:" << endl;
    for (int i = 0; i < SIZE; i++)
    {
        cout << randomDigits[i] << " ";
    }
    cout << endl;
}

void Bubble_Sort_Parallel(int* randomDigits,int thrs)
{
    omp_set_num_threads(thrs); //колличество использованных потоков
#pragma omp parallel for reduction(+: integral) //прагма дает инструкции компилятору
    for (int i = 0; i < SIZE - 1; i++)
    {
        for (int j = 0; j < SIZE - i - 1; j++)
        {
            if (randomDigits[j] > randomDigits[j + 1])
            {

                int temp = randomDigits[j];
                randomDigits[j] = randomDigits[j + 1];
                randomDigits[j + 1] = temp;
            }
        }
    }

    cout << "Уже отсортированный массив:" << endl;
    for (int i = 0; i < SIZE; i++)
    {
        cout << randomDigits[i] << " ";
    }
    cout << endl;
}

void Shell_Sort_Consistently(int* randomDigits)
{
    int i, j, step;
    int tmp;
    for (step = SIZE / 2; step > 0; step /= 2)
    {
        for (i = step; i < SIZE; i++)
        {
            tmp = randomDigits[i];
            for (j = i; j >= step; j -= step)
            {
                if (tmp < randomDigits[j - step])
                {
                    randomDigits[j] = randomDigits[j - step];
                }
                else
                {
                    break;
                }
            }
            randomDigits[j] = tmp;
        }
    }

    cout << "Уже отсортированный массив:" << endl;
    for (int i = 0; i < SIZE; i++)
    {
        cout << randomDigits[i] << " ";
    }
    cout << endl;
}

void Shell_Sort_Parallel(int* randomDigits, int thrs)
{
    omp_set_num_threads(thrs); //колличество использованных потоков
#pragma omp parallel for reduction(+: integral) //прагма дает инструкции компилятору
    int i, j, step;
    int tmp;
    for (step = SIZE / 2; step > 0; step /= 2)
    {
        for (i = step; i < SIZE; i++)
        {
            tmp = randomDigits[i];
            for (j = i; j >= step; j -= step)
            {
                if (tmp < randomDigits[j - step])
                {
                    randomDigits[j] = randomDigits[j - step];
                }
                else
                {
                    break;
                }
            }
            randomDigits[j] = tmp;
        }
    }

    cout << "Уже отсортированный массив:" << endl;
    for (int i = 0; i < SIZE; i++)
    {
        cout << randomDigits[i] << " ";
    }
    cout << endl;
}

void Quick_Sort_Consistently(int* numbers, int left, int right)
{
    int pivot; // разрешающий элемент
    int l_hold = left; //левая граница
    int r_hold = right; // правая граница
    pivot = numbers[left];
    while (left < right) // пока границы не сомкнутся
    {
        while ((numbers[right] >= pivot) && (left < right))
            right--; // сдвигаем правую границу пока элемент [right] больше [pivot]
        if (left != right) // если границы не сомкнулись
        {
            numbers[left] = numbers[right]; // перемещаем элемент [right] на место разрешающего
            left++; // сдвигаем левую границу вправо
        }
        while ((numbers[left] <= pivot) && (left < right))
            left++; // сдвигаем левую границу пока элемент [left] меньше [pivot]
        if (left != right) // если границы не сомкнулись
        {
            numbers[right] = numbers[left]; // перемещаем элемент [left] на место [right]
            right--; // сдвигаем правую границу вправо
        }
    }
    numbers[left] = pivot; // ставим разрешающий элемент на место
    pivot = left;
    left = l_hold;
    right = r_hold;
    if (left < pivot) // Рекурсивно вызываем сортировку для левой и правой части массива
        Quick_Sort_Consistently(numbers, left, pivot - 1);
    if (right > pivot)
        Quick_Sort_Consistently(numbers, pivot + 1, right);

    cout << "Уже отсортированный массив:" << endl;
    for (int i = 0; i < SIZE; i++)
    {
        cout << numbers[i] << " ";
    }
    cout << endl;
}

void Quick_Sort_Parallel(int* numbers, int left, int right,int thrs)
{
    omp_set_num_threads(thrs); //колличество использованных потоков
#pragma omp parallel for reduction(+: integral) //прагма дает инструкции компилятору
    int pivot; // разрешающий элемент
    int l_hold = left; //левая граница
    int r_hold = right; // правая граница
    pivot = numbers[left];
    while (left < right) // пока границы не сомкнутся
    {
        while ((numbers[right] >= pivot) && (left < right))
            right--; // сдвигаем правую границу пока элемент [right] больше [pivot]
        if (left != right) // если границы не сомкнулись
        {
            numbers[left] = numbers[right]; // перемещаем элемент [right] на место разрешающего
            left++; // сдвигаем левую границу вправо
        }
        while ((numbers[left] <= pivot) && (left < right))
            left++; // сдвигаем левую границу пока элемент [left] меньше [pivot]
        if (left != right) // если границы не сомкнулись
        {
            numbers[right] = numbers[left]; // перемещаем элемент [left] на место [right]
            right--; // сдвигаем правую границу вправо
        }
    }
    numbers[left] = pivot; // ставим разрешающий элемент на место
    pivot = left;
    left = l_hold;
    right = r_hold;
    if (left < pivot) // Рекурсивно вызываем сортировку для левой и правой части массива
        Quick_Sort_Consistently(numbers, left, pivot - 1);
    if (right > pivot)
        Quick_Sort_Consistently(numbers, pivot + 1, right);
}

void Bubble_Sort(int i)
{
    double t1 = omp_get_wtime();
    double t2 = omp_get_wtime();

    if (i == 1)
    {
        srand(time(NULL));
        int randomDigits[SIZE] = {};
        for (int i = 0; i < SIZE; i++)
        {
            randomDigits[i] = rand() % 10001;
        }
        cout << endl;
        
        t1 = omp_get_wtime() - t1;

        Bubble_Sort_Consistently(randomDigits);

        cout << "Время = " << t1 << "\n\n";

    }
    if (i == 2)
    {
        srand(time(NULL));
        int randomDigits1[SIZE] = {};
        for (int i = 0; i < SIZE; i++)
        {
            randomDigits1[i] = rand() % 10001;
        }
        cout << endl;

        Bubble_Sort_Parallel(randomDigits1,8);
        t1 = omp_get_wtime() - t1;

        cout << "Время = " << t1 << endl;
        cout << "Потоков = 8\n\n";

        int randomDigits2[SIZE] = {};
        for (int i = 0; i < SIZE; i++)
        {
            randomDigits2[i] = rand() % 10001;
        }
        cout << endl;

        Bubble_Sort_Parallel(randomDigits2, 16);
        t2 = omp_get_wtime() - t2;

        cout << "Время = " << t2 << endl;
        cout << "Потоков = 16\n\n";

    }
}

void Shell_Sort(int i)
{
    double t1 = omp_get_wtime();
    double t2 = omp_get_wtime();

    if (i == 1)
    {
        srand(time(NULL));
        int randomDigits[SIZE] = {};
        for (int i = 0; i < SIZE; i++)
        {
            randomDigits[i] = rand() % 10001;
        }
        cout << endl;

        t1 = omp_get_wtime() - t1;

        Shell_Sort_Consistently(randomDigits);

        cout << "Время = " << t1 << "\n\n";

    }
    if (i == 2)
    {
        srand(time(NULL));
        int randomDigits1[SIZE] = {};
        for (int i = 0; i < SIZE; i++)
        {
            randomDigits1[i] = rand() % 10001;
        }
        cout << endl;

        Shell_Sort_Parallel(randomDigits1, 8);
        t1 = omp_get_wtime() - t1;

        cout << "Время = " << t1 << endl;
        cout << "Потоков = 8\n\n";

        int randomDigits2[SIZE] = {};
        for (int i = 0; i < SIZE; i++)
        {
            randomDigits2[i] = rand() % 10001;
        }
        cout << endl;

        Shell_Sort_Parallel(randomDigits2, 16);
        t2 = omp_get_wtime() - t2;

        cout << "Время = " << t2 << endl;
        cout << "Потоков = 16\n\n";

    }
}

void Quick_Sort(int i)
{
    double t1 = omp_get_wtime();
    double t2 = omp_get_wtime();

    if (i == 1)
    {
        srand(time(NULL));
        int randomDigits[SIZE] = {};
        for (int i = 0; i < SIZE; i++)
        {
            randomDigits[i] = rand() % 10001;
        }
        cout << endl;

        t1 = omp_get_wtime() - t1;

        Quick_Sort_Consistently(randomDigits,0, SIZE - 1);

        cout << "Время = " << t1 << "\n\n";

    }
    if (i == 2)
    {
        srand(time(NULL));
        int randomDigits1[SIZE] = {};
        for (int i = 0; i < SIZE; i++)
        {
            randomDigits1[i] = rand() % 10001;
        }
        cout << endl;

        Quick_Sort_Parallel(randomDigits1, 0, SIZE - 1,8);
        t1 = omp_get_wtime() - t1;

        cout << "Уже отсортированный массив:" << endl;
        for (int i = 0; i < SIZE; i++)
        {
            cout << randomDigits1[i] << " ";
        }
        cout << endl;

        cout << "Время = " << t1 << endl;
        cout << "Потоков = 8\n\n";

        int randomDigits2[SIZE] = {};
        for (int i = 0; i < SIZE; i++)
        {
            randomDigits2[i] = rand() % 10001;
        }
        cout << endl;

        Quick_Sort_Parallel(randomDigits2, 0, SIZE - 1, 16);
        t2 = omp_get_wtime() - t2;

        cout << "Уже отсортированный массив:" << endl;
        for (int i = 0; i < SIZE; i++)
        {
            cout << randomDigits2[i] << " ";
        }
        cout << endl;

        cout << "Время = " << t2 << endl;
        cout << "Потоков = 16\n\n";

    }
}

void switcher()
{
    int choser;
    cout << "Выберите интересующий вариант:\n" <<
        "1 - Сортировка пузырьком (последовательный метод)\n" <<
        "2 - Сортировка пузырьком (параллельный метод)\n" <<
        "3 - Сортировка Шелла (последовательный метод)\n" <<
        "4 - Сортировка Шелла (параллельный метод)\n" <<
        "5 - Быстрая сортировка (последовательный метод)\n" <<
        "6 - Быстрая сортировка (параллельный метод)\n" <<
        "\nДля отмены выберите любую другую цифру!\n";
    cin >> choser;

    system("cls");

    switch (choser)
    {
    case(1):
    {
        Bubble_Sort(1);
        switcher();
    }
    case(2):
    {
        Bubble_Sort(2);
        switcher();
    }
    case(3):
    {
        Shell_Sort(1);
        switcher();
    }
    case(4):
    {
        Shell_Sort(2);
        switcher();
    }
    case(5):
    {
        Quick_Sort(1);
        switcher();
    }
    case(6):
    {
        Quick_Sort(2);
        switcher();
    }
    default:
        break;
    }
}

int main()
{
    setlocale(LC_ALL, "");

    switcher();

    system("pause");
    return 0;
}