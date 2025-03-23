import os
import numpy as np
import matplotlib.pyplot as plt

def main():
    # Create the plots directory if it does not exist
    if not os.path.exists('plots'):
        os.makedirs('plots')

    # Load simulation data
    data = np.loadtxt('results.txt', delimiter=',', skiprows=0)  # Skip header
    n_values = np.unique(data[:, 0])  # Extract unique n values

    # Initialize placeholders for averages
    l_d1_avg, l_d2_avg = [], []

    # Compute averages for each n
    for n in n_values:
        subset1 = data[(data[:, 0] == n) & (data[:, 1] == 1)]
        subset2 = data[(data[:, 0] == n) & (data[:, 1] == 2)]
        l_d1_avg.append(np.mean(subset1[:, 2]))  # Mean of L for d=1
        l_d2_avg.append(np.mean(subset2[:, 2]))  # Mean of L for d=2

    # Convert averages to arrays for easier computation
    l_d1_avg, l_d2_avg = map(np.array, [l_d1_avg, l_d2_avg])

    # Compute ratios for the plots
    ratios = {
        "l_d1 * ln(ln(n))/ln(n)": l_d1_avg * np.log(np.log(n_values)) / np.log(n_values),
        "l_d1 * ln(2) / ln(ln(n))": l_d1_avg * np.log(2) / np.log(np.log(n_values)),
        "l_d2 * ln(ln(n))/ln(n)": l_d2_avg * np.log(np.log(n_values)) / np.log(n_values),
        "l_d2 * ln(2) / ln(ln(n))": l_d2_avg * np.log(2) / np.log(np.log(n_values))
    }

    # Map labels to their corresponding average arrays
    avg_map = {
        "L_d1": l_d1_avg,
        "L_d2": l_d2_avg
    }

    # Plot individual repetitions and mean values
    variables = {
        "L_d1": 1,
        "L_d2": 2
    }

    for label, col in variables.items():
        plt.figure()  # Create a new figure for each plot
        for n in n_values:
            subset = data[(data[:, 0] == n) & (data[:, 1] == col)]
            # Individual repetitions scatter plot
            plt.scatter([n] * len(subset), subset[:, 2], s=10, alpha=0.5, label='Individual repetitions' if n == n_values[0] else "")
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
