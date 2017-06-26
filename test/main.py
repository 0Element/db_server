#!/usr/bin/env python3.5

from socket import socket, AF_INET, SOCK_STREAM, timeout as sock_exc_time
import json


if __name__ == "__main__":
    key = "\0"
    body = {"name": "func1", "type": "select", "base": "postgress"}
    line = key + json.dumps(body) + key

    sock = socket(AF_INET, SOCK_STREAM)
    sock.settimeout(1)
    try:
        sock.connect(("127.0.0.1", 1050))
        print(sock.send(line.encode()))
        print(sock.recv(1024))
    except sock_exc_time as err:
        print("Except")

    sock.close()

    print("END")
