#!/usr/bin/python3
# Main file to
# Prepreq
# sudo apt-get install python-rpi.gpio
# sudo pip3 install keyboard

import keyboard

# Speed for left and right
# car_speed = [0, 0]

def move(car_speed):
    if car_speed[0] == 0:
        # This is stop function
        print("Left enging stop")
    elif car_speed[0] > 0:
        # Left forward
        print("Left enging moving forward")
    else:
        # Left backward
        print("Left enging moving backward")
    # Right side
    if car_speed[1] == 0:
        # This is stop function
        print("Rhgt enging stop")
    elif car_speed[1] > 0:
        # Right forward
        print("Rhgt enging moving forward")
    else:
        # Right backward
        print("Rhgt enging moving backward")

def car_fw():
    move([1, 1])

def car_lf():
    move([1, 0])

def car_rt():
    move([0, 1])

def car_st():
    move([0, 0])

keyboard.add_hotkey('up', car_fw)
keyboard.add_hotkey('left', car_lf)
keyboard.add_hotkey('right', car_rt)
keyboard.add_hotkey('down', car_st)

print("Press ESC to stop.")
keyboard.wait('esc')