import random
import sys
from concurrent.futures import ProcessPoolExecutor
import matplotlib.pyplot as plt
from tqdm import tqdm

sys.stdout = open('results.txt', 'w')

# Funkcja do sortowania przez wstawianie
def insertion_sort(arr):
    comparisons = 0  # liczba porównań
    swaps = 0  # liczba przestawień
    n = len(arr)

    for i in range(1, n):
        key = arr[i]
        j = i - 1

        # Przesuwamy elementy, które są większe od klucza, o jedno miejsce w prawo
        while j >= 0 and arr[j] > key:
            arr[j + 1] = arr[j]
            j -= 1
            comparisons += 1
            swaps += 1

        arr[j + 1] = key

        if j >= 0:  # Jeżeli nie wykonano żadnego przesunięcia, należy policzyć jedno porównanie
            comparisons += 1
    return comparisons, swaps

# Funkcja do generowania permutacji
def generate_permutation(n):
    return random.sample(range(1, n + 1), n)

# Funkcja do przeprowadzenia eksperymentu
def run_experiment(n, k):
    for kk in range(k):
        arr = generate_permutation(n)
        comparisons, swaps = insertion_sort(arr)
        print(f"{n} {kk} {comparisons} {swaps}")

# Funkcja do zbierania danych i generowania wykresów
def start_experiment(n_values, k):
    with ProcessPoolExecutor() as executor:
        futures = [executor.submit(run_experiment, n, k) for n in n_values]
    for future in tqdm(futures, desc="Simulating for each n"):
        future.result()

if __name__ == '__main__':
    n_values = range(100, 10001, 100)
    k = 50
    start_experiment(n_values, k)