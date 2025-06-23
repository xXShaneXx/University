#ifndef NETWORK_HPP
#define NETWORK_HPP
#include <vector>
#include <string>
#include <functional>
#include <memory>
#include <cmath>

struct ICost {
    virtual double operator()(const std::vector<double>& output, const std::vector<double>& target) const = 0;
    virtual std::vector<double> derivative(const std::vector<double>& output, const std::vector<double>& target) const = 0;
    virtual ~ICost() = default;
};

struct MSECost : public ICost {
    // Mean Squared Error
    double operator()(const std::vector<double>& output, const std::vector<double>& target) const override {
        double sum = 0.0;
        for (size_t i = 0; i < output.size(); ++i) {
            double diff = output[i] - target[i];
            sum += diff * diff;
        }
        return sum / output.size();
    }
    std::vector<double> derivative(const std::vector<double>& output, const std::vector<double>& target) const override {
        std::vector<double> grad(output.size());
        for (size_t i = 0; i < output.size(); ++i) {
            grad[i] = 2.0 * (output[i] - target[i]) / output.size();
        }
        return grad;
    }
};

struct CrossEntropyCost : public ICost {
    // Binary Cross-Entropy
    double operator()(const std::vector<double>& output, const std::vector<double>& target) const override {
        double sum = 0.0;
        for (size_t i = 0; i < output.size(); ++i) {
            sum -= target[i] * std::log(output[i] + 1e-12) + (1 - target[i]) * std::log(1 - output[i] + 1e-12);
        }
        return sum / output.size();
    }
    std::vector<double> derivative(const std::vector<double>& output, const std::vector<double>& target) const override {
        std::vector<double> grad(output.size());
        for (size_t i = 0; i < output.size(); ++i) {
            grad[i] = (output[i] - target[i]) / ((output[i] + 1e-12) * (1 - output[i] + 1e-12)) / output.size();
        }
        return grad;
    }
};

struct IActivation {
    virtual double operator()(double x) const = 0;
    virtual double derivative(double x) const = 0;
    virtual ~IActivation() = default;
};

struct Sigmoid : public IActivation {
    double operator()(double x) const override {
        return 1.0 / (1.0 + exp(-x));
    }
    double derivative(double x) const override {
        double s = (*this)(x);
        return s * (1 - s);
    }
};

struct Tanh : public IActivation {
    double operator()(double x) const override {
        return tanh(x);
    }
    double derivative(double x) const override {
        double t = tanh(x);
        return 1 - t * t;
    }
};

class Layer {
public:
    std::vector<std::vector<double>> weights; // [neuron][input]
    std::vector<double> biases; // [neuron]
    std::shared_ptr<IActivation> activation;

    Layer(size_t num_inputs, size_t num_neurons, std::shared_ptr<IActivation> act)
        : activation(act)
    {
        weights.resize(num_neurons, std::vector<double>(num_inputs));
        biases.resize(num_neurons);
        // Initialize weights and biases
    }
};

class Network {
private:
    std::vector<Layer> layers;
    std::shared_ptr<ICost> cost;
    double learning_rate; // learnig rate
    unsigned int epoch; // number of iteration
    unsigned int batch_size; // batch size for gradient deccent

public:
    Network(const std::vector<size_t>& topology, std::shared_ptr<IActivation> act = std::make_shared<Sigmoid>()) {
        for (size_t i = 1; i < topology.size(); ++i) {
            layers.emplace_back(topology[i-1], topology[i], act);
        }
    }

    void setCost(std::shared_ptr<ICost> new_cost) {
        cost = new_cost;
    }

    std::vector<std::vector<double>> feedforward(const std::vector<double>& input);

    // stochastic gradient descent
    void SGD(const std::vector<std::vector<double>>& training_data,
             const std::vector<std::vector<double>>& training_labels,
             unsigned int epochs,
             unsigned int batch_size,
             double learning_rate);

    void updateMiniBatch(const std::vector<double>& x, const std::vector<double>& y);

    // Returns a tuple of nabla_w and nabla_b (gradients for weights and biases) layer by layer
    std::pair<std::vector<std::vector<std::vector<double>>>, std::vector<std::vector<double>>>
    backprop(const std::vector<double>& x, const std::vector<double>& y);

    // Evaluate the network's accuracy on test data
    double evaluate(const std::vector<std::vector<double>>& test_data,
                   const std::vector<std::vector<double>>& test_labels);

    // Save and load network parameters
    void save(const std::string& filename) const;
    void load(const std::string& filename);
};

#endif