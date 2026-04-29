//Реализовать в среде программирования пример гибридной программы с
//использованием MPI и OpenMP : суммирование элементов массива.
#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <vector>
#include <windows.h>

int main(int argc, char** argv) {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 100000000; // 100 млн элементов
    int chunk = N / size;    // сколько элементов на процесс

    // Выделяем память под локальный кусок массива
    std::vector<int> local(chunk);

    // Инициализация массива (только процесс 0)
    std::vector<int> full;
    if (rank == 0) {
        full.resize(N);
        for (int i = 0; i < N; i++)
            full[i] = 1; // можно любые числа
    }

    // Рассылаем части массива процессам
    MPI_Scatter(full.data(), chunk, MPI_INT,
        local.data(), chunk, MPI_INT,
        0, MPI_COMM_WORLD);

    double start = MPI_Wtime();

    long long local_sum = 0;

    // Параллельная обработка OpenMP внутри процесса
#pragma omp parallel for reduction(+:local_sum)
    for (int i = 0; i < chunk; i++) {
        local_sum += local[i];
    }

    long long global_sum = 0;

    // Сбор результатов от всех процессов
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    double end = MPI_Wtime();

    if (rank == 0) {
        std::cout << "Hybrid MPI + OpenMP" << std::endl;
        std::cout << "The sum of the array elements: " << global_sum << std::endl;
        std::cout << "Working hours: " << (end - start) * 1000 << " ms" << std::endl;
    }

    MPI_Finalize();
    return 0;
}





/* 1. MPI запускает, например, 4 процесса
2. Каждый процесс получает свой кусок массива
3. Внутри каждого процесса запускается OpenMP‑цикл #pragma omp parallel for
4. Каждый процесс считает свою часть быстрее
5. MPI собирает результаты через MPI_Reduce
*/

/*MPI распределяет массив между процессами, а внутри каждого процесса OpenMP параллелит суммирование по потокам.
Таким образом достигается двухуровневая параллельность: межпроцессная и внутрипроцессная.
*/

//OpenMP — параллелит работу внутри каждого процесса
//MPI — распределяет работу между процессами
//MPI делит массив на части → каждый процесс считает свою часть → внутри процесса OpenMP ускоряет вычисления потоками