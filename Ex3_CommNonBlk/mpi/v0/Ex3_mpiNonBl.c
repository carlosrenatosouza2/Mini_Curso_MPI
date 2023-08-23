#include <stdio.h>
#include <mpi.h>

int main(int argc, char const *argv[])
{  
   int r, p, esq, dir, buf[2], tag1=1, tag2=2;
    
   MPI_Status stats[4];
   MPI_Request reqs[4]; // controle para comunicaoes nao-bloqueantes

   MPI_Init(NULL, NULL);
   MPI_Comm_rank(MPI_COMM_WORLD, &r);
   MPI_Comm_size(MPI_COMM_WORLD, &p);

   // Determinando os ranks vizinhos: a esquerda e a direita:
   esq = r - 1;
   dir = r + 1;
   if ( r == 0 )       esq = p -1; // se sou rank 0, meu viz. a esq = ultimo
   if ( r == (p - 1) ) dir = 0;    // se sou o ultimo, meu viz. a dir = primeiro

   // Recebendo (postando) mensagens nao-bloqueantes para meus vizinhos:
   MPI_Irecv(&buf[0], 1, MPI_INT, esq, tag1, MPI_COMM_WORLD, &reqs[0]);
   MPI_Irecv(&buf[1], 1, MPI_INT, dir, tag2, MPI_COMM_WORLD, &reqs[1]);

   // Enviando (postando) mensagens nao-bloqueantes para meus vizinhos:
   MPI_Isend(&r, 1, MPI_INT, esq, tag2, MPI_COMM_WORLD, &reqs[2]);
   MPI_Isend(&r, 1, MPI_INT, dir, tag1, MPI_COMM_WORLD, &reqs[3]);

   //
   // Faco algum trabalho de computacao enquanto as mensagens sao transmitidas
   //

   // aguardando, verificando as mensagens chegarem:
   MPI_Waitall(4, reqs, stats);
   printf("\n Rank %d :: viz esq: %d viz dir: %d\n", r, buf[0], buf[1]);

   //
   // continue a computacao dependente dos dados transmitidos
   //

   MPI_Finalize();
    return 0;
}