#include <iostream>

void solve()
{
    auto x = 4;

    auto f = [](int x)
    {
        return (x-2)*(x-3);
    };
    auto dx = newton_method(x,f);

    std:: cout<<dx<<std::endl;
}

int main()
{
    solve();
}
