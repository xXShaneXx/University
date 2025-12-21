import math
import numpy as np
import scipy.stats as stats
import matplotlib.pyplot as plt

def plot_binomial_vs_normal(n):
    x = np.arange(n + 1)
    cdf = stats.binom.cdf(x, n, 0.5)
    mean = stats.binom.mean(n, 0.5)
    variance = stats.binom.var(n, 0.5)

    transformed_x = 2 * x - n
    transformed_cdf = cdf
    transformed_mean = 2 * mean - n
    transformed_variance = 4 * variance

    norm_x = np.linspace(-n, n, 1000)
    norm_cdf = stats.norm.cdf(norm_x, transformed_mean, math.sqrt(transformed_variance))

    plt.figure()
    plt.title(f'N = {n}')
    plt.bar(transformed_x[:-1], transformed_cdf[:-1], width=np.diff(transformed_x), align='edge', color='royalblue', label='Sn CDF')
    plt.plot(norm_x, norm_cdf, color='red', label='Normal distribution CDF')
    plt.ylim([0, 1])
    plt.xlabel('x')
    plt.ylabel('P(X <= x)')
    plt.legend()
    plt.show()
    plt.close()

n_values = [10, 15, 20, 25, 30, 100]
for n in n_values:
    plot_binomial_vs_normal(n)