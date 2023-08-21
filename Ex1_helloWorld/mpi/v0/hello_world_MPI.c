#include <stdio.h>
#include <mpi.h>

int main(int argc, char const *argv[])
{

    MPI_Init(NULL, NULL);


    printf("\n Hello World \n"); 

    MPI_Finalize();

    return 0;
}
