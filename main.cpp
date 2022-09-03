#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>

#include <opencv2/opencv.hpp>

#include <eigen-3.4.0/Eigen/Dense>
#include <eigen-3.4.0/Eigen/Jacobi>

#include "compute.h"
#include "five_points.h"
#include "matrix.h"


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
}

int main()
{
    type::matrix_t<double> container(3,3);
    container = {-1.0, 1.0, 2.0,
                 3.0, -1.0, 1.0,
                 -1.0, 3.0, 4.0};

    std::cout << container << std::endl;

    auto inv = inverse(container);
    auto mat = transpose<type::matrix_t<double>>(inv);

    // solve();

    // std::tuple fs{
    //     [](int x, int y)
    //     {
    //         return x*x*y;
    //     },
    //     [](int x, int y)
    //     {
    //         return 2*x;
    //     },
    //     [](int x, int y)
    //     {
    //         return y*y;
    //     }
    // };

    // std::tuple x{3,2};

    // auto y = std::apply(fs,x);

    // std::cout << y << std::endl;

    // auto result = evaluate_functions<std::vector>(std::tuple{3,4},
    //         [](int x, int y){return x + y;},
    //         [](int x, int y){return x - y;},
    //         [](int x, int y){return x * y;},
    //         [](int x, int y){return x / y;}
    // );

    // for(auto a : result)
    // {
    //     std::cout << a << ", ";
    // }
}
