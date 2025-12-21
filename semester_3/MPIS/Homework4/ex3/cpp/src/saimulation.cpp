#include "simulation.hpp"
#include <random>

double simulate_random_walk(int N) 
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 1);

    int S = 0;  // pozycja w błądzeniu losowym
    int L = 0;  // liczba kroków nad osią OX

    for (int n = 1; n <= N; ++n) 
    {
        int step = (dist(gen) == 0) ? -1 : 1; // krok w górę (1) lub w dół (-1)
        S += step;

        if (S > 0 || (S == 0 && step == -1)) 
        {
            L++;
        }
    }

    return static_cast<double>(L) / N;
}