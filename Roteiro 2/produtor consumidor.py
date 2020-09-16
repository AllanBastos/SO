from threading import Thread, Lock, Condition
from time import sleep
import sys
from random import randint

LIMITE_BUFFER = 5

buffer = list()
cheio = 0

Mutex = Condition()
def produtor():
    global buffer, cheio
    while True:
        Mutex.acquire()
        buffer.append(randint(0, 10))
        print("Produced X {}".format(buffer))

        if (len(buffer) == LIMITE_BUFFER):
            Mutex.wait()
#           print("Cheio")
        Mutex.notify()
        Mutex.release()
        sleep(1)


def consumidor():
    global buffer, cheio
    while True:
        Mutex.acquire()

        if len(buffer) == 0:
            Mutex.wait()
#           print("vazio")

        print("Consumed X {}".format(buffer.pop()))

        Mutex.notify()
        Mutex.release()
        sleep(2)

c = Thread(target=consumidor)
p = Thread(target=produtor)
p.start()
c.start()
