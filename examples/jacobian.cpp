#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>

#include <opencv2/opencv.hpp>

#include <eigen-3.4.0/Eigen/Dense>
#include <eigen-3.4.0/Eigen/Jacobi>

#include "compute.h"
#include "five_points.h"



void solve()
{
    // auto f= [](double x, double y)
    // {
    //     // return x*x + 3.0*x*y + y - 1.0;
    //     return y - x*x;
    // };

    // auto dx_y = [&f](double y_fix)
    // {
    //     return [&](double x)
    //     {
    //         return f(x, y_fix);
    //     };
    // };

    // auto f = [](auto x)
    // {
    //     return (x-3.0)*(x-5.0);
    // };

    // auto ans = newton_method(-10.0, f);
    // std::cout << ans << std::endl;

    auto f = [](auto&& x, auto&&y)
    {
        return x*x*y;
    };

    // auto f_y = [&f](auto&& y_fix)
    // {
    //     return [&](double x)
    //     {
    //         return f(x, y_fix);
    //     };
    // };

    // auto f_x = [&f](auto&& x_fix)
    // {
    //     return [&](double y)
    //     {
    //         return f(x_fix, y);
    //     };
    // };

    // auto dx = diff_stencil(2.0, f_y(3.0));
    // auto dy = diff_stencil(3.0, f_x(2.0));

    // std::cout << dx << ", " << dy << std::endl;

    std::tuple val{2.0, 3.0};

    // auto fx = fix_arguments_other_than_nth<0>(f, 0.0, 3.0);
    // auto fy = fix_arguments_other_than_nth<1>(f, 1.0, 0.0);

    // auto xx = diff_stencil(fx, 1.0);
    // auto yy = diff_stencil(fy, 3.0);

//     auto xx = partial_derivative<0>(f, val);
//     auto yy = partial_derivative<1>(f, val);

//     std::cout << xx << ", " << yy << std::endl;

    std::tuple fs{
        [](double x, double y)
        {
            // return x*x + 3.0*x*y + y - 1.0;
            return x*x*y;
        }
    };
    auto jacobian = create_jacobian_matrix<std::vector<std::vector<double>>(fs, val);

}

// int main()
// {
//     solve();
// }
