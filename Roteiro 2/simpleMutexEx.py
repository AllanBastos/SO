from threading import Thread, Lock
from random import randint
from time import sleep
import sys

NTHRDS = 5

shared_data = 0

mutex = Lock()

def add1000():
    global shared_data
    # mutex.acquire() # Obtain exclusive mutex lock
    j = shared_data
    sleep(2)
    shared_data = j + 1000
    # mutex.release() # Release mutex lock
    print("1000 added!")
    sys.exit()

threads = [] # creates a list of threads

for i in range(NTHRDS):
    threads.append(Thread(target=add1000, args=[]))
    threads[i].start()

for i in range(NTHRDS):
    threads[i].join()

print("Shared data = {}".format(shared_data))# ...& display net result.
