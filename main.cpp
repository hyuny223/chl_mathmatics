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
}

int main()
{
    type::matrix_t<double> mat(2,2);
    mat = {1,2,3,4};

    std::cout << mat << std::endl;
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

    // type::matrix_t<double> mat(3,3);
    // mat = {-2.0, -5.0, 2.0,
    //         1.0, 3.0, 0.0,
    //         0.0, 1.0, 3.0};
    // std::tuple args{-3.0, 4.0, 6.0};
    // gauss_jordar_elimination(args, mat);

    // mat.resize(3, type::matrix<double>(4,0));

    // type::compile_loop<3>([&args, &mat](auto i)
    // {
    //     // mat(i.value,3) = std::get<i.value>(args);
    // });
    // auto m = std::shared_ptr<int>(3);

    // std::cout << *m << std::endl;

}
