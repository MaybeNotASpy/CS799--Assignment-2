#pragma once
#include "algorithm.hpp"
extern std::mt19937 &get_generator();
#include <ranges>

class CHC : public Algorithm
{
public:
    CHC(
        size_t pop_size,
        size_t num_of_gens,
        double crossover_p,
        double mutation_p,
        size_t variable_size,
        size_t num_of_variables,
        OptimizationFunction &func) : Algorithm(pop_size, num_of_gens, crossover_p, mutation_p, variable_size, num_of_variables, func) {}
    std::vector<GenerationPerformance> run() override;

private:
    /**
     * @brief Generate an initial population.
     * @return std::vector<Individual> The initial population.
     */
    std::vector<Individual> generate_initial_population();

    /**
     * @brief Select parents from the population.
     * For CHC, the parents are selected randomly, without replacement (shuffle).
     * @param population The population.
     * @return std::vector<Individual> The selected parents.
     */
    std::vector<Individual> select_parents(const std::vector<Individual> &population);

    /**
     * @brief Find the hamming distance between two individuals.
     * @param individual1 The first individual.
     * @param individual2 The second individual.
     * @return size_t The hamming distance.
     */
    size_t hamming_distance(const Individual &individual1, const Individual &individual2);

    /**
     * @brief Get indices that are not the same between two vectors.
     * @param vector1 The first vector.
     * @param vector2 The second vector.
     * @return std::vector<size_t> The indices that are not the same.
     */
    std::vector<size_t> get_different_indices(
        const bitstring &vector1,
        const bitstring &vector2);

    /**
     * @brief Crossover the parents to generate children.
     * For CHC, the crossover is uniform crossover.
     * @param parents The parents.
     * @param difference_threshold The threshold for the hamming distance.
     * @return std::vector<Individual> The children.
     */
    std::vector<Individual> crossover(
        const std::vector<Individual> &recomb_parents,
        double difference_threshold);

    /**
     * @brief Mutate the children.
     * For CHC, the mutation is bit-flip mutation.
     * @param children The children.
     */
    void mutate(std::vector<Individual> &children);

    /**
     * @brief Select survivors from the population.
     * For CHC, the survivors are selected using elitism.
     * @param children The children.
     * @return std::vector<Individual> The survivors.
     */
    std::vector<Individual> select_survivors(
        std::vector<Individual> &parents,
        std::vector<Individual> &children);

    /**
     * @brief Diverge if the population has converged.
     * For CHC, the population has converged if all individuals are the same.
     * This will generate a new population by first copying the best individual
     * and then mutating all but one of the copies.
     * @param population The population.
     * @return std::vector<Individual> The new population.
     */
    std::vector<Individual> diverge_if_converged(const std::vector<Individual> &population);
};