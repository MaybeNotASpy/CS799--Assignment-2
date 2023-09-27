#pragma once
#include <vector>
#include <chrono>

#include "../Functions/function.hpp"
#include "../individual.hpp"

struct GenerationPerformance
{
    size_t generation;
    double best_fitness;
    double average_fitness;
    double worst_fitness;
    double best_objective_function_value;
    double average_objective_function_value;
    double worst_objective_function_value;
    std::vector<double> best_solution;
    std::vector<double> worst_solution;

    GenerationPerformance() = default;

    GenerationPerformance(
        size_t generation,
        double best_fitness,
        double average_fitness,
        double worst_fitness,
        double best_objective_function_value,
        double average_objective_function_value,
        double worst_objective_function_value,
        const std::vector<double> &best_solution,
        const std::vector<double> &worst_solution) : generation(generation),
                                                     best_fitness(best_fitness),
                                                     average_fitness(average_fitness),
                                                     worst_fitness(worst_fitness),
                                                     best_objective_function_value(best_objective_function_value),
                                                     average_objective_function_value(average_objective_function_value),
                                                     worst_objective_function_value(worst_objective_function_value),
                                                     best_solution(best_solution),
                                                     worst_solution(worst_solution)
    {
        assert(best_solution.size() == worst_solution.size());
        assert(best_solution.size() > 0);
    }
};

class Algorithm
{
protected:
    size_t population_size;
    size_t num_of_generations;
    double crossover_prob;
    double mutation_prob;
    size_t variable_size;
    size_t number_of_variables;
    OptimizationFunction &function;

    std::default_random_engine generator = std::default_random_engine((long unsigned int)std::chrono::system_clock::now().time_since_epoch().count());

public:
    Algorithm(
        size_t pop_size,
        size_t num_of_gens,
        double crossover_p,
        double mutation_p,
        size_t variable_size,
        size_t num_of_variables,
        OptimizationFunction &func) : population_size(pop_size),
                                      num_of_generations(num_of_gens),
                                      crossover_prob(crossover_p),
                                      mutation_prob(mutation_p),
                                      variable_size(variable_size),
                                      number_of_variables(num_of_variables),
                                      function(func) {}
    virtual std::vector<GenerationPerformance> run() = 0;
};