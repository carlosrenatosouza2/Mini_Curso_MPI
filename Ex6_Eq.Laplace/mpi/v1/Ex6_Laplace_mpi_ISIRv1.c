#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

//--------------------------------------------------/
// Compilacao:
// mpicc prog.c -o prog.x 
//     
// Execucao:      $0     n   energy niters px py
//    time mpirun prog.x 100 1      200    2  2
//
//--------------------------------------------------/

// row-major order
#define ind(i,j) (j)*(bx+2)+(i)

// Qtde pontos de energia
#define nsources 3

int main(int argc, char **argv) {
double heat; // total heat in system per core
   
  int r,p;
  int n, energy, niters, px, py;
  double t=0.0, tstencil=0.0, tcomm=0.0, tswap=0.0;
  
  
  MPI_Comm comm = MPI_COMM_WORLD;
  MPI_Status Allstats[8];
  MPI_Request reqs[8];

  MPI_Init(&argc, &argv);
  
  MPI_Comm_rank(comm, &r);
  MPI_Comm_size(comm, &p);
  
  if (r==0) 
  {
      // argument checking
      if(argc < 6) 
      {
          if(!r) printf("usage: stencil_mpi <n> <energy> <niters> <px> <py>\n");
          MPI_Finalize();
          exit(1);       
      }
      
      n = atoi(argv[1]); // nxn grid
      energy = atoi(argv[2]); // energy to be injected per iteration
      niters = atoi(argv[3]); // number of iterations
      px=atoi(argv[4]); // 1st dim processes
      py=atoi(argv[5]); // 2nd dim processes
      
      if(px * py != p) MPI_Abort(comm, 1);// abort if px or py are wrong
      if(n % py != 0) MPI_Abort(comm, 2); // abort px needs to divide n
      if(n % px != 0) MPI_Abort(comm, 3); // abort py needs to divide n
      
      // distribute arguments
      int args[5] = {n, energy, niters, px,  py};
      MPI_Bcast(args, 5, MPI_INT, 0, comm);
  }
  else 
  {
      int args[5];
      MPI_Bcast(args, 5, MPI_INT, 0, comm);
      n=args[0]; energy=args[1]; niters=args[2]; px=args[3]; py=args[4];
  }

  // determine my coordinates (x,y) -- r=x*a+y in the 2d processor array
  int rx = r % px;
  int ry = r / px;
  // determine my four neighbors
  int north = (ry-1)*px+rx; if(ry-1 < 0)   north = MPI_PROC_NULL;
  int south = (ry+1)*px+rx; if(ry+1 >= py) south = MPI_PROC_NULL;
  int west= ry*px+rx-1;     if(rx-1 < 0)   west = MPI_PROC_NULL;
  int east = ry*px+rx+1;    if(rx+1 >= px) east = MPI_PROC_NULL;
  // decompose the domain
  int bx = n/px; // block size in x
  int by = n/py; // block size in y
  int offx = rx*bx; // offset in x
  int offy = ry*by; // offset in y

  //printf("%i (%i,%i) - w: %i, e: %i, n: %i, s: %i\n", r, ry,rx,west,east,north,south);

  // allocate two work arrays
  double *aold = (double*)calloc(1,(bx+2)*(by+2)*sizeof(double)); // 1-wide halo zones!
  double *anew = (double*)calloc(1,(bx+2)*(by+2)*sizeof(double)); // 1-wide halo zones!
  double *tmp;

  // initialize three heat sources
  int sources[nsources][2] = {{n/2,n/2}, {n/3,n/3}, {n*4/5,n*8/9}};
  int locnsources=0; // number of sources in my area
  int locsources[nsources][2]; // sources local to my rank
  for (int i=0; i<nsources; ++i) // determine which sources are in my patch
  {
    int locx = sources[i][0] - offx;
    int locy = sources[i][1] - offy;
    if(locx >= 0 && locx < bx && locy >= 0 && locy < by) 
    {
      locsources[locnsources][0] = locx+1; // offset by halo zone
      locsources[locnsources][1] = locy+1; // offset by halo zone
      locnsources++;
    }
  }

  
  // allocate communication buffers
  double *sbufnorth = (double*)calloc(1,bx*sizeof(double)); // send buffers
  double *sbufsouth = (double*)calloc(1,bx*sizeof(double));
  double *sbufeast = (double*)calloc(1,by*sizeof(double));
  double *sbufwest = (double*)calloc(1,by*sizeof(double));
  double *rbufnorth = (double*)calloc(1,bx*sizeof(double)); // receive buffers
  double *rbufsouth = (double*)calloc(1,bx*sizeof(double));
  double *rbufeast = (double*)calloc(1,by*sizeof(double));
  double *rbufwest = (double*)calloc(1,by*sizeof(double));

  for(int i=0; i<(bx+2)*(by+2); i++) 
   {
      aold[i]=0.0;
      anew[i]=0.0;
   }

  t=-MPI_Wtime(); // take time
  for(int iter=0; iter<niters; ++iter) {
    // refresh heat sources
    for(int i=0; i<locnsources; ++i) 
      aold[ind(locsources[i][0],locsources[i][1])] += energy; // heat source
    

    // exchange data with neighbors
   
    for(int i=0; i<bx; ++i) sbufnorth[i] = aold[ind(i+1,1)]; // pack loop - last valid region
    for(int i=0; i<bx; ++i) sbufsouth[i] = aold[ind(i+1,by)]; // pack loop
    for(int i=0; i<by; ++i) sbufeast[i] = aold[ind(bx,i+1)]; // pack loop
    for(int i=0; i<by; ++i) sbufwest[i] = aold[ind(1,i+1)]; // pack loop

   tcomm=-MPI_Wtime(); // take time
    MPI_Isend(sbufnorth, bx, MPI_DOUBLE, north, 9, comm, &reqs[0]);
    MPI_Isend(sbufsouth, bx, MPI_DOUBLE, south, 9, comm, &reqs[1]);
    MPI_Isend(sbufeast, by, MPI_DOUBLE, east, 9, comm, &reqs[2]);
    MPI_Isend(sbufwest, by, MPI_DOUBLE, west, 9, comm, &reqs[3]);
    MPI_Irecv(rbufnorth, bx, MPI_DOUBLE, north, 9, comm, &reqs[4]);
    MPI_Irecv(rbufsouth, bx, MPI_DOUBLE, south, 9, comm, &reqs[5]);
    MPI_Irecv(rbufeast, by, MPI_DOUBLE, east, 9, comm, &reqs[6]);
    MPI_Irecv(rbufwest, by, MPI_DOUBLE, west, 9, comm, &reqs[7]);
    MPI_Waitall(8, reqs, Allstats);
   tcomm+=MPI_Wtime(); // take time

    if (north != MPI_PROC_NULL) for(int i=0; i<bx; ++i) aold[ind(i+1,0)] = rbufnorth[i]; // unpack loop - into ghost cells
    if (south != MPI_PROC_NULL) for(int i=0; i<bx; ++i) aold[ind(i+1,by+1)] = rbufsouth[i]; // unpack loop
    if (east != MPI_PROC_NULL) for(int i=0; i<by; ++i) aold[ind(bx+1,i+1)] = rbufeast[i]; // unpack loop
    if (west != MPI_PROC_NULL) for(int i=0; i<by; ++i) aold[ind(0,i+1)] = rbufwest[i]; // unpack loop

   tstencil=-MPI_Wtime(); // take time
    // update grid points
    heat = 0.0;
    for(int j=1; j<by+1; ++j)
      for(int i=1; i<bx+1; ++i) {
        anew[ind(i,j)] = aold[ind(i,j)]/2.0 + (aold[ind(i-1,j)] + aold[ind(i+1,j)] + aold[ind(i,j-1)] + aold[ind(i,j+1)])/4.0/2.0;
        heat += anew[ind(i,j)];
      }
   tstencil+=MPI_Wtime();

    // swap arrays
    tswap=-MPI_Wtime();
    tmp=anew; anew=aold; aold=tmp;
tswap+=MPI_Wtime();

  }
  t+=MPI_Wtime();

  // get final heat in the system
  double rheat;
  MPI_Allreduce(&heat, &rheat, 1, MPI_DOUBLE, MPI_SUM, comm);
  if(!r) printf("[%i] last heat: \t %f, \nTotal time: \t %f, \nStencil time: \t %f, \nCommunic. time:  %f, \nSwap time: %f\n", 
  r, rheat, t, tstencil, tcomm, tswap);

  MPI_Finalize();
}
