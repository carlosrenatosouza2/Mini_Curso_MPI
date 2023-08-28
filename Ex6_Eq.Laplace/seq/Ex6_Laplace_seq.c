#include <stdio.h>
#include <stdlib.h>

//--------------------------------------------------/
// Compilacao:
// gcc prog.c -o prog.x 
//     
// Execucao:      $0     n   energy niters
//           time prog.x 100 1      200 
//
//--------------------------------------------------/


// row-major order
#define ind(i,j) (j)*(n+2)+i

// Qtde pontos de energia
#define nsources 3

int main(int argc, char **argv) 
{
   int i, iter, j;
   int n = atoi(argv[1]);      // nxn grid :: 100x100=10000
   int energy = atoi(argv[2]); // energy to be injected per iteration :: 1
   int niters = atoi(argv[3]); // number of iterations :: 200
   int sources[nsources][2] = {{n/2,n/2}, {n/3,n/3}, {n*4/5,n*4/5}}; // def. pontos de energia
   double *aold = (double*)calloc(1,(n+2)*(n+2)*sizeof(double)); // 1-wide halo zones!
   double *anew = (double*)calloc(1,(n+2)*(n+2)*sizeof(double)); // 1-wide halo-zones!
   double heat=0.0;            // total heat in system
   
   if(argc != 4)  // argument checking
   {
      printf("usage: exec <n> <energy> <niters> \n");
      exit(1);       
   }

   for(i=0; i<(n+2)*(n+2); i++)  // inicializando as matrizes com zero
   {                             // incluindo as ghost-zones
      aold[i]=0.0;
      anew[i]=0.0;
   }

   for(iter=0; iter<niters; iter+=2) 
   {
      for(j=1; j<n+1; ++j) 
         for(i=1; i<n+1; ++i) 
            anew[ind(i,j)] = aold[ind(i,j)]/2.0 + (aold[ind(i-1,j)] + aold[ind(i+1,j)] + aold[ind(i,j-1)] + aold[ind(i,j+1)])/4.0/2.0;

      for(i=0; i<nsources; ++i) 
         anew[ind(sources[i][0],sources[i][1])] += energy; // heat source
   
      for(j=1; j<n+1; ++j) 
         for(i=1; i<n+1; ++i) 
            aold[ind(i,j)] = anew[ind(i,j)]/2.0 + (anew[ind(i-1,j)] + anew[ind(i+1,j)] + anew[ind(i,j-1)] + anew[ind(i,j+1)])/4.0/2.0;

      for(i=0; i<nsources; ++i) 
         aold[ind(sources[i][0],sources[i][1])] += energy; // heat source
   }

   heat=0.0;
   for(j=1; j<n+1; ++j) 
      for(i=1; i<n+1; ++i) 
         heat += aold[ind(i,j)];

   printf("SEQ_C  execucao com %2d procs  heat= %lf  \n", 1, heat);

   return(0);
}