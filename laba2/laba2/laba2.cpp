#include <iostream>
#include <omp.h>
#include <windows.h>
#include <cstdlib>   // rand()
#include <ctime>     // clock()

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    const int n = 20000000;
    int* a = new int[n];

    // --- Заполнение массива случайными числами ---
    for (int i = 0; i < n; i++) {
        a[i] = rand() % 100; // числа от 0 до 99
    }

    //Последовательная сумма
    clock_t start_seq = clock();

    long long sum_seq = 0;
    for (int i = 0; i < n; i++) {
        sum_seq += a[i];
    }

    clock_t end_seq = clock();
    double time_seq = double(end_seq - start_seq) / CLOCKS_PER_SEC;

    std::cout << "Последовательная сумма = " << sum_seq << "\n";
    std::cout << "Время (последовательно) = " << time_seq << " сек\n\n";

    // Параллельная сумма
    //omp_set_num_threads(2);
    //omp_set_num_threads(4);
    omp_set_num_threads(8);
    //omp_set_num_threads(16);

    double start_par = omp_get_wtime();

    long long sum_par = 0;

#pragma omp parallel for reduction(+:sum_par)
    for (int i = 0; i < n; i++) {
        sum_par += a[i];
    }

    double end_par = omp_get_wtime();
    double time_par = end_par - start_par;

    std::cout << "Параллельная сумма = " << sum_par << "\n";
    std::cout << "Время (параллельно) = " << time_par << " сек\n";

    delete[] a;
    return 0;
}
