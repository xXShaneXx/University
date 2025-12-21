#include "algorithms.hpp"
#include <vector>
#include <limits>
#include <algorithm>

void runDial(const Graph& g, int startNode, std::vector<long long>& dist) {
    dist.assign(g.n + 1, INF);
    dist[startNode] = 0;

    int C = g.max_weight;
    if (C == 0) C = 1; 
    
    int bucketsSize = C + 1;
    
    // buckets[k] przechowuje wierzchołki u, dla których dist[u] % bucketsSize == k
    std::vector<std::vector<int>> buckets(bucketsSize);

    // Wstawienie źródła do kubełka 0
    buckets[0].push_back(startNode);

    int cursor = 0;           // Wskaźnik na obecny kubełek
    int processedCount = 0;   // Licznik przetworzonych wierzchołków
    
    long long elementsInQueue = 1;

    while (elementsInQueue > 0) {

        while (buckets[cursor].empty()) {
            cursor = (cursor + 1) % bucketsSize;
        }
        
        while (!buckets[cursor].empty()) {
            int u = buckets[cursor].back();
            buckets[cursor].pop_back();
            elementsInQueue--;

            if (dist[u] % bucketsSize != cursor) {
                continue;
            }
            
            for (const auto& edge : g.adj[u]) {
                int v = edge.to;
                int w = edge.weight;

                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    
                    int newBucketIdx = dist[v] % bucketsSize;
                    buckets[newBucketIdx].push_back(v);
                    elementsInQueue++;
                }
            }
        }
        
        cursor = (cursor + 1) % bucketsSize;
    }
}