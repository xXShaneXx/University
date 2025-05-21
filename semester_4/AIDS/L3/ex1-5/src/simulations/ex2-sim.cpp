#include <simframework/interfaces.hpp>
#include <simframework/simulation.hpp>
#include <matplot/matplot.h>

#include "operationCounter.hpp"
#include "selects.hpp"
#include "logger.hpp"

#include <vector>
#include <random> 

using namespace simframework;
using namespace matplot;

struct Input {
    std::vector<int> values;
};

struct Output {
    double comparsisons;
    double swaps;
};

class InputGenerator : public IInputGenerator<Input> {
    public:
        Input generate(int size) const override {
            Input input;
            input.values.resize(size); // Resize the vector to the given size
        
            static thread_local std::random_device rd; // Static to avoid reinitialization
            static thread_local std::mt19937 gen(rd()); // Static and thread-local for safety
            std::uniform_int_distribution<> dist(0, 2 * size - 1); // Generate values in range [0, 2n-1]
        
            for (int& x : input.values) {
                x = dist(gen);
            }
            return input;
        }
};

class ComparisonsMetric : public IMetric<Output> {
    public:
        std::string name() const override { return "comparisons"; }
        double extract(const Output& output) const override {
            return output.comparsisons;
        }
};
    
class SwapsMetric : public IMetric<Output> {
    public:
        std::string name() const override { return "swaps"; }
        double extract(const Output& output) const override {
            return output.swaps;
        }
};

class SelectSimulator : public ISimulator<Input, Output> {
    public:
        Output operator()(Input& input) const override {
            Output output;
            OperationCounter::reset(); // Reset counters before running Select
    
            // Copy the input values to avoid modifying the original data
            std::vector<int> data = input.values;
    
            // Test Select for different values of k
            int n = data.size();
            for (int k = 1; k <= n; k += n / 10) { // Test for 10 evenly spaced k values
                selects::Select(data.begin(), data.end(), k);
            }
    
            // Record the number of comparisons and swaps
            output.comparsisons = OperationCounter::comparisons;
            output.swaps = OperationCounter::swaps;
    
            return output;
        }
};

class RandomSelectSimulator : public ISimulator<Input, Output> {
    public:
        Output operator()(Input& input) const override {
            Output output;
            OperationCounter::reset(); // Reset counters before running Select
    
            // Copy the input values to avoid modifying the original data
            std::vector<int> data = input.values;
    
            // Test Select for different values of k
            int n = data.size();
            for (int k = 1; k <= n; k += n / 10) { // Test for 10 evenly spaced k values
                selects::RandomSelect(data.begin(), data.end(), k);
            }
    
            // Record the number of comparisons and swaps
            output.comparsisons = OperationCounter::comparisons;
            output.swaps = OperationCounter::swaps;
    
            return output;
        }
};

void plot_results(const std::map<std::string, std::map<int, double>>& results) {
    using namespace matplot;

    std::vector<std::vector<double>> all_x;
    std::vector<std::vector<double>> all_y;
    std::vector<std::string> labels;

    // Prepare the data first
    for (const auto& [metric_name, values] : results) {
        std::vector<double> x, y;
        for (const auto& [n, val] : values) {
            x.push_back(static_cast<double>(n));
            y.push_back(val);
        }
        all_x.push_back(std::move(x));
        all_y.push_back(std::move(y));
        labels.push_back(metric_name);
    }

    // Create one figure
    auto f = figure(true);
    f->size(1000, 700); // bigger figure

    hold(on); // Important: allow multiple lines on same plot

    for (size_t i = 0; i < all_x.size(); ++i) {
        auto p = plot(all_x[i], all_y[i], "-o");
        p->line_width(2);
        p->marker_size(8);
        p->display_name(labels[i]);
    }

    hold(off);

    // Add common labels and legend
    matplot::legend();
    xlabel("Input Size (n)");
    ylabel("Metric Value");
    title("Simulation Results");

    grid(on);
    gca()->font_size(14);
    gca()->x_axis().label_font_size(16);
    gca()->y_axis().label_font_size(16);

    // Save the combined plot
    std::string filename = "plots/all_metrics_results.png";
    save(filename);

    // Display the plot
    show();
}


    int main() {
        Logger::enabled = false;

        auto inputGenerator = std::make_shared<InputGenerator>();
        auto simulator = std::make_shared<RandomSelectSimulator>();
    
        auto comparisonsMetric = std::make_shared<ComparisonsMetric>();
        auto swapsMetric = std::make_shared<SwapsMetric>();
    
        auto simulation = simframework::Simulation<Input, Output>::create()
            .set_range(100, 50000, 100) // n ∈ {100, 200, ..., 50000}
            .set_repetitions(50)        // m = 50 repetitions
            .set_input_generator(inputGenerator)
            .set_simulator(simulator)
            .add_metric(comparisonsMetric)
            .add_metric(swapsMetric)
            .build();
    
        simulation->run();
    
        // Retrieve and print results
        auto results = simulation->get_results(std::make_shared<MeanAggregator>());
        plot_results(results);

    
        return 0;
    }