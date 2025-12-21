import sympy as sp
import matplotlib.pyplot as plt
import numpy as np


class Simulation():
    @staticmethod
    def generate_points(n, a, b, c=0, M=100):
        points = []
        rng = np.random.Generator(np.random.MT19937())
        for _ in range(n):
            x = rng.uniform(a, b)
            y = rng.uniform(0, M)
            points.append((x, y))
        return points

    @staticmethod
    def find_sup(a, b, f):
        x = sp.symbols('x')
        f_sympy = f(x)
        f_prime = sp.diff(f_sympy, x)
        critical_points = sp.solve(f_prime, x)
        values = [f_sympy.subs(x, point) for point in critical_points if a <= point <= b]
        values.append(f_sympy.subs(x, a))
        values.append(f_sympy.subs(x, b))
        return max(values)

    @staticmethod
    def calc_ratio(points, f):
        count = 0
        for x, y in points:
            if f(x) >= y:
                count += 1
        return count / len(points)

    @staticmethod
    def integrate(f, a, b):
        x = sp.symbols('x')
        f_sympy = f(x)
        return sp.integrate(f_sympy, (x, a, b))

class Plot:
    @staticmethod
    def create_plot(integral, avg, results, title, suptitle , filename):
        x = [x for x, y in results]
        y = [y for x, y in results]

        plt.scatter(x, y, color='blue', s=1, label='Approximated Area')

        x_avg = [x for x, y in avg]
        y_avg = [y for x, y in avg]
        plt.scatter(x_avg, y_avg, color='red', s=20, label='Mean Approximated Area')

        plt.axhline(y=integral, color='green', linewidth=3, label=f'Integral = {integral:.2f}')

        plt.xlabel('Number of Points (n)', fontsize=14)
        plt.ylabel('Estimated Area', fontsize=14)
        plt.title(title, fontsize=16)
        plt.suptitle(suptitle, fontsize=10)
        plt.grid(True)
        plt.legend(fontsize=12)
        plt.tight_layout()

        plt.savefig(filename, dpi=300)
        plt.show()

def run_simulation(a, b, begin, end, step, f, k, title="", subtitle="", filename="plot.png"):
    M = Simulation.find_sup(a, b, f)
    range_area = (b - a) * M
    avg = []
    results = []

    # Convert the symbolic function to a numerical function
    x = sp.symbols('x')
    f_numeric = sp.lambdify(x, f(x), 'numpy')

    for n in range(begin, end, step):
        cur_results = []
        for i in range(k):  # Perform `k` simulations for each n
            points = Simulation.generate_points(n, a, b, M=M)
            area = Simulation.calc_ratio(points, f_numeric) * range_area
            cur_results.append((n, area))

            print(f"n={n}, k={i}, area={area}")
        # Take the mean of the areas across `k` trials
        avg.append((n, np.mean([area for n, area in cur_results])))
        results.extend(cur_results)

    integral = Simulation.integrate(f, a, b)
    Plot.create_plot(integral, avg, results, title, subtitle, filename)

    return avg[-1][1]

if __name__ == '__main__':
    # Integral 1: ∫[0, 8] √x dx
    run_simulation(0, 8, 50, 5000, 50, lambda x: x**(1/3), 5, "Integral 1: $\\int_{0}^{8} \\sqrt[3]{x} \\, dx$", f'k={5}', 'integral1_k=5.png')
    run_simulation(0, 8, 50, 5000, 50, lambda x: x**(1/3), 50, "Integral 1: $\\int_{0}^{8} \\sqrt[3]{x} \\, dx$", f'k={50}', 'integral1_k=50.png')

    # Integral 1: ∫[0, π] sin(x) dx
    run_simulation(0, sp.pi, 50, 5000, 50, lambda x: sp.sin(x), 5, "Integral 2: $\\int_{0}^{\\pi} \\sin(x) \\, dx$", f'k={5}', 'integral2_k=5.png')
    run_simulation(0, sp.pi, 50, 5000, 50, lambda x: sp.sin(x), 50, "Integral 2: $\\int_{0}^{\\pi} \\sin(x) \\, dx$", f'k={50}', 'integral2_k=50.png')

    # Integral 2: ∫[0, 1] 4x(1 − x)^3 dx
    #run_simulation(0, 1, 50, 5000, 50, lambda x: 4*x*(1-x)**3, 5, "Integral 3: $\\int_{0}^{1} 4x(1-x)^3 \\, dx$", f'k={5}', 'integral3_k=5.png')
    #run_simulation(0, 1, 50, 5000, 50, lambda x: 4*x*(1-x)**3, 50, "Integral 3: $\\int_{0}^{1} 4x(1-x)^3 \\, dx$", f'k={50}', 'integral3_k=50.png')

    # Approximate π using the area of a quarter circle
    avg_pi = run_simulation(0, 1, 50, 5000, 50, lambda x: 4 * sp.sqrt(1 - x**2), 50, "Approximation of π", "Monte Carlo Method: $\\pi \\approx 4 \\times \\int_{0}^{1} \\sqrt{1 - x^2} \\, dx$", 'approx_pi.png')
    print(f"Approximated value of π: {avg_pi}")
