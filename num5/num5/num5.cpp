/*Написать последовательный вариант программы вычисления числа pi
Написать параллельный вариант программы вычисления числа pi с применением
средств библиотеки OpenMP.

Сравнить время работы последовательной и параллельной программ.
*/
#include <iostream>
#include <ctime>
//последовательный
int main() {
    long long num_steps = 100000000;
    double step = 1.0 / (double)num_steps;
    double sum = 0.0;

    clock_t start = clock();

    for (long long i = 0; i < num_steps; i++) {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }

    double pi = sum * step;

    clock_t end = clock();
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;

    std::cout << "Number pi = " << pi << std::endl;
    std::cout << "Time of action : " << seconds << " seconds\n";

    return 0;
}
