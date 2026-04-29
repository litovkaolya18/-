/*Написать параллельный вариант программы вычисления числа  с применением
CUDA C. Сравнить время работы последовательной, параллельных программ
OpenMP, MPI, CUDA.
*/
#include <iostream>
#include <omp.h>

//параллельная версия
int main() {
    int n = 100000000;
    double h = 1.0 / n;
    double sum = 0.0;

    double start = omp_get_wtime();

#pragma omp parallel for reduction(+:sum)
    for (int i = 1; i <= n; i++) {
        double x = h * (i - 0.5);
        sum += 1.0 / (1.0 + x * x);
    }

    double pi = 4.0 * h * sum;

    double end = omp_get_wtime();

    std::cout << "pi (OpenMP): " << pi << std::endl;
    std::cout << "Time OpenMP: " << (end - start) << " sec" << std::endl;

    return 0;
}
