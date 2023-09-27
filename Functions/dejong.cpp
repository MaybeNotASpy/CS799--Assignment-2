#include "dejong.hpp"

double dejong::DeJong1::eval(std::span<double> X) const
{
    assert(X.size() == 3);
    return std::accumulate(X.begin(), X.end(), 0.0, [](double acc, double x)
                           { return acc + x * x; });
}

double dejong::DeJong2::eval(std::span<double> X) const
{
    assert(X.size() == 2);
    auto x1_squared = X[0] * X[0];
    auto x1_squared_minus_x2 = x1_squared - X[1];
    auto one_minus_x1 = 1 - X[0];

    return 100 * x1_squared_minus_x2 * x1_squared_minus_x2 + one_minus_x1 * one_minus_x1;
}

double dejong::DeJong3::eval(std::span<double> X) const
{
    assert(X.size() == 5);
    return 30. + std::accumulate(X.begin(), X.end(), 0.0, [](double acc, double x)
                           { return acc + std::floor(x); });
}

double dejong::DeJong4::eval(std::span<double> X) const
{
    assert(X.size() == 10);
    double sum = 3.;
    for (auto [it, i] = std::tuple{X.begin(), 0}; it != X.end(); it++, i++)
    {
        sum += (i + 1) * std::pow(*it, 4.);
    }
    return sum + gauss();
}

double dejong::DeJong5::eval(std::span<double> X) const
{
    assert(X.size() == 2);
    double sum = 0.002;
    for (size_t j = 0; j < 25; j++)
    {
        double sum2 = (double)j + 1.;
        sum2 += std::pow(X[0] - a[0][j], 6.);
        sum2 += std::pow(X[1] - a[1][j], 6.);
        sum += 1. / sum2;
    }
    return 1. / sum;
}