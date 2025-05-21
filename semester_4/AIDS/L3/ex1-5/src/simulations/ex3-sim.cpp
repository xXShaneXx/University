#include <simframework/interfaces.hpp>
#include <simframework/simulation.hpp>
#include <matplot/matplot.h>

#include "operationCounter.hpp"
#include "selects.hpp"
#include "logger.hpp"

#include <vector>
#include <random> 
#include <chrono>

using namespace simframework;
using namespace matplot;

struct Input {
    std::vector<int> values;
};

struct Output {
    double comparisons;
    double swaps;
    double time; // Now will store time in microseconds
};

class InputGenerator : public IInputGenerator<Input> {
    public:
        Input generate(int size) const override {
            Input input;
            input.values.resize(size);
        
            static thread_local std::random_device rd;
            static thread_local std::mt19937 gen(rd());
            std::uniform_int_distribution<> dist(0, 2 * size - 1);
        
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
            return output.comparisons;
        }
};
    
class SwapsMetric : public IMetric<Output> {
    public:
        std::string name() const override { return "swaps"; }
        double extract(const Output& output) const override {
            return output.swaps;
        }
};

class TimeMetric : public IMetric<Output> {
    public:
        std::string name() const override { return "time"; }
        double extract(const Output& output) const override {
            return output.time;
        }
};

class SelectSimulator : public ISimulator<Input, Output> {
    private:
        int part_size;
    public:
        SelectSimulator(int part_size) : part_size(part_size) {}
        Output operator()(Input& input) const override {
            Output output;
            OperationCounter::reset();
    
            // Copy the input data to avoid modifying the original
            std::vector<int> data = input.values;
            auto mid = (data.end() - data.begin()) / 2;
    
            auto start = std::chrono::high_resolution_clock::now();
            selects::Select(data.begin(), data.end(), mid, part_size);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;
    
            output.comparisons = OperationCounter::comparisons;
            output.swaps = OperationCounter::swaps;
            output.time = elapsed.count() * 1e6; // Time in microseconds
    
            return output;
        }
    };

void plot_results(const std::map<int, std::map<std::string, std::map<int, double>>>& all_results) {
    std::vector<std::string> colors = {"r", "b", "g", "m"};
    std::vector<std::string> line_styles = {"-", "--", ":", "-."};
    std::vector<int> group_sizes = {3, 5, 7, 9};

    std::vector<std::string> metrics = {"comparisons", "swaps", "time"};
    for (const auto& metric : metrics) {
        auto f = figure(true);
        f->size(800, 600);
        hold(on);

        for (size_t k_idx = 0; k_idx < group_sizes.size(); ++k_idx) {
            int k = group_sizes[k_idx];
            const auto& results = all_results.at(k).at(metric);

            std::vector<double> x, y;
            for (const auto& [n, val] : results) {
                x.push_back(n);
                y.push_back(val);
            }

            auto p = plot(x, y);
            p->color(colors[k_idx]);
            p->line_style(line_styles[k_idx]);
            p->line_width(1.5);
            p->marker_size(6);
            p->display_name("k=" + std::to_string(k)); // Only first line!
        }

        hold(off);
        title(metric);
        xlabel("Input Size (n)");
        ylabel(metric == "time" ? "Time (μs)" : "Count");
        matplot::legend();
        grid(on);

        std::string filename = "plots/ex2_" + metric + "_results.png";
        save(filename);
        show();
    }
}


int main() {
    Logger::enabled = false;
    auto inputGenerator = std::make_shared<InputGenerator>();
    std::vector<int> group_sizes = {3, 5, 7, 9};

    std::map<int, std::map<std::string, std::map<int, double>>> all_results;

    for (int k : group_sizes) {
        auto simulator = std::make_shared<SelectSimulator>(k);
        auto comparisonsMetric = std::make_shared<ComparisonsMetric>();
        auto swapsMetric = std::make_shared<SwapsMetric>();
        auto timeMetric = std::make_shared<TimeMetric>();

        auto simulation = simframework::Simulation<Input, Output>::create()
            .set_range(100, 5000, 100) // Larger range
            .set_repetitions(30)         // Reasonable number of repetitions
            .set_input_generator(inputGenerator)
            .set_simulator(simulator)
            .add_metric(comparisonsMetric)
            .add_metric(swapsMetric)
            .add_metric(timeMetric)
            .build();

        simulation->run();
        auto results = simulation->get_results(std::make_shared<MeanAggregator>());
        all_results[k] = results;
    }

    plot_results(all_results);
    return 0;
}
