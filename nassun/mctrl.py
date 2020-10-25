#!/usr/bin/env python3
"""This is the local car module to contol the car."""

import RPi.GPIO as GPIO
import time
import keyboard

# Set Control pin
lf_motor_en = 17
lf_motor_fw = 27
lf_motor_bw = 22

rt_motor_en = 23
rt_motor_fw = 25
rt_motor_bw = 24

GPIO.setmode(GPIO.BCM)
GPIO.setup(lf_motor_en, GPIO.OUT)
lf_speed = GPIO.PWM(lf_motor_en, 100)
lf_speed.start(0)
GPIO.setup(lf_motor_fw, GPIO.OUT)
GPIO.setup(lf_motor_bw, GPIO.OUT)
GPIO.setup(rt_motor_en, GPIO.OUT)
rt_speed = GPIO.PWM(rt_motor_en, 100)
rt_speed.start(0)
GPIO.setup(rt_motor_fw, GPIO.OUT)
GPIO.setup(rt_motor_bw, GPIO.OUT)


def gpioInit():
    # First init our board
    print("Left enging stop")

def carMove(speedLeft, speedRight):
    # Car contol function. +1 +1 - moving forward, 0,1 moving back and so one
    # Left side
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
        # speedLeft = speedLeft * -1
        lf_speed.ChangeDutyCycle(speedLeft * -1)
    # Right side
    if speedRight == 0:
        # This is stop function
        print("Rhgt enging stop")
        # GPIO.setup(lf_motor_en, GPIO.HIGH)
        GPIO.output(rt_motor_fw, GPIO.LOW)
        GPIO.output(rt_motor_bw, GPIO.LOW)
        GPIO.output(rt_motor_en, GPIO.LOW)
        rt_speed.ChangeDutyCycle(0)
    elif speedRight > 0:
        # Left
        print("Rhgt enging moving forward")
        GPIO.output(rt_motor_fw, GPIO.HIGH)
        GPIO.output(rt_motor_bw, GPIO.LOW)
        # GPIO.output(lf_motor_en, GPIO.HIGH)
        # Set speed
        rt_speed.ChangeDutyCycle(speedRight)
    else:
        # Right
        print("Rhgt enging moving backward")
        GPIO.output(rt_motor_fw, GPIO.LOW)
        GPIO.output(rt_motor_bw, GPIO.HIGH)
        # GPIO.output(lf_motor_en, GPIO.HIGH)
        # Set speed
        rt_speed.ChangeDutyCycle(speedRight * -1)

def car_fw():
    carMove(50, 50)

def car_lf():
    carMove(50, 0)

def car_rt():
    carMove(0, 50)

def car_bw():
    carMove(-50, -50)

def car_st():
    carMove(0, 0)
# Test on
print("Start testing")
# carMove(50, 50)
#time.sleep(1)
#carMove(50, 0)
#time.sleep(1)
#carMove(0, 50)
#time.sleep(1)
#carMove(-50, -50)
#time.sleep(1)
#carMove(0, 0)

keyboard.add_hotkey('up', car_fw)
keyboard.add_hotkey('left', car_lf)
keyboard.add_hotkey('right', car_rt)
keyboard.add_hotkey('down', car_bw)
keyboard.add_hotkey('space', car_st)

print("Press ESC to stop.")
keyboard.wait('esc')

print("Stop testing")
# End of the programm
lf_speed.stop()
rt_speed.stop()
GPIO.cleanup()
