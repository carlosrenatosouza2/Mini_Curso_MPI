#include <stdio.h>
#include <mpi.h>

int main(int argc, char const *argv[])
{
    int r, p;

    MPI_Init(NULL, NULL);

    MPI_Comm_rank(MPI_COMM_WORLD, &r);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    printf("\n Hello World from rank %d of %d processes.\n", r, p); 

    MPI_Finalize();

    return 0;
}
