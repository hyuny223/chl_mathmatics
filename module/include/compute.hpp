#pragma once

#include <iostream>
#include <tuple>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <utility>
#include <limits>
#include <functional>

#include "five_points.hpp"
#include "compile_loop.hpp"
#include "matrix.hpp"

// template < template<typename...> class ContainerType, typename TupleType, typename... FuncTypes>
// template <typename TupleType, typename... FuncTypes, std::size_t size=sizeof...(FuncTypes)>
// auto evaluate_functions(TupleType&& args, const std::tuple<FuncTypes...>& functions)
// {
//     std::vector<double> container;
//     type::compile_loop<size>([&functions, &args, &container](auto i)
//     {
//         auto& f = std::get<i.value>(functions);
//         container.push_back(std::apply( std::forward<double>(f), std::forward<TupleType>(args) ));
//     });
//     return container;
// }

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


template <typename... FuncType, typename... ArgTypes, std::size_t size = sizeof...(ArgTypes), std::size_t f_size = sizeof...(FuncType)>
auto gauss_newton_method(
    const std::tuple<FuncType...>& functions,
    std::tuple<ArgTypes...>& initial_values, int max_try = 20)
{
    // constexpr auto ep = 3.0 * std::numeric_limits<ValueType>::epsilon();
    auto ep = 0.01;

    type::matrix_t<long double> mat(1,2);
    type::compile_loop<size>([&mat, &initial_values](auto i)
    {
        mat(0,i.value) = std::get<i.value>(initial_values);
    });

    double fx;
    type::compile_loop<f_size>([&functions, &mat, &initial_values, &ep, &fx](auto i)
    {
        auto& f = std::get<i.value>(functions);
        while(fabs(fx = f(mat(0,0), mat(0,1))) > ep)
        {
            std::cout << "fx : " << fx << std::endl;
            auto J = create_jacobian_matrix(functions, initial_values);

            mat = mat - (((J.t()*J).inv())*J.t()*fx).t();
            std::cout << "mat : \n" << mat << std::endl;

            std::get<0>(initial_values) = mat(0,0);
            std::get<1>(initial_values) = mat(0,1);
            std::cout << "===========" << std::endl;
        }
    });

    return mat;
}

template <typename... FuncTypes, typename... ArgTypes, std::size_t f_size = sizeof...(FuncTypes), std::size_t arg_size = sizeof...(ArgTypes)>
auto create_jacobian_matrix(const std::tuple<FuncTypes...>& function, const std::tuple<ArgTypes...>& args)
{
    type::matrix_t<long double> jacobian(f_size, arg_size);
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
        auto maxi = -std::numeric_limits<long double>::infinity();
        int index = 0;
        if(row == container.rows -1)
        {
            return;
        }
        for(std::size_t i = row; i < container.rows; ++i)
        {
            if(container(i, row) > maxi)
            {
                maxi = container(i, row);
                index = i;
            }
        }
        type::matrix<long double> tmp = container[row];
        container[row] = container[index];
        container[index] = tmp;
        return;
    };

    for(int row = 0; row < container.rows; ++row)
    {
        std::cout << "container 1 : \n" << container << std::endl;
        pivot(row);
        std::cout << "container 2 : \n" << container << std::endl;
        long double d = container(row, row);
        for(int mid = row; mid < container.rows; ++mid)
        {
            long double m = container(mid, row);
            for(int col = 0; col < container.cols; ++col)
            {

                if(row == mid)
                {
                    container(mid, col) /= d;
                }
                else
                {
                    long double k = m / container(row, row);
                    container(mid,col) = container(mid,col) -k * container(row,col);
                }
                std::cout << "----------------------" << std::endl;
                std::cout << container << std::endl;
                std::cout << "----------------------" << std::endl;
                
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

    auto ans = type::matrixToTuple<arg_size>(mat);

    return ans;
}

// template <typename... FuncType, typename... ArgTypes, std::size_t size = sizeof...(ArgTypes)>
// auto solve_nonlinear_equations_newton_method(
//     const std::tuple<FuncType...>& functions,
//     std::tuple<ArgTypes...> initial_values, int max_try = 20)
// {
//     int try_count = 0;

//     while(try_count < max_try)
//     {
//         auto jacobian = create_jacobian_matrix(functions, initial_values);
//         auto y = gauss_jordar_elimination(initial_values, jacobian);

//         type::compile_loop<size>([&initial_values, &y](auto i)
//         {
//             std::get<i.value>(initial_values) += std::get<i.value>(y);
//         });

//         // auto Fx = evaluate_lambdas<std::vector>(functions, initial_values);
//         // for(auto f:Fx)
//         // {
//         //     std::cout << f << ", ";
//         // }
//         // std::cout << "\n======================\n";

//         ++try_count;
//     }

//     return initial_values;
// }



namespace hidden
{
    // template< template<typename, size_t> class ReturnClass,
    //         typename TupleType, auto... Indices, typename... ArgTypes>
    // auto evaluate_lambdas(typed_sequence_t<Indices...>, TupleType&& tuple, ArgTypes&&... args)
    // {
    //     return ReturnClass{ (std::get<Indices>(tuple)(std::forward<ArgTypes>(args)...))... };
    // }

    // template< template<typename...> class ReturnClass,
    //         typename TupleType, auto... Indices, typename... ArgTypes>
    // auto evaluate_lambdas(typed_sequence_t<Indices...>, TupleType&& tuple, ArgTypes&&... args)
    // {
    //     return ReturnClass{ (std::get<Indices>(tuple)(std::forward<ArgTypes>(args)...))... };
    // }

    template< template<typename, size_t> class ReturnClass,
            typename TupleType, typename ArgTuple, auto... Indices>
    auto evaluate_lambdas_tuple(type::sequence_t<Indices...>, TupleType&& tuple, ArgTuple&& arguments)
    {
        return ReturnClass{ std::apply(std::get<Indices>(tuple), std::forward<ArgTuple>(arguments))... };
    }

    template< template<typename...> class ReturnClass,
            typename TupleType, typename ArgTuple, auto... Indices>
    auto evaluate_lambdas_tuple(type::sequence_t<Indices...>, TupleType&& tuple, ArgTuple&& arguments)
    {
        return ReturnClass{ std::apply(std::get<Indices>(tuple), std::forward<ArgTuple>(arguments))... };
    }

}
// end of namespace hidden

// template< template<typename, size_t> class ReturnClass,
//     typename FuncType, typename... FuncTypes, typename ArgFirst,  typename... ArgTypes>
// auto evaluate_lambdas(const std::tuple<FuncType, FuncTypes...>& tuple, ArgFirst&& arg, ArgTypes&&... args)
// {
//     constexpr auto Size = sizeof...(FuncTypes) + 1;

//     return hidden::evaluate_lambdas<ReturnClass>(make_typed_sequence_t<Size>{},
//         tuple, std::forward<ArgFirst>(arg), std::forward<ArgTypes>(args)...);
// }

// template< template<typename...> class ReturnClass,
//     typename FuncType, typename... FuncTypes, typename ArgFirst,  typename... ArgTypes>
// auto evaluate_lambdas(const std::tuple<FuncType, FuncTypes...>& tuple, ArgFirst&& arg, ArgTypes&&... args)
// {
//     constexpr auto Size = sizeof...(FuncTypes) + 1;

//     return hidden::evaluate_lambdas<ReturnClass>(make_typed_sequence_t<Size>{},
//         tuple, std::forward<ArgFirst>(arg), std::forward<ArgTypes>(args)...);
// }

template< template<typename, size_t> class ReturnClass,
    typename FuncType, typename... FuncTypes, typename ArgFirst>
auto evaluate_lambdas(const std::tuple<FuncType, FuncTypes...>& tuple, ArgFirst&& arg)
{
    constexpr auto Size = sizeof...(FuncTypes) + 1;

    // if constexpr(is_tuple_v<decltype(arg)>)
    // {
    return hidden::evaluate_lambdas_tuple<ReturnClass>(type::make_sequence_t<Size>{},
        tuple, std::forward<ArgFirst>(arg));
    // }
    // else
    // {
    //     return hidden::evaluate_lambdas<ReturnClass>(make_typed_sequence_t<Size>{},
    //         tuple, std::forward<ArgFirst>(arg));
    // }
}

template< template<typename...> class ReturnClass,
    typename FuncType, typename... FuncTypes, typename ArgFirst>
auto evaluate_lambdas(const std::tuple<FuncType, FuncTypes...>& tuple, ArgFirst&& arg)
{
    constexpr auto Size = sizeof...(FuncTypes) + 1;

    // if constexpr(is_tuple_v<decltype(arg)>)
    // {
    return hidden::evaluate_lambdas_tuple<ReturnClass>(type::make_sequence_t<Size>{},
        tuple, std::forward<ArgFirst>(arg));
    // }
    // else
    // {
    //     return hidden::evaluate_lambdas<ReturnClass>(make_typed_sequence_t<Size>{},
    //         tuple, std::forward<ArgFirst>(arg));
    // }
}
