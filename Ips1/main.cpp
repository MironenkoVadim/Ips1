#include <iostream>
#include <chrono>
#include <locale.h>
#include <cilk/cilk.h>
#include <cilk/reducer_opadd.h>
#include <fstream>

const std::size_t N = 1e6;

const double a = 0;
const double b = 1;

//! Integral = 2 * M_PI = 6.2832
double func(double x)
{
	return 8 / sqrt(1 + x * x);
}

//! middle rectangles
//! Sequential Integral Calculation
double sequential_intergal(int n)
{
	double sum = 0.0;
	double h = (b - a) / (n - 1);
	for (auto i = 0; i < n; i++)
		sum += func(i * h) + func((i + 1)*h);
	return sum * h / 2.0;
}

//! Parallel Integral Calculation
double parallel_integral(int n)
{
	double h = (b - a) / n;
	cilk::reducer_opadd<double> sum(0.0);
	cilk_for (auto i = 0; i < n; i++)
	{
		sum += func(i * h) + func((i + 1)*h);
	}
	return sum->get_value()*h / 2.0;
}

//! Print 
void write_result(const char *filename, int n, double time_work)
{
	std::ofstream timework(filename, std::ios::app, std::ios::trunc);
	timework << n << " " << time_work << "\n";
}

int main()
{
	std::chrono::high_resolution_clock::time_point t_begin = std::chrono::high_resolution_clock::now();
	double result = sequential_intergal(N);
	std::chrono::duration<double> elapsed_time_serial = std::chrono::high_resolution_clock::now() - t_begin;
	std::cout << "Integral = " << result << std::endl;
	std::cout << "Elapsed time = " << elapsed_time_serial.count() << std::endl;

	t_begin = std::chrono::high_resolution_clock::now();
	result = parallel_integral(N);
	std::chrono::duration<double> elapsed_time_parallel = std::chrono::high_resolution_clock::now() - t_begin;
	std::cout << "Parallel Integral = " << result << std::endl;
	std::cout << "Elapsed time = " << elapsed_time_parallel.count() << std::endl;

	std::cout << "Boost = " << elapsed_time_serial.count() / elapsed_time_parallel.count() << std::endl;

	for (auto i = 100; i < N*2; i *= 2)
	{
		t_begin = std::chrono::high_resolution_clock::now();
		result = parallel_integral(i);
		elapsed_time_parallel = std::chrono::high_resolution_clock::now() - t_begin;
		write_result("parallel.txt", i, elapsed_time_parallel.count());
	}

	for (auto i = 100; i < N*2; i *= 2)
	{
		t_begin = std::chrono::high_resolution_clock::now();
		result = sequential_intergal(i);
		elapsed_time_serial = std::chrono::high_resolution_clock::now() - t_begin;
		write_result("serial.txt", i, elapsed_time_serial.count());
	}

	return 0;
}