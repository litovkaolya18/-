#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <ctime>
#include <windows.h>

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    const int n = 50'000'000; 
    double* A = new double[n];
    double* B = new double[n];
    double* Cadd = new double[n];
    double* Csub = new double[n];
    double* Cmul = new double[n];
    double* Cdiv = new double[n];

    // Заполнение массивов случайными числами
    for (int i = 0; i < n; ++i) {
        A[i] = std::rand() % 1000;        
        B[i] = (std::rand() % 999) + 1;   
    }

    
    // ПОСЛЕДОВАТЕЛЬНЫЙ ВАРИАНТ
    double start_seq = omp_get_wtime();
    for (int i = 0; i < n; ++i) {
        Cadd[i] = A[i] + B[i];
        Csub[i] = A[i] - B[i];
        Cmul[i] = A[i] * B[i];
        Cdiv[i] = A[i] / B[i];
    }
    double end_seq = omp_get_wtime();
    double time_seq = end_seq - start_seq;

    std::cout << "Время последовательного варианта = "
        << time_seq << " сек\n";


    // ПАРАЛЛЕЛЬНЫЙ ВАРИАНТ 
    // omp_set_num_threads(2);
    // omp_set_num_threads(4);
    omp_set_num_threads(8);

    double start_par = omp_get_wtime();
#pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        Cadd[i] = A[i] + B[i];
        Csub[i] = A[i] - B[i];
        Cmul[i] = A[i] * B[i];
        Cdiv[i] = A[i] / B[i];
    }
    double end_par = omp_get_wtime();
    double time_par = end_par - start_par;

    std::cout << "Время параллельного варианта = "
        << time_par << " сек\n";

    // проверка корректности: пересчитаем один элемент
    bool ok = true;
    for (int i = 0; i < 10; ++i) {
        double add = A[i] + B[i];
        double sub = A[i] - B[i];
        double mul = A[i] * B[i];
        double div = A[i] / B[i];
        if (Cadd[i] != add || Csub[i] != sub || Cmul[i] != mul || Cdiv[i] != div) {
            ok = false;
            break;
        }
    }
    std::cout << (ok ? "OK: результаты корректны\n" : "ERROR: некорректные результаты\n");

    delete[] A;
    delete[] B;
    delete[] Cadd;
    delete[] Csub;
    delete[] Cmul;
    delete[] Cdiv;

    return 0;
}
