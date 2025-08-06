#pragma once

#include <vector>
#include <stdexcept>

namespace Differentiation {

    double forward_differential(double (*f)(double), double x, double h, int order);
    double centered_differential(double (*f)(double), double x, double h, int order);
    double backward_differential(double (*f)(double), double x, double h, int order);

    double forward_differential(const std::vector<double>& f_values, int index, int order);
    double centered_differential(const std::vector<double>& f_values, int index, int order);
    double backward_differential(const std::vector<double>& f_values, int index, int order);

}
