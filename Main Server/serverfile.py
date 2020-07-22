import socket
import time

class webme:
        def __init__(self):
            self.s = socket.socket()
            self.s.connect(('192.168.1.105', 2116))

        def send(self, coordinates):
            if coordinates == "J":
                print(coordinates)
                self.s.send(bytes(coordinates, 'utf-8'))
            else:
                print(coordinates[0])
                for coordinate in coordinates:
                    self.s.send(bytes(coordinate, 'utf-8'))
                    time.sleep(0.30)
