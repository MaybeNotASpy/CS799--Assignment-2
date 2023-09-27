#pragma once

#include <span>
#include <cmath>
#include <random>
#include <vector>
#include <span>
#include <tuple>
#include <cassert>
#include <initializer_list>

/**
 * The OptimizationFunction class represents the function that we are trying to
 * optimize. It provides a method to evaluate the fitness of a bit vector.
 */
class OptimizationFunction
{
public:
    /**
     * Evaluate the fitness of a bit vector.
     * @param input The bit vector to evaluate.
     * @return The fitness of the bit vector.
     */
    virtual double eval(const std::span<double> X) const = 0;

    /**
     * Get the domain of the function.
     * @return The domain of the function.
     */
    virtual const std::pair<double, double> getXRange() const = 0;

    /**
     * Get the minimum input of the function.
     * @return The minimum input of the function.
     */
    virtual const std::vector<double> getMinX() const = 0;

    /**
     * Get the minimum value of the function.
     * @return The minimum value of the function.
     */
    virtual double getMinY() const = 0;

    /**
     * Get the number of variables of the function.
     * @return The number of variables of the function.
     */
    virtual size_t getNumberOfVariables() const = 0;

    /**
     * Convert a result to a fitness value.
     * @param solution The result to convert.
     * @return The fitness value of the result.
     */
    virtual double fitnessFunction(double result) = 0;
};
