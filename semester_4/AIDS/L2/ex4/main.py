import subprocess
import numpy as np
import matplotlib.pyplot as plt
import re

# Function to run the sorting algorithm and collect results
def run_sorting_algorithm(algorithm, generator, n, k):
    comparisons = []
    swaps = []
    for _ in range(k):
        process1 = subprocess.Popen([f'./bin/{generator}', str(n)], stdout=subprocess.PIPE)
        process2 = subprocess.Popen([f'./bin/{algorithm}'], stdin=process1.stdout, stdout=subprocess.PIPE)
        process1.stdout.close()

        output, _ = process2.communicate()
        output_str = output.decode().strip()

    # Use regular expressions to find the number of comparisons and moves
    comparison_match = re.search(r'Total comparisons:\s*(\d+)', output_str)
    moves_match = re.search(r'Total moves:\s*(\d+)', output_str)

    if comparison_match and moves_match:
        comparisons.append(int(comparison_match.group(1)))
        swaps.append(int(moves_match.group(1)))
    else:
        comparisons.append(0)
        swaps.append(0)

    return np.mean(comparisons), np.mean(swaps)

# Experiment parameters
algorithms = ['quick-sort', 'dual-pivot-quicksort']
generators = ['random-generator', 'increase-generator', 'decrease-generator']
n_values = range(8, 32, 1)
k_values = [100]

results = {algorithm: {generator: {k: {'n': [], 'comparisons': [], 'swaps': []} for k in k_values} for generator in generators} for algorithm in algorithms}

# Storing data
for algorithm in algorithms:
    for generator in generators:
        for n in n_values:
            for k in k_values:
                print(f'Running {algorithm} with n={n} and k={k}')
                mean_comparisons, mean_swaps = run_sorting_algorithm(algorithm, generator, n, k)
                results[algorithm][generator][k]['n'].append(n)
                results[algorithm][generator][k]['comparisons'].append(mean_comparisons)
                results[algorithm][generator][k]['swaps'].append(mean_swaps)
                print(f'Completed {algorithm} for {generator} with n={n} and k={k}: comparisons={mean_comparisons}, swaps={mean_swaps}')

# Plotting quick-sort and dual-pivot quick-sort
for k in k_values:
    for generator in generators:
        plt.figure(figsize=(12, 8))
        for algorithm in algorithms:
            n_values = results[algorithm][generator][k]['n']
            comparisons = results[algorithm][generator][k]['comparisons']
            swaps = results[algorithm][generator][k]['swaps']
            plt.plot(n_values, comparisons, label=f'{algorithm} comparisons (k={k})')
            plt.plot(n_values, swaps, label=f'{algorithm} swaps (k={k})')
        plt.xlabel('n')
        plt.ylabel('Count')
        plt.title(f'Comparisons and Swaps for k={k} with {generator}')
        plt.legend()
        plt.show()