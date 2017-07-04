#!/usr/bin/env python3.5
from srv.server import Server
from client import Client
import time


if __name__ == "__main__":
    srv_addr = "127.0.0.1"
    srv_port = 1050
    max_client = 1

    client = Client()

    try:
        server = Server(srv_addr, srv_port)
        server.start(max_client)
        time.sleep(10)
        server.stop()
    except:
        print("Cant run server")

    print("END")