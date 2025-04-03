import subprocess
import numpy as np
import matplotlib.pyplot as plt
import re

# Function to run the sorting algorithm and collect results
def run_sorting_algorithm(algorithm, n, k):
    comparisons = []
    swaps = []
    for _ in range(k):
        process1 = subprocess.Popen(['./bin/random-generator', str(n)], stdout=subprocess.PIPE)
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
algorithms = ['insertion-sort', 'quick-sort', 'hybrid-sort']
n_values_small = range(10, 51, 10)
n_values_large = range(1000, 50001, 1000)
k_values = [1, 10, 100]

# Collecting results
results = {algorithm: {'n': [], 'comparisons': [], 'swaps': []} for algorithm in algorithms}

for algorithm in algorithms:
    n_values = n_values_small if algorithm == 'insertion-sort' else n_values_large
    for n in n_values:
        for k in k_values:
            print(f'Running {algorithm} with n={n} and k={k}')
            mean_comparisons, mean_swaps = run_sorting_algorithm(algorithm, n, k)
            results[algorithm]['n'].append(n)
            results[algorithm]['comparisons'].append(mean_comparisons)
            results[algorithm]['swaps'].append(mean_swaps)
            print(f'Completed {algorithm} with n={n} and k={k}: comparisons={mean_comparisons}, swaps={mean_swaps}')

# Saving results to file
with open('sorting_results.txt', 'w') as f:
    for algorithm in algorithms:
        f.write(f'{algorithm}\n')
        for n, c, s in zip(results[algorithm]['n'], results[algorithm]['comparisons'], results[algorithm]['swaps']):
            f.write(f'n={n}, comparisons={c}, swaps={s}\n')

# Plotting graphs
for k in k_values:
    plt.figure(figsize=(12, 8))
    for algorithm in algorithms:
        n_values = results[algorithm]['n']
        comparisons = results[algorithm]['comparisons']
        swaps = results[algorithm]['swaps']
        plt.plot(n_values, comparisons, label=f'{algorithm} comparisons (k={k})')
        plt.plot(n_values, swaps, label=f'{algorithm} swaps (k={k})')
    plt.xlabel('n')
    plt.ylabel('Count')
    plt.title(f'Comparisons and Swaps for k={k}')
    plt.legend()
    plt.show()