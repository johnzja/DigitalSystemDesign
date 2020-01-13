# import jetson gpio
import sys
sys.path.append('/opt/nvidia/jetson-gpio/lib/python')
sys.path.append('/opt/nvidia/jetson-gpio/lib/python/Jetson/GPIO')

import Jetson.GPIO as GPIO
import time

output_pin = 11


def main():
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(output_pin, GPIO.OUT, initial=GPIO.LOW)
    try:
        while True:
            time.sleep(2)
            GPIO.output(output_pin, GPIO.HIGH)
            time.sleep(1)
            GPIO.output(output_pin, GPIO.LOW)
    finally:
        GPIO.cleanup()


if __name__ == '__main__':
    main()
