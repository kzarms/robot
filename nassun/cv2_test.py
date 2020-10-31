#!/usr/bin/env python3

from flask import Flask, render_template, Response, request
from camera import VideoCamera
import datetime

app = Flask(__name__)

@app.route('/')
def index():
    print(request.data)
    now = datetime.datetime.now()
    timeString = now.strftime("%Y-%m-%d %H:%M")
    #
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
            print("None of them!")            
    elif request.method == 'GET':
        # return render_template("index.html")
        print("No Post Back Call")
    return render_template('index.html', title='Robot Nassun', time=timeString)

def gen(camera):
    while True:
        frame = camera.get_frame()
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n\r\n')

@app.route('/video_feed')
def video_feed():
    return Response(gen(VideoCamera()),
                    mimetype='multipart/x-mixed-replace; boundary=frame')

if __name__ == '__main__':
    app.run(host='0.0.0.0', debug=True)
