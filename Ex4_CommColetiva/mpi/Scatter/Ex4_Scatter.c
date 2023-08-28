#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define TAM_VET 10

int main(int argc, char const *argv[])
{  
   int r, p, valor, mestre = 0;
   int *vet_envia, vet_recebe[TAM_VET];

   MPI_Init(NULL, NULL);
   MPI_Comm_rank(MPI_COMM_WORLD, &r);
   MPI_Comm_size(MPI_COMM_WORLD, &p);

   // o mestre aloca o espaço de memória e inicia o veto
   if (r == mestre)
   {
      vet_envia = (int*) malloc (p*TAM_VET*sizeof(int));
      for (int i = 0; i < (TAM_VET*p); i++)
      {
         vet_envia[i] = i*10;
         printf("r %d, vet_envia[%d]: %d\n", r, i, vet_envia[i]);
      }
      printf("\n");
   }

   // O vetor é distribuído em partes iguais entre todos ranks
   MPI_Scatter(vet_envia, TAM_VET, MPI_INT, vet_recebe, TAM_VET, MPI_INT, mestre, MPI_COMM_WORLD);
   
   // Cada processo imprime a parte que recebeu */
   for (int i = 0; i < TAM_VET; i ++)
      printf("Processo = %d, vet_recebe[%d]: %d\n", r, i, vet_recebe[i]);
   printf("\n");
   
   MPI_Finalize();
   return 0;
}