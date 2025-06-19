import random
import heapq

def generate_complete_graph_edges(n):
    edges = []
    for i in range(n):
        for j in range(i + 1, n):
            weight = random.uniform(0, 1)
            edges.append((weight, i, j))
    return edges

def comparator(a, b):
    return a[1] - b[1]

# Disjoint set data structure
class DSU:
    def __init__(self, n):
        self.parent = list(range(n))
        self.rank = [1] * n

    def find(self, i):
        if self.parent[i] != i:
            self.parent[i] = self.find(self.parent[i])
        return self.parent[i]

    def union(self, x, y):
        s1 = self.find(x)
        s2 = self.find(y)
        if s1 != s2:
            if self.rank[s1] < self.rank[s2]:
                self.parent[s1] = s2
            elif self.rank[s1] > self.rank[s2]:
                self.parent[s2] = s1
            else:
                self.parent[s2] = s1
                self.rank[s1] += 1

def kruskal(n, edges):
    mst = []
    dsu = DSU(n)
    edges = sorted(edges)
    for weight, u, v in edges:
        if dsu.find(u) != dsu.find(v):
            dsu.union(u, v)
            mst.append((weight, u, v))
    return mst

def prim(n, edges):
    # Build adjacency list
    adj = [[] for _ in range(n)]
    for weight, u, v in edges:
        adj[u].append((weight, v))
        adj[v].append((weight, u))

    visited = [False] * n
    min_heap = [(0, 0, -1)]  # (weight, current_vertex, previous_vertex)
    mst = []

    while min_heap and len(mst) < n - 1:
        weight, u, prev = heapq.heappop(min_heap)
        if visited[u]:
            continue
        visited[u] = True
        if prev != -1:
            mst.append((weight, prev, u))
        for next_weight, v in adj[u]:
            if not visited[v]:
                heapq.heappush(min_heap, (next_weight, v, u))
    return mst


if __name__ == "__main__":
    n = int(input("Enter number of vertices: "))
    edges = generate_complete_graph_edges(n)
    for edge in sorted(edges):
        print(f"{edge[1]} -- {edge[2]} [weight={edge[0]:.3f}]")