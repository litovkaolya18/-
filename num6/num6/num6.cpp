/*Дан фрагмент программы обмена процессов сообщениями. Запустить программу при
разных количествах процессов (1, 2, 4, 8, 16), проанализировать результаты. Запускать в
командной строке: mpiexec -n 2 ./имя исполняемого файла*/

#include <iostream>
#include "mpi.h"
using namespace std;

int main()
{
	cout << "without MPI\n";
	int size, rank, a, b;
	MPI_Status status;
	MPI_Init(NULL, NULL);
	cout << "with MPI\n";
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	a = rank;
	b = -1;
	cout << size << endl;
	if ((rank % 2) == 0) {
		if (rank < size - 1) MPI_Send(&a, 1, MPI_INT, rank + 1, 5, MPI_COMM_WORLD);
	}
	else MPI_Recv(&b, 1, MPI_INT, rank - 1, 5, MPI_COMM_WORLD, &status);
	std::cout << "process " << rank << " a = " << a << " b = " << b << endl;
	MPI_Finalize();
	cout << "without MPI\n";
	return 0;
}