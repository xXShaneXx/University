#include "algorithms.hpp"
#include <queue>
#include <vector>
#include <limits>

using ull = unsigned long long;

void runDijkstraStandard(const Graph& g, int startNode, std::vector<long long>& dist) {
    ull start = static_cast<ull>(startNode);
    ull n = g.n; // Wierzchołki są indeksowane 1..n, więc rozmiar to n+1
    
    dist.assign(n + 1, std::numeric_limits<long long>::max());
    dist[start] = 0;

    using Pair = std::pair<ull, ull>; // {distance, u}
    std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> pq;
    pq.emplace(0, start);

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > static_cast<ull>(dist[u])) continue;

        for (const auto& edge : g.adj[u]) {
            ull v = edge.to;
            ull weight = edge.weight;

            if (dist[u] + weight < static_cast<ull>(dist[v])) {
                dist[v] = dist[u] + weight;
                pq.emplace(dist[v], v);
            }
        }
    }
}