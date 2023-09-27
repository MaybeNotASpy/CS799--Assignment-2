#include "chc.hpp"

std::vector<Individual> CHC::generate_initial_population()
{
    std::vector<Individual> population(
        population_size,
        Individual(variable_size, number_of_variables, function));
    for (auto &individual : population)
    {
        individual.randomize();
    }
    return population;
}

std::vector<Individual> CHC::select_parents(const std::vector<Individual> &population)
{
    std::vector parents = population;
    std::shuffle(parents.begin(), parents.end(), generator); 
    return parents;
}

size_t CHC::hamming_distance(const Individual &individual1, const Individual &individual2)
{
    const auto &vector1 = individual1.getVector();
    const auto &vector2 = individual2.getVector();
    assert(vector1.size() == vector2.size());
    // Magical one-liner to calculate hamming distance
    // Iterate through the two vectors and add 1 to the sum if the bits are different
    // Takes the following parameters:
    // 1. The beginning iterator of vector1
    // 2. The ending iterator of vector1
    // 3. The beginning iterator of vector2 (since both vectors are the same size,
    // the ending iterator is at the same point as same as vector1)
    // 4. The initial value of the sum
    // 5. The function to apply to difference vector
    // 6. The function to apply to the pairs of bits
    return (size_t)std::transform_reduce(
        vector1.begin(),
        vector1.end(),
        vector2.begin(),
        0,
        std::plus<size_t>(),       // Summation
        [](bool bit1, bool bit2) { // Bit difference
            return bit1 != bit2;
        });
}

std::vector<size_t> CHC::get_different_indices(
    const bitstring &vector1,
    const bitstring &vector2)
{
    assert(vector1.size() == vector2.size());
    std::vector<size_t> different_indices;
    for (size_t i = 0; i < vector1.size(); i++)
    {
        if (vector1[i] != vector2[i])
        {
            different_indices.push_back(i);
        }
    }
    return different_indices;
}

std::vector<Individual> CHC::crossover(
    const std::vector<Individual> &recomb_parents,
    double difference_threshold)
{
    std::vector<Individual> children;
    for (size_t i = 0; i < recomb_parents.size() - 1; i += 2)
    {
        auto hamming_dist = hamming_distance(recomb_parents[i], recomb_parents[i + 1]);
        if (((double)hamming_dist) / 2.0 > difference_threshold)
        {
            auto different_indices = get_different_indices(
                recomb_parents[i].getVector(),
                recomb_parents[i + 1].getVector());
            auto child1 = recomb_parents[i];
            auto child2 = recomb_parents[i + 1];
            //Select half of the different indices to be flipped randomly
            std::shuffle(different_indices.begin(), different_indices.end(), generator);
            for (auto index : std::ranges::take_view{different_indices, (int) different_indices.size() / 2})
            {
                child1.setValueAt(index, recomb_parents[i + 1].getVector()[index]);
                child2.setValueAt(index, recomb_parents[i].getVector()[index]);
            }
            children.push_back(child1);
            children.push_back(child2);
        }
        else
        {
            children.push_back(recomb_parents[i]);
            children.push_back(recomb_parents[i + 1]);
        }
    }
    return children;
}

void CHC::mutate(std::vector<Individual> &children)
{
    for (auto &individual : children)
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
}

std::vector<Individual> CHC::select_survivors(
    std::vector<Individual> &parents,
    std::vector<Individual> &children)
{
    // Check if the parents and children have been evaluated.
    for (size_t i = 0; i < population_size; i++)
    {
        assert(parents[i].isEvaluated());
        assert(children[i].isEvaluated());
    }
    auto sorted_parents(parents);
    //std::sort(sorted_parents.begin(), sorted_parents.end());
    //Manually sort the parents
    for (size_t i = 0; i < sorted_parents.size(); i++)
    {
        for (size_t j = i + 1; j < sorted_parents.size(); j++)
        {
            if (sorted_parents[i] < sorted_parents[j])
            {
                std::swap(sorted_parents[i], sorted_parents[j]);
            }
        }
    }
    auto sorted_children(children);
    //std::sort(sorted_children.begin(), sorted_children.end());
    //Manually sort the children
    for (size_t i = 0; i < sorted_children.size(); i++)
    {
        for (size_t j = i + 1; j < sorted_children.size(); j++)
        {
            if (sorted_children[i] < sorted_children[j])
            {
                std::swap(sorted_children[i], sorted_children[j]);
            }
        }
    }
    std::vector<Individual> survivors(parents);

    auto parent_it = sorted_parents.begin();
    auto child_it = sorted_children.begin();
    for (size_t i = 0; i < survivors.size(); i++)
    {
        if (*child_it < *parent_it)
        {
            survivors[i] = *parent_it;
            parent_it++;
        }
        else
        {
            survivors[i] = *child_it;
            child_it++;
        }
    }
    return survivors;
}

std::vector<Individual> CHC::diverge_if_converged(const std::vector<Individual> &population)
{
    auto best_individual = *std::max_element(
        population.begin(),
        population.end());
    std::vector<Individual> new_population(
        population_size,
        best_individual);
    // Mutate all but the first of the copies.
    // Mutation is bit-flip mutation of (mutation_prob * size of bitstring) random bits
    for (auto &individual : std::ranges::drop_view{new_population, 1})
    {
        auto number_of_bit_flips = (size_t)std::round(mutation_prob * (double)individual.getVector().size());
        // Magical way to generate a random permutation of indices
        std::vector<size_t> indices(individual.getVector().size());
        std::iota(indices.begin(), indices.end(), 0);
        std::shuffle(indices.begin(), indices.end(), generator);

        for (auto index : indices | std::views::take(number_of_bit_flips))
        {
            individual.flip(index);
        }
    }
    for (auto &individual : new_population)
    {
        individual.evaluate();
    }
    return new_population;
}

std::vector<GenerationPerformance> CHC::run()
{
    std::vector<GenerationPerformance> performance;
    performance.resize(num_of_generations);
    std::vector<Individual> population = generate_initial_population();
    for (auto &individual : population)
    {
        individual.evaluate();
    }
    double difference_threshold = (double)variable_size * (double)number_of_variables / 4.0;
    for (size_t gen = 0; gen < num_of_generations; gen++)
    {
        auto parents = select_parents(population);
        auto children = crossover(parents, difference_threshold);
        for (auto &individual : children)
        {
            individual.evaluate();
        }
        auto survivors = select_survivors(parents, children);
        if (std::is_permutation(survivors.begin(), survivors.end(), population.begin(), population.end()))
        {
            difference_threshold -= 1.;
        }
        population = survivors;
        if (difference_threshold < 0)
        {
            population = diverge_if_converged(survivors);
            difference_threshold = mutation_prob * (1. - mutation_prob) * (double)population_size;
        }

        auto best_individual = *std::max_element(
            population.begin(),
            population.end());
        auto worst_individual = *std::min_element(
            population.begin(),
            population.end());
        auto best_fitness = std::get<0>(best_individual.getFitness());
        auto worst_fitness = std::get<0>(worst_individual.getFitness());
        auto average_fitness = std::accumulate(
                                   population.begin(),
                                   population.end(),
                                   0.0,
                                   [](double sum, const Individual &individual) {
                                       return sum + std::get<0>(individual.getFitness());
                                   }) /
                               (double)population_size;
        auto best_objective_function_value = std::get<1>(best_individual.getFitness());
        auto worst_objective_function_value = std::get<1>(worst_individual.getFitness());
        auto average_objective_function_value = std::accumulate(
                                                    population.begin(),
                                                    population.end(),
                                                    0.0,
                                                    [](double sum, const Individual &individual) {
                                                        return sum + std::get<1>(individual.getFitness());
                                                    }) /
                                                (double)population_size;
        performance[gen] = GenerationPerformance(
            gen,
            best_fitness,
            average_fitness,
            worst_fitness,
            best_objective_function_value,
            average_objective_function_value,
            worst_objective_function_value,
            best_individual.getVector().decode(),
            worst_individual.getVector().decode());
    }
    return performance;
}