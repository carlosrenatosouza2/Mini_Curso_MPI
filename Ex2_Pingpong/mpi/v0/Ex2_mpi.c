#include <stdio.h>
#include <mpi.h>

int main(int argc, char const *argv[])
{
    int r, p;
    int source, dest, tag=1;
    int dado=0, ibuff;
    MPI_Status stat;

   MPI_Init(NULL, NULL);

   MPI_Comm_rank(MPI_COMM_WORLD, &r);
   MPI_Comm_size(MPI_COMM_WORLD, &p);

   printf("\nAntes da com.: rank: %d, dado: %d.\n", r, dado);

   if ( r == 0 ){
      dest = 1;
      source = 1;
      ibuff=10;
      MPI_Recv(&dado, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &stat);
      MPI_Send(&ibuff, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);   }
   else {
      dest = 0;
      source = 0;
      ibuff=50;
      MPI_Send(&ibuff, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
      MPI_Recv(&dado, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &stat);}

   printf("\nDepois da com.: rank: %d, dado: %d.\n", r, dado);

   MPI_Finalize();
   
    return 0;
}