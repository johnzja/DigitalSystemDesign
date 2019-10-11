from __future__ import absolute_import, division, print_function, unicode_literals

# TensorFlow and tf.keras
import tensorflow as tf
from tensorflow import keras

# Helper libraries
import numpy as np
import matplotlib.pyplot as plt
from LoadFiles import load_files


print(tf.__version__)

# === Import the Fashion MNIST dataset ===

train_images, train_labels, test_images, test_labels = load_files()

class_names = ['T-shirt/top', 'Trouser', 'Pullover', 'Dress', 'Coat',
               'Sandal', 'Shirt', 'Sneaker', 'Bag', 'Ankle boot']


# Write a class named LossHistory to save loss and acc
class LossHistory(keras.callbacks.Callback):
    def on_train_begin(self, logs={}):
        self.losses = {'batch':[], 'epoch':[]}
        self.acc = {'batch':[], 'epoch':[]}
        self.val_losses = {'batch': [], 'epoch': []}
        self.val_acc = {'batch': [], 'epoch': []}

    def on_batch_end(self, batch, logs={}):
        self.losses['batch'].append(logs.get('loss'))
        self.acc['batch'].append(logs.get('accuracy'))
        self.val_losses['batch'].append(logs.get('val_loss'))
        self.val_acc['batch'].append(logs.get('val_accuracy'))

    def on_epoch_end(self, batch, logs={}):
        self.losses['epoch'].append(logs.get('loss'))
        self.acc['epoch'].append(logs.get('accuracy'))
        self.val_losses['epoch'].append(logs.get('val_loss'))
        self.val_acc['epoch'].append(logs.get('val_accuracy'))

    def loss_plot(self, loss_type, curve_color, curve_label):
        iters = range(len(self.losses[loss_type]))
        plt.figure(1)
        plt.plot(iters, self.val_losses[loss_type], curve_color, label='loss'+curve_label)
        plt.grid(True)
        plt.xlabel(loss_type)
        plt.ylabel('loss')
        plt.legend(loc="upper right")
        plt.figure(2)
        plt.plot(iters, self.val_acc[loss_type], curve_color, label='acc'+curve_label)
        plt.grid(True)
        plt.xlabel(loss_type)
        plt.ylabel('acc')
        plt.legend(loc="upper right")


def train_and_plot(model_struc, x_train, y_train, x_test, y_test, optimer, curve_color, curve_label):
    # Compile the model
    model_struc.compile(optimizer=optimer,
                        loss='sparse_categorical_crossentropy',
                        metrics=['accuracy'])
    # Train the model
    model_struc.fit(x_train, y_train,
              batch_size=1000, epochs=10, verbose=2,
              validation_data=(x_test, y_test),
              callbacks=[history])
    # Evaluate accuracy
    test_loss, test_acc = model_struc.evaluate(x_test, y_test, verbose=2)
    print('\nTest accuracy:', test_acc)
    # plot loss curve
    history.loss_plot('epoch', curve_color, curve_label)


# === Preprocess the data ===
train_images_conv = train_images.reshape(train_images.shape[0], 28, 28, 1)
test_images_conv = test_images.reshape(test_images.shape[0], 28, 28, 1)
train_images = train_images / 255.0
test_images = test_images / 255.0
train_images_conv = train_images_conv / 255.0
test_images_conv = test_images_conv / 255.0


# creat a instanse of LossHistory
history = LossHistory()

# Set up the layers
model = keras.Sequential([
    keras.layers.Flatten(input_shape=(28, 28)),
    keras.layers.Dense(128, activation='relu'),
    keras.layers.Dense(10, activation='softmax')
])

model_1 = keras.Sequential([
    keras.layers.Flatten(input_shape=(28, 28)),
    keras.layers.Dense(128, activation='relu'),
    keras.layers.Dense(10, activation='softmax')
])

train_and_plot(model, train_images, train_labels, test_images, test_labels, 'adam', 'r', '(Adam)')

train_and_plot(model_1, train_images, train_labels, test_images, test_labels, 'sgd', 'g', '(SGD)')

plt.show()

# save model
# model.save('mod1.h5')
