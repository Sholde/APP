#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int P, rank;
    int Nib, Njb, ib, jb;

    MPI_Comm COMM_ROW, COMM_COL;

    MPI_Init(&argc, &argv);

    Nib = atoi(argv[1]);
    Njb = atoi(argv[2]);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &P);

    if (P != Nib*Njb)
    {
        if (rank == 0)
        {
            printf("Nib x Njb = %d x %d != %d processus MPI\n", Nib, Njb, P);
            abort();
        }
    }

    /*
       A ECRIRE : creation communicateurs comm_row et comm_col
     */

    // Determine index
    ib = rank / Njb;
    jb = rank % Njb;

    // Create communicator
    MPI_Comm_split(MPI_COMM_WORLD, jb, rank, &COMM_ROW);
    MPI_Comm_split(MPI_COMM_WORLD, ib, rank, &COMM_COL);

    // Recup Nib and Njb in tmp_nib and tmp_njb
    int tmp_nib = 0;
    int tmp_njb = 0;

    MPI_Comm_size(COMM_ROW, &tmp_nib);
    MPI_Comm_size(COMM_COL, &tmp_njb);

    if (Nib != tmp_nib)
      {
        printf("nib and tmp_nib are not equal\n");
      }

    if (Njb != tmp_njb)
      {
        printf("njb and tmp_njb are not equal\n");
      }

    // Recup ib and jb in rank_row and rank_col
    int rank_row = 0;
    int rank_col = 0;

    MPI_Comm_rank(COMM_ROW, &rank_row);
    MPI_Comm_rank(COMM_COL, &rank_col);

    if (ib != rank_row)
      {
        printf("ib and rank_row are not equal\n");
      }

    if (jb != rank_col)
      {
        printf("jb and rank_col are not equal\n");
      }

    MPI_Finalize();

    return 0;
}

