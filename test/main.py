#!/usr/bin/env python3.5

from socket import socket, AF_INET, SOCK_STREAM, timeout as sock_exc_time
import json


if __name__ == "__main__":


    key = "\0"
    body = {"db": "postgres", "type": "INSERT", "keys": "id_buf,from_ip,method", "vals": "\'111\',\'12.7.7.7\',\'GET\'"}
    request = key + json.dumps(body) + key

    sock = socket(AF_INET, SOCK_STREAM)
    sock.settimeout(1)
    try:
        sock.connect(("127.0.0.1", 1050))
        print(sock.send(request.encode()))
        response = sock.recv(1024)
        print(response)
        response = json.loads(response.decode())

        if (response["status"] == "0"):
            print("request: ok")

    except sock_exc_time as err:
        print("Except")
        print(err)

    sock.close()

    print("END")
