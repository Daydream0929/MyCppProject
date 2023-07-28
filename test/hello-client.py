#! /usr/bin/python3
import os
import socket
import sys

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(('localhost', 8089))
sock.send(str(os.getlogin()) + '\n')
message = sock.recv(4096)
sock.close()
