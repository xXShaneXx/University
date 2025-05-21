import tensorflow as tf
from tensorflow.keras.preprocessing import image
import numpy as np
import os

# Load the saved model
model = tf.keras.models.load_model('model/my_model.h5')

# Load and preprocess your images
def load_and_preprocess_image(img_path):
    img = image.load_img(img_path, color_mode='grayscale', target_size=(28, 28))
    img_array = image.img_to_array(img)
    img_array = img_array / 255.0  # Normalize to [0, 1]
    return img_array

# Directory containing your images
img_dir = os.path.join(os.path.dirname(__file__), 'GebalaMNIST')

# Load and preprocess all images in the directory
img_arrays = []
img_names = []
for img_name in os.listdir(img_dir):
    img_path = os.path.join(img_dir, img_name)
    img_array = load_and_preprocess_image(img_path)
    img_arrays.append(img_array)
    img_names.append(img_name)

# Convert list to numpy array and add batch dimension
img_arrays = np.array(img_arrays)

# Predict using the loaded model
predictions = model.predict(img_arrays)
predicted_classes = np.argmax(predictions, axis=1)

# Calculate accuracy
correct_predictions = 0
for img_name, predicted_class in zip(img_names, predicted_classes):
    print(f'Image: {img_name}, Predicted class: {predicted_class}')
    # Extract true class from filename, e.g., '0.1.png' -> 0
    true_class = int(img_name.split('.')[0])
    if predicted_class == true_class:
        correct_predictions += 1

accuracy = correct_predictions / len(img_names)
print(f'Accuracy: {accuracy:.2f}')