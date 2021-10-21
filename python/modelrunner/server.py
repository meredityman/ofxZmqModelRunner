
import zmq
import time

import numpy as np

class ModelServer:
    def __init__(self, socket_addr, model):
        print(f"Starting Reply on {socket_addr}")

        self.model = model
        self.socket_addr = socket_addr
        self.running = False



        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.REP)
        self.socket.bind(self.socket_addr)
        print(f"Bind at: {socket_addr}")

    def run(self):
        if self.running:
            return

        self.running = True
        while self.running:
            print("Waiting for connection")
            #  Wait for next request from client

            input = self.model.receive(self.socket)

            output = self.model._forward(input)
            self.model.send(self.socket, output)

