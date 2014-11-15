#! /usr/bin/python
import pennapprobot
import pennappcam
import struct
import socket
import argparse
import json
import re
import binascii
#import threading

import struct
from base64 import b64encode
from hashlib import sha1
from io import StringIO

try:
    import SocketServer
except ImportError:
    import socketserver as SocketServer

try:
    json_data=open('settings.json')
    settings = json.load(json_data)
    json_data.close()
except IOError:
    print("Please run setup-stuff.py")
    exit(1)

# get the arguments
parser = argparse.ArgumentParser()
parser.add_argument("-i", "--ip", type=str, help="IP address or hostname of the server you want to connect to. Default is the device's current external ip.", default="")
parser.add_argument("-p", "--port", type=int, help="Port you want to connect to. Default is in settings.json.", default=int(settings['port']))
args = parser.parse_args()
global FLAG_L
global FLAG_U
FLAG_L = 0
FLAG_U = 0

class WebSocketHandler(SocketServer.StreamRequestHandler):
    magic = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
 
    def setup(self):
        SocketServer.StreamRequestHandler.setup(self)
        print("connection established", self.client_address)
        self.handshake_done = False
 
    def handle(self):
        while True:
            if not self.handshake_done:
                self.handshake()
            else:
                self.read_next_message()
 
    def read_next_message(self):
        length = ord(self.rfile.read(2)[1]) & 127
        if length == 126:
            length = struct.unpack(">H", self.rfile.read(2))[0]
        elif length == 127:
            length = struct.unpack(">Q", self.rfile.read(8))[0]
        masks = [ord(byte) for byte in self.rfile.read(4)]
        decoded = ""
        for char in self.rfile.read(length):
            decoded += chr(ord(char) ^ masks[len(decoded) % 4])
        self.on_message(decoded)
 
    def send_message(self, message):
        self.request.send(chr(129))
        length = len(message)
        if length <= 125:
            self.request.send(chr(length))
        elif length >= 126 and length <= 65535:
            self.request.send(126)
            self.request.send(struct.pack(">H", length))
        else:
            self.request.send(127)
            self.request.send(struct.pack(">Q", length))
        self.request.send(message)
 
    def handshake(self):
        data = self.request.recv(1024).strip().decode()
        headers = dict(re.findall(r"(?P<name>.*?): (?P<value>.*?)\r\n", data))
        if headers["Upgrade"] is None or headers["Upgrade"] != "websocket":
            return
        print('Handshaking...')
        key = headers['Sec-WebSocket-Key']
        str_to_encode = key + self.magic
        encoded = str_to_encode.encode('UTF-8')
        digest = b64encode(sha1(encoded).hexdigest().encode('utf-8'))
        response = 'HTTP/1.1 101 Switching Protocols\r\n'
        response += 'Upgrade: websocket\r\n'
        response += 'Connection: Upgrade\r\n'
        response += 'Sec-WebSocket-Accept: %s\r\n\r\n' % digest
        self.handshake_done = self.request.send(response.encode('utf-8'))
 
    def on_message(self, message):
        print("WebSockHandler: ", messsage)


class MyTCPHandler(SocketServer.BaseRequestHandler):

    def handle(self):
        self.data = self.request.recv(1024).strip()
        global FLAG_L
        global FLAG_U

        print("{0} wrote {1}.".format(self.client_address[0], ord(self.data)))
        # TODO handle data
	
        #0 1 2 3 4 is movement of bot
        #0 for rest, 1 for fwd, 2 for back, 3 for turnLeft, 4 for turnRight
        #5 6 7 8 is movement of webcam arms
        #5 for lookLeft, 6 for lookRight, 7 for lookUp, 8 for lookDown
        if ord(self.data) == 49:
                pennapprobot.move_forward()
        elif ord(self.data) == 50:
                pennapprobot.move_backward()
        elif ord(self.data) == 51:
                pennapprobot.turn_left()
        elif ord(self.data) == 52:
                pennapprobot.turn_right()
        #CHANGE PORT!!!
        elif ord(self.data) == 53 and FLAG_L > -6:
                pennappcam.lower_l()
                FLAG_L-=1
        elif ord(self.data) == 54 and FLAG_L < 6:
                pennappcam.lower_r()
                FLAG_L+=1
        elif ord(self.data) == 55 and FLAG_U > -1:
                pennappcam.upper_b()
                FLAG_U-=1
        elif ord(self.data) == 56 and FLAG_U < 3:
                pennappcam.upper_f()
                FLAG_U+=1
        else:
                print("Nothing found")
        print("{0},{1}".format(FLAG_L,FLAG_U))		

#class ThreadedTCPServer(SocketServer.ThreadingMixIn, SocketServer.TCPServer):
#    pass



# hack to the the pi's external ip
if args.ip is None or len(args.ip) < 1:
    dummy_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    dummy_socket.connect(('8.8.8.8', 80))
    host = dummy_socket.getsockname()[0]
    dummy_socket.close()
else:
    host = args.ip

server = SocketServer.TCPServer((host, settings['port']), MyTCPHandler)
#server = ThreadedTCPServer((host, settings['port']), ThreadedTCPRequestHandler)
#server2 = ThreadedTCPServer((host,9990),ThreadedTCPRequestHandler)

print("Listening")
#server_thread = threading.Thread(target=server.serve_forever())
#server_thread2 = threading.Thread(target=server2.serve_forever())

#server_thread.setDaemon(True)
#server_thread2.setDaemon(True)

#server_thread.start()
#server_thread2.start()

#while 1:
#    time.sleep(1)

server.serve_forever()
#server2.serve_forever()
