#include <stdio.h>

// mpi
#include <mpi.h>

int main(int argc, char **argv)
{
  int rank = 0;
  int size = 0;
  int tag = 0;
  double p[10];
  
  MPI_Init(&argc, &argv);
  
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  printf("Hello from rank %d of %d\n", rank, size);

  if (size != 2)
    {
      printf("Need exatly 2 processus!\n");
      MPI_Finalize();
      return 1;
    }

  if (rank == 0)
    {
      MPI_Recv(p, 10, MPI_DOUBLE, 1, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      for (int i = 0; i < 10; i++)
        {
          printf("P%d receive real %lf\n", rank, *(p + i));
        }
    }
  else
    {
      for (int i = 0; i < 10; i++)
        {
          p[i] = i;
          printf("P%d send real %lf\n", rank, *(p + i));
        }
      MPI_Send(p, 10, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
    }
  
  MPI_Finalize();
  
  return 0;
}
