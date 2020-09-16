/*
 * Producer/Consumer demo using POSIX threads without synchronization
 * Linux version
 * MJB Apr'05
 */
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

/* buffer using a shared integer variable */
typedef struct {
  int writeable; /*true/false*/
  int sharedData;
} buffer;

buffer theBuffer; /* global */

int store(int item, buffer *pb) {
  pb->sharedData = item;      /*put data in buffer... */
  pb->writeable = !pb->writeable;
  return 0;
}

int retrieve(buffer *pb) {
  int data;
  data = pb->sharedData;       /*get data from buffer...*/
  pb->writeable = !pb->writeable;
  return data;
}

void delay(int secs) { /*utility function*/
  time_t beg = time(NULL), end = beg + secs;
  do {
    ;
  } while (time(NULL) < end);
}

/* core routine for the producer thread */
void *producer(void *n) {
  int j=1;
  while (j<=10) {
    store(j, &theBuffer);
    printf("Stored: %d\n", j);
     j++;
    delay(rand() % 6);  /* up to 5 sec */
  }
  return n;
}

/* core routine for the consumer thread */
void *consumer(void *n) {
  int j=0, tot=0;
  while (j < 10) {
    j = retrieve(&theBuffer);
    tot += j;
    printf("Retrieved: %d\n", j);
    delay(rand() % 6);  /* up to 5 sec */
  }
  printf("Retrieved total = %d\n", tot);
  return(n);
}

int main() {
  pthread_attr_t *attr = NULL;
  pthread_t prodThread, consThread;

  theBuffer.writeable = 1;
  srand(time(NULL)); /* initialise the rng */

  pthread_create(&prodThread, attr, producer, NULL);
  pthread_create(&consThread, attr, consumer, NULL);

  while (1)
    ;        /*loop forever*/
  return 0;  /*!!!*/
}
