#!/usr/bin/env python
"""
This is the initial module to control the car
"""
import sys
import time
import pygame
#import keyboard
import serial

#Check number of the COM port after BT paring
COMPORT = "COM4"

#region functions
def small_test(comport):
    """ Small test to run forward for 3 secs """
    ser = serial.Serial(comport)
    start = '11#'
    left = '2#'
    right = '3#'
    stop = '0#'
    #Run the car
    ser.write(start.encode('ascii'))
    #pause for 2.5 sec
    time.sleep(2.5)
    ser.write(left.encode('ascii'))
    #pause for 2.5 sec
    time.sleep(1)
    ser.write(right.encode('ascii'))
    #pause for 2.5 sec
    time.sleep(1)
    #Stop the car
    ser.write(stop.encode('ascii'))
    #Close the com port
    ser.close()

#endregion
#small_test(COMPORT)


pygame.init()

display_width = 500
display_height = 500

black = (0,0,0)
white = (255,255,255)
red = (255,0,0)

car_width = 73
car_height = 123

gameDisplay = pygame.display.set_mode((display_width, display_height))
pygame.display.set_caption('Remote control')
clock = pygame.time.Clock()


#Car definition
carImg = pygame.image.load('racecar.png')
def car(x, y):
    gameDisplay.blit(carImg, (x, y))

#Test display
def text_objects(text, font):
    textSurface = font.render(text, True, black)
    return textSurface, textSurface.get_rect()
def message_display(text):
    largeText = pygame.font.Font('freesansbold.ttf', 25)
    TextSurf, TextRect = text_objects(text, largeText)
    TextRect.center = ((display_width/2),(display_height/2))
    gameDisplay.blit(TextSurf, TextRect)

    pygame.display.update()

    time.sleep(2)

    game_loop()

def crash():
    message_display('You Crashed')

def game_loop():
    x = (display_width * 0.45)
    y = (display_height * 0.45)
    x_change = 0
    y_change = 0

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
                elif event.key == pygame.K_RIGHT:
                    x_change = 5
                elif event.key == pygame.K_UP:
                    y_change = -5
                elif event.key == pygame.K_DOWN:
                    y_change = 5
            if event.type == pygame.KEYUP:
                if  event.key == pygame.K_LEFT or \
                    event.key == pygame.K_RIGHT or \
                    event.key == pygame.K_UP or \
                    event.key == pygame.K_DOWN:
                        x_change = 0
                        y_change = 0
            ############################
        x += x_change
        y += y_change

        if x > (display_width * 0.8):
            x = (display_width * 0.8)
        if x < (display_width * 0.1):
            x = (display_width * 0.1)
        if y > (display_height * 0.8):
            y = (display_height * 0.8)
        if y < (display_height * 0.1):
            y = (display_height * 0.1)
        gameDisplay.fill(white)
        car(x, y)
        #    crash()

        pygame.display.update()
        clock.tick(60)

game_loop()
pygame.quit()
quit()

"""
def navigation(x):
    return {
        'w': "Frwd",
        's': print("Bkwd"),
        'a': print("Left"),
        'd': print("Rite")
    },get(x, print("None"))


result = {
  'w': lambda x: x * 5,
  'b': lambda x: x + 7,
  'c': lambda x: x - 2
}['w'](x)


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
        'd': rite
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