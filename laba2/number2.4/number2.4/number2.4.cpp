#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <ctime>
#include <windows.h> 

int main() {
    SetConsoleOutputCP(1251); 
    SetConsoleCP(1251);

    std::srand((unsigned)std::time(nullptr));

    const int N = 1000;
    const int M = 1000;
    const int K = 1000;

    // Выделение памяти
    double** A = new double* [N];
    double** B = new double* [M];
    double** C = new double* [N];
    double** Cpar = new double* [N];

    for (int i = 0; i < N; i++) {
        A[i] = new double[M];
        C[i] = new double[K];
        Cpar[i] = new double[K];
    }
    for (int i = 0; i < M; i++)
        B[i] = new double[K];

    // Заполнение матриц случайными числами
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            A[i][j] = std::rand() % 100;

    for (int i = 0; i < M; i++)
        for (int j = 0; j < K; j++)
            B[i][j] = std::rand() % 100;


    // ПОСЛЕДОВАТЕЛЬНЫЙ ВАРИАНТ
    double t1 = omp_get_wtime();

    for (int i = 0; i < N; i++)
        for (int j = 0; j < K; j++) {
            double sum = 0;
            for (int t = 0; t < M; t++)
                sum += A[i][t] * B[t][j];
            C[i][j] = sum;
        }

    double t2 = omp_get_wtime();
    std::cout << "Время последовательного варианта = " << (t2 - t1) << " сек\n";

  
    // ПАРАЛЛЕЛЬНЫЙ ВАРИАНТ
    // omp_set_num_threads(2);
    // omp_set_num_threads(4);
    // omp_set_num_threads(8);

    double t3 = omp_get_wtime();

#pragma omp parallel for
    for (int i = 0; i < N; i++)
        for (int j = 0; j < K; j++) {
            double sum = 0;
            for (int t = 0; t < M; t++)
                sum += A[i][t] * B[t][j];
            Cpar[i][j] = sum;
        }

    double t4 = omp_get_wtime();
    std::cout << "Время параллельного варианта = " << (t4 - t3) << " сек\n";

    // Проверка корректности
    bool ok = true;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < K; j++)
            if (C[i][j] != Cpar[i][j]) ok = false;

    std::cout << (ok ? "OK: результаты совпадают\n" : "ERROR: результаты различаются\n");

    return 0;
}
