/*Написать программу обмена процессов сообщениями. Процесс с четным номером
отправляет соседнему процессу (с нечетным номером) сообщение “Hello, I’m <rank>”, процесс
с нечетным номером отвечает процессу с четным номером “Nice to meet you. I’m <rank>”.*/
#include <mpi.h>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Status status;

    // Чётный процесс отправляет сообщение соседу справа
    if (rank % 2 == 0) {
        if (rank + 1 < size) {
            std::string msg = "Hello, I'm " + std::to_string(rank);
            MPI_Send(msg.c_str(), msg.size() + 1, MPI_CHAR, rank + 1, 0, MPI_COMM_WORLD);

            // Ждём ответ от соседа
            char buffer[100];
            MPI_Recv(buffer, 100, MPI_CHAR, rank + 1, 0, MPI_COMM_WORLD, &status);
            std::cout << "Process " << rank << " received: " << buffer << std::endl;
        }
    }
    // Нечётный процесс отвечает чётному слева
    else {
        // Получаем сообщение от соседа слева
        char buffer[100];
        MPI_Recv(buffer, 100, MPI_CHAR, rank - 1, 0, MPI_COMM_WORLD, &status);

        // Отправляем ответ
        std::string reply = "Nice to meet you. I'm " + std::to_string(rank);
        MPI_Send(reply.c_str(), reply.size() + 1, MPI_CHAR, rank - 1, 0, MPI_COMM_WORLD);

        std::cout << "Process " << rank << " received: " << buffer << std::endl;
    }

    MPI_Finalize();
    return 0;
}
//mpiexec -n 2 num6.1.exe
