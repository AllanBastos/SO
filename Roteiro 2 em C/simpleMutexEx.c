/*
 * Simple mutex demo using POSIX threads and mutexes (Linux version)
 * MJB Sep 07
 */
#include <pthread.h> 
#include <stdlib.h>
#include <stdio.h>

#define NTHRDS 5

int sharedData = 0; 
pthread_mutex_t mutex;

void delay(int secs) { //utility function
  time_t beg = time(NULL), end = beg + secs;
  do ; while (time(NULL) < end);
}

//Try commenting out the calls to the lock and unlock functions.
void *add1000(void *n) {
  pthread_mutex_lock(&mutex); //Obtain exclusive mutex lock
  int j = sharedData;
  delay(rand() % 6);  
  sharedData = j + 1000;
  pthread_mutex_unlock(&mutex); //Release mutex lock
  printf("1000 added!\n");
  return n;
}

int main() {
  pthread_attr_t *attr = NULL;
  pthread_t thrd[NTHRDS]; //Data strs form managing several threads
  int t;

  pthread_mutex_init(&mutex, NULL);
  srand(time(NULL)); // initialise random num generator

  for (t=0; t<NTHRDS; t++) //create & initialise several thread instances
    pthread_create(&thrd[t], attr, add1000, NULL);

  for (t=0; t<NTHRDS; t++) //wait for all threads to finish ...
    pthread_join(thrd[t], NULL);

  printf("Shared data = %d\n", sharedData); //...& display net result.
  return 0;
}
