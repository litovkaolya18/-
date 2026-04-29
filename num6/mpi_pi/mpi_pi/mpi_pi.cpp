/*Написать параллельный вариант программы вычисления числа pi с применением
средств библиотеки MPI.

Сравнить время работы последовательной и параллельных программ OpenMP и
MPI.*/
#include <stdio.h>
#include "mpi.h"

int main(int argc, char* argv[]) {
    int n = 100000000;
    double h = 1.0 / (double)n;
    double local_sum = 0.0, x, pi;
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double start = MPI_Wtime();

    int chunk = n / size;
    int start_i = rank * chunk + 1;
    int end_i = (rank == size - 1) ? n : (rank + 1) * chunk;

    for (int i = start_i; i <= end_i; i++) {
        x = h * ((double)i - 0.5);
        local_sum += 4.0 / (1.0 + x * x);
    }

    double global_sum = 0.0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    double end = MPI_Wtime();

    if (rank == 0) {
        pi = h * global_sum;
        printf("MPI pi = %.16f\n", pi);
        printf("Time: %f seconds\n", end - start);
    }

    MPI_Finalize();
    return 0;
}
