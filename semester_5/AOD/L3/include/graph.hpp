#pragma once
#include <vector>
#include <string>
#include <limits>

const long long INF = std::numeric_limits<long long>::max();

struct Edge {
    int to;
    int weight;
};

struct Graph {
    int n = 0; // wierzchołki
    int m = 0; // łuki
    int min_weight = std::numeric_limits<int>::max();
    int max_weight = 0;
    
    std::vector<std::vector<Edge>> adj;

    void loadFromFile(const std::string& filename);
};