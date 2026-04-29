/*Написать параллельный вариант программы вычисления числа pi с применением
средств библиотеки MPI.

Сравнить время работы последовательной и параллельных программ OpenMP и
MPI.*/
#include <stdio.h>
#include <omp.h>

int main() {
    int n = 100000000;
    double h = 1.0 / (double)n;
    double sum = 0.0, pi;

    double start = omp_get_wtime();

#pragma omp parallel for reduction(+:sum)
    for (int i = 1; i <= n; i++) {
        double x = h * ((double)i - 0.5);
        sum += 4.0 / (1.0 + x * x);
    }

    pi = h * sum;

    double end = omp_get_wtime();

    printf("OpenMP pi = %.16f\n", pi);
    printf("Time: %f seconds\n", end - start);

    return 0;
}