#include "algorithms.hpp"
#include <vector>
#include <limits>
#include <bit>     
#include <algorithm>

using ull = unsigned long long;

template<typename Key, typename Value>
class RadixHeap {
public:
    RadixHeap() : last(0) {
        buckets.resize(65); // enough for 64-bit integers
    }

    bool empty() const {
        return sz == 0;
    }

    void push(Key key, Value value) {
        ull b = bucketIndex(key);
        buckets[b].push_back({key, value});
        ++sz;
    }

    std::pair<Key, Value> pop_min() {
        if (!buckets[0].empty()) {
            auto [k, v] = buckets[0].back();
            buckets[0].pop_back();
            --sz;
            last = k;
            return {k, v};
        }

        // Find first non-empty bucket
        ull b = 1;
        while (b < buckets.size() && buckets[b].empty()) b++;

        if (b == buckets.size()) return {last, Value()}; // Should not happen if !empty()

        // Rebucket elements
        Key new_min = std::numeric_limits<Key>::max();
        for (auto &p : buckets[b]) {
            new_min = std::min(new_min, p.first);
        }
        last = new_min;

        std::vector<std::pair<Key, Value>> tmp;
        tmp.swap(buckets[b]);

        for (auto &[k, v] : tmp) {
            buckets[bucketIndex(k)].push_back({k, v});
        }

        return pop_min(); // now bucket 0 is non-empty
    }

private:
    std::vector<std::vector<std::pair<Key, Value>>> buckets;
    Key last = 0;
    ull sz = 0;

    ull bucketIndex(Key key) const {
        Key diff = key ^ last;
        if (diff == 0) return 0;
        // std::bit_width zwraca liczbę bitów potrzebnych do zapisu liczby.
        return static_cast<ull>(std::bit_width(static_cast<std::make_unsigned_t<Key>>(diff)));
    }
};


void runRadixHeap(const Graph& g, int startNode, std::vector<long long>& dist) {
    ull start = static_cast<ull>(startNode);
    ull n = g.n;
    const long long INF = std::numeric_limits<long long>::max();

    dist.assign(n + 1, INF);
    dist[start] = 0;

    RadixHeap<ull, ull> heap;
    heap.push(0, start);

    while (!heap.empty()) {
        auto [d, u] = heap.pop_min();
        
        // Skip outdated entries
        if (d != static_cast<ull>(dist[u])) continue; 

        for (const auto& edge : g.adj[u]) {
            ull v = edge.to;
            ull w = edge.weight;
            ull nd = d + w;
            
            if (nd < static_cast<ull>(dist[v])) {
                dist[v] = nd;
                heap.push(nd, v);
            }
        }
    }
}