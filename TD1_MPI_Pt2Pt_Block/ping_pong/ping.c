#include <stdio.h>

// mpi
#include <mpi.h>

int main(int argc, char **argv)
{
  int rank = 0;
  int size = 0;

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
      int p = 10;
      MPI_Send(&p, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
  else
    {
      int p = 0;
      MPI_Status sta;
      MPI_Recv(&p, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &sta);
      printf("%d receive integer %d\n", rank, p);
    }
  
  MPI_Finalize();
  
  return 0;
}
