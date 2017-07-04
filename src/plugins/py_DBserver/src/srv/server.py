import threading
import socket

from plug.plugins import Plugins


BUFF_SIZE = 1024

class Server(object):
    def __init__(self, addr, port):
        self.addr = addr
        self.port = port
        self.quit = False
        self.sock = 0
        #self.buff_size = 1024

        plugs = Plugins("src/plug", 5)

        print("Constructor Server")


    def start(self, max_client):
        try:
            self.sock = socket.socket()
            self.sock.bind((self.addr, self.port))
            self.sock.listen(max_client)
            t = threading.Thread(target=self._start)
            t.setDaemon(True)
            t.start()
        except PermissionError as err:
            self.sock.close()
            print("run with sudo: {0}".format(err))
            raise err


    def _start(self):
        while(self.sock and not self.quit):
            cl_sock, addr = self.sock.accept()
            print(addr)
            t = threading.Thread(target=self.recv, args=(cl_sock,))
            t.setDaemon(True)
            t.start()
        self.sock.close()


    def recv(self, cl_sock):
        data = cl_sock.recv(BUFF_SIZE)
        for plug in plugins:
            if (data[0:len(plug.name)]) == plug.name:
                print("FOUND")
        print(len(data))
        print(data)
        cl_sock.close()


    def stop(self):
        self.quit = True


    def __del__(self):
        if (self.sock):
            self.sock.close()
        print("Destructor Server")