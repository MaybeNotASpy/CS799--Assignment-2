#include "main.hpp"

void parameter_search()
{
    auto dejong1 = dejong::DeJong1();
    auto dejong2 = dejong::DeJong2();
    auto dejong3 = dejong::DeJong3();
    auto dejong4 = dejong::DeJong4();
    auto dejong5 = dejong::DeJong5();
    random_parameter_search(50, 100, 0.7, 0.001, 32, 3, dejong1, 1000, "dejong1.csv");
    random_parameter_search(50, 100, 0.7, 0.001, 32, 2, dejong2, 1000, "dejong2.csv");
    random_parameter_search(50, 100, 0.7, 0.001, 32, 5, dejong3, 1000, "dejong3.csv");
    random_parameter_search(50, 100, 0.7, 0.001, 32, 10, dejong4, 1000, "dejong4.csv");
    random_parameter_search(50, 100, 0.7, 0.001, 32, 2, dejong5, 1000, "dejong5.csv");
}

void GAPerformance()
{
    auto dejong1 = dejong::DeJong1();
    auto dejong2 = dejong::DeJong2();
    auto dejong3 = dejong::DeJong3();
    auto dejong4 = dejong::DeJong4();
    auto dejong5 = dejong::DeJong5();
    run_simple_ga(180, 130, 0.66, 0.0064, 32, 3, dejong1, 30, "ga_performance_dejong1.csv");
    run_simple_ga(130, 170, 0.6, 0.001, 32, 2, dejong2, 30, "ga_performance_dejong2.csv");
    run_simple_ga(140, 140, 0.1085, 0.0025, 32, 5, dejong3, 30, "ga_performance_dejong3.csv");
    run_simple_ga(180, 100, 0.68, 0.058, 32, 10, dejong4, 30, "ga_performance_dejong4.csv");
    run_simple_ga(60, 30, 0.013, 0.0028, 32, 2, dejong5, 30, "ga_performance_dejong5.csv");
}

void CHCPerformance()
{
    auto dejong1 = dejong::DeJong1();
    auto dejong2 = dejong::DeJong2();
    auto dejong3 = dejong::DeJong3();
    auto dejong4 = dejong::DeJong4();
    auto dejong5 = dejong::DeJong5();
    run_chc(50, 75, 0.95, 0.05, 32, 3, dejong1, 30, "chc_performance_dejong1.csv");
    run_chc(50, 75, 0.95, 0.05, 32, 2, dejong2, 30, "chc_performance_dejong2.csv");
    run_chc(50, 75, 0.95, 0.05, 32, 5, dejong3, 30, "chc_performance_dejong3.csv");
    run_chc(50, 75, 0.95, 0.05, 32, 10, dejong4, 30, "chc_performance_dejong4.csv");
    run_chc(50, 75, 0.95, 0.05, 32, 2, dejong5, 30, "chc_performance_dejong5.csv");
}

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        if (strcmp(argv[1], "parameter_search") == 0)
        {
            parameter_search();
        }
        else if (strcmp(argv[1], "ga_performance") == 0)
        {
            GAPerformance();
        }
        else if (strcmp(argv[1], "chc_performance") == 0)
        {
            CHCPerformance();
        }
        else
        {
            std::cout << "Invalid argument" << std::endl;
        }
    }
    else
    {
        std::cout << "Invalid number of arguments" << std::endl;
        std::cout << "Usage: " << argv[0] << " <parameter_search|ga_performance|chc_performance> "  << std::endl;
    }
    return 0;
}