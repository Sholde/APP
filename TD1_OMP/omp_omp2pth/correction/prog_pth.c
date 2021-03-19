#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct func_args_s
{
    int rank;
    int N;

    pthread_mutex_t *mut;
    int *sum;
};
typedef struct func_args_s func_args_t;

void *func(void *p_args)
{
    func_args_t *args = (func_args_t*)p_args;
    int i, sum_loc = 0;

    printf("Je suis le maitre ! %d\n", args->rank);
    if (args->rank == 0)
    {
        printf("Je suis le maitre !\n");
    }

    for(i = 0 ; i < args->N ; i++)
    {
        sum_loc += i;
    }

    pthread_mutex_lock(args->mut);
    *(args->sum) += sum_loc;
    pthread_mutex_unlock(args->mut);

    return NULL;
}

int main(int argc, char **argv)
{
    int N = atoi(argv[1]);
    int sum = 0;

    int nthreads, t;
    pthread_t *tid;
    func_args_t *all_args;
    pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

    nthreads = atoi(getenv("OMP_NUM_THREADS"));

    /* Debut de la region parallele ou nthreads vont s'executer en concurrence */
    tid = (pthread_t*)malloc(nthreads*sizeof(pthread_t));
    all_args = (func_args_t*)malloc(nthreads*sizeof(func_args_t));

    for(t = 0 ; t < nthreads ; t++)
    {
        all_args[t].rank = t;
        all_args[t].N    = N;
        all_args[t].mut  = &mut;
        all_args[t].sum  = &sum;

        pthread_create(tid+t, NULL, func, all_args+t);
    }

    for(t = 0 ; t < nthreads ; t++)
    {
        pthread_join(tid[t], NULL);
    }
    /* Fin de la region parallele */

    printf("sum = %d\n", sum);

    free(tid);
    free(all_args);

    return 0;
}

