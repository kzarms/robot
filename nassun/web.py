#!/usr/bin/python3
'''
Install module before usage:
sudo apt-get install python3-flask
'''

from flask import Flask, render_template, request, Response
import datetime
import cv2
import numpy as np

PAGE="""\
<!DOCTYPE html>
    <head>
        <title>{title}</title>
    </head>
    <body>
        <center><h1>{title}</h1></center>
        <center><img src="/video_feed" width="100%"></center>        
        <a href="/action/up" class="button">UP</a>
        <a href="/action/down" class="button">DOWN</a>
        <form method="post" action="/">
            <input type="submit" name="up" value="UP">
            <input type="submit" name="down" value="DOWN">
        </form>
        <input type="button" value="search" onclick="alert('submitted');">
        <center><h3>The date and time on the server is: {time}</h3></center>
    </body>
</html>
"""

width = 640
height = 480

app = Flask(__name__)

cap = cv2.VideoCapture(0)

@app.route("/", methods=['GET', 'POST'])
def index():
    print(request.data)
    now = datetime.datetime.now()
    timeString = now.strftime("%Y-%m-%d %H:%M")
    indexPage = PAGE.format(
            title='Robot Nassun',
            width='640',
            height='480',
            time=timeString
            )

    if request.method == 'POST':
        print(request.method)
        print(request.form.to_dict())
        if request.form.get('Up') == 'Up':
            # pass
            print("Encrypted")
        elif  request.form.get('Down') == 'Down':
            # pass # do something else
            print("Decrypted")
        else:
            # pass # unknown
            return indexPage
    elif request.method == 'GET':
        # return render_template("index.html")
        print("No Post Back Call")
    return indexPage



    if 'UP' in request.form:
        print("up")
    elif 'DOWN' in request.form:
        print("down")
    else:
        print("else")

    if request.method == 'POST':
        if request.form['up'] == 'UP':
            print("up")
        elif request.form['down'] == 'DOWN':
            print("down")
        else:
            print("else")

    elif request.method == 'GET':

        return 
    


    while True:
        success, image = video.read()
        ret, jpeg = cv2.imencode('.jpg', image)
        frame = jpeg.tobytes()
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n\r\n')

def gen_frames():  
    while True:
        success, frame = cap.read()  # read the camera frame
        if not success:
            break
        else:
            ret, buffer = cv2.imencode('.jpg', frame)
            frame = buffer.tobytes()
            yield (b'--frame\r\n'
                   b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n') 

@app.route('/video_feed')
def video_feed():
    return Response(gen_frames(), mimetype='multipart/x-mixed-replace; boundary=frame')  

# Start the web server
if __name__ == '__main__':
    app.run(debug=False, port=8080, host='0.0.0.0')
