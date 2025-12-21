# MNIST Classification Project

## 📌 Overview
This repository contains three different implementations for classifying handwritten digits from the MNIST dataset:

1. **Deep Learning Approach** - Neural Network using TensorFlow/Keras
2. **Model Inference** - Loading and using a pre-trained model on custom images
3. **Machine Learning Approach** - Random Forest Classifier using scikit-learn

## 🗂️ File Descriptions

### 1. Neural Network Implementation (`main.py`)
Trains a sequential neural network model with TensorFlow/Keras.

**Key Features:**
- Creates a model with Input, Flatten, Dense, and Dropout layers
- Uses Adam optimizer and sparse categorical crossentropy loss
- Implements TensorBoard callbacks for visualization
- Saves trained model to `model/my_model.h5`
