#include <matplot/matplot.h>
#include "histogram.hpp"
#include <string>

void histogram(std::vector<double> P_values, int num_bins, std::string title, std::string filename)
{
    auto h = matplot::hist(P_values, 20); 
    h->face_color("blue");
    matplot::title(title);
    matplot::xlabel("P_N");
    matplot::ylabel("PDF");
    matplot::grid(true);

    matplot::save(filename);
}
