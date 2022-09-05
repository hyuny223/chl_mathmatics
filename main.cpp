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
    std::tuple functions{
        [](auto x1, auto x2, auto x3)
        {
            return 3.0 * x1 - std::cos(x2 * x3) - 0.5;
        },
        [](auto x1, auto x2, auto x3)
        {
            return x1 * x1 - 81.0 * (x2 + 0.1) * (x2 + 0.1) + std::sin(x3) - 1.06;
        },
        [](auto x1, auto x2, auto x3)
        {
            return std::exp(-x1 * x2) + 20.0 * x3 + (10.0 * std::numbers::pi - 3.0) / 3.0;
        }
    };

    std::tuple x{0.1, 0.1, -0.1};

    auto solution = solve_nonlinear_equations_newton_method(functions, x);
    // auto [a,b,c] = solution;

    // std::tuple functions{
    //     [](auto x1, auto x2)
    //     {
    //         return x1 + x2;
    //     },
    //     [](auto x1, auto x2)
    //     {
    //         return x1 - x2;
    //     },        [](auto x1, auto x2)
    //     {
    //         return x1 * x2;
    //     }
    // };

    // std::tuple args{2, 3};

    // auto ans = evaluate_lambdas<std::vector>(functions, args);

    // for(auto a : ans)
    // {
    //     std::cout << a << ", ";
    // }
    // std::cout << std::endl;


}
