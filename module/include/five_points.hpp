#pragma once

#include <iostream>
#include <concepts>
#include <cmath>

template <typename ValueType, typename FunctionType>
ValueType diff_stencil_lower(ValueType x, FunctionType&& f, int n = 1, ValueType h = 0.001)
{
    if(n==0)
    {
        return f(x);
    }
    else if(n==1)
    {
        auto p = -f(x+2.0*h) + 8.0*f(x+h) - 8.0*f(x-h) + f(x-2.0*h);
        return p / (12.0*h);
    }
    else if(n==2)
    {
        auto p = -f(x+2.0*h) + 16.0*f(x+h) - 30.0*f(x) + 16.0*f(x-h) + f(x-2.0*h);
        return p / (12.0*h*h);
    }
    else if(n==3)
    {
        auto p = f(x+2.0*h) - 2.0*f(x+h) + 2.0*f(x-h) + f(x-2.0*h);
        return p / (2.0*h*h*h);
    }
    else if(n==4)
    {
        auto p = f(x+2.0*h) - 4.0*f(x+h) + 6.0*f(x) -4.0*f(x-h) + f(x-2.0*h);
        return p / (h*h*h*h);
    }
    else
    {
        throw std::runtime_error("error");
    }
}

template <typename ValueType, typename FunctionType>
ValueType diff_stencil_higher(ValueType x, FunctionType&& f, int n = 1, ValueType h = 0.01)
{
    if(n==0)
    {
        return f(x);
    }
    else if(n==1)
    {
        auto p = -f(x+2.0*h) + 8.0*f(x+h) - 8.0*f(x-h) + f(x-2.0*h);
        return p / (12.0*h);
    }
    else
    {
        auto f1 = diff_stencil_higher(x+2.0*h, f, n-1);
        auto f2 = diff_stencil_higher(x+h, f, n-1);
        auto f3 = diff_stencil_higher(x-h, f, n-1);
        auto f4 = diff_stencil_higher(x-2.0*h, f, n-1);

        auto p = -f1 + 8.0*f2 - 8.0*f3 + f4;
        return p / (12.0*h);
    }
}

template <typename ValueType, typename FunctionType>
ValueType diff_stencil(ValueType x, FunctionType&& f, int n = 1, ValueType h_lower= 0.001, ValueType h_higher = 0.01)
    requires requires
    {
        { f(x) } -> std::convertible_to<ValueType>;
    }
{
    if(n < 3)
    {
        return diff_stencil_lower(x, f, n, h_lower);
    }
    else
    {
        return diff_stencil_higher(x, f, n, h_higher);
    }
}

template<typename FunctionType, typename ValueType>
ValueType diff_stencil(FunctionType&& f, ValueType x, int n = 1, ValueType h_lower = 0.001, ValueType h_higher = 0.01)
    requires requires
    {
        { f(x) } -> std::convertible_to<ValueType>;
    }
{
    return diff_stencil(x, f, n, h_lower, h_higher);
}
