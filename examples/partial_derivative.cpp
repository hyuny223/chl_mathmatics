#include <iostream>

void solve()
{
    auto a = 1.0, b = 1.0;

    std::tuple xyz
    {
        [&a, &b](auto u, auto v)
        { return a * u * std::cos(v);},

        [&a, &b](auto u, auto v)
        { return b * u * std::sin(v);},

        [&a, &b](auto u, auto v)
        { return u;}

    };
    auto& [x, y, z] = xyz;

    std::tuple uv(0.5, std::numbers::pi / 6.0);

    auto dx_du = partial_derivative<0>(x, uv);

    std::cout << dx_du << std::endl;
}

int main()
{
    solve();
}
