#include <stdio.h>
#include <mpi.h>

int numerotation(int me, int west, int east)
{
  int tmp = 0;
  MPI_Status sta;
  
  if (me == 0)
    {
      MPI_Send(&me, 1, MPI_INT, east, 0, MPI_COMM_WORLD);
      MPI_Recv(&tmp, 1, MPI_INT, west, 0, MPI_COMM_WORLD, &sta);
    }
  else
    {
      MPI_Recv(&me, 1, MPI_INT, west, 0, MPI_COMM_WORLD, &sta);
      me++;
      MPI_Send(&me, 1, MPI_INT, east, 0, MPI_COMM_WORLD);
    }

  return me;
}

void numerotation_proc(int west, int east, int *me, int *nb)
{
  MPI_Status sta;
  
  if (*me == 0)
    {
      // me
      MPI_Send(me, 1, MPI_INT, east, 0, MPI_COMM_WORLD);

      // nb proc
      MPI_Recv(nb, 1, MPI_INT, west, 0, MPI_COMM_WORLD, &sta);
      nb++;
      MPI_Send(nb, 1, MPI_INT, east, 0, MPI_COMM_WORLD);
    }
  else
    {
      // me
      MPI_Recv(me, 1, MPI_INT, west, 0, MPI_COMM_WORLD, &sta);
      (*me)++;
      MPI_Send(me, 1, MPI_INT, east, 0, MPI_COMM_WORLD);

      // nb proc
      MPI_Recv(nb, 1, MPI_INT, west, 0, MPI_COMM_WORLD, &sta);
      if (*nb - 1 != *me)
        MPI_Send(nb, 1, MPI_INT, east, 0, MPI_COMM_WORLD);
    }
}

int main(int argc, char **argv)
{
  int size = 0;
  int rank = 0;
  int me = -1;
  int east = 0;
  int west = 0;
  int nb;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0)
    me = 0;

  east = (rank + 1) % size;
  west = (rank + size - 1) % size;

  // Renum
  //me = numerotation(me, west, east);
  numerotation_proc(west, east, &me, &nb);
  if (me != rank)
    {
      printf("Error: me %d, rank %d\n", me, rank);
    }
  else
    {
      printf("Process %d, me %d, west %d, east %d\n", rank, me, west, east);
    }

  MPI_Finalize();
  
  return 0;
}
