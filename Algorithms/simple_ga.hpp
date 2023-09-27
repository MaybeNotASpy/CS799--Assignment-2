#pragma once
#include "algorithm.hpp"
#include <array>

extern std::mt19937 &get_generator();

class SimpleGA : public Algorithm
{
public:
    SimpleGA(
        size_t pop_size,
        size_t num_of_gens,
        double crossover_p,
        double mutation_p,
        size_t var_size,
        size_t num_of_variables,
        OptimizationFunction &func) : Algorithm(pop_size,
                                                num_of_gens,
                                                crossover_p,
                                                mutation_p,
                                                var_size,
                                                num_of_variables,
                                                func)
    {
        check_initialization();
    }

    std::vector<GenerationPerformance> run() override;

private:
    /**
     * @brief Select two individuals from a population using proportional selection.
     *
     * @param population The population.
     * @param fitness The fitnesses of the population.
     * @return std::pair<size_t, size_t>& The indices of the two selected individuals.
     */
    std::pair<size_t, size_t> proportional_selection(std::vector<Individual> &population, std::vector<double> &fitness);

    /**
     * @brief One-point crossover two individuals.
     *
     * @param parent1 The first parent.
     * @param parent2 The second parent.
     * @return std::pair<Individual, Individual> The two children.
     */
    std::pair<Individual, Individual> crossover(Individual &parent1, Individual &parent2);

    /**
     * @brief Bit-flip mutate an individual.
     *
     * @param individual The individual to mutate.
     */
    void mutate(Individual &individual);

    /**
     * @brief Debug assertions to check if initializations are correct.
     */
    void check_initialization();
};