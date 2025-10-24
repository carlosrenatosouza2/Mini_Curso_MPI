#include <stdio.h>
#include <math.h>
#include <mpi.h>

#define TAM 5

int main(int argc, char *argv[]) 
{
   int r, p, i, mestre = 0;
   float vet_envia [TAM] ; /* Vetor a ser enviado */
   float vet_recebe [TAM]; /* Vetor a ser recebido */
 

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &r);
   MPI_Comm_size(MPI_COMM_WORLD, &p);
 
   /* Preenche o vetor com valores que dependem do ranque */
   for (i = 0; i < TAM; i++)
   {
      vet_envia[i] = (float) (r*TAM+i);
      vet_recebe[i] = 0.0;
   }
 
   /* Faz a redução, encontrando o valor máximo do vetor */
   MPI_Reduce(vet_envia, vet_recebe, TAM, MPI_FLOAT, MPI_MAX, mestre, MPI_COMM_WORLD);
 
   /* O processo raiz imprime o resultado da redução */
   if (r == mestre) 
   {
      for (i = 0; i < TAM; i++)
         printf("vet_recebe[%d] = %3.1f \n", i,vet_recebe[i]);
      printf("\n\n"); 
   }

 MPI_Finalize();
 return(0);
}
