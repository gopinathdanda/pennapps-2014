#! /usr/bin/python
import json

default_port = "9999"

def setup():
    data = {}

    print("Input your settings. Default values are listed in [].\n")

    port = input("Please enter the port you would like to use [{0}]:\n".format(default_port))
    if port is None or len(port) < 1:
        port = default_port
    data['port'] = int(port)

    with open('settings.json', 'w') as outfile:
        json.dump(data, outfile)

if __name__ == '__main__':
    setup()
