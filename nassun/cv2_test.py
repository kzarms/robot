#!/usr/bin/env python3

from flask import Flask, render_template, Response, request

app = Flask(__name__)

LEFT, RIGHT, UP, DOWN, RESET = "left", "right", "up", "down", "reset"
AVAILABLE_COMMANDS = {
    'Left': LEFT,
    'Right': RIGHT,
    'Up': UP,
    'Down': DOWN,
    'Reset': RESET
}

@app.route('/')
def execute():
    return render_template('main.html', commands=AVAILABLE_COMMANDS)

@app.route('/<cmd>')
def command(cmd=None):
    if cmd == RESET:
       camera_command = "X"
       response = "Resetting ..."
    else:
        camera_command = cmd[0].upper()
        response = "Moving {}".format(cmd.capitalize())

    # ser.write(camera_command)
    return response, 200, {'Content-Type': 'text/plain'}

if __name__ == '__main__':
    app.run(host='0.0.0.0', debug=True)
