#! /usr/bin/python3

import socket, time

serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
serversocket.bind(('localhost', 8089))
serversocket.listen(5) # become a server socket, maximum 5 connections

while True:
    connection, address = serversocket.accept()
    data = connection.recv(4096)
    datetime = time.asctime() + '\n'
    time.sleep(1)
    connection.send('Hello ' + str(data))
    connection.send('My time is ' + datetime)
    connection.close()
