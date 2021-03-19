#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <pthread.h>

typedef struct message_s
{
  int rank;
  int nb_thread;
  
  int N;
  int *sum;

  pthread_mutex_t *mut;
} message_t;

static void msg_init(message_t *msg)
{
  msg->rank = 0;
  msg->nb_thread = 0;
    
  msg->N = 0;
  msg->sum = NULL;
  
  // pthread
  pthread_mutex_t mut_buff = PTHREAD_MUTEX_INITIALIZER;
  msg->mut = &mut_buff;
}

static void msg_destroy(message_t *msg)
{
  pthread_mutex_destroy((msg->mut));
}

static void *run(void *arg)
{
  message_t *msg = (message_t *)arg;

  int i = 0;
  int N = msg->N;
  int *sum = msg->sum;
  int sum_loc = 0;

  printf("Je suis le maitre ! %d\n");
  if (msg->rank == 0)
  {
    printf("Je suis le maitre !\n");
  }

  for(i = 0 ; i < N ; i++)
    {
      sum_loc += i;
    }

  pthread_mutex_lock((msg->mut));
  {
    *(sum) += sum_loc;
  }
  pthread_mutex_unlock((msg->mut));

  return NULL;
}

int main(int argc, char **argv)
{
    int N = atoi(argv[1]);
    int sum = 0;
    int nb_thread = atoi(getenv("OMP_NUM_THREADS"));
    pthread_t *pt = malloc(sizeof(pthread_t) * nb_thread);

    message_t msg;
    msg_init(&msg);

    for (int i = 0; i < nb_thread; i++)
      {
        printf("here %d\n", i);
        msg.rank = i;
        printf("here %d\n", msg.rank);
        msg.nb_thread = nb_thread;
        msg.N = N;
        msg.sum = &sum;
        pthread_create(&(pt[i]), NULL, run, &msg);
      }

    for (int i = 0; i < nb_thread; i++)
      {
        pthread_join(pt[i], NULL);
      }

    msg_destroy(&msg);

    free(pt);

    printf("sum = %d\n", sum);

    return 0;
}

