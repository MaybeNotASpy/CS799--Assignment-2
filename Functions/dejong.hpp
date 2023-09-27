#include "function.hpp"

namespace dejong
{
    /**
     * Spherical function
     * f(x) = sum_1^3(x_i^2)
     * x_i in [-5.12, 5.12]
     * Minimum: f(0, 0, 0) = 0
     */
    class DeJong1 : public OptimizationFunction
    {
    public:
        double eval(std::span<double> X) const override;
        const std::pair<double, double> getXRange() const override { return {-5.12, 5.12}; };
        const std::vector<double> getMinX() const override { return {0., 0., 0.}; };
        double getMinY() const override { return 0.; };
        size_t getNumberOfVariables() const override { return 3; };
        double fitnessFunction(double result) override { return 80. - result; };
    };

    /**
     * Rosenbrock function
     * f(x) = 100(x_2 - x_1^2)^2 + (1-x_1)^2
     * x_i in [-5.12, 5.12]
     * Minimum: f(1, 1) = 0
     */
    class DeJong2 : public OptimizationFunction
    {
    public:
        double eval(std::span<double> X) const override;
        const std::pair<double, double> getXRange() const override { return {-5.12, 5.12}; };
        const std::vector<double> getMinX() const override { return {1., 1.}; };
        double getMinY() const override { return 0.; };
        size_t getNumberOfVariables() const override { return 2; };
        double fitnessFunction(double result) override { return 100000. - result; };
    };

    /**
     * Step function
     * f(x) = sum_1^5(floor(x_i))
     * x_i in [-5.12, 5.12]
     * Minimum: f(0, 0, 0, 0, 0) = 0
     */
    class DeJong3 : public OptimizationFunction
    {

    public:
        double eval(std::span<double> X) const override;
        const std::pair<double, double> getXRange() const override { return {-5.12, 5.12}; };
        const std::vector<double> getMinX() const override { return {-5.12, -5.12, -5.12, -5.12, -5.12}; }
        double getMinY() const override { return 0.; }
        size_t getNumberOfVariables() const override { return 5; }
        double fitnessFunction(double result) override { return 25. - result; };
    };

    /**
     * Quartic function with noise
     * f(x) = sum_1^10(ix_i^4) + gauss(0,1)
     * x_i in [-1.28, 1.28]
     * Minimum: f(0, 0, 0, 0, 0, 0, 0, 0, 0, 0) = 0
     */
    class DeJong4 : public OptimizationFunction
    {
        double gauss() const
        {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            static std::normal_distribution<> d(0, 1);
            return d(gen);
        };

    public:
        double eval(std::span<double> X) const override;
        const std::pair<double, double> getXRange() const override { return {-1.28, 1.28}; }
        const std::vector<double> getMinX() const override
        {
            return {0., 0., 0., 0., 0., 0., 0., 0., 0., 0.};
        }
        double getMinY() const override { return 0.; }
        size_t getNumberOfVariables() const override { return 10; }
        double fitnessFunction(double result) override { return 152. - result; };
    };

    /**
     * Shekel's foxholes function
     * f(x) = 0.002 + sum_1^25(1/(j + sum_1^2(x_i - a_ij)^6))
     * x_i in [-65.536, 65.536]
     * Minimum: f(-31.978, -31.978) = 0.998
     */
    class DeJong5 : public OptimizationFunction
    {
        const std::array<std::array<double, 25>, 2> a = {{{
                                                              -32.,
                                                              -16.,
                                                              0.,
                                                              16.,
                                                              32.,
                                                              -32.,
                                                              -16.,
                                                              0.,
                                                              16.,
                                                              32.,
                                                              -32.,
                                                              -16.,
                                                              0.,
                                                              16.,
                                                              32.,
                                                              -32.,
                                                              -16.,
                                                              0.,
                                                              16.,
                                                              32.,
                                                              -32.,
                                                              -16.,
                                                              0.,
                                                              16.,
                                                              32.,
                                                          },
                                                          {
                                                              -32.,
                                                              -32.,
                                                              -32.,
                                                              -32.,
                                                              -32.,
                                                              -16.,
                                                              -16.,
                                                              -16.,
                                                              -16.,
                                                              -16.,
                                                              0.,
                                                              0.,
                                                              0.,
                                                              0.,
                                                              0.,
                                                              16.,
                                                              16.,
                                                              16.,
                                                              16.,
                                                              16.,
                                                              32.,
                                                              32.,
                                                              32.,
                                                              32.,
                                                              32.,
                                                          }}};

    public:
        double eval(std::span<double> X) const override;
        const std::pair<double, double> getXRange() const override { return {-65.536, 65.536}; }
        const std::vector<double> getMinX() const override { return {-31.978, -31.978}; }
        double getMinY() const override { return 0.998; }
        size_t getNumberOfVariables() const override { return 2; }
        double fitnessFunction(double result) override { return 500. - result; };
    };
}