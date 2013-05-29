// Daniel R. Reynolds
// SMU HPC Workshop
// 20 May 2013

// Inclusions
#include <stdio.h>
#include <string.h>
#include "advection.h"


// Writes current solution to disk
int output(double *u, double t, int nx, int ny, int noutput) {

  // set output file name
  char outname[100];
  sprintf(outname, "u_sol.%03i.txt", noutput);

  // open output file
  FILE *FID = fopen(outname,"w");
  if (FID == NULL) {
    fprintf(stderr, "output: error opening output file %s\n", outname);
    return 1;
  }

  // output the solution values 
  for (int j=0; j<ny; j++) 
    for (int i=0; i<nx; i++) 
      fprintf(FID, "%.16e\n",u[idx(i,j,nx)]);

  // write current solution time and close the data set
  fprintf(FID, "%.16e\n", t);
  fclose(FID);
    
  // now output a metadata file, containing general run information
  FID = fopen("u_sol_meta.txt","w");
  fprintf(FID, "%i\n", nx);
  fprintf(FID, "%i\n", ny);
  fprintf(FID, "%i\n", noutput);
  fclose(FID);

  return 0;

} // end output

