import os
import numpy as np
import matplotlib.pyplot as plt

def main():
    # Create the plots directory if it does not exist
    if not os.path.exists('plots'):
        os.makedirs('plots')

    # Load simulation data
    data = np.loadtxt('logfile.txt', delimiter=',', skiprows=0)  # Skip header
    n_values = np.unique(data[:, 0])  # Extract unique n values

    # Initialize placeholders for averages
    b_avg, u_avg, c_avg, d_avg, dc_avg = [], [], [], [], []

    # Compute averages for each n
    for n in n_values:
        subset = data[data[:, 0] == n]
        b_avg.append(np.mean(subset[:, 1]))  # Mean of B
        u_avg.append(np.mean(subset[:, 2]))  # Mean of U
        c_avg.append(np.mean(subset[:, 3]))  # Mean of C
        d_avg.append(np.mean(subset[:, 4]))  # Mean of D
        dc_avg.append(np.mean(subset[:, 5]))  # Mean of D - C

    # Convert averages to arrays for easier computation
    b_avg, u_avg, c_avg, d_avg, dc_avg = map(np.array, [b_avg, u_avg, c_avg, d_avg, dc_avg])

    # Compute ratios for the plots
    ratios = {
        "b(n)/n": b_avg / n_values,
        "b(n)/sqrt(n)": b_avg / np.sqrt(n_values),
        "u(n)/n": u_avg / n_values,
        "c(n)/n": c_avg / n_values,
        "c(n)/(nln(n))": c_avg / (n_values * np.log(n_values)),
        "c(n)/n^2": c_avg / (n_values**2),
        "d(n)/n": d_avg / n_values,
        "d(n)/(nln(n))": d_avg / (n_values * np.log(n_values)),
        "d(n)/n^2": d_avg / (n_values**2),
        "d(n)-c(n)/n": dc_avg / n_values,
        "d(n)-c(n)/(nln(n))": dc_avg / (n_values * np.log(n_values)),
        "d(n)-c(n)/(nln(ln(n)))": dc_avg / (n_values * np.log(np.log(n_values))),
    }

    # Map labels to their corresponding average arrays
    avg_map = {
        "B": b_avg,
        "U": u_avg,
        "C": c_avg,
        "D": d_avg,
        "D-C": dc_avg
    }

    # Plot individual repetitions and mean values for B, U, C, D, D-C
    variables = {
        "B": 1,
        "U": 2,
        "C": 3,
        "D": 4,
        "D-C": 5
    }

    for label, col in variables.items():
        for n in n_values:
            subset = data[data[:, 0] == n]
            plt.scatter([n] * len(subset), subset[:, col], s=1, label='Individual repetitions' if n == n_values[0] else "")  # Individual repetitions
        mean_values = avg_map[label]
        plt.plot(n_values, mean_values, color='green', linewidth=2, label=f'Mean {label}')  # Mean values
        plt.xlabel('n')
        plt.ylabel(label)
        plt.title(f'{label} vs n')
        plt.grid(True)
        plt.legend()
        plt.tight_layout()
        plt.savefig(f'plots/{label}_vs_n.png', dpi=300)
        plt.close()

    # Plot each ratio individually
    for label, ratio in ratios.items():
        plt.plot(n_values, ratio, color='green', linewidth=2, label=label)
        plt.xlabel('n')
        plt.ylabel(label)
        plt.title(label)
        plt.grid(True)
        plt.legend()
        plt.tight_layout()
        safe_label = label.replace('/', '_')
        plt.savefig(f'plots/{safe_label}.png', dpi=300)
        plt.close()

if __name__ == '__main__':
    main()