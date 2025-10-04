#include "Network.hpp"
#include <vector>


std::vector<std::vector<double>> Network::feedforward(const std::vector<double>& input) {
    std::vector<std::vector<double>> activations;
    activations.push_back(input);
    std::vector<double> activation = input;
    for (const auto& layer : layers) {
        std::vector<double> next_activation(layer.biases.size());
        for (size_t neuron = 0; neuron < layer.weights.size(); ++neuron) {
            double sum = layer.biases[neuron];
            for (size_t i = 0; i < layer.weights[neuron].size(); ++i) {
                sum += layer.weights[neuron][i] * activation[i];
            }
            next_activation[neuron] = layer.activation ? (*layer.activation)(sum) : sum;
        }
        activations.push_back(next_activation);
        activation = next_activation;
    }
    return activations;
}

void Network::updateMiniBatch(const std::vector<double>& x, const std::vector<double>& y) {
    
}

std::pair<std::vector<std::vector<std::vector<double>>>, std::vector<std::vector<double>>>
Network::backprop(const std::vector<double>& x, const std::vector<double>& y) {
    // Initialize nabla_b and nabla_w with zeros, matching the shapes of biases and weights
    std::vector<std::vector<std::vector<double>>> nabla_w;
    std::vector<std::vector<double>> nabla_b;
    for (const auto& layer : layers) {
        nabla_b.push_back(std::vector<double>(layer.biases.size(), 0.0));
        nabla_w.push_back(std::vector<std::vector<double>>(layer.weights.size(),
            std::vector<double>(layer.weights[0].size(), 0.0)));
    }

    // Feedforward
    std::vector<std::vector<double>> activations;
    std::vector<std::vector<double>> zs;
    std::vector<double> activation = x;
    activations.push_back(activation);
    for (const auto& layer : layers) {
        std::vector<double> z(layer.biases.size());
        std::vector<double> next_activation(layer.biases.size());
        for (size_t neuron = 0; neuron < layer.biases.size(); ++neuron) {
            double sum = layer.biases[neuron];
            for (size_t i = 0; i < layer.weights[neuron].size(); ++i) {
                sum += layer.weights[neuron][i] * activation[i];
            }
            z[neuron] = sum;
            next_activation[neuron] = layer.activation ? layer.activation->operator()(sum) : sum;
        }
        zs.push_back(z);
        activations.push_back(next_activation);
        activation = next_activation;
    }

    // Backward pass
    size_t L = layers.size();
    // Output error delta
    std::vector<double> delta(activations.back().size());

    for (size_t i = 0; i < delta.size(); ++i) {
        double dC_da = cost->derivative(activations.back(), y)[i];
        double da_dz = layers.back().activation->derivative(zs.back()[i]);
        delta[i] = dC_da * da_dz;
    }
    nabla_b[L-1] = delta;
    // nabla_w for last layer: outer product of delta and activations from previous layer
    for (size_t j = 0; j < delta.size(); ++j) {
        for (size_t k = 0; k < activations[L-1].size(); ++k) {
            nabla_w[L-1][j][k] = delta[j] * activations[L-1][k];
        }
    }

    // Backpropagate the error
    for (int l = L - 2; l >= 0; --l) {
        std::vector<double> sp(zs[l].size());
        for (size_t j = 0; j < zs[l].size(); ++j) {
            sp[j] = layers[l].activation->derivative(zs[l][j]);
        }
        std::vector<double> new_delta(layers[l].biases.size(), 0.0);
        for (size_t j = 0; j < layers[l].biases.size(); ++j) {
            double sum = 0.0;
            for (size_t k = 0; k < layers[l+1].biases.size(); ++k) {
                sum += layers[l+1].weights[k][j] * delta[k];
            }
            new_delta[j] = sum * sp[j];
        }
        delta = new_delta;
        nabla_b[l] = delta;
        for (size_t j = 0; j < delta.size(); ++j) {
            for (size_t k = 0; k < activations[l].size(); ++k) {
                nabla_w[l][j][k] = delta[j] * activations[l][k];
            }
        }
    }

    return {nabla_w, nabla_b};
}