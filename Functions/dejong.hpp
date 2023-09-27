#include "function.hpp"

namespace dejong
{
    /**
     * Spherical function
     * f(x) = sum_1^3(x_i^2)
     * x_i in [-5.12, 5.12]
     * Minimum: f(0, 0, 0) = 0
     * Maximum: f(+-5.12, +-5.12, +-5.12) = 78.6432
     */
    class DeJong1 : public OptimizationFunction
    {
    public:
        double eval(std::span<double> X) const override;
        const std::pair<double, double> getXRange() const override { return {-5.12, 5.12}; };
        const std::vector<double> getMinX() const override { return {0., 0., 0.}; };
        double getMinY() const override { return 0.; };
        double getMaxY() const override { return 78.6432; };
        size_t getNumberOfVariables() const override { return 3; };
    };

    /**
     * Rosenbrock function
     * f(x) = 100(x_2 - x_1^2)^2 + (1-x_1)^2
     * x_i in [-5.12, 5.12]
     * Minimum: f(1, 1) = 0
     * Maximum: f(5.12, -5.12) = 98201.4
     */
    class DeJong2 : public OptimizationFunction
    {
    public:
        double eval(std::span<double> X) const override;
        const std::pair<double, double> getXRange() const override { return {-5.12, 5.12}; };
        const std::vector<double> getMinX() const override { return {1., 1.}; };
        double getMinY() const override { return 0.; };
        double getMaxY() const override { return 98201.4; };
        size_t getNumberOfVariables() const override { return 2; }
    };

    /**
     * Step function
     * f(x) = 30 + sum_1^5(floor(x_i))
     * x_i in [-5.12, 5.12]
     * Minimum: f(-5.12,-5.12, -5.12, -5.12, -5.12) = 0
     * Maximum: f(5.12, 5.12, 5.12, 5.12, 5.12) = 55
     */
    class DeJong3 : public OptimizationFunction
    {

    public:
        double eval(std::span<double> X) const override;
        const std::pair<double, double> getXRange() const override { return {-5.12, 5.12}; };
        const std::vector<double> getMinX() const override { return {-5.12, -5.12, -5.12, -5.12, -5.12}; }
        double getMinY() const override { return 0.; }
        double getMaxY() const override { return 55.; }
        size_t getNumberOfVariables() const override { return 5; }
    };

    /**
     * Quartic function with noise
     * f(x) = sum_1^10(ix_i^4) + gauss(0,1)
     * x_i in [-1.28, 1.28]
     * (Assuming min and max of gauss(0,1) to 6sigma, i.e. -3 and 3)
     * Minimum: f(0, 0, 0, 0, 0, 0, 0, 0, 0, 0) = 0
     * Maximum: f(+-1.28, +-1.28, +-1.28, +-1.28, +-1.28, +-1.28, +-1.28, +-1.28, +-1.28, +-1.28) = 150.64
     */
    class DeJong4 : public OptimizationFunction
    {
        double gauss() const
        {
            static std::normal_distribution<> d(0, 1);
            return d(get_generator());
        };

    public:
        double eval(std::span<double> X) const override;
        const std::pair<double, double> getXRange() const override { return {-1.28, 1.28}; }
        const std::vector<double> getMinX() const override
        {
            return {0., 0., 0., 0., 0., 0., 0., 0., 0., 0.};
        }
        double getMinY() const override { return 0.; }
        double getMaxY() const override { return 150.64; }
        size_t getNumberOfVariables() const override { return 10; }
    };

    /**
     * Shekel's foxholes function
     * f(x) = 0.002 + sum_1^25(1/(j + sum_1^2(x_i - a_ij)^6))
     * x_i in [-65.536, 65.536]
     * Minimum: f(-32, -32) approx 1
     * Maximum: f(various, various) = 510
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
        const std::vector<double> getMinX() const override { return {-32., -32.}; }
        double getMinY() const override { return 1.; }
        double getMaxY() const override { return 500.; }
        size_t getNumberOfVariables() const override { return 2; }
    };
}