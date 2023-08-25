#include <stdio.h>
#include <stdlib.h>


// Compilacao:
//cc stencil.c -o stencil_pgi_bro.x -lm
//     

// Execucao:      $0     n   energy niters
//time aprun -n 1 prog.x 100 1      200 
//Xmitswan.sh 1 1 01 BROADWELL MPI


// row-major order
#define ind(i,j) (j)*(n+2)+i



int main(int argc, char **argv) 
{
   FILE *arq;
  int n = atoi(argv[1]); // nxn grid :: 100x100=10000
  int energy = atoi(argv[2]); // energy to be injected per iteration :: 1
  int niters = atoi(argv[3]); // number of iterations :: 200
  double *aold = (double*)calloc(1,(n+2)*(n+2)*sizeof(double)); // 1-wide halo zones!
  double *anew = (double*)calloc(1,(n+2)*(n+2)*sizeof(double)); // 1-wide halo-zones!
  double heat=0.0; // total heat in system
  int i, iter, j;

  arq = fopen("temp.txt", "w");
  
  for(i=0; i<(n+2)*(n+2); i++)
  {
    aold[i]=0.0;
    anew[i]=0.0;
  }

   for(j=1; j<n+1; ++j) 
      for(i=1; i<n+1; ++i) 
         fprintf(arq, "%lf ", aold[ind(i,j)]);
   fprintf(arq, "\n");
   


  #define nsources 3
  int sources[nsources][2] = {{n/2,n/2}, {n/3,n/3}, {n*4/5,n*4/5}};
  
  for(iter=0; iter<niters; iter+=2) 
  {
   for(j=1; j<n+1; ++j) 
      for(i=1; i<n+1; ++i) 
         anew[ind(i,j)] = aold[ind(i,j)]/2.0 + (aold[ind(i-1,j)] + aold[ind(i+1,j)] + aold[ind(i,j-1)] + aold[ind(i,j+1)])/4.0/2.0;

   for(i=0; i<nsources; ++i) 
      anew[ind(sources[i][0],sources[i][1])] += energy; // heat source

   for(j=1; j<n+1; ++j) 
      for(i=1; i<n+1; ++i) 
         fprintf(arq, "%lf ", anew[ind(i,j)]);
   fprintf(arq, "\n");
   
   for(j=1; j<n+1; ++j) 
      for(i=1; i<n+1; ++i) 
         aold[ind(i,j)] = anew[ind(i,j)]/2.0 + (anew[ind(i-1,j)] + anew[ind(i+1,j)] + anew[ind(i,j-1)] + anew[ind(i,j+1)])/4.0/2.0;

   
   for(i=0; i<nsources; ++i) 
      aold[ind(sources[i][0],sources[i][1])] += energy; // heat source

   for(j=1; j<n+1; ++j) 
      for(i=1; i<n+1; ++i) 
         fprintf(arq, "%lf ", aold[ind(i,j)]);
   fprintf(arq, "\n");
  }


  heat=0.0;
  for(j=1; j<n+1; ++j) 
    for(i=1; i<n+1; ++i) 
      heat += aold[ind(i,j)];

   printf("SEQ_C  execucao com %2d procs  heat= %lf  \n", 1, heat);

      
   fclose(arq);
   return(0);
}