#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct
{
  /*A compléter*/ 
  pthread_cond_t cond;
  pthread_mutex_t mut;
  int count;
  int count_max;
} my_barrier_t;



/* Initialise une barrière pour num threads*/
int my_barrier_init(my_barrier_t * barrier, int num)
{
  /*A compléter*/
  barrier->count = 0;
  barrier->count_max = num;
  pthread_cond_init(&(barrier->cond), NULL);
}

/* Bloque tant que tous les threads ne sont pas arrivés */
int my_barrier_wait(my_barrier_t * barrier)
{
  /*A compléter*/
  pthread_mutex_lock(&(barrier->mut));
  barrier->count++;
  if (barrier->count == barrier->count_max)
    {
      barrier->count = 0;
      pthread_cond_broadcast(&(barrier->cond));
    }
  else
    {
      pthread_cond_wait(&(barrier->cond), &(barrier->mut));
    }
  pthread_mutex_unlock(&(barrier->mut));
}


my_barrier_t barrier;

void * run(void * arg)
{
  long rank = (long)arg;
  int i;
  for(i = 0 ; i < 5 ; i++)
    {
      sleep( rand()%5 );
      printf("*Iter %d,Thread %ld* AVANT \n", i, rank);
      my_barrier_wait(&barrier);
      printf("*Iter %d,Thread %ld* APRES \n", i, rank);
    }
  return NULL;
}



#define NUM_THREADS 5

int main(int argc, char ** argv)
{  
  pthread_t threads[NUM_THREADS];
  long i;

  pthread_mutex_init(&(barrier.mut), NULL);
  my_barrier_init(&barrier, NUM_THREADS);
      
  srand(time(0));
   
  for(i=0; i < NUM_THREADS; ++i)
    pthread_create(&threads[i], NULL, run, (void*)i);
   
  for(i=0; i < NUM_THREADS; ++i)
    pthread_join(threads[i], NULL);

  pthread_cond_destroy(&(barrier.cond));
  pthread_mutex_destroy(&(barrier.mut));
  return 0;
}
