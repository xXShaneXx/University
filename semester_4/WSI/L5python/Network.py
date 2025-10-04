import numpy as np

class NeuralNetwork:
    def __init__(self, activation='sigmoid', learning_rate=0.1):
        np.random.seed(42)
        self.weights1 = np.random.randn(2, 4) * 0.1
        self.weights2 = np.random.randn(4, 1) * 0.1
        self.bias1 = np.zeros(4)
        self.bias2 = np.zeros(1)
        self.activation = activation
        self.learning_rate = learning_rate
        self.loss_history = []

    def sigmoid(self, x):
        return 1 / (1 + np.exp(-x))

    def relu(self, x):
        return np.maximum(0, x)

    def sigmoid_derivative(self, x):
        return self.sigmoid(x) * (1 - self.sigmoid(x))

    def relu_derivative(self, x):
        return (x > 0).astype(float)

    def forward(self, x):
        self.z1 = np.dot(x, self.weights1) + self.bias1
        if self.activation == 'sigmoid':
            self.a1 = self.sigmoid(self.z1)
        else:
            self.a1 = self.relu(self.z1)

        self.z2 = np.dot(self.a1, self.weights2) + self.bias2
        self.output = self.sigmoid(self.z2)
        return self.output

    def backward(self, x, y, output):
        error = output - y

        delta2 = error * self.sigmoid_derivative(self.z2)
        d_weights2 = np.dot(self.a1.T, delta2)
        d_bias2 = np.sum(delta2, axis=0)

        if self.activation == 'sigmoid':
            delta1 = np.dot(delta2, self.weights2.T) * self.sigmoid_derivative(self.z1)
        else:
            delta1 = np.dot(delta2, self.weights2.T) * self.relu_derivative(self.z1)
        d_weights1 = np.dot(x.T, delta1)
        d_bias1 = np.sum(delta1, axis=0)

        self.weights2 -= self.learning_rate * d_weights2
        self.bias2 -= self.learning_rate * d_bias2
        self.weights1 -= self.learning_rate * d_weights1
        self.bias1 -= self.learning_rate * d_bias1

    def train(self, X, y, epochs=1000):
        self.loss_history = []
        for epoch in range(epochs):
            output = self.forward(X)
            self.backward(X, y, output)
            loss = np.mean(np.square(output - y))
            self.loss_history.append(loss)

    def predict(self, x):
        return np.round(self.forward(x))