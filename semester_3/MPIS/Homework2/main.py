import numpy as np
from tqdm import tqdm
from concurrent.futures import ProcessPoolExecutor
import sys

sys.stdout = open('logfile.txt', 'w')

class Simulation():
    def _simulate_single(self, n, k):
        for _ in range(k):
            bins = np.zeros(n, dtype=int)
            empty_bins = n
            non_zero_bins = 0
            bins_with_two = 0

            B, C, D = 0, 0, 0
            throws = 0

            while D == 0:
                throws += 1
                binNum = np.random.randint(n)

                if bins[binNum] == 0:
                    if throws < n:
                        empty_bins -= 1
                    non_zero_bins += 1
                elif bins[binNum] == 1:
                    bins_with_two += 1
                bins[binNum] += 1

                if B == 0 and bins[binNum] == 2:
                    B = throws
                if C == 0 and non_zero_bins == n:
                    C = throws
                if D == 0 and bins_with_two == n:
                    D = throws

            print(f"{n},{B},{empty_bins},{C},{D},{D - C}")

    def simulate(self, n_values, k=50):
        with ProcessPoolExecutor() as executor:
            futures = [executor.submit(self._simulate_single, n, k) for n in n_values]
            for future in tqdm(futures, desc="Simulating for each n"):
                future.result()

if __name__ == '__main__':
    sim = Simulation()
    n_values = range(1000, 100001, 1000)
    sim.simulate(n_values, k=50)