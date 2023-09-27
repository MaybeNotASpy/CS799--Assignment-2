#pragma once

#include <vector>
#include <optional>
#include <cassert>

#include "Functions/function.hpp"
#include "bitstring.hpp"

/**
 * The individual class represents a single individual in the population.
 * In our case, it is a vector of 1-bit integers of size N.
 * This also stores the fitness of the individual and provides methods to
 * initialize the vector, evaluate the fitness, and flip a single value.
 */
class Individual
{
public:
    /**
     * Initialize the individual with a random vector of 1-bit integers.
     * @param size The size of the vector.
     */
    Individual(size_t variable_size, size_t number_of_variables, OptimizationFunction &function) : vector(variable_size, function.getXRange().first, function.getXRange().second, number_of_variables),
                                                                                                   function(function)
    {
        // Initialize the vector with random values
        assert(variable_size > 0);
        assert(number_of_variables > 0);
        assert(function.getXRange().first < function.getXRange().second);
        assert(function.getNumberOfVariables() == number_of_variables);
        assert(vector.size() == variable_size * number_of_variables);
        randomize();
    }

    /**
     * Initialize the individual with a given vector of 1-bit integers.
     * @param vector The vector to initialize the individual with.
     */
    Individual(const bitstring &bits, OptimizationFunction &function) : vector(bits),
                                                                        function(function){};

    /**
     * Destructor.
     */
    ~Individual() {}

    /**
     * Copy constructor.
     * @param other The individual to copy.
     */
    Individual(const Individual &other) : vector(other.vector),
                                          function(other.function),
                                          cached_fitness(other.cached_fitness) {}
    Individual(Individual &&other) : vector(std::move(other.vector)),
                                     function(other.function),
                                     cached_fitness(other.cached_fitness) {}

    /**
     * Overload the assignment operator.
     * @param other The individual to assign to this individual.
     * @return This individual.
     */
    Individual &operator=(const Individual &other)
    {
        this->vector = other.vector;
        this->function = other.function;
        this->cached_fitness = other.cached_fitness;
        return *this;
    }
    Individual &operator=(Individual &&other)
    {
        this->vector = std::move(other.vector);
        this->function = other.function;
        this->cached_fitness = std::move(other.cached_fitness);
        return *this;
    }

    /**
     * Overload equality operator.
     */
    bool operator==(const Individual &other) const
    {
        return this->vector == other.vector;
    }

    /**
     * Overload less than operator.
     * Checks if the fitness of this individual is less than the fitness of the other individual.
     */
    bool operator<(const Individual &other) const
    {
        assert(this->cached_fitness.has_value());
        assert(other.cached_fitness.has_value());
        auto [fitness1, result1] = this->cached_fitness.value();
        auto [fitness2, result2] = other.cached_fitness.value();
        return fitness1 < fitness2;
    }

    /**
     * Get the value at the given index.
     * @param index The index of the value to get.
     * @return The value at the given index.
     */
    uint8_t getValueAt(const size_t index) const
    {
        return this->vector[index];
    }

    /**
     * Set the value at the given index.
     * @warning Will invalidate the cached fitness.
     * Must call evaluate() sometime after calling this method.
     * @param index The index of the value to set.
     * @param value The value to set.
     */
    void setValueAt(size_t index, uint8_t value)
    {
        this->cached_fitness = std::nullopt;
        this->vector[index] = value;
    }

    typedef std::tuple<double, double> fitness_result;
    /**
     * Evaluate the fitness of the individual.
     * Fitness is defined as 100 - the result of the function.
     * This is because we are trying to minimize the result of the function.
     * Stores the fitness in the cached_fitness variable.
     */
    void evaluate()
    {
        // Evaluate the fitness of the individual
        auto input = this->vector.decode();
        auto result = this->function.eval(input);
        auto fitness = this->function.fitnessFunction(result);
        assert(fitness >= 0.0);
        cached_fitness = std::make_tuple(fitness, result);
    }

    /**
     * Get the fitness and value of the individual from the cached_fitness variable.
     * @return The fitness and value of the individual.
     */
    fitness_result getFitness() const
    {
        assert(this->cached_fitness.has_value());
        return this->cached_fitness.value();
    }

    /**
     * Check if the individual has been evaluated.
     * @return True if the individual has been evaluated, false otherwise.
     */
    bool isEvaluated() const { return this->cached_fitness.has_value(); }

    /**
     * Flip a single value in the vector.
     * @warning Will invalidate the cached fitness.
     * Must call evaluate() sometime after calling this method.
     * @param index The index of the value to flip.
     */
    void flip(size_t index)
    {
        this->cached_fitness = std::nullopt;
        this->vector.flip(index);
    }

    /**
     * Get the vector of 1-bit integers.
     * @return The vector of 1-bit integers.
     */
    bitstring getVectorCopy() const
    {
        return this->vector;
    }

    /**
     * Get the vector of 1-bit integers but const.
     * @return The vector of 1-bit integers.
     */
    const bitstring &getVector() const
    {
        return this->vector;
    }

    /**
     * Randomize the vector.
     */
    void randomize() { this->vector.randomize(); }

private:
    bitstring vector;
    OptimizationFunction &function;
    std::optional<fitness_result> cached_fitness = std::nullopt;
};