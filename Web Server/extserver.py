import socket
from threading import Thread

msgs = []
stopvalue=0

def send(c):
    global msgs
    global stopvalue
    while True:
        if stopvalue=="J":
           c.send(bytes(stopvalue, encoding='utf-8'))
           stopvalue=0
        elif len(msgs)==4:
            #print(msgs)
            for msg in msgs:
                c.send(bytes(msg, encoding='utf-8'))
            msgs = []

def receive(c):
    global msgs
    global stopvalue
    while True:
        msg=c.recv(1000).decode('utf-8')
        if msg:
            if msg=="J":
                stopvalue=msg
            else:
                msgs.append(msg)

def robot():
    soc1 = socket.socket()
    soc1.bind(('', 2455))
    soc1.listen(10)
    while True:
       c, addr = soc1.accept()
       print(msgs)
       t1 = Thread(target=send, args=(c, ))
       t1.start()


def web():
    soc2 = socket.socket()
    soc2.bind(('', 2116))
    soc2.listen(10)
    while True:
        c, addr = soc2.accept()
        t2 = Thread(target=receive, args=(c, ))
        t2.start()

ro = Thread(target=web)
we = Thread(target=robot)

ro.start()
we.start()
