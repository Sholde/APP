#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_ar = PTHREAD_COND_INITIALIZER;

int valB = 0;
int count = 0;

int work(int valeur)
{   
   /* Fait semblant de bosser */
   sleep(rand()%5);
   
   return valeur+1;;
}

void * runA(void * arg)
{
  int round = 0;
  int valA = 0;
  while(1)
    {
      valA = work(valA);
    
      pthread_mutex_lock(&mutex);
      {
        if (count == 2 * round)
          {
            count++;
            printf("*A* En attente de B\n");
            pthread_cond_wait(&cond, &mutex);
          }
        else
          {
            printf("*A* N'attent pas B\n");
            count++;
          }
        pthread_cond_signal(&cond_ar);
      }
      pthread_mutex_unlock(&mutex);

      /* Les deux valeurs devraient toujours être égales */
      printf("*A* Valeur de A: %d Valeur de B: %d\n", valA, valB);
      round++;
   }
  return NULL;
}


void * runB(void * arg)
{
  int round = 0;
  while(1)
    {
      valB = work(valB);

      pthread_mutex_lock(&mutex);
      {
        if (count != 2 * round)
          {
            count++;
            /*Signale à A que j'ai calculé valB*/
            pthread_cond_signal(&cond);     
            printf("*B* Valeur calculée %d \n", valB);
          }
        else
          {
            count++;
            printf("*B* fini avant A\n"); 
          }
        pthread_cond_wait(&cond_ar, &mutex);
      }
      pthread_mutex_unlock(&mutex);
      round++;
    }
   return NULL;
}


int main(int argc, char ** argv)
{  
   pthread_t threadA, threadB;
   
   srand(time(0));
   
   pthread_create(&threadA, NULL, runA, NULL);
   pthread_create(&threadB, NULL, runB, NULL);
      
   pthread_join(threadA, NULL);
   pthread_join(threadB, NULL);    
   
   return 0;
}
