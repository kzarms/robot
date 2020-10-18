#!/usr/bin/env python3
"""This is the local car module to contol the car."""

import RPi.GPIO as GPIO
import time

# Set Control pin
lf_motor_en = 17
lf_motor_fw = 27
lf_motor_bw = 22

rt_motor_en = 23
rt_motor_fw = 24
rt_motor_bw = 25

GPIO.setmode(GPIO.BCM)
GPIO.setup(lf_motor_en, GPIO.OUT)
lf_speed = GPIO.PWM(lf_motor_en, 100)
lf_speed.start(0)
GPIO.setup(lf_motor_fw, GPIO.OUT)
GPIO.setup(lf_motor_bw, GPIO.OUT)
GPIO.setup(rt_motor_en, GPIO.OUT)
rt_speed = GPIO.PWM(rt_motor_en, 100)
GPIO.setup(rt_motor_fw, GPIO.OUT)
GPIO.setup(rt_motor_bw, GPIO.OUT)


def gpioInit():
    # First init our board
    print("Left enging stop")

def carMove(speedLeft, speedRight):
    # Car contol function. +1 +1 - moving forward, 0,1 moving back and so one
    if speedLeft == 0:
        # This is stop function
        print("Left enging stop")
        # GPIO.setup(lf_motor_en, GPIO.HIGH)
        GPIO.output(lf_motor_fw, GPIO.LOW)
        GPIO.output(lf_motor_bw, GPIO.LOW)
        GPIO.output(lf_motor_en, GPIO.LOW)
        lf_speed.ChangeDutyCycle(0)
    elif speedLeft > 0:
        # Left
        print("Left enging moving forward")
        GPIO.output(lf_motor_fw, GPIO.HIGH)
        GPIO.output(lf_motor_bw, GPIO.LOW)
        # GPIO.output(lf_motor_en, GPIO.HIGH)
        # Set speed
        lf_speed.ChangeDutyCycle(speedLeft)
    else:
        # Right
        print("Left enging moving backward")
        GPIO.output(lf_motor_fw, GPIO.LOW)
        GPIO.output(lf_motor_bw, GPIO.HIGH)
        # GPIO.output(lf_motor_en, GPIO.HIGH)
        # Set speed
        speedLeft = speedLeft * -1
        lf_speed.ChangeDutyCycle(speedLeft)


# Test on
print("Start testing")
carMove(100, 0)
time.sleep(1)
carMove(-100, 0)
time.sleep(1)
carMove(0, 0)

print("Stop testing")

# End of the programm
lf_speed.stop()
rt_speed.stop()
GPIO.cleanup()
