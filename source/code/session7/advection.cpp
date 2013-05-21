// Daniel R. Reynolds
// SMU HPC Workshop
// 20 May 2013

// Inclusions
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "advection.h"


// Example routine to evolve the first-order 2D wave equations in time
int main(int argc, char* argv[]) {

  // declarations
  int nx, ny, nt, ier;
  double tstop, c, dt_output;

  /* read problem parameters from input file (should be in this order):
        nx - number of nodes in x-direction
	ny - number of nodes in y-direction
	nt - number of time steps
	tstop - final time (will stop at nt or stop, whichever is 1st)
	c - wave speed
	dtoutput - time frequency for outputting solutions */
  FILE *FID = fopen("input.txt","r");
  ier = fscanf(FID,"nx = %i,\n", &nx);
  ier = fscanf(FID,"ny = %i,\n", &ny);
  ier = fscanf(FID,"nt = %i,\n", &nt);
  ier = fscanf(FID,"tstop = %lf,\n", &tstop);
  ier = fscanf(FID,"c = %lf,\n", &c);
  ier = fscanf(FID,"dt_output = %lf,\n", &dt_output);
  fclose(FID);
  printf("\nRunning wave problem:\n");
  printf("  nx = %i,  ny = %i\n", nx, ny);
  printf("  nt = %i,  tstop = %g\n", nt, tstop);
  printf("  c = %g\n",c);
  printf("  dt_output = %g\n",dt_output);

  // allocate arrays
  double *u  = new double[nx*ny];
  double *v1 = new double[nx*ny];
  double *v2 = new double[nx*ny];
  double *v3 = new double[nx*ny];
  
  // set grid spacing
  double dx = 1.0/nx;
  double dy = 1.0/ny;

  // set initial conditions
  if (initialize(u, v1, v2, v3, c, dx, dy, nx, ny) != 0) {
    fprintf(stderr, "Error in initialize routine\n");
    return 1;
  }

  // set initial time, output initial solution
  double t = 0.0;
  double toutput = 0.0;
  int noutput = 0;
  printf("writing output file %i, step = %i, t = %g\n", noutput, 0, t);
  if (output(u, t, nx, ny, noutput) != 0) {
    fprintf(stderr, "Error in output routine\n");
    return 1;
  }

  // start timer
  clock_t stime = clock();

  // set time step
  double dt = (dx < dy) ? dx/c/50.0 : dy/c/50.0;

  // start time stepping 
  int i, j, it;
  double v1_N, v1_S, v1_E, v1_W, v2_E, v2_W, v3_N, v3_S;
  for (it=0; it<nt; it++) {

    // first update v1 to get to half time step
    // get relevant values for this location
    for (i=0; i<nx; i++) {
      for (j=0; j<ny; j++) {

	// access relevant components of v2 and v3
	v2_E = (i == nx-1) ? v2[idx(0,j,nx)] : v2[idx(i+1,j,nx)];
	v2_W = v2[idx(i,j,nx)];
	v3_N = (j == ny-1) ? v3[idx(i,0,nx)] : v3[idx(i,j+1,nx)];
	v3_S = v3[idx(i,j,nx)];

	// update v1
	v1[idx(i,j,nx)] += c*dt/dx*(v2_E - v2_W)
	                 + c*dt/dy*(v3_N - v3_S);

      } // for j
    } // for i

    // next update v2 & v3 to get to full time step
    // get relevant values for this location
    for (i=0; i<nx; i++) {
      for (j=0; j<ny; j++) {

	// access relevant components of v1
	v1_W = (i == 0) ? v1[idx(nx-1,j,nx)] : v1[idx(i-1,j,nx)];
	v1_E = v1[idx(i,j,nx)];
	v1_S = (j == 0) ? v1[idx(i,ny-1,nx)] : v1[idx(i,j-1,nx)];
	v1_N = v1[idx(i,j,nx)];

	// update v2 and v3
	v2[idx(i,j,nx)] += c*dt/dx*(v1_E - v1_W);
	v3[idx(i,j,nx)] += c*dt/dy*(v1_N - v1_S);
	
      } // for j
    } // for i

    // update solution for plotting
    for (i=0; i<nx*ny; i++)  u[i] += dt*v1[i];

    // update time
    t += dt;

    // stop simulation if we've reached tstop
    if (t >= tstop)  break;

    // output solution periodically
    if (fabs(t-toutput-dt_output) <= 1.e-14) {
      toutput = t;
      noutput++;
      printf("writing output file %i, step = %i, t = %g\n", noutput, it, t);
      if (output(u, t, nx, ny, noutput) != 0) {
	fprintf(stderr, "Error in output routine\n");
	return 1;
      }
    }

  } // for it
  
  // stop timer and output runtime
  clock_t ftime = clock();
  double runtime = ((double) (ftime - stime))/CLOCKS_PER_SEC;
  printf(" total runtime = %.16e\n",runtime);

  // free memory
  delete[] u;
  delete[] v1;
  delete[] v2;
  delete[] v3;

  return 0;
} // end main

