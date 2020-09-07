#!/usr/bin/env python3
"""This is the initial module to control the car."""
# import sys
import time

import pygame
import serial
from serial import Serial

# Check number of the COM port after BT paring
COMPORT = 'COM3'
COMPORT = '/dev/rfcomm0'

ser = serial.Serial('/dev/rfcomm0')  # open serial port
>>> print(ser.name)         # check which port was really used
>>> ser.write(b'hello')     # write a string
>>> ser.close()             # close port

# region functions


def send_cmd(ser, cmd):
    """Send command to serial."""
    ser.write(cmd.encode('ascii'))

def small_test(comport):
    """Small test to run forward for 3 secs."""
    local_ser = serial.Serial(comport)
    local_delay = 1.5
    # Run the car
    send_cmd(local_ser, "1,1#")
    time.sleep(local_delay)
    send_cmd(local_ser, "3,4#")
    time.sleep(local_delay)
    send_cmd(local_ser, "-1,2#")
    time.sleep(local_delay)
    send_cmd(local_ser, "0,0#")
    # Close the com port
    local_ser.close()

# endregion
# small_test(COMPORT)

# Open serial port on the COMPORT
ser = serial.Serial(COMPORT)

# Start the pygame
pygame.init()

display_width = 500
display_height = 500

black = (0, 0, 0)
white = (255, 255, 255)
red = (255, 0, 0)

car_width = 73
car_height = 123

gameDisplay = pygame.display.set_mode((display_width, display_height))
pygame.display.set_caption('Remote control')
clock = pygame.time.Clock()

# Car definition
# carImg = pygame.image.load('racecar.jpg')


def things(thingx, thingy, thingw, thingh, color):
    pygame.draw.rect(gameDisplay, color, [thingx, thingy, thingw, thingh])


def car(x, y):
    """The car function."""
    gameDisplay.blit(carImg, (x, y))


# Text display
def text_objects(text, font):
    """The text function."""
    textSurface = font.render(text, True, black)
    return textSurface, textSurface.get_rect()


def message_display(text):
    """The test show function."""
    largeText = pygame.font.Font('freesansbold.ttf', 14)
    TextSurf, TextRect = text_objects(text, largeText)
    TextRect.center = (30, 10)
    gameDisplay.blit(TextSurf, TextRect)

    pygame.display.update()

    time.sleep(2)

    game_loop()


def crash():
    """The crash function."""
    message_display('You Crashed')


def game_loop():
    """The main game function."""
    center_position = 0.45
    x_change = 0
    y_change = 0
    border_a = 0.1
    border_b = 0.9
    set_timer = 60

    x = (display_width * center_position)
    y = (display_height * center_position)
    car_x = 0
    car_y = 0

    gameExit = False

    while not gameExit:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                quit()

            ############################
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_LEFT:
                    x_change = -5
                    car_y += 1
                    #if difference less than 4 make it
                    if abs(car_x - car_y) < 4:
                        car_x += -1
                elif event.key == pygame.K_RIGHT:
                    x_change = 5
                    car_x += 1
                    #if difference less than 4 make it
                    if abs(car_x - car_y) < 4:
                        car_y += -1
                elif event.key == pygame.K_UP:
                    y_change = -5
                    car_x += 1
                    car_y += 1
                elif event.key == pygame.K_DOWN:
                    car_x += -1
                    car_y += -1
                    y_change = 5
                elif event.key == pygame.K_SPACE:
                    x = (display_width * center_position)
                    y = (display_height * center_position)
                    car_x = 0
                    car_y = 0
                #Set maximum to the sending, no more then 9
                if car_x > 9: car_x = 9
                if car_x < -9: car_x = -9
                if car_y > 9: car_y = 9
                if car_y < -9: car_y = -9
                send_cmd(ser, (str(car_x) +',' + str(car_y) + '#'))
            if event.type == pygame.KEYUP:
                if  event.key == pygame.K_LEFT or \
                    event.key == pygame.K_RIGHT or \
                    event.key == pygame.K_UP or \
                    event.key == pygame.K_DOWN:
                        #send_cmd(ser, "0,0#")
                        x_change = 0
                        y_change = 0
            ############################

        x += x_change
        y += y_change
        msg = "X: " + str(x) + " Y: " + str(y)

        if x > (display_width * border_b):
            x = (display_width * border_b)
        if x < (display_width * border_a):
            x = (display_width * border_a)
        if y > (display_height * border_b):
            y = (display_height * border_b)
        if y < (display_height * border_a):
            y = (display_height * border_a)

        gameDisplay.fill(white)
        things(x, y, 25, 25, black)

        #car(x, y)

        #    crash()

        pygame.display.update()
        clock.tick(set_timer)

game_loop()
ser.close()
pygame.quit()

quit()

"""
def frwd():
    print("Frwd")
def bkwd():
    print("Bkwd")
def left():
    print("Left")
def rite():
    print("Rite")

def navigation(x):
    return {
        'w': frwd,
        's': bkwd,
        'a': left,
        'd': right
    }[x]()


    button = keyboard.read_key()
    switcher[button]()

navigation('w')

while True:
    if keyboard.is_pressed("p"):

    if keyboard.read_key() == "w":
        print("Forward")
        break
"""