#!/usr/bin/python3
# Web streaming example
# Source code from the official PiCamera package
# http://picamera.readthedocs.io/en/latest/recipes2.html#web-streaming

import io
import picamera
import logging
import socketserver
from threading import Condition
from http import server
import RPi.GPIO as GPIO

#===============================================================================================================
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
    carMove(0, 50)

def car_rt():
    carMove(50, 0)

def car_bw():
    carMove(-50, -50)

def car_st():
    carMove(0, 0)

#===============================================================================================================

PAGE="""\
<script src="//ajax.googleapis.com/ajax/libs/jquery/1.9.1/jquery.min.js"></script>
<script type=text/javascript>
        $(function() {
          $("#up, #down, #left, #right, #stop").on("click", function(e) {
            e.preventDefault();
            $.ajax({
              type: "GET",
              url: "/cmd",
              timeout:10,
              data: {
                id: $(this).attr('id')
              }                 
            });
          });
        });
    document.onkeydown = function (evt) {
    var keyCode = evt ? (evt.which ? evt.which : evt.keyCode) : event.keyCode;
    if (keyCode == 38) {
      document.getElementById('up').click();
    }
    if (keyCode == 40) {
      document.getElementById('down').click();
    }
    if (keyCode == 37) {
      document.getElementById('left').click();
    }
    if (keyCode == 39) {
      document.getElementById('right').click();
    }
    if (keyCode == 32) {
      document.getElementById('stop').click();
    }
    else {
      return true;
    }
};
</script>
<html>
    <head>
        <title>Robot Nassun</title>
    </head>
    <body>
        <center><h1>Robot Nassun</h1></center>
         <center><form>
            <a href=# id=up><button class='btn btn-default'>Up</button></a>
            <a href=# id=down><button class='btn btn-default'>Down</button></a>
            <a href=# id=left><button class='btn btn-default'>Left</button></a>
            <a href=# id=right><button class='btn btn-default'>Right</button></a>
            <a href=# id=stop><button class='btn btn-default'>Stop</button></a>
        </form></center>
        <center><img src="stream.mjpg" width="640" height="480"></center>
    </body>
</html>
"""

class StreamingOutput(object):
    def __init__(self):
        self.frame = None
        self.buffer = io.BytesIO()
        self.condition = Condition()

    def write(self, buf):
        if buf.startswith(b'\xff\xd8'):
            # New frame, copy the existing buffer's content and notify all
            # clients it's available
            self.buffer.truncate()
            with self.condition:
                self.frame = self.buffer.getvalue()
                self.condition.notify_all()
            self.buffer.seek(0)
        return self.buffer.write(buf)

class StreamingHandler(server.BaseHTTPRequestHandler):
    def do_GET(self):        
        print(self.path)
        if self.path == '/':
            self.send_response(301)
            self.send_header('Location', '/index.html')
            self.end_headers()
        elif '/cmd' in self.path:
            print("Command from the code!")
            cmd = self.path.split('=')[1]
            if cmd == 'up':
                car_fw()
                print("Going up!")                
            elif cmd == 'down':
                car_bw()
                print("Going down!")
            elif cmd == 'left':
                car_lf()
                print("Going <-")
            elif cmd == 'right':
                car_rt()
                print("Going ->!")
            elif cmd == 'stop':
                car_st()
                print("STOP")
            else:
                print("Command is not recognized!")
            self.send_response(200)
        elif self.path == '/index.html':
            content = PAGE.encode('utf-8')
            self.send_response(200)
            self.send_header('Content-Type', 'text/html')
            self.send_header('Content-Length', len(content))
            self.end_headers()
            self.wfile.write(content)
        elif self.path == '/stream.mjpg':
            self.send_response(200)
            self.send_header('Age', 0)
            self.send_header('Cache-Control', 'no-cache, private')
            self.send_header('Pragma', 'no-cache')
            self.send_header('Content-Type', 'multipart/x-mixed-replace; boundary=FRAME')
            self.end_headers()
            try:
                while True:
                    with output.condition:
                        output.condition.wait()
                        frame = output.frame
                    self.wfile.write(b'--FRAME\r\n')
                    self.send_header('Content-Type', 'image/jpeg')
                    self.send_header('Content-Length', len(frame))
                    self.end_headers()
                    self.wfile.write(frame)
                    self.wfile.write(b'\r\n')
            except Exception as e:
                logging.warning(
                    'Removed streaming client %s: %s',
                    self.client_address, str(e))
        else:
            self.send_error(404)
            self.end_headers()

class StreamingServer(socketserver.ThreadingMixIn, server.HTTPServer):
    allow_reuse_address = True
    daemon_threads = True

with picamera.PiCamera(resolution='640x480', framerate=24) as camera:
    output = StreamingOutput()
    #Uncomment the next line to change your Pi's Camera rotation (in degrees)
    camera.rotation = 180
    camera.start_recording(output, format='mjpeg')
    try:
        address = ('', 8000)
        server = StreamingServer(address, StreamingHandler)
        server.serve_forever()
    finally:
        camera.stop_recording()
#
# Start line
# python3 \nassun\video.py
# sudo chmod +x nassun/video.py