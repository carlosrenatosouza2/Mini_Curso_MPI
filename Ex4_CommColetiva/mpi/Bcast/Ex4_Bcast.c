#include <stdio.h>
#include <mpi.h>

int main(int argc, char const *argv[])
{  
   int r, p, valor, mestre = 0;
    
   MPI_Init(NULL, NULL);
   MPI_Comm_rank(MPI_COMM_WORLD, &r);
   MPI_Comm_size(MPI_COMM_WORLD, &p);

   // Cada processo tem um valor inicial diferente 
   valor = r * 10;

   // cada rank imprime seu valor antes do BCast:
   printf("O processo %d possui valor: %d\n", r, valor);

   if (r==mestre)
   {
      printf("Entre um valor: \n");
      scanf("%d", &valor);
   }

   // A rotina de difusão é chamada por todos processos 
   MPI_Bcast(&valor, 1, MPI_INT, mestre, MPI_COMM_WORLD);
   
   // O valor agora é o mesmo em todos os processos
   printf("O processo %d recebeu o valor: %d\n", r, valor);

   MPI_Finalize();
    return 0;
}