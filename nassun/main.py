#!/usr/bin/python3
# Main file to
# Prepreq
# sudo apt-get install python-rpi.gpio

import RPi.GPIO as GPIO
import time
# Set Control pin
led = 17
GPIO.setmode(GPIO.BCM)
GPIO.setup(led ,GPIO.OUT)

# Turn ON
print("LED on")
GPIO.output(led,GPIO.HIGH)

time.sleep(1)
# Trun OFF
print("LED off")
GPIO.output(led,GPIO.LOW)

GPIO.cleanup()
