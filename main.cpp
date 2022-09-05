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

    std::tuple val{4.0, 3.0};

    auto jacobian = create_jacobian_matrix(fs, val);

    std::cout << jacobian << std::endl;
}


int main()
{
    std::tuple t{-200.0, -20.0};

    cv::Mat board(cv::Size(400,200), CV_8UC3, cv::Scalar::all(255));

    cv::Point2d original_xy{50, 50};
    cv::Point2d target_xy{300,100};

    cv::circle(board, original_xy, 10, cv::Scalar(0,255,0),3);
    cv::circle(board, target_xy, 10, cv::Scalar(0,0,255),3);
    cv::circle(board, cv::Point2d(static_cast<int>(300 - 253.125),static_cast<int>(100 - 73.125)), 10, cv::Scalar(255,0,0),3);

    cv::imshow("board", board);
    while(1)
    {
        if(cv::waitKey(0) == 27)
        {
            break;
        }
    }

    // std::tuple function{
    //     [](long double tx, long double ty)
    //     {
    //         return (50 - (300+tx))*(50 - (300+tx)) + (50 - (100+ty))*(50 - (100+ty));
    //     }
    // };

    // auto y = gauss_newton_method(function, t);



}
