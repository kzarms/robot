#!/usr/bin/env python3

from flask import Flask, render_template, Response, request

app = Flask(__name__)

@app.route('/')
def json():
    return render_template('json.html')

#background process happening without any refreshing
@app.route('/cmd')
def control():
    # print(request)
    command = request.args.get('id')
    if command == 'up':
        print("Going up!")
    elif command == 'down':
        print("Going down!")
    else:
        print("Wrong command, ignore!")    
    return "Nothing"


if __name__ == '__main__':
    app.run(host='0.0.0.0', debug=True)
