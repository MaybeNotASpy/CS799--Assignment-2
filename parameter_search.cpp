#include <algorithm>
#include <iostream>
#include <fstream>
#include <numeric>
#include <random>

#include "Functions/function.hpp"
#include "Functions/dejong.hpp"
#include "Algorithms/simple_ga.hpp"

extern std::mt19937 &get_generator();

// Function to perform a random parameter search on the SimpleGA algorithm.
// The results are written to a file.
void random_parameter_search(size_t population_size, size_t num_of_generations, double crossover_prob, double mutation_prob, size_t chromosome_size, size_t number_of_chromosomes, OptimizationFunction &function, size_t num_of_runs, std::string filename)
{
    // Open the file and write the header.
    std::ofstream file;
    file.open(filename, std::ios::out | std::ios::trunc);
    if (!file.is_open())
    {
        std::cout << "Could not open file " << filename << std::endl;
        return;
    }
    file << "Run,Best Fitness,Best Value,Population Size,Generations,Crossover Prob., Mutation Prob." << std::endl;

    // Set up the random number distributions.
    std::uniform_int_distribution<int> population_size_dist(10, 200);
    std::uniform_int_distribution<int> generations_dist(10, 200);
    std::uniform_real_distribution<double> crossover_dist(0.0, 1.0);
    std::uniform_real_distribution<double> mutation_dist(0.0, 0.1);

// Run the algorithm num_of_runs times.
// If i == 0, use the parameters passed to the function.
// Otherwise, generate random parameters.
// Write the results to the file.
// Uses OpenMP to parallelize the runs.
#pragma omp parallel for
    for (size_t i = 0; i < num_of_runs; i++)
    {
        auto internal_population_size = population_size;
        auto internal_num_of_generations = num_of_generations;
        auto internal_crossover_prob = crossover_prob;
        auto internal_mutation_prob = mutation_prob;
        if (i != 0)
        {
            internal_population_size = (size_t)population_size_dist(get_generator());
            internal_num_of_generations = (size_t)generations_dist(get_generator());
            internal_crossover_prob = crossover_dist(get_generator());
            internal_mutation_prob = mutation_dist(get_generator());
        }
        std::cout << "Run " << i << " of " << num_of_runs - 1 << std::endl;
        auto algorithm = SimpleGA(internal_population_size, internal_num_of_generations, internal_crossover_prob, internal_mutation_prob, chromosome_size, number_of_chromosomes, function);

        auto performance = algorithm.run();
        auto best_fitness = performance.back().best_fitness;
        auto best_x = performance.back().best_solution;
// Write the results to the file.
// Has to be within this critical block to prevent race conditions.
#pragma omp critical
        {
            file << i << "," << best_fitness << ",( ";
            for (auto &x : best_x)
            {
                file << x << " ";
            }
            file << ")," << internal_population_size << "," << internal_num_of_generations << "," << internal_crossover_prob << "," << internal_mutation_prob << std::endl;
        }
    }
    file.close();
}