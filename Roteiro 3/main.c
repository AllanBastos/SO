#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>


#define N        5
#define LEFT     (i+N-1)%N
#define RIGHT    (i+1)%N
#define THINKING 0
#define HUNGRY   1
#define EATING   2
#define CICLO_J  2

#define down    pthread_mutex_lock
#define up      pthread_mutex_unlock 



int state[N], i;
// declarando semafaros 
pthread_mutex_t mutex;
pthread_mutex_t forks[N];
pthread_t philosofers[N];


// funçoes
void * philosofer(void * id);
void think(int i);
void take_forks(int i);
void eat(int i);
void put_forks(int i);
void test(int i);
void print_status(int i);



void* philosofer(void * id){
  
  int i = * (int*)id;  
  printf("filosofo %d entrou\n", i + 1);
  for(int k=0; k < CICLO_J; k++){
    think(i);
    take_forks(i);
    eat(i);
    put_forks(i);
      
  }
  printf("filosofo %d saindo\n",i + 1);
  pthread_exit((void *)0);
  return 0;
}


void think(int i){
  int t = (rand()%10);
  sleep(t);
}

void eat(int i){
  int t = (rand()%10);
  sleep(t);
}

void take_forks(int i){
  down((&mutex));
  state[i] = HUNGRY;
  print_status(i);
  test(i);
  up(&mutex);
  down(&forks[i]);
}

void put_forks(int i){
  down(&mutex);
  state[i] = THINKING;
  print_status(i);
  test(LEFT);
  test(RIGHT);
  up(&mutex);
}

void test(i){
  if(state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING ){
    state[i] = EATING;
    print_status(i);
    up(&forks[i]);
  
  }
}

void print_status(int i){
  printf("\n");
  for(i=1; i<=N; i++){
       if(state[i-1] == THINKING){
          printf("O filosofo %d esta pensando!\n", i);
       }
       if(state[i-1] == HUNGRY){
          printf("O filosofo %d esta com fome!\n", i);
       }
       if(state[i-1] == EATING){
          printf("O filosofo %d esta comendo!\n", i);
       }
   }
   printf("\n");
}

int main(void) {

  pthread_mutex_init( &(mutex), NULL);
  for (int i = 0; i < N; i++ ){
    pthread_create(&philosofers[i], NULL, philosofer, (void*) &(i) );
    sleep(1);
  }

  pthread_mutex_destroy(&(mutex));

  for (int i = 0; i < N; i++ ){
    pthread_mutex_destroy(&forks[i]);

  }

  pthread_join(philosofers[0], NULL);
  pthread_join(philosofers[1], NULL);
  pthread_join(philosofers[2], NULL);
  pthread_join(philosofers[3], NULL);
  pthread_join(philosofers[4], NULL);
  
  printf("todas os filosofos sairam\n");
  
  
  return 0;
}