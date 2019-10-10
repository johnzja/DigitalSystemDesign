import numpy as np


# display data.
def load_files():
    data = np.load('train_files.npz')
    train_images = data['tri']
    train_labels = data['trl']
    data = np.load('test_files.npz')
    test_images = data['tei']
    test_labels = data['tel']
    return train_images, train_labels, test_images, test_labels
