/* Daniel R. Reynolds
   SMU Mathematics
   Math 6370
   7 January 2015 */

// Inclusions
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "mpi.h"
#include "omp.h"


// Example routine using the basic 6 MPI functions, along with OpenMP 
// threading within loops, for a simple "hybrid" MPI+OpenMP program
int main(int argc, char* argv[]) {

  // local variables
  int ierr, numprocs, myid, n, is, ie, i;
  double *a, *b, stime, ftime, runtime, sum, mysum=0.0;

  // initialize MPI, get total number of processes, this proc's ID
  ierr = MPI_Init(&argc, &argv);
  ierr = MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &myid);

  // set problem parameters
  n = 10000000;

  // each node outputs MPI+OpenMP parallelism information to screen
  printf("Proc %i of %i: has %i OpenMP threads\n", 
	 myid, numprocs, omp_get_max_threads());

  // determine this proc's interval in overall problem domain
  // (assumes homogeneous system, giving all procs equal work)
  is = (int) (floor(1.0*n/numprocs)*myid + 1);
  ie = (int) (floor(1.0*n/numprocs)*(myid+1));
  if (myid == numprocs-1)  ie = n;

  // initialize the vectors (only those parts that reside on this proc)
  a = new double[ie-is+1];
  b = new double[ie-is+1];
#pragma omp parallel for private(i) shared(is,ie,a,b,n)
  for (i=0; i<(ie-is+1); i++) {
    a[i] = 0.001 * (i+is) / n;
    b[i] = 0.001 * (n-i-is) / n;
  }

  // start timer
  stime = MPI_Wtime();

  // perform local portion of dot-product
#pragma omp parallel for private(i) shared(is,ie,a,b) reduction(+:mysum)
  for (i=0; i<(ie-is+1); i++) {
    mysum += a[i]*b[i];
  }

  // root node collects result
  ierr = MPI_Reduce(&mysum, &sum, 1, MPI_DOUBLE, 
		    MPI_SUM, 0, MPI_COMM_WORLD);

  // stop timer
  ftime = MPI_Wtime();
  runtime = ftime - stime;

  // root node outputs compute value and runtime
  if (myid == 0) {
    printf(" dot-product = %.12e\n",sum);
    printf("     runtime = %.12e\n",runtime);
  }

  // free vectors
  delete[] a;
  delete[] b;

  // finalize MPI
  ierr = MPI_Finalize();

} // end main
