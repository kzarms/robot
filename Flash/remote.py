"""
This is the initial module to control the car
"""
#!/usr/bin/env python

import time
import serial

#Check number of the COM port after BT paring
COMPORT = "COM4"

#region functions
def small_test(comport):
    """ Small test to run forward for 3 secs """
    ser = serial.Serial(comport)
    start = '$1#'
    stop = '$0#'
    #Run the car
    ser.write(start.encode('ascii'))
    #pause for 2.5 sec
    time.sleep(2.5)
    #Stop the car
    ser.write(stop.encode('ascii'))
    #Close the com port
    ser.close()

#endregion

small_test(COMPORT)
