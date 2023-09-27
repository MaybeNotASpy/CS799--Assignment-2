#include "simple_ga.hpp"

std::pair<size_t, size_t> SimpleGA::proportional_selection(std::vector<Individual> &population, std::vector<double> &fitness)
{
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    double sum = std::accumulate(fitness.begin(), fitness.end(), 0.0);
    double random = distribution(generator) * sum;
    size_t parent1_index = 0;
    size_t parent2_index = population.size() - 1;
    double partial_sum = 0.0;
    for (size_t i = 0; i < population.size(); i++)
    {
        partial_sum += fitness[i];
        if (partial_sum >= random)
        {
            parent1_index = i;
            break;
        }
    }
    partial_sum = 0.0;
    for (size_t i = 0; i < population.size(); i++)
    {
        partial_sum += fitness[i];
        if (partial_sum >= random && i != parent1_index)
        {
            parent2_index = i;
            break;
        }
    }
    assert(parent1_index < population.size());
    assert(parent2_index < population.size());
    return std::make_pair(parent1_index, parent2_index);
}

std::pair<Individual, Individual> SimpleGA::crossover(Individual &parent1, Individual &parent2)
{
    std::uniform_int_distribution<size_t> distribution(0, variable_size * number_of_variables - 1);
    auto crossover_point = distribution(generator);
    Individual child1 = parent1;
    Individual child2 = parent2;
    for (size_t i = crossover_point; i < variable_size * number_of_variables; i++)
    {
        child1.setValueAt(i, parent2.getValueAt(i));
        child2.setValueAt(i, parent1.getValueAt(i));
    }
    return std::make_pair(child1, child2);
}

void SimpleGA::mutate(Individual &individual)
{
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    for (size_t i = 0; i < variable_size * number_of_variables; i++)
    {
        if (distribution(generator) < mutation_prob)
        {
            individual.flip(i);
        }
    }
}

std::vector<GenerationPerformance> SimpleGA::run()
{
    std::vector<GenerationPerformance> performance;
    performance.resize(num_of_generations);
    std::vector<Individual> population(
        population_size,
        Individual(variable_size, number_of_variables, function));

    std::vector<double> generation_fitness(population_size);
    std::vector<double> objective_function_values(population_size);
    for (size_t generation = 0; generation < num_of_generations; generation++)
    {
        generation_fitness.clear();
        objective_function_values.clear();
        // Calculate fitness and objective function values.
        for (Individual &individual : population)
        {
            individual.evaluate();
            auto [fitness, objective_function_value] = individual.getFitness();
            generation_fitness.push_back(fitness);
            objective_function_values.push_back(objective_function_value);
        }

        // Find best, average and worst fitness and objective function values.
        auto it_of_best_fitness = std::max_element(generation_fitness.begin(), generation_fitness.end());
        auto it_of_worst_fitness = std::min_element(generation_fitness.begin(), generation_fitness.end());

        auto best_fitness = *it_of_best_fitness;
        auto average_fitness = std::accumulate(generation_fitness.begin(), generation_fitness.end(), 0.0) / (double)population_size;
        auto worst_fitness = *it_of_worst_fitness;
        auto best_objective_function_value = *std::max_element(objective_function_values.begin(), objective_function_values.end());
        auto average_objective_function_value = std::accumulate(objective_function_values.begin(), objective_function_values.end(), 0.0) / (double)population_size;
        auto worst_objective_function_value = *std::min_element(objective_function_values.begin(), objective_function_values.end());

        auto index_of_best_x = (size_t)std::distance(generation_fitness.begin(), it_of_best_fitness);
        auto index_of_worst_x = (size_t)std::distance(generation_fitness.begin(), it_of_worst_fitness);

        auto best_x = population[index_of_best_x].getVector().decode();
        auto worst_x = population[index_of_worst_x].getVector().decode();

        assert(best_x.size() == number_of_variables);
        assert(worst_x.size() == number_of_variables);
        performance[generation] = {
            generation,
            best_fitness,
            average_fitness,
            worst_fitness,
            best_objective_function_value,
            average_objective_function_value,
            worst_objective_function_value,
            best_x,
            worst_x};

        // Create new population.
        std::vector<Individual> new_population(population_size, Individual(variable_size, number_of_variables, function));
        for (size_t i = 0; i < population_size - 1; i += 2)
        {
            // Select parents.
            auto parents_indices = proportional_selection(population, generation_fitness);
            auto parent1 = population[parents_indices.first];
            auto parent2 = population[parents_indices.second];
            auto child1 = parent1;
            auto child2 = parent2;

            // Crossover.

            auto distribution_of_chances = std::uniform_real_distribution<double>(0.0, 1.0);
            if (distribution_of_chances(generator) < crossover_prob)
            {
                auto children = crossover(parent1, parent2);
                child1 = children.first;
                child2 = children.second;
            }

            // Mutate.
            mutate(child1);
            mutate(child2);

            // Add children to new population.
            new_population[i] = child1;
            new_population[i + 1] = child2;
        }
        population = new_population;
    }
    return performance;
}

#ifndef NDEBUG
void SimpleGA::check_initialization()
{
    assert(population_size > 0);
    assert(num_of_generations > 0);
    assert(crossover_prob >= 0.0 && crossover_prob <= 1.0);
    assert(mutation_prob >= 0.0 && mutation_prob <= 1.0);
    assert(variable_size > 0);
    assert(number_of_variables > 0);
}
#else
void SimpleGA::check_initialization() {}
#endif