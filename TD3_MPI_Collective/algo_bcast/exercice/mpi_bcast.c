#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

void mpi_bcast(char *buf, int n)
{
  MPI_Bcast(buf, n, MPI_BYTE, 0, MPI_COMM_WORLD);
}

void binary_tree_bcast(char *buf, int n, int rank, int size)
{
  int root = 0;

  if (rank == root)
    {
      for (int i = 1; i < size; i++)
        {
          MPI_Send(buf, n, MPI_BYTE, i, 0, MPI_COMM_WORLD);
        }
    }
  else
    {
      MPI_Recv(buf, n, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
}

void linear_bcast(char *buf, int n, int rank, int size)
{
  int root = 0;
  int square = 1;

  for (int i = 0; i < log(size); i++)
    {
      if (rank < square)
        {
          for (int j = 0; j < i; j++)
            {
              MPI_Recv(buf, n, MPI_BYTE, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }

          for (int j = 0; j < i + 1; j++)
            {
              MPI_Send(buf, n, MPI_BYTE, j, 0, MPI_COMM_WORLD);
            }
        }
      
      square *= 2;
    }
}

#define NITER 100

int main(int argc, char **argv)
{
    int n = atoi(argv[1]);
    int rank, iter;
    char *buf;
    double tend, tbeg;
    int size = 0;
  
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    buf = (char*)malloc(n*sizeof(char));

    if (rank == /*root=*/0)
    {
        printf("%d bytes\n", n);
        fflush(stdout);
        memset(buf, 0, n);
    }

    tbeg = MPI_Wtime();

    for(iter = 0 ; iter < NITER ; iter++)
    {
      //mpi_bcast(buf, n);
      //linear_bcast(buf, n, rank, size);
      binary_tree_bcast(buf, n, rank, size);
    }

    tend = MPI_Wtime();

    if (rank == 0)
    {
        printf("Telaps for %d bcast = %.3e s\n", NITER, tend - tbeg);
    }

    free(buf);

    MPI_Finalize();
    return 0;
}

