#include "Algorithms/simple_ga.hpp"
#include <fstream>

void run_simple_ga(size_t population_size, size_t num_of_generations, double crossover_prob, double mutation_prob, size_t chromosome_size, size_t number_of_chromosomes, OptimizationFunction &function, size_t num_of_runs, std::string filename)
{
    //Need to gather min,max,avg fitness and objective function value for each generation across all runs
    std::vector<std::vector<GenerationPerformance>> run_performances;
    run_performances.resize(num_of_runs);
    #pragma omp parallel for
    for (size_t run = 0; run < num_of_runs; run++)
    {
        auto ga = SimpleGA(population_size, num_of_generations, crossover_prob, mutation_prob, chromosome_size, number_of_chromosomes, function);
        run_performances[run] = ga.run();
    }

    // Print the results to a file
    std::ofstream file;
    file.open(filename);
    if (!file.is_open())
    {
        std::cout << "Could not open file " << filename << std::endl;
        return;
    }
    file << "Run,Generation,Best Fitness,Average Fitness,Worst Fitness,Best Value,Average Value,Worst Value" << std::endl;
    for (size_t run = 0; run < num_of_runs; run++)
    {
        for (size_t generation = 0; generation < num_of_generations; generation++)
        {
            auto performance = run_performances[run][generation];
            file << run << "," << generation << "," << performance.best_fitness << "," << performance.average_fitness << "," << performance.worst_fitness << "," << performance.best_objective_function_value << "," << performance.average_objective_function_value << "," << performance.worst_objective_function_value << std::endl;
        }
    }
    file.close();

}
