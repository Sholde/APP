#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int valB = 0;
sem_t sem;

int work(int valeur)
{   
  /* Fait semblant de bosser */
  sleep(rand()%5);
   
  return valeur+1;;
}

void * runA(void * arg)
{
  int valA = 0;
  valA = work(valA);

  printf("*A* En attente du résultat de B\n");
  sem_wait(&sem);
   
  printf("*A* Valeur de A: %d Valeur de B: %d\n", valA, valB);
  return NULL;
}


void * runB(void * arg)
{
  valB = work(valB);

  /*Signale à A que j'ai calculé valB*/
  printf("*B* Valeur calculée %d \n", valB);

  sem_post(&sem);
   
  return NULL;
}


int main(int argc, char ** argv)
{  
  pthread_t threadA, threadB;

  srand(time(0));

  sem_init(&sem, 1, 0);
  
  pthread_create(&threadA, NULL, runA, NULL);
  pthread_create(&threadB, NULL, runB, NULL);
      
  pthread_join(threadA, NULL);
  pthread_join(threadB, NULL);

  sem_destroy(&sem);
   
  return 0;
}
