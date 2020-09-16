from threading import Thread
from random import randint
import sys

NTHRDS = 6

def theWorks(n): # main function of the "worker thread"
    r = 0
    for i in range(1000000): # do lots of work
        r += randint(0,50)
    print('I am {}, the result is: {}'.format(n, r))
    sys.exit()


threads = [] # creates a list of threads

for i in range(NTHRDS):
    threads.append(Thread(target=theWorks, args=[i]))
    threads[i].start()

for i in range(NTHRDS):
    threads[i].join()