import subprocess
import numpy as np
import matplotlib.pyplot as plt
import re

# Function to run the sorting algorithm and collect results (unchanged)
def run_sorting_algorithm(algorithm, n, k):
    comparisons = []
    swaps = []
    for _ in range(k):
        process1 = subprocess.Popen(['./bin/random-generator', str(n)], stdout=subprocess.PIPE)
        process2 = subprocess.Popen([f'./bin/{algorithm}'], stdin=process1.stdout, stdout=subprocess.PIPE)
        process1.stdout.close()

        output, _ = process2.communicate()
        output_str = output.decode().strip()

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
algorithms = ['merge-sort', 'smart-merge-sort']
n_values = range(100, 10001, 100)
k_values = [100]

# Results dictionary
results = {
    algorithm: {k: {
        'n': [],
        'comparisons': [],
        'swaps': [],
        'comparisons_per_n': [],
        'swaps_per_n': []
    } for k in k_values}
    for algorithm in algorithms
}

# Run experiments
for algorithm in algorithms:
    for n in n_values:
        for k in k_values:
            print(f'Running {algorithm} with n={n} and k={k}')
            mean_comparisons, mean_swaps = run_sorting_algorithm(algorithm, n, k)
            results[algorithm][k]['n'].append(n)
            results[algorithm][k]['comparisons'].append(mean_comparisons)
            results[algorithm][k]['swaps'].append(mean_swaps)
            results[algorithm][k]['comparisons_per_n'].append(mean_comparisons / n)
            results[algorithm][k]['swaps_per_n'].append(mean_swaps / n)
            print(f'→ Done: comparisons={mean_comparisons}, swaps={mean_swaps}')

# Generic plot function
def plot_results(title, y_key, ylabel, algorithms_to_plot, n_filter=lambda n: True):
    for k in k_values:
        plt.figure(figsize=(12, 8))
        for algorithm in algorithms_to_plot:
            data = results[algorithm][k]
            filtered = [
                (n, y_val) for n, y_val in zip(data['n'], data[y_key]) if n_filter(n)
            ]
            if not filtered:
                continue
            n_vals, y_vals = zip(*filtered)
            plt.plot(n_vals, y_vals, label=f'{algorithm} (k={k})')
        plt.xlabel('n')
        plt.ylabel(ylabel)
        plt.title(f'{title} (k={k})')
        plt.legend()
        plt.grid(True)
        plt.tight_layout()
        plt.show()

# Plot raw counts
plot_results("Total Comparisons", "comparisons", "Comparisons", algorithms)
plot_results("Total Swaps", "swaps", "Swaps", algorithms)

# Plot normalized counts
plot_results("Comparisons per n", "comparisons_per_n", "Comparisons / n", algorithms)
plot_results("Swaps per n", "swaps_per_n", "Swaps / n", algorithms)

