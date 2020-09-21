/*
 * Producer/Consumer demo using POSIX threads without synchronization
 * Linux version
 * MJB Apr'05
 */
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

/* buffer using a shared integer variable */
typedef struct
{
  int writeable; /*true/false*/
  int sharedData;
  int finish; /*true/false*/
} buffer;

buffer theBuffer; /* global */
pthread_mutex_t mutex;


void delay(int secs)
{
  time_t beg = time(NULL), end = beg + secs;
  do
  {
    ;
  } while (time(NULL) < end);
}

int store(int item, buffer *pb)
{
  pthread_mutex_lock(&mutex);
  pb->sharedData += item;
  pb->writeable = !pb->writeable;
  pthread_mutex_unlock(&mutex);
  return 0;
}

int retrieve(buffer *pb)
{
  pthread_mutex_lock(&mutex);
  int data = pb->sharedData;
  pb->sharedData -= data;
  pb->writeable = !pb->writeable;
  
  return data;
}

void *producer(void *n)
{
  int j = 1, tot_prod = 0;
  while (j <= 10)
  {
    store(j, &theBuffer);
    printf("Produzido: %d | Total no buffer: %d\n", j, theBuffer.sharedData);
    tot_prod += j++;
    delay(rand() % 6); /* up to 5 sec */
  }
  theBuffer.finish = 0;
  printf("Total produzido = %d\n", tot_prod);
  return n;
}

void *consumer(void *n)
{
  int j = 0, tot = 0;
  while (theBuffer.finish)
  {
    if (theBuffer.sharedData > 0) {
      j = retrieve(&theBuffer);
      tot += j;
      pthread_mutex_unlock(&mutex);
      printf("Consumido: %d | Total no buffer: %d\n", j, theBuffer.sharedData);
      delay(rand() % 6); /* up to 5 sec */
    }
  }
  printf("Total consumido = %d\n", tot);
  return (n);
}

int main()
{
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  pthread_t prodThread, consThread;
  int e, st;

  theBuffer.writeable = 1;
  theBuffer.finish = 1;
  pthread_mutex_init(&mutex, NULL);
  srand(time(NULL));
  printf("Iniciou!\n");

  if (e = pthread_create(&prodThread, &attr, producer, NULL) != 0) {
    printf("Criação da thread produtora falhou! Erro %d\n", e);
    exit(-1);
  }
  
  if (e = pthread_create(&consThread, &attr, consumer, NULL) != 0) {
    printf("Criação da thread consumidora falhou! Erro %d\n", e);
    exit(-1);
  }
  
  if (e = pthread_join(prodThread, (void **)&st) != 0) {
    printf("Thread produtora erro ao juntar %d\n", e);
    exit(-1);
  }
  printf("Thread produtora uniu: status = %x\n", st);

  if (e = pthread_join(consThread, (void **)&st) != 0) {
    printf("Thread consumidora erro ao juntar %d\n", e);
    exit(-1);
  }
  printf("Thread consumidora uniu: status = %x\n", st);

  pthread_exit(NULL);
}
