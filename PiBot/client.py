#! /usr/bin/python

## Sample client.
## 

import socket
import json
import argparse
#from pprint import pprint

# get settings
try:
    json_data = open('settings.json')
    settings = json.load(json_data)
    json_data.close()
except IOError:
    raise IOError("settings.json not found. Please run setup-stuff.py")

parser = argparse.ArgumentParser()
parser.add_argument("-i", "--ip", type=str, help="IP address or hostname of the server you want to connect to.", default=socket.gethostname())
parser.add_argument("-p", "--port", type=int, help="Port you want to connect to.", default=int(settings['port']))
args = parser.parse_args()

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

print("Connecting to: {0}:{1}".format(args.ip, args.port))

encoding = 'UTF-8'

try:
    sock.connect((args.ip, args.port))

    for i in range(2):
        msg = "hello! {0}".format(i)
        print("sending: {0}.".format(msg))
        sock.sendall(bytes(msg, encoding))
finally:
    sock.close()
