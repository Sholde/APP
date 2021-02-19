#include <stdio.h>
#include <mpi.h>

/* piegeA piegeA piegeA piegeA */

void piegeA(int rang)
{
    int i, n = 10;

    if (rang == 0)
    {
        int tab[n];

        for(i = 0 ; i < n ; i++)
        {
            tab[i] = i;
        }

        MPI_Send(tab, n, MPI_INT, /*dest=*/1, /*tag=*/1000, MPI_COMM_WORLD);
    }
    else if (rang == 1)
    {
        int recue[n];

        MPI_Recv(&recue, n, MPI_INT, /*src=*/0,  /*tag=*/1000, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for(i = 0 ; i < n ; i++)
        {
            printf("%d\n", recue[i]);
        }
    }
}

int main(int argc, char **argv)
{
    int rang;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rang);

    piegeA(rang);

    MPI_Finalize();

    return 0;
}

