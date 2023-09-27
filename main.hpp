#pragma once

// --------------------
// Standard library includes.
// --------------------
#include <iostream>
#include <numeric>
#include <span>
#include <algorithm>
#include <ranges>
#include <cmath>
#include <span>
#include <bitset>
#include <cstring>

// --------------------
// Custom header includes.
// --------------------
#include "Functions/function.hpp"
#include "Functions/dejong.hpp"
#include "bitstring.hpp"
#include "util.hpp"

// --------------------
// Third-party library includes.
// --------------------
#include "omp.h"

// --------------------
// Constants
// --------------------

// The minimum value of a double
const auto _EPSILON = std::numeric_limits<double>::epsilon();

// --------------------
// Typedefs
// --------------------

// --------------------
// Classes
// --------------------

// --------------------
// Concepts
// --------------------

// --------------------
// Function declarations
// --------------------
extern void random_parameter_search(size_t population_size, size_t num_of_generations, double crossover_prob, double mutation_prob, size_t chromosome_size, size_t number_of_chromosomes, OptimizationFunction &function, size_t num_of_runs, std::string filename);
extern void run_simple_ga(size_t population_size, size_t num_of_generations, double crossover_prob, double mutation_prob, size_t chromosome_size, size_t number_of_chromosomes, OptimizationFunction &function, size_t num_of_runs, std::string filename);
extern void run_chc(size_t population_size, size_t num_of_generations, double crossover_prob, double mutation_prob, size_t chromosome_size, size_t number_of_chromosomes, OptimizationFunction &function, size_t num_of_runs, std::string filename);
