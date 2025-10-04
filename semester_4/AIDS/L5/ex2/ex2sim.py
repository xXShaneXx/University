import matplotlib.pyplot as plt
from ex2 import inform_order
from ex1 import kruskal, generate_complete_graph_edges
from collections import defaultdict
import random

def run_experiment(n_values, trials=10):
    avg_rounds = []
    min_rounds = []
    max_rounds = []

    for n in n_values:
        rounds_list = []
        for _ in range(trials):
            # Wygeneruj graf i MST
            edges = generate_complete_graph_edges(n)
            mst = kruskal(n, edges)

            # Zamień MST na listę sąsiedztwa
            tree_adj = defaultdict(list)
            for _, u, v in mst:
                tree_adj[u].append(v)
                tree_adj[v].append(u)

            # Losowy korzeń
            root = random.randint(0, n - 1)
            order, dp = inform_order(tree_adj, root)

            rounds_list.append(dp[root])

        avg_rounds.append(sum(rounds_list) / trials)
        min_rounds.append(min(rounds_list))
        max_rounds.append(max(rounds_list))

    return avg_rounds, min_rounds, max_rounds

def plot_results(n_values, avg, minv, maxv):
    plt.figure(figsize=(10, 6))
    plt.plot(n_values, avg, label="Średnia", marker='o')
    plt.plot(n_values, minv, label="Min", linestyle='--')
    plt.plot(n_values, maxv, label="Max", linestyle='--')
    plt.xlabel("Liczba wierzchołków")
    plt.ylabel("Liczba rund")
    plt.title("Rozsyłanie informacji w drzewie (Average Case Analysis)")
    plt.legend()
    plt.grid(True)
    plt.show()


if __name__ == "__main__":
    n_values = list(range(100, 5000, 100))
    avg, minv, maxv = run_experiment(n_values, trials=10)
    plot_results(n_values, avg, minv, maxv)
