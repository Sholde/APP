#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_VAL 1000

typedef struct info_s
{
  int *tab;
  int nelt;
  int *max;
  sem_t *sem;
} info_t;

int max_seq(int *tab, int nelt)
{
    int i, maxv;

    maxv = 0;
    for( i = 0 ; i < nelt ; i++ )
    {
        if (tab[i] > maxv)
        {
            maxv = tab[i];
        }
    }

    return maxv;
}

void *max_parallel(void *arg)
{
  info_t *info = (info_t *)arg;

  int max_local = 0;

  for (int i = 0; i < info->nelt; i++)
    {
      if (info->tab[i] > max_local)
        {
          max_local = info->tab[i];
        }
    }

  sem_wait(info->sem);
  if (*(info->max) < max_local)
    {
      *(info->max) = max_local;
    }
  sem_post(info->sem);
    
  return NULL;
}

int main(int argc, char **argv)
{
    int nthreads, nelt, i, t, maxv_seq, maxv_mt;
    int *tab;

    if (argc != 3)
      return 1;
    
    nelt     = atoi(argv[1]); /* Taille du tableau */
    nthreads = atoi(argv[2]); /* Nombre de threads a creer */

    /* Creation du tableau et remplissage aleatoire */
    tab = (int*)malloc(nelt*sizeof(int));

    srand(nelt);
    for( i = 0 ; i < nelt ; i++)
    {
        tab[i] = 1 + (rand() % MAX_VAL);
    }

    /* Recherche du max de tab en contexte multithread => maxv_mt*/
    maxv_mt = 0;
    /* A ECRIRE */
    pthread_t *tid = malloc(sizeof(pthread_t) * nthreads);
    info_t *info = malloc(sizeof(info_t) * nthreads);

    sem_t sem_buff;
    sem_init(&sem_buff, 0, 1);

    int truc = nelt / nthreads;
    int lol  = nelt % nthreads;

    int ideb = 0;
    
    for (int i = 0; i < nthreads; i++)
      {
        info[i].nelt = (i < lol ? truc + 1 : truc);
        info[i].tab = tab + ideb;
        info[i].max = &maxv_mt;
        info[i].sem = &sem_buff;
        
        pthread_create(&tid[i], NULL, max_parallel, &info[i]);

        ideb += info[i].nelt;
      }

    for (int i = 0; i < nthreads; i++)
      {
        pthread_join(tid[i], NULL);
      }

    sem_destroy(&sem_buff);

    /* Recherche du max en sequentiel pour verification => maxv_seq */
    maxv_seq = max_seq(tab, nelt);

    if (maxv_seq == maxv_mt)
    {
        printf("PASSED\n");
    }
    else
    {
        printf("FAILED\n");
        printf("Valeur correcte : %d\n", maxv_seq);
        printf("Votre valeur    : %d\n", maxv_mt);
    }

    free(tab);

    return 0;
}

