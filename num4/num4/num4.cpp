/*Написать последовательный вариант программы вычисления числа pi
Написать параллельный вариант программы вычисления числа pi с применением
средств библиотеки OpenMP.

Сравнить время работы последовательной и параллельной программ.
*/
#include <iostream>
#include <omp.h>
//параллельный
int main() {
	long long num_steps = 100000000;
	double step = 1.0 / (double)num_steps;
	double sum = 0.0;

	double start = omp_get_wtime();
	
#pragma omp parallel
	{
		double local_sum = 0.0;

#pragma omp for
		for (long long i = 0; i < num_steps; i++) {
			double x = (i + 0.5) * step;
			local_sum += 4.0 / (1.0 + x * x);
		}

		sum += local_sum;
	}
	double pi = sum * step;
	double end = omp_get_wtime();
	std::cout << "Number pi = " << pi << std::endl;
	std::cout << "Time of action " << end - start << " sec.\n";

	return 0;
}