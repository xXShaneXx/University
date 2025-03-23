#include <cmath>
#include <matplot/matplot.h>
#include <array>
#include <filesystem>
#include "simulation.hpp"
#include "histogram.hpp"

// Use the OUTPUT_DIRECTORY
#ifndef OUTPUT_DIRECTORY
#define OUTPUT_DIRECTORY "plots"
#endif

int main() 
{
    std::array<int, 3> n{100, 1000, 10000};
    int k = 5000;

    std::filesystem::create_directory(OUTPUT_DIRECTORY);

    for (const auto& n_value : n) 
    {
        std::vector<double> P_values;
        for (int i = 0; i < k; ++i) 
        {
            P_values.push_back(simulate_random_walk(n_value));
        }

        std::string title = "Histogram for N=" + std::to_string(n_value);
        std::string filename = std::string(OUTPUT_DIRECTORY) + "/Histogram_N=" + std::to_string(n_value) + ".png";
        histogram(P_values, 20, title, filename);
    }

    return 0;
}
