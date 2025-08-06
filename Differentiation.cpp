#include <iostream>
#include <vector>
#include <stdexcept>
#include <cmath>

namespace Differentiation
{
	/*
	 Numerical Differentiation using Centered Finite Difference Method (Support for up to 4th order derivatives)

	 TODO: Test the implementation with various functions and edge cases.
	 TODO: Investigate adding boilerplate template code for DRY code and reusability.
	*/

	/*
	Source: https://engcourses-uofa.ca/books/numericalanalysis/numerical-differentiation/high-accuracy-numerical-differentiation-formulas/
	
	forward_difference_1st_order
		f'(x) = -f(x+2) + 4f(x+1) - 3f(x) / 2h	
	centered_difference_1st_order
		 f'(x) = f(x-2) + 8f(x+1) - 8f(x-1) - f(x+2) / 12h
	backward_difference_1st_order
		f'(x) = f(x-2) - 4f(x-1) + 3f(x) / 2h	
	
	forward_difference_2nd_order
		f''(x) = -f(x+3) + 4f(x+2) - 5f(x+1) + 2f(x) / h^2
	centered_difference_2nd_order
		f''(x) = -f(x-2) + 16f(x+1) - 30f(x) + 16f(x-1) - f(x+2) / 12h^2
	backward_difference_2nd_order
		f''(x) = -f(x-3) + 4f(x-2) - 5f(x-1) + 2f(x) / h^2
	
	forward_difference_3rd_order
		f'''(x) = (-3f(x+4) + 14f(x+3) - 24f(x+2) + 18f(x+1) - 5f(x)) / 2h^3
	centered_difference_3rd_order
		f'''(x) = (f(x-3) - f(x+3)) + 8(f(x+2) - f(x-2)) + 13(f(x-1) - f(x+1)) / 8h^3
	backward_difference_3nd_order
		f'''(x) = (3f(x-4) - 14f(x-3) + 24f(x-2) - 18f(x-1) + 5f(x)) / 2h^3

	forward_difference_4th_order
		f''''(x) = -2f(x+5) + 11f(x+4) - 24f(x+3) + 26f(x+2) - 14f(x+1) + 3f(x) / h^4
	centered_difference_4th_order
		f''''(x) = -(f(x+3) + f(x-3)) + 12(f(x+2) + f(x-2)) - 39(f(x+1) + f(x-1)) + 56f(x) / 6h^4
	backward_difference_4th_order
		f''''(x) = -2f(x-5) + 11f(x-4) - 24f(x-3) + 26f(x-2) - 14f(x-1) + 3f(x) / h^4
	
	
	Algorithm:
	1. Define the function f(x) as a function pointer or vector of function values.
	
	2. Define the step size h if a function pointer is used, else use the indices of the vector 
	   to calculate the step size based on the mean distance between points. (mean because it is the maximum likehood estimator of the step size)
	
	3. Implement the formulas for each order of derivative as functions if using function pointers else use the vector indices to access the function values.
		Note: Use Forward or Backward difference methods onle near the boundaries of the data set where points are not available for centered difference.
	4. Return the calculated derivative value as a double.
	*/

	// Function pointer type for the function to differentiate
	// Arguments: Function pointer to the function to differentiate, x: point at which to evaluate the derivative, h: step size, n: order of derivative
	double forward_differential(double (*f)(double), double x, double h, int n)
	{
		switch (n) {
		case 1:
			return (-f(x + 2 * h) + 4 * f(x + h) - 3 * f(x)) / (2 * h);
		case 2:
			return (-f(x + 3 * h) + 4 * f(x + 2 * h) - 5 * f(x + h) + 2 * f(x)) / (h * h);
		case 3:
			return (-3 * f(x + 4 * h) + 14 * f(x + 3 * h) - 24 * f(x + 2 * h) + 18 * f(x + h) - 5 * f(x)) / (2 * h * h * h);
		case 4:
			return (-2 * f(x + 5 * h) + 11 * f(x + 4 * h) - 24 * f(x + 3 * h) + 26 * f(x + 2 * h) - 14 * f(x + h) + 3 * f(x)) / (h * h * h * h);
		default:
			throw std::invalid_argument("Unsupported derivative order");
		}
	}
	double centered_differential(double (*f)(double), double x, double h, int n)
	{
		switch (n) {
		case 1:
			return (f(x - 2 * h) + 8 * f(x + h) - 8 * f(x - h) - f(x + 2 * h)) / (12 * h);
		case 2:
			return (-f(x - 2 * h) + 16 * f(x + h) - 30 * f(x) + 16 * f(x - h) - f(x + 2 * h)) / (12 * h * h);
		case 3:
			return (f(x - 3 * h) - f(x + 3 * h) + 8 * (f(x + 2 * h) - f(x - 2 * h)) + 13 * (f(x - h) - f(x + h))) / (8 * h * h * h);
		case 4:
			return -(f(x + 3 * h) + f(x - 3 * h)) + 12 * (f(x + 2 * h) + f(x - 2 * h)) - 39 * (f(x + h) + f(x - h)) + 56 * f(x) / (6 * h * h * h * h);
		default:
			throw std::invalid_argument("Unsupported derivative order");
		}
	}
	double backward_differential(double (*f)(double), double x, double h, int n)
	{
		switch (n) {
		case 1:
			return (f(x - 2 * h) - 4 * f(x - h) + 3 * f(x)) / (2 * h);
		case 2:
			return (-f(x - 3 * h) + 4 * f(x - 2 * h) - 5 * f(x - h) + 2 * f(x)) / (h * h);
		case 3:
			return (3 * f(x - 4 * h) - 14 * f(x - 3 * h) + 24 * f(x - 2 * h) - 18 * f(x - h) + 5 * f(x)) / (2 * h * h * h);
		case 4:
			return (-2 * f(x - 5 * h) + 11 * f(x - 4 * h) - 24 * f(x - 3 * h) + 26 * f(x - 2 * h) - 14 * f(x - h) + 3 * f(x)) / (h * h * h * h);
		default:
			throw std::invalid_argument("Unsupported derivative order");
		}
	}
	// Function is a vector of function values
	// Arguments: Vector of function values, x: point at which to evaluate the derivative, h: step size, n: order of derivative
	double forward_differential(const std::vector<double>& f, int x, int n) 
	{
		double h = 0.0;
		for(int i = 0; i < f.size() - 1; i++)
		{
			h += f[i + 1] - f[i];
		}
		h /= (f.size() - 1); // Mean step size

		switch (n)
		{
		case 1:
			if (x + 2 >= f.size()) {
				throw std::out_of_range("Index out of range for forward difference");
			}
			return (-f[x + 2] + 4 * f[x + 1] - 3 * f[x]) / (2 * h);
		case 2:
			if (x + 3 >= f.size()) {
				throw std::out_of_range("Index out of range for forward difference");
			}
			return (-f[x + 3] + 4 * f[x + 2] - 5 * f[x + 1] + 2 * f[x]) / (h * h);
		case 3:
			if (x + 4 >= f.size()) {
				throw std::out_of_range("Index out of range for forward difference");
			}
			return (-3 * f[x + 4] + 14 * f[x + 3] - 24 * f[x + 2] + 18 * f[x + 1] - 5 * f[x]) / (2 * h * h * h);
		case 4:
			if (x + 5 >= f.size()) {
				throw std::out_of_range("Index out of range for forward difference");
			}
			return (-2 * f[x + 5] + 11 * f[x + 4] - 24 * f[x + 3] + 26 * f[x + 2] - 14 * f[x + 1] + 3 * f[x]) / (h * h * h * h);
		default:
			throw std::invalid_argument("Unsupported derivative order");
		}
	}
	double centered_differential(const std::vector<double>& f, int x, int n) 
	{
		double h = 0.0;
		for(int i = 0; i < f.size() - 1; i++)
		{
			h += f[i + 1] - f[i];
		}
		h /= (f.size() - 1); // Mean step size
		switch (n)
		{
		case 1:
			if (x - 2 < 0 || x + 2 >= f.size()) {
				throw std::out_of_range("Index out of range for centered difference");
			}
			return (f[x - 2] + 8 * f[x + 1] - 8 * f[x - 1] - f[x + 2]) / (12 * h);
		case 2:
			if (x - 2 < 0 || x + 2 >= f.size()) {
				throw std::out_of_range("Index out of range for centered difference");
			}
			return (-f[x - 2] + 16 * f[x + 1] - 30 * f[x] + 16 * f[x - 1] - f[x + 2]) / (12 * h * h);
		case 3:
			if (x - 3 < 0 || x + 3 >= f.size()) {
				throw std::out_of_range("Index out of range for centered difference");
			}
			return (f[x - 3] - f[x + 3] + 8 * (f[x + 2] - f[x - 2]) + 13 * (f[x - 1] - f[x + 1])) / (8 * h * h * h);
		case 4:
			if (x - 3 < 0 || x + 3 >= f.size()) {
				throw std::out_of_range("Index out of range for centered difference");
			}
			return -(f[x + 3] + f[x - 3]) + 12 * (f[x + 2] + f[x - 2]) - 39 * (f[x + 1] + f[x - 1]) + 56 * f[x] / (6 * h * h * h * h);
		default:
			throw std::invalid_argument("Unsupported derivative order");
		}
	}
	double backward_differential(const std::vector<double>& f, int x, int n) 
	{
		double h = 0.0;
		for(int i = 0; i < f.size() - 1; i++)
		{
			h += f[i + 1] - f[i];
		}
		h /= (f.size() - 1); // Mean step size
		switch (n)
		{
		case 1:
			if (x - 2 < 0) {
				throw std::out_of_range("Index out of range for backward difference");
			}
			return (f[x - 2] - 4 * f[x - 1] + 3 * f[x]) / (2 * h);
		case 2:
			if (x - 3 < 0) {
				throw std::out_of_range("Index out of range for backward difference");
			}
			return (-f[x - 3] + 4 * f[x - 2] - 5 * f[x - 1] + 2 * f[x]) / (h * h);
		case 3:
			if (x - 4 < 0) {
				throw std::out_of_range("Index out of range for backward difference");
			}
			return (3 * f[x - 4] - 14 * f[x - 3] + 24 * f[x - 2] - 18 * f[x - 1] + 5 * f[x]) / (2 * h * h * h);
		case 4:
			if (x - 5 < 0) {
				throw std::out_of_range("Index out of range for backward difference");
			}
			return (-2 * f[x - 5] + 11 * f[x - 4] - 24 * f[x - 3] + 26 * f[x - 2] - 14 * f[x - 1] + 3 * f[x]) / (h * h * h * h);
		default:
			throw std::invalid_argument("Unsupported derivative order");
		}
	}
};

