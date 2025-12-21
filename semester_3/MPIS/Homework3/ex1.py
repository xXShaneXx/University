import numpy as np
from tqdm import tqdm
from concurrent.futures import ProcessPoolExecutor
import sys

sys.stdout = open('results.txt', 'w')

class Simulation():
    def _simulate_single(self, n, k, d):
        for _ in range(k):
            bins = np.zeros(n, dtype=int)
            max_load = 0
            bin = 0
            for _ in range(n):
                if d == 1:
                    bin = np.random.randint(n)
                elif d == 2:
                    bin1 = np.random.randint(n)
                    bin2 = bin1
                    while bin1 == bin2:
                        bin2 = np.random.randint(n)
                    bin = bin1 if bins[bin1] < bins[bin2] else bin2
                bins[bin] += 1
                max_load = max(max_load, bins[bin])




            print(f"{n},{d},{max_load}")

    def simulate(self, n_values, d, k=50):
        with ProcessPoolExecutor() as executor:
            futures = [executor.submit(self._simulate_single, n, k, d) for n in n_values]
            for future in tqdm(futures, desc="Simulating for each n"):
                future.result()

if __name__ == '__main__':
    sim = Simulation()
    n_values = range(1000, 100001, 1000)
    sim.simulate(n_values, k=50, d=1)
    sim.simulate(n_values, k=50, d=2)