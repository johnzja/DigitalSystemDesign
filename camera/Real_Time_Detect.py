import numpy as np
from yolo import YOLO
from PIL import Image
import cv2

cap = cv2.VideoCapture(0)
if not cap.isOpened():
    print('fuck!')
    exit(-1)

width, height, w = 640, 480, 360
cap.set(cv2.CAP_PROP_FRAME_WIDTH, width)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, height)

# Initialize Yolo Analyzer
FLAGS = {'image': True, 'input': '', 'output': '', 'model': 'model_data/yolo-tiny.h5', 'anchors': 'model_data/tiny_yolo_anchors.txt'}

yolo_analyzer = YOLO(**FLAGS)

while True:
    ret, frame = cap.read()
    # Process frame.
    img = Image.fromarray(frame.astype('uint8')).convert('RGB')
    r_image = np.array(yolo_analyzer.detect_image(img))
    cv2.imshow('capture', r_image)
    # r_image.show()
    action = cv2.waitKey(1)
    if action == ord('q'):
        break

cap.release()
print('fuck!!')
