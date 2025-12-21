import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import MaxNLocator
from Network import NeuralNetwork

def normalize_l1(X):
    return X / np.sum(np.abs(X), axis=1, keepdims=True)

def normalize_l2(X):
    return X / np.sqrt(np.sum(np.square(X), axis=1, keepdims=True))

def main():
    # Generowanie danych
    np.random.seed(42)
    X = np.random.uniform(-1, 1, (100, 2))
    X = X[(X[:,0] != 0) & (X[:,1] != 0)]
    y = ((X[:,0] > 0) == (X[:,1] > 0)).astype(int).reshape(-1, 1)

    # Przygotowanie wykresów
    plt.figure(figsize=(18, 12))

    # Testowanie różnych konfiguracji
    configurations = [
        ('Nieznormalizowane', X),
        ('L1 znormalizowane', normalize_l1(X)),
        ('L2 znormalizowane', normalize_l2(X))
    ]

    learning_rates = [0.01, 0.1, 0.5]
    activations = ['sigmoid', 'relu']

    for i, (name, X_norm) in enumerate(configurations):
        for j, activation in enumerate(activations):
            plt.subplot(3, 2, i*2 + j + 1)
            plt.title(f'{name} - {activation}')
            plt.xlabel('Epoka')
            plt.ylabel('Loss')
            plt.grid(True)

            for lr in learning_rates:
                nn = NeuralNetwork(activation=activation, learning_rate=lr)
                nn.train(X_norm, y, epochs=500)
                plt.plot(nn.loss_history, label=f'lr={lr}')
                plt.legend()

            # Dodatkowe ustawienia wykresu
            ax = plt.gca()
            ax.xaxis.set_major_locator(MaxNLocator(integer=True))
            if i == 0 and j == 0:
                plt.ylim(0, 0.3)  # Dla lepszej czytelności pierwszego wykresu

    plt.tight_layout()
    plt.show()

    # Wykres porównawczy dla najlepszej konfiguracji
    best_nn = NeuralNetwork(activation='relu', learning_rate=0.1)
    best_nn.train(normalize_l2(X), y, epochs=500)

    plt.figure(figsize=(10, 6))
    plt.plot(best_nn.loss_history)
    plt.title('Najlepsza konfiguracja: L2 + ReLU + lr=0.1')
    plt.xlabel('Epoka')
    plt.ylabel('Loss')
    plt.grid(True)
    plt.show()

    # Testowanie dokładności
    test_data = np.array([[0.5, 0.3], [-0.4, 0.6], [0.7, -0.2], [-0.1, -0.9]])
    test_data_norm = normalize_l2(test_data)
    predictions = best_nn.predict(test_data_norm)

    print("Przewidywania dla danych testowych:")
    for data, pred in zip(test_data, predictions):
        print(f"{data} -> {pred[0]}")

if __name__ == "__main__":
    main()