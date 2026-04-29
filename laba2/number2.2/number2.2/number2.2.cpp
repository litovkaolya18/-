#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <ctime>
#include <windows.h>

//последовательная часть
void quicksort_seq(int* a, int left, int right) {
    if (left >= right) return;

    int i = left;
    int j = right;
    int pivot = a[(left + right) / 2];

    while (i <= j) {
        while (a[i] < pivot) i++;
        while (a[j] > pivot) j--;
        if (i <= j) {
            std::swap(a[i], a[j]);
            i++;
            j--;
        }
    }

    if (left < j)  quicksort_seq(a, left, j);
    if (i < right) quicksort_seq(a, i, right);
}

//параллельная часть
void quicksort_par(int* a, int left, int right, int depth = 0) {
    if (left >= right) return;

    int i = left;
    int j = right;
    int pivot = a[(left + right) / 2];

    while (i <= j) {
        while (a[i] < pivot) i++;
        while (a[j] > pivot) j--;
        if (i <= j) {
            std::swap(a[i], a[j]);
            i++;
            j--;
        }
    }

    // Ограничиваем глубину распараллеливания
    if (depth < 4) {
#pragma omp task
        quicksort_par(a, left, j, depth + 1);

#pragma omp task
        quicksort_par(a, i, right, depth + 1);
    }
    else {
        quicksort_seq(a, left, j);
        quicksort_seq(a, i, right);
    }
}

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    const int n = 5000000;
    int* a1 = new int[n];
    int* a2 = new int[n];

    // Заполнение массива случайными числами
    for (int i = 0; i < n; i++) {
        int v = rand();
        a1[i] = v;
        a2[i] = v;
    }

    // Последовательная сортировка
    double start_seq = omp_get_wtime();
    quicksort_seq(a1, 0, n - 1);
    double end_seq = omp_get_wtime();
    double time_seq = end_seq - start_seq;

    std::cout << "Время последовательной сортировки = " << time_seq << " сек\n";

    //Параллельная сортировка
    // omp_set_num_threads(2);
    // omp_set_num_threads(4);
     omp_set_num_threads(8);
    // omp_set_num_threads(16);

    double start_par = omp_get_wtime();

#pragma omp parallel
    {
#pragma omp single
        quicksort_par(a2, 0, n - 1);
    }

    double end_par = omp_get_wtime();
    double time_par = end_par - start_par;

    std::cout << "Время параллельной сортировки = " << time_par << " сек\n";

    // Проверка корректности
    bool ok = true;
    for (int i = 0; i < n; i++) {
        if (a1[i] != a2[i]) {
            ok = false;
            break;
        }
    }
    std::cout << (ok ? "OK: массивы отсортированы одинаково\n" : "ERROR: сортировки различаются!\n");

    delete[] a1;
    delete[] a2;
    return 0;
}
