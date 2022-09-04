#pragma once

#include <iostream>
#include <tuple>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <utility>

#include "five_points.hpp"
#include "compile_loop.hpp"
#include "matrix.hpp"

template < template<typename ...> class ContainerType, typename TupleType, typename... FuncTypes>
auto evaluate_functions(TupleType&& arguments, FuncTypes&&... func)
{
    return ContainerType{std::apply(std::forward<FuncTypes>(func), std::forward<TupleType>(arguments))...};
}

template <auto N, typename FuncType, typename... ArgTypes>
    requires std::is_invocable_v<FuncType, ArgTypes...>
auto fix_arguments_other_than_nth(FuncType&& func, const std::tuple<ArgTypes...>& args)
{

    return [&func, args](auto nth_arg)
    {
        auto copied_args = args;
        std::get<N>(copied_args) = nth_arg;
        return std::apply(func, copied_args);
    };
}

template <auto N, typename FuncType, typename... ArgTypes>
    requires std::is_invocable_v<FuncType, ArgTypes...>
auto fix_arguments_other_than_nth(FuncType&& func, ArgTypes&&... args)
{
    return fix_arguments_other_than_nth<N>(func, std::tuple{ args... });
}

template <auto VariableIndex, typename FuncType, typename...ArgTypes>
    requires (std::is_invocable_v<FuncType, ArgTypes...> && (VariableIndex < sizeof...(ArgTypes)))
auto partial_derivative(FuncType&& function, const std::tuple<ArgTypes...>& args)
{
    auto fixed_func = fix_arguments_other_than_nth<VariableIndex>(function, args);
    return diff_stencil(fixed_func, std::get<VariableIndex>(args));
}

// template<typename ValueType, typename FuncType>
// ValueType newton_method(FuncType&& f, ValueType x)
// {
//     // constexpr auto ep = 3.0 * std::numeric_limits<ValueType>::epsilon();
//     auto ep = 0.01;
//     ValueType fx; // function f(x)
//     ValueType fp; // first order derivative of f(x)
//     while(fabs(fx = f(x)) > ep)
//     {
//         fp = diff_stencil_lower(x,f);
//         x = x - fx/ fp;
//     }
//     return x;
// }

/*
자코비안
1) std::tuple로 람다 함수를 받아들인다
2) 하나의 행에는 하나의 람다 함수에 대한 다변수 편미분 진행
3) 도출된 값을 vector로 넣거나, 아예 Eigen Matrix로 만들어도 좋을 것
일단은 vector로 진행 
*/

template <typename ContainerType, typename... FuncTypes, typename... ArgTypes, std::size_t f_size = sizeof...(FuncTypes), std::size_t arg_size = sizeof...(ArgTypes)>
ContainerType create_jacobian_matrix(const std::tuple<FuncTypes...>& function, const std::tuple<ArgTypes...>& args)
{
    ContainerType jacobian(f_size, arg_size);
    // jacobian.reserve(arg_size * arg_size);

    type::compile_loop<f_size>([&function, &args, &jacobian](auto i)
    {
        auto& f = std::get<i.value>(function);

        type::compile_loop<arg_size>([&](auto j)
        {
            auto d = partial_derivative<j.value>(f,args);
            jacobian(i.value, j.value) = d;
        });
    });
    return jacobian;
}

template<typename ContainerType>
void upper_triangular_matrix(ContainerType& container)
{

    auto pivot = [&](const std::size_t row)
    {
        auto maxi = -1;

        if(row == container.rows -1)
        {
            return;
        }

        for(std::size_t i = row; i < container.rows; ++i)
        {
            if(container(i, row) > maxi)
            {
                maxi = i;
            }
        }
        type::matrix<double> tmp = container[row];
        container[row] = container[maxi];
        container[maxi] = tmp;
        return;
    };

    for(int row = 0; row < container.rows; ++row)
    {
        pivot(row);
        auto d = container(row, row);
        for(int mid = row; mid < container.rows; ++mid)
        {
            auto m = container(mid, row);
            for(int col = 0; col < container.cols; ++col)
            {

                if(row == mid)
                {
                    container(mid, col) /= d;
                }
                else
                {
                    auto k = m / container(row, row);
                    container(mid,col) = container(mid,col) -k * container(row,col);
                }
            }
        }
    }
}

template<typename ContainerType>
void lower_triangular_matrix(ContainerType& container)
{
    for(int row = container.rows - 1; row > -1; --row)
    {
        auto d = container(row,row);
        for(int mid = row; mid > -1; --mid)
        {
            auto m = container(mid,row);
            // std::cout << row << mid << k << std::endl;
            for(int col = 0; col < container.cols; ++col)
            {
                if(row == mid)
                {
                    container(mid,col) /= d;
                }
                else
                {
                    auto k = m / container(row,row);
                    container(mid,col) = container(mid,col) -k * container(row,col);
                }
            }
        }
    }
};

template<typename... ArgTypes, std::size_t arg_size = sizeof...(ArgTypes)>
auto gauss_jordar_elimination(const std::tuple<ArgTypes...>& args, type::matrix_t<double>& mat)
{
    mat.resize(arg_size, type::matrix<double>(mat.cols + 1, 0));
    type::compile_loop<arg_size>([&args, &mat](auto i)
    {
        mat(i.value, mat.cols - 1) = std::get<i.value>(args);
    });
    upper_triangular_matrix(mat);
    lower_triangular_matrix(mat);

    return mat.slice(0,mat.rows,-1,mat.cols);
}

template<typename... FuncTypes, typename... ArgTypes>
auto newton_method_with_jacobian(const std::tuple<FuncTypes...>& functions, const std::tuple<ArgTypes...>& args)
{
    // constexpr auto ep = 3.0 * std::numeric_limits<ValueType>::epsilon();
    auto ep = 0.01;

    auto jacobian = create_jacobian_matrix<type::matrix_t<double>>(functions, args);
    auto trans_jacobian = jacobian.t();

    // std::tuple ans;

    // auto workhorse1 = [&functions, &args, &jacobian](auto i)
    // {
    //     constexpr auto row = type_extention::extract_value(i);
    //     auto& f = std::get<row>(functions);
    //     auto val = f(args);

    //     auto workhorse1 = [&](auto j)
    //     {
    //         constexpr auto col  = type_extention::extract_value(j);
    //         auto x = std::get<col>(args);


    //     };
    //     return 0;
    // };



    // ValueType fx; // function f(x)
    // ValueType fp; // first order derivative of f(x)

    // while(fabs(fx = f(x)) > ep)
    // {
    //     fp = diff_stencil_lower(x,f);
    //     x = x - fx/ fp;
    // }
    // return x;
}


// template <typename... FuncType, typename... ArgTypes, std::size_t Size = sizeof...(ArgTypes)>
// auto solve_nonlinear_equations_newton_method(const std::tuple<FuncType...>& functions, std::tuple<ArgTypes...> initial_values, int max_try = 20)
// {
//     int try_count = 0;

//     while(try_count < max_try)
//     {

//     }
// }
