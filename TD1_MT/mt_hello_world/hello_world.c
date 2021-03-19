#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct info_s
{
  int rank;
  char str[128];
} info_t;

void *print_hello_world(void *arg)
{
  printf("Hello world!\n");
  return NULL;
}

void *print_hello_world_rank(void *arg)
{
  printf("Hello world from %d!\n", *(int *)arg);
  return NULL;
}

void *print_hello_world_info(void *arg)
{
  info_t info = *(info_t *)arg;
  printf("From: %2d, Message: %s\n", info.rank, info.str);
  return NULL;
}

int main(int argc, char **argv)
{
  // Check argument
  if (argc != 2)
    return printf("Error: need 1 argument\n"), 1;

  int n_thread = atoi(argv[1]);

  if (n_thread < 0)
    return printf("Error: need at least 0 thread\n"), 1;

  // Init
  pthread_t *tid = malloc(sizeof(pthread_t) * n_thread);
  int *rank = malloc(sizeof(int) * n_thread);
  info_t *info = malloc(sizeof(info_t) * n_thread);

  printf("Version 1 (without argument)\n");
  
  // Create threads
  for (int i = 0; i < n_thread; i++)
    {
      pthread_create(&tid[i], NULL, print_hello_world, NULL);
    }

  // Join threads
  for (int i = 0; i < n_thread; i++)
    {
      pthread_join(tid[i], NULL);
    }

  printf("Version 2 (with argument int)\n");

  // Create threads
  for (int i = 0; i < n_thread; i++)
    {
      rank[i] = i;
      pthread_create(&tid[i], NULL, print_hello_world_rank, &rank[i]);
    }

  // Join threads
  for (int i = 0; i < n_thread; i++)
    {
      pthread_join(tid[i], NULL);
    }

  printf("Version 3 (with argument info)\n");
  
  // Create threads
  for (int i = 0; i < n_thread; i++)
    {
      info[i].rank = i;
      sprintf(info[i].str,"Hello world from %d!", i);
      pthread_create(&tid[i], NULL, print_hello_world_info, &info[i]);
    }

  // Join threads
  for (int i = 0; i < n_thread; i++)
    {
      pthread_join(tid[i], NULL);
    }

  // Clean
  free(tid);
  free(rank);
  free(info);
  
  return 0;
}
