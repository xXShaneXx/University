import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd
from sklearn.datasets import fetch_openml
from sklearn.preprocessing import StandardScaler
import umap
from collections import deque, Counter
from sklearn.metrics import adjusted_rand_score

# ------------------ DBSCAN manual implementation ------------------

def euclidean_dist(p1, p2):
    return np.linalg.norm(p1 - p2)

def region_query(X, point_idx, eps):
    neighbors = []
    for i in range(len(X)):
        if euclidean_dist(X[point_idx], X[i]) <= eps:
            neighbors.append(i)
    return neighbors

def expand_cluster(X, labels, point_idx, neighbors, cluster_id, eps, min_samples):
    labels[point_idx] = cluster_id
    queue = deque(neighbors)

    while queue:
        current_point = queue.popleft()
        if labels[current_point] == -1:  # szum
            labels[current_point] = cluster_id
        if labels[current_point] != 0:
            continue

        labels[current_point] = cluster_id
        current_neighbors = region_query(X, current_point, eps)
        if len(current_neighbors) >= min_samples:
            queue.extend(current_neighbors)

def dbscan(X, eps, min_samples):
    labels = [0] * len(X)  # 0 = niezbadane, -1 = szum, >0 = ID klastra
    cluster_id = 0

    for i in range(len(X)):
        if labels[i] != 0:
            continue

        neighbors = region_query(X, i, eps)
        if len(neighbors) < min_samples:
            labels[i] = -1  # szum
        else:
            cluster_id += 1
            expand_cluster(X, labels, i, neighbors, cluster_id, eps, min_samples)

    return np.array(labels)

# ------------------ Wczytywanie i przetwarzanie danych ------------------

print("Wczytywanie MNIST...")
mnist = fetch_openml('mnist_784', version=1)
X = mnist.data.astype(np.float32)
y = mnist.target.astype(int)

# Dla szybkości – 5000 próbek
X = X[:5000]
y = y[:5000]

print("Skalowanie...")
scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)

print("Redukcja wymiarowości UMAP...")
X_umap = umap.UMAP(n_neighbors=15, min_dist=0.1, metric='euclidean', random_state=42).fit_transform(X_scaled)

# ------------------ Klasteryzacja DBSCAN ------------------

print("Klasteryzacja (ręczna implementacja DBSCAN)...")
eps = 0.25
min_samples = 10
labels = dbscan(X_umap, eps, min_samples)

# ------------------ Wizualizacja ------------------

plt.figure(figsize=(10, 8))
sns.scatterplot(x=X_umap[:, 0], y=X_umap[:, 1], hue=labels, palette='tab20', s=5, legend=None)
plt.title("DBSCAN (manualna implementacja) na MNIST po UMAP")
plt.show()

# ------------------ Ewaluacja ------------------

# Adjusted Rand Index
mask = labels != -1
ari = adjusted_rand_score(y[mask], labels[mask])
print(f"\n🎯 Adjusted Rand Index (bez szumu): {ari:.4f}")

# Procent szumu
total_points = len(labels)
noise_points = np.sum(labels == -1)
noise_percent = 100 * noise_points / total_points
print(f"📉 Procent punktów oznaczonych jako szum: {noise_percent:.2f}%")

# Dokładność klasyfikacji w klastrach
df = pd.DataFrame({'true': y, 'cluster': labels})
correct = 0
total_clustered = 0

for cluster_id, group in df[df.cluster != -1].groupby('cluster'):
    most_common_digit, count = Counter(group['true']).most_common(1)[0]
    correct += count
    total_clustered += len(group)

accuracy = 100 * correct / total_clustered
error = 100 - accuracy

print(f"✅ Dokładność klasyfikacji (tylko w klastrach): {accuracy:.2f}%")
print(f"❌ Procent błędnych klasyfikacji w klastrach: {error:.2f}%")

# Dominujące cyfry w klastrach
dominant_digits = df[df.cluster != -1].groupby('cluster')['true'].agg(lambda x: Counter(x).most_common(1)[0])
print("\n📋 Dominująca cyfra w każdym klastrze:")
print(dominant_digits)
