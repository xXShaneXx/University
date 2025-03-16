#include "comperer.hpp"

bool Comperer::compare(T& x1, T& x2)
{

    comparsion++;
    return x1 > x2;
}