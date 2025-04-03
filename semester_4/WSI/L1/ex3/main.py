import numpy as np
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score
from sklearn.metrics import classification_report, confusion_matrix
import seaborn as sns
import matplotlib.pyplot as plt
import tensorflow as tf

# Load the MNIST dataset
mnist = tf.keras.datasets.mnist
(x_train, y_train), (x_test, y_test) = mnist.load_data()

# Flatten the images and normalize the pixel values
x_train = x_train.reshape((x_train.shape[0], -1)) / 255.0
x_test = x_test.reshape((x_test.shape[0], -1)) / 255.0

# Train the RandomForest model
# estimators - number of trees
# random_state - seed for reproducibility
rf_model = RandomForestClassifier(n_estimators=100, random_state=42)
rf_model.fit(x_train, y_train)

# Evaluate the model
y_pred = rf_model.predict(x_test)
print(f'Accuracy: {accuracy_score(y_test, y_pred)}')

# Calculate the confusion matrix
conf_matrix = confusion_matrix(y_test, y_pred)

# Visualize the confusion matrix
plt.figure(figsize=(10,7))
sns.heatmap(conf_matrix, annot=True, fmt='d', cmap='Blues')
plt.xlabel('Predykcje')
plt.ylabel('Prawdziwe etykiety')
plt.title('Macierz Pomyłek - RandomForest')
plt.show()

# Calculate recall and precision
report = classification_report(y_test, y_pred, digits=4)
print(report)

