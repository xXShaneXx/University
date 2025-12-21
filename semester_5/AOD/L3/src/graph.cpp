#include "graph.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

void Graph::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Nie mozna otworzyc pliku: " + filename);

    std::string line, temp;
    char type;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == 'c') continue;

        std::stringstream ss(line);
        ss >> type;

        if (type == 'p') {
            // Format: p sp <n> <m> 
            ss >> temp >> n >> m;
            adj.resize(n + 1);
        } else if (type == 'a') {
            // Format: a <u> <v> <w>
            int u, v, w;
            ss >> u >> v >> w;
            adj[u].push_back({v, w});
            
            // Aktualizacja statystyk dla pliku wynikowego
            if (w < min_weight) min_weight = w;
            if (w > max_weight) max_weight = w;
        }
    }
}