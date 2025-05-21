import tensorflow as tf
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
from sklearn.metrics import confusion_matrix, classification_report

# Read the model
model = tf.keras.models.load_model('model/my_model.h5')

# Load MNIST
mnist = tf.keras.datasets.mnist
(_, _), (x_test, y_test) = mnist.load_data()
x_test = x_test / 255.0  # Normalization to [0,1] range

# Reshape the test data to match the input shape of the model
y_pred = model.predict(x_test)
y_pred_classes = np.argmax(y_pred, axis=1)  # Get the predicted classes

# Obliczenie macierzy pomyłek
conf_matrix = confusion_matrix(y_test, y_pred_classes)

# Visualisation of the confusion matrix
plt.figure(figsize=(10,7))
sns.heatmap(conf_matrix, annot=True, fmt='d', cmap='Blues')
plt.xlabel('Predykcje')
plt.ylabel('Prawdziwe etykiety')
plt.title('Macierz Pomyłek')
plt.show()

# Obliczenie czułości i precyzji dla każdej klasy
report = classification_report(y_test, y_pred_classes, digits=4)
print(report)
