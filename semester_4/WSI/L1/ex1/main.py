import tensorflow as tf
import datetime

mnist = tf.keras.datasets.mnist

# Load the MNIST dataset
(x_train, y_train),(x_test, y_test) = mnist.load_data()
x_train, x_test = x_train / 255.0, x_test / 255.0 # Normalize the images to [0,1] range

def create_model():
    return tf.keras.models.Sequential([
        tf.keras.layers.Input(shape=(28, 28), name='layers_input'),
        tf.keras.layers.Flatten(name='layers_flatten'),
        tf.keras.layers.Dense(512, activation='relu', name='layers_dense'),
        tf.keras.layers.Dropout(0.2, name='layers_dropout'),
        tf.keras.layers.Dense(10, activation='softmax', name='layers_dense_2')
    ])
# units - number of neurons in the layer
# activation - activation function to use (e.g., 'relu', 'sigmoid', 'softmax')
# relu - [ f(x) = \max(0, x) ]

# Leayers
# Input This layer defines the shape of the input data, which is 28x28 pixels (the size of MNIST images).
# It does not perform any computation but serves as a placeholder for the input data.

# Flatten
# This layer flattens the 2D input (28x28 pixels) into a 1D array of 784 pixels.

# Dense
# This layer is a fully connected layer with 512 neurons and ReLU activation.

# Dropout
#The Dropout layer randomly sets input units to 0 with a frequency of rate at each step during training time, which helps prevent overfitting.
# Inputs not set to 0 are scaled up by 1 / (1 - rate) such that the sum over all inputs is unchanged

# Dense
# This layer is another fully connected layer with 10 neurons (one for each digit) and softmax activation,
# which outputs a probability distribution over the 10 classes.


model = create_model()
model.compile(optimizer='adam',
              loss='sparse_categorical_crossentropy',
              metrics=['accuracy'])
# An optimizer in the context of machine learning and neural networks is an algorithm or method used to adjust the weights of the network to minimize the loss function during training.
# optimizer='adam': Uses the Adam optimizer for training.
# loss='sparse_categorical_crossentropy': Uses sparse categorical cross-entropy as the loss function, suitable for multi-class classification problems with integer labels.
# metrics=['accuracy']: Evaluates the model using accuracy as the metric.

# log_dir: This specifies the directory where TensorBoard logs will be saved.
log_dir = "logs/fit/" + datetime.datetime.now().strftime("%Y%m%d-%H%M%S")
tensorboard_callback = tf.keras.callbacks.TensorBoard(log_dir=log_dir, histogram_freq=1)

model.fit(x=x_train,
          y=y_train,
          epochs=5, # Number of epochs to train the model.
          validation_data=(x_test, y_test),
          callbacks=[tensorboard_callback]) # callbask: This is a list of callbacks to apply during training. In this case, it includes the TensorBoard callback for logging.

model.save('model/my_model.h5')