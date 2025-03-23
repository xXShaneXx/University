import os
import numpy as np
import matplotlib.pyplot as plt

def main():
    # Create the plots directory if it does not exist
    if not os.path.exists('plots'):
        os.makedirs('plots')

    # Load simulation data
    data = np.loadtxt('results.txt', delimiter=' ', skiprows=0)  # Skip header
    n_values = np.unique(data[:, 0])  # Extract unique n values

    # Initialize placeholders for averages
    cmp_avg, s_avg = [], []

    # Compute averages for each n
    for n in n_values:
        subset = data[(data[:, 0] == n)]
        cmp_avg.append(np.mean(subset[:, 2]))  # Mean of L for d=1
        s_avg.append(np.mean(subset[:, 3]))  # Mean of L for d=2

    # Convert averages to arrays for easier computation
    cmp_avg, s_avg = map(np.array, [cmp_avg, s_avg])

    # Compute ratios for the plots
    ratios = {
        "cmp_avg/n": cmp_avg / n_values,
        "cmp_avg/n**2": cmp_avg / n_values**2,
        "s_avg/n": s_avg / n_values,
        "s_avg/n**2": s_avg / n_values**2
    }

    # Map labels to their corresponding average arrays
    avg_map = {
        "cmp_avg": cmp_avg,
        "s_avg": s_avg
    }

    # Plot individual repetitions and mean values
    variables = {
        "cmp_avg": 1,
        "s_avg": 2
    }

    for label, col in variables.items():
        plt.figure()  # Create a new figure for each plot
        for n in n_values:
            subset = data[(data[:, 0] == n)]
            # Individual repetitions scatter plot
            plt.scatter([n] * len(subset), subset[:, col + 1], color='blue' , s=10, alpha=0.5, label='Individual repetitions' if n == n_values[0] else "")
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

    # Plot each ratio individually
    for label, ratio in ratios.items():
        plt.figure()
        plt.plot(n_values, ratio, color='green', linewidth=2, label=label)
        plt.xlabel('$n$')
        plt.ylabel(label)
        plt.title(label)
        plt.grid(True)
        plt.legend()
        plt.tight_layout()
        safe_label = label.replace('/', '_').replace('*', '_')
        plt.savefig(f'plots/{safe_label}.png', dpi=300)
        plt.close()

if __name__ == '__main__':
    main()