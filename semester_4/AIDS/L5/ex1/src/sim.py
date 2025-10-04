import time
import matplotlib.pyplot as plt

# Wczytaj algorytmy i generowanie grafu z main.py
from main import generate_complete_graph_edges, kruskal, prim

nMin = 250
nMax = 5000
step = 250
rep = 5

ns = list(range(nMin, nMax + 1, step))
kruskal_times = []
prim_times = []

for n in ns:
    total_kruskal_time = 0
    total_prim_time = 0
    for _ in range(rep):
        edges = generate_complete_graph_edges(n)

        start = time.perf_counter()
        kruskal(n, edges)
        total_kruskal_time += time.perf_counter() - start

        start = time.perf_counter()
        prim(n, edges)
        total_prim_time += time.perf_counter() - start

    kruskal_times.append(total_kruskal_time / rep)
    prim_times.append(total_prim_time / rep)
    print(f"n = {n}: Kruskal = {kruskal_times[-1]:.5f}s, Prim = {prim_times[-1]:.5f}s")

# Rysowanie wykresu
plt.figure(figsize=(10, 6))
plt.plot(ns, kruskal_times, label="Kruskal", marker="o")
plt.plot(ns, prim_times, label="Prim", marker="s")
plt.xlabel("Liczba wierzchołków (n)")
plt.ylabel("Średni czas wykonania [s]")
plt.title("Porównanie czasu działania algorytmów Kruskala i Prima")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()


