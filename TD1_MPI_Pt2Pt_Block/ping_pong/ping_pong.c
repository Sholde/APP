#include <stdio.h>
#include <stdlib.h>

// sleep
#include <unistd.h>

// mpi
#include <mpi.h>

int main(int argc, char **argv)
{
  // Variable
  int rank = 0;
  int size = 0;
  int tag = 0;
  double *p = NULL;
  int len = 0;

  // Start mpi
  MPI_Init(&argc, &argv);

  // Recup rank and size
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // Print rank
  printf("Hello from rank %d of %d\n", rank, size);

  // Check number of process and arguments
  if (size != 2 || argc != 2)
    {
      printf("Need exatly 2 processus!\n");
      printf("Need exatly 1 argument!\n");
      MPI_Finalize();
      return 1;
    }

  if (rank == 0)
    {
      // Init array
      len = atoi(argv[1]);
      p = malloc(sizeof(double) * len);

      // Send size
      MPI_Send(&len, 1, MPI_INT, 1, tag, MPI_COMM_WORLD);

      // Receive array
      MPI_Recv(p, len, MPI_DOUBLE, 1, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      // Print array receive
      for (int i = 0; i < len; i++)
        {
          printf("P%d recv real %a\n", rank, *(p + i));
        }

      // Free array
      free(p);
    }
  else
    {
      // Receive size
      MPI_Recv(&len, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      // Waiting 5 seconds
      printf("P%d sleep...\n", rank);
      sleep(5);

      // Init array
      p = malloc(sizeof(double) * len);

      for (int i = 0; i < len; i++)
        {
          p[i] = 1 / (double)i;
          printf("P%d send real %a\n", rank, *(p + i));
        }

      // Send array
      MPI_Send(p, len, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);

      // Free array
      free(p);
    }

  // End mpi
  MPI_Finalize();
  
  return 0;
}
