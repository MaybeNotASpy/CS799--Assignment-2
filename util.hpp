#pragma once
#include <random>

// Global random number generator.
std::mt19937& get_generator()
{
    static std::random_device rd;
    static thread_local std::mt19937 generator = std::mt19937(rd());
    return generator;
}