#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <algorithm>
#include <map>
#include <iomanip>

#include "graph.hpp"
#include "algorithms.hpp" 

char* getCmdOption(char** begin, char** end, const std::string& option) {
    char** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end) return *itr;
    return nullptr;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option) {
    return std::find(begin, end, option) != end;
}

void runSelectedAlgorithm(const Graph& g, int startNode, std::vector<long long>& dist) {
#if ALGO_TYPE == 0
    runDijkstraStandard(g, startNode, dist);
#elif ALGO_TYPE == 1
    runDial(g, startNode, dist); 
#elif ALGO_TYPE == 2
    runRadixHeap(g, startNode, dist);
#endif
}

int main(int argc, char* argv[]) {
    // 1. Wczytanie flagi -d (plik z grafem) 
    char* graphFileRaw = getCmdOption(argv, argv + argc, "-d");
    if (!graphFileRaw) {
        std::cerr << "Blad: Brak flagi -d <plik.gr>\n";
        return 1;
    }
    std::string graphFile = graphFileRaw;

    // Wczytanie grafu
    Graph g;
    try {
        std::cout << "Wczytywanie grafu: " << graphFile << " ... ";
        g.loadFromFile(graphFile);
        std::cout << "Gotowe (N=" << g.n << ", M=" << g.m << ")\n";
    } catch (const std::exception& e) {
        std::cerr << "\nBlad: " << e.what() << std::endl;
        return 1;
    }

    // --- TRYB SS (Single Source) 
    if (cmdOptionExists(argv, argv + argc, "-ss")) {
        std::string sourceFile = getCmdOption(argv, argv + argc, "-ss");
        std::string resFile = getCmdOption(argv, argv + argc, "-oss");

        std::ifstream srcIn(sourceFile);
        std::ofstream resOut(resFile);
        
        std::vector<int> sources;
        std::string line, temp;
        
        // Parsowanie pliku .ss 
        while (std::getline(srcIn, line)) {
            if (line.empty() || line[0] == 'c' || line[0] == 'p') continue;
            if (line[0] == 's') {
                std::stringstream ss(line);
                ss >> temp >> temp; // "s" "ID"
                sources.push_back(std::stoi(temp));
            }
        }

        // Nagłówek wyniku .ss.res 
        resOut << "p res sp ss " << ALGO_NAME << "\n";
        resOut << "f " << graphFile << " " << sourceFile << "\n";
        resOut << "g " << g.n << " " << g.m << " " << g.min_weight << " " << g.max_weight << "\n";

        double totalTimeMs = 0;
        std::vector<long long> dist;

        for (int s : sources) {
            auto start = std::chrono::high_resolution_clock::now();
            runSelectedAlgorithm(g, s, dist);
            auto end = std::chrono::high_resolution_clock::now();
            
            totalTimeMs += std::chrono::duration<double, std::milli>(end - start).count();
        }

        // Zapis średniego czasu 
        double avgTime = sources.empty() ? 0.0 : totalTimeMs / sources.size();
        resOut << std::fixed << std::setprecision(2) << "t " << avgTime << "\n";
        
        std::cout << "Zakonczono test SS. Sredni czas: " << avgTime << " ms. Wynik w: " << resFile << "\n";
    }

    // --- TRYB P2P (Point-to-Point)  ---
    if (cmdOptionExists(argv, argv + argc, "-p2p")) {
        std::string pairFile = getCmdOption(argv, argv + argc, "-p2p");
        std::string resFile = getCmdOption(argv, argv + argc, "-op2p");

        std::ifstream pairIn(pairFile);
        std::ofstream resOut(resFile);

        // Optymalizacja: Grupujemy zapytania po źródle
        // Mapa: SourceID -> vector<TargetID>
        std::map<int, std::vector<int>> pairsBySource;
        
        struct Query { int u, v; };
        std::vector<Query> originalOrder; 

        std::string line, temp;
        // Parsowanie pliku .p2p 
        while (std::getline(pairIn, line)) {
            if (line.empty() || line[0] == 'c' || line[0] == 'p') continue;
            if (line[0] == 'q') {
                int u, v;
                std::stringstream ss(line);
                ss >> temp >> u >> v; // "q" "u" "v"
                pairsBySource[u].push_back(v);
                originalOrder.push_back({u, v});
            }
        }

        // Nagłówek wyniku .p2p.res 
        resOut << "p res sp p2p " << ALGO_NAME << "\n";
        resOut << "f " << graphFile << " " << pairFile << "\n";
        resOut << "g " << g.n << " " << g.m << " " << g.min_weight << " " << g.max_weight << "\n";

        // Mapa wyników: "u_v" -> distance 
        std::map<std::string, long long> results;
        std::vector<long long> dist;

        // Wykonanie algorytmu 
        for (auto const& [src, targets] : pairsBySource) {
            runSelectedAlgorithm(g, src, dist);
            
            for (int target : targets) {
                std::string key = std::to_string(src) + "_" + std::to_string(target);
                results[key] = dist[target];
            }
        }

        // Zapis wyników 
        for (const auto& q : originalOrder) {
            std::string key = std::to_string(q.u) + "_" + std::to_string(q.v);
            long long d = results[key];
            
            resOut << "d " << q.u << " " << q.v << " " << d << "\n";
        }

        std::cout << "Zakonczono test P2P. Obliczono " << originalOrder.size() << " par. Wynik w: " << resFile << "\n";
    }

    return 0;
}