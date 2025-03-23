import os
import random
import sys
from concurrent.futures import ProcessPoolExecutor
import matplotlib.pyplot as plt
import numpy as np
from tqdm import tqdm

sys.stdout = open('results.txt', 'w')

class Simulation:
    @staticmethod
    def rand():
        return np.random.random()
    @staticmethod
    def simulate(n: int, p: float, k: int = 50):
        for _ in range(k):
           stations = np.array(np.zeros(n), dtype=bool)
           rounds = 0
           while not all(stations):
               rounds += 1
               for i in range(n):
                    if Simulation.rand() <= p:
                        stations[i] = True
           print(f"{n} {p} {rounds}")

    @staticmethod
    def run(n_values, p: float):
        with ProcessPoolExecutor() as executor:
            futures = [executor.submit(Simulation.simulate, n, p) for n in n_values]
        for future in tqdm(futures, desc="Simulating for each n"):
            future.result()

def plot():
    if not os.path.exists('plots'):
        os.makedirs('plots')

    data = np.loadtxt('results.txt', delimiter=' ')
    n_values =  np.unique(data[:, 0])

    p1_avg, p2_avg = [], []

    for n in n_values:
        subset1 = data[(data[:, 0] == n) & (data[:, 1] == 0.1)]
        subset2 = data[(data[:, 0] == n) & (data[:, 1] == 0.5)]
        p1_avg.append(np.mean(subset1[:, 2]))
        p2_avg.append(np.mean(subset2[:, 2]))

    p1_avg, p2_avg = map(np.array, [p1_avg, p2_avg])

    avg_map = {
        "p=0.1" : p1_avg,
        "p=0.5" : p2_avg
    }

    variables ={
        "p=0.1" : 0.1,
        "p=0.5" : 0.5,
    }

    for label, col in variables.items():
        plt.figure()  # Create a new figure for each plot
        for n in n_values:
            subset = data[(data[:, 0] == n) & (data[:, 1] == col)]
            # Individual repetitions scatter plot
            plt.scatter([n] * len(subset), subset[:, 2], s=2, alpha=0.5, color='blue' , label='Individual repetitions' if n == n_values[0] else "")
        # Plot mean values
        mean_values = avg_map[label]
        plt.plot(n_values, mean_values, color='green', linewidth=2, label=f'Mean {label}')
        plt.xlabel('$n$')
        plt.ylabel(label)
        plt.title(f'{label} vs $n$')
        plt.grid(True)
        plt.legend()
        plt.tight_layout()
        plt.savefig(f'plots/{label}_vs_n.png', dpi=300)
        plt.close()



if __name__ == "__main__":
    n_values = range(1000, 100001, 1000)
    Simulation.run(n_values, 0.5)
    Simulation.run(n_values, 0.1)
    plot()



