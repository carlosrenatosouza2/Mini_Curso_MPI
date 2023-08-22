#include <stdio.h>
#include <mpi.h>

int main(int argc, char const *argv[])
{
    int r, p;

    // Inicializa o MPI: inicio da regiao paralela.
    MPI_Init(NULL, NULL);

    // Qual processo sou eu? 
    // Consulta o numero do meu processo:
    MPI_Comm_rank(MPI_COMM_WORLD, &r);

    // Consulta quantos processos existem no total:
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    printf("\n Hello World from rank %d of %d processes.\n", r, p); 

    // Finaliza o MPI: fecha a regiao paralela.
    MPI_Finalize();

    return 0;
}
