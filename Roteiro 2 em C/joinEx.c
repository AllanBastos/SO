#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHRDS 3

void * theWork(void * n) { //main function of the "worker thread"
  int i; double r = 0.0;
  for (i=0; i<1000000; i++) //do lots of work
    r += (double)random();
  printf("Result: %e\n", r);  
  pthread_exit((void *)0);
}

int main(int argc, char ** argv) { 
  pthread_t thrd[NTHRDS];
      //data structure for managing several worker thread instances
  pthread_attr_t attr; //data structure for managing thread attributes
  pthread_attr_init(&attr); //initialise thread attribute data ...
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE); 
                           //...and set "joinable" attribute 
  int t, e, st;
  for (t=0; t<NTHRDS; t++) { // create several worker thread instances
    printf("Creating thread %d\n", t);
    if (e = pthread_create(&thrd[t], &attr, theWork, NULL)!= 0) {
      printf("Thread create error %d\n", e);
      exit(-1);
    }
  }

  pthread_attr_destroy(&attr);
  //garbage-collect the thread attribute oject, no longer needed

  for (t=0; t<NTHRDS; t++) { //wait for each worker thread to join
    if (e = pthread_join(thrd[t], (void **)&st)!= 0) {
      printf("Thread %d join error %d\n", t, e);
      exit(-1);
    }
    printf("Thread %d has joined: status = %x\n", t, st);
  }
  pthread_exit(NULL); //master-thread (program main) is done.
}
