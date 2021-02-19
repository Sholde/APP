#include <stdio.h>

// mpi
#include <mpi.h>

// pid / getpid
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
  int rank = 0;
  int size = 0;
  char proc_name[MPI_MAX_PROCESSOR_NAME];
  pid_t pid;
  int len = 0;
  int var_after = 0;

  printf("After Init\n");
  
  MPI_Init(&argc, &argv);
  
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Get_processor_name(proc_name, &len);
  pid = getpid();

  printf("Hello from rank %d of %d, with pid = %d on %s\n", rank, size, pid, proc_name);

  // Just to separate question
  MPI_Barrier(MPI_COMM_WORLD);
  
  int var_before = 0;

  printf("[%d] after = %p, before = %p\n", rank, &var_after, &var_before);
  
  MPI_Finalize();
  
  return 0;
}
