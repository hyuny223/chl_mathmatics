#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>

#include <opencv2/opencv.hpp>

#include <eigen-3.4.0/Eigen/Dense>
#include <eigen-3.4.0/Eigen/Jacobi>

#include "compute.hpp"
#include "five_points.hpp"
#include "matrix.hpp"
#include "compile_loop.hpp"


void solve()
{

    auto f = [](auto&& x, auto&&y)
    {
        return x*x*y;
    };

    std::tuple fs{
        [](double x, double y)
        {
            return x*x*y;
        }
    };

    std::tuple val{2.0, 3.0};

    auto jacobian = create_jacobian_matrix<type::matrix_t<double>>(fs, val);

    std::cout << jacobian << std::endl;
}

int main()
{

}
