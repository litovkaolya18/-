// Написать параллельный вариант программы вычисления числа  с применением
// CUDA C.Сравнить время работы последовательной, параллельных программ
// OpenMP, MPI, CUDA.
#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int n = 100000000;
    double h = 1.0 / n;

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double local_sum = 0.0;

    double start = MPI_Wtime();

    for (int i = rank + 1; i <= n; i += size) {
        double x = h * (i - 0.5);
        local_sum += 1.0 / (1.0 + x * x);
    }

    double global_sum = 0.0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    double end = MPI_Wtime();

    if (rank == 0) {
        double pi = 4.0 * h * global_sum;
        std::cout << "pi (MPI): " << pi << std::endl;
        std::cout << "Time MPI: " << (end - start)  << " sec" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
