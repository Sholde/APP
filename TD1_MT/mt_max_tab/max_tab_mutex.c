#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_VAL 1000

typedef struct info_s
{
  int *tab;
  int nelt;
  int *max;
  pthread_mutex_t *mut;
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

  pthread_mutex_lock(info->mut);
  if (*(info->max) < max_local)
    {
      *(info->max) = max_local;
    }
  pthread_mutex_unlock(info->mut);
    
  return NULL;
}

int main(int argc, char **argv)
{
    int nthreads, nelt, i, t, maxv_seq, maxv_mt;
    int *tab;

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

    pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

    int truc = nelt / nthreads;
    int lol  = nelt % nthreads;

    int ideb = 0;
    
    for (int i = 0; i < nthreads; i++)
      {
        info[i].nelt = (i < lol ? truc + 1 : truc);
        info[i].tab = tab + ideb;
        info[i].max = &maxv_mt;
        info[i].mut = &mut;
        
        pthread_create(&tid[i], NULL, max_parallel, &info[i]);

        ideb += info[i].nelt;
      }

    for (int i = 0; i < nthreads; i++)
      {
        pthread_join(tid[i], NULL);
      }

    pthread_mutex_destroy(&mut);

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

