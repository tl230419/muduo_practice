#!/usr/bin/python

import socket, time

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(('', 8888))
server_socket.listen(5)

while True:
    (client_socket, address) = server_socket.accept()
    data = client_socket.recv(4096)
    datetime = time.asctime() + '\r\n'
    client_socket.send('Hello ' + data)
    client_socket.send('My time is ' + datetime)
    client_socket.close()