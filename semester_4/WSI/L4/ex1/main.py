import numpy as np
import matplotlib.pyplot as plt
from sklearn.datasets import fetch_openml
from sklearn.preprocessing import StandardScaler
from collections import Counter
import random

# Wczytaj dane EMNIST MNIST (784 px obrazy 28x28)
def load_emnist_mnist():
    emnist = fetch_openml('mnist_784', version=1)
    X = emnist.data.astype(np.float32)
    y = emnist.target.astype(np.int32)
    return X, y

# Inicjalizacja centroidów: k-means++
def initialize_centroids(X, k):
    n_samples = X.shape[0]
    centroids = [X[np.random.choice(n_samples)]]

    for _ in range(1, k):
        distances = np.min([np.sum((X - c) ** 2, axis=1) for c in centroids], axis=0)
        probs = distances / distances.sum()
        cumulative_probs = np.cumsum(probs)
        r = np.random.rand()
        for j, p in enumerate(cumulative_probs):
            if r < p:
                centroids.append(X[j])
                break
    return np.array(centroids)

def kmeans(X, k, max_iter=100):
    centroids = initialize_centroids(X, k)
    for _ in range(max_iter):
        distances = np.zeros((X.shape[0], k))
        for i in range(k):
            distances[:, i] = np.sum((X - centroids[i])**2, axis=1)  # Squared Euclidean
        labels = np.argmin(distances, axis=1)

        new_centroids = np.array([X[labels == i].mean(axis=0) if len(X[labels == i]) > 0 else centroids[i] for i in range(k)])

        if np.allclose(centroids, new_centroids):
            break
        centroids = new_centroids

    # Oblicz inercję
    inertia = np.sum((X - centroids[labels]) ** 2)
    return centroids, labels, inertia

# Macierz przydziału cyfr do klastrów
def cluster_label_distribution(labels, true_labels, k):
    matrix = np.zeros((10, k), dtype=int)
    for i in range(k):
        indices = np.where(labels == i)[0]
        cluster_digits = true_labels[indices]
        counts = Counter(cluster_digits)
        for digit in range(10):
            matrix[digit, i] = counts[digit]
    return matrix

# Normalizacja danych
def normalize(X):
    return StandardScaler().fit_transform(X)

# Wizualizacja: macierz przydziałów
def plot_confusion_matrix(matrix, k):
    percentages = matrix / matrix.sum(axis=1, keepdims=True) * 100
    plt.figure(figsize=(10, 8))
    plt.imshow(percentages, cmap='viridis')
    plt.colorbar(label='Procent (%)')
    plt.xlabel("Klastry")
    plt.ylabel("Cyfry")
    plt.title(f'Procentowy przydział cyfr do {k} klastrów')
    plt.xticks(range(k))
    plt.yticks(range(10))
    plt.show()

# Wizualizacja: centroidy
def plot_centroids(centroids, k):
    fig, axes = plt.subplots(1, k, figsize=(k, 2))
    for i in range(k):
        axes[i].imshow(centroids[i].reshape(28, 28), cmap='gray')
        axes[i].axis('off')
        axes[i].set_title(f"C{i}")
    plt.show()

# Główna procedura
def run_for_k(k_values=[10, 15, 20, 30]):
    X, y = load_emnist_mnist()
    X = normalize(X)

    for k in k_values:
        best_inertia = float('inf')
        for attempt in range(3):
            centroids, labels, inertia = kmeans(X, k)
            if inertia < best_inertia:
                best_centroids = centroids
                best_labels = labels
                best_inertia = inertia

        print(f'\nLiczba klastrów: {k} | Najlepsza inercja: {best_inertia:.2f}')
        matrix = cluster_label_distribution(best_labels, y, k)
        plot_confusion_matrix(matrix, k)
        plot_centroids(best_centroids, k)

# Uruchom eksperyment
run_for_k([30])
