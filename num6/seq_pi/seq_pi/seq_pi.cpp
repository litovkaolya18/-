/*Написать параллельный вариант программы вычисления числа pi с применением
средств библиотеки MPI.

Сравнить время работы последовательной и параллельных программ OpenMP и
MPI.*/
#include <stdio.h>
#include <time.h>

int main() {
    int n = 100000000;
    double h = 1.0 / (double)n;
    double sum = 0.0, x, pi;

    clock_t start = clock();

    for (int i = 1; i <= n; i++) {
        x = h * ((double)i - 0.5);
        sum += 4.0 / (1.0 + x * x);
    }

    pi = h * sum;

    clock_t end = clock();
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Sequential pi = %.16f\n", pi);
    printf("Time: %f seconds\n", seconds);

    return 0;
}