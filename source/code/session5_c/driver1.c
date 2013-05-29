/* Daniel R. Reynolds
   SMU HPC Workshop
   20 May 2013 */


/* Inclusions */
#include <stdlib.h>   /* malloc(), free() */
#include <stdio.h>    /* printf() */
#include <time.h>     /* clock() */
#include <math.h>     /* pow() */


/* Prototypes */
void vector_sum(int, int, int, double***, double***, double***);
void vector_scale(int, int, int, double, double***, double***);
double vector_infnorm(int, int, int, double***);
double vector_dotprod(int, int, int, double***, double***);



/* Allocates and fills the multi-dimensional vectors x, y, z 
   with random numbers.
   Calls a set of vector routines.
   Writes the total time taken for all vector routines. */
int main(int argc, char* argv[]) {

  /* set problem parameters */
  int l = 100;
  int m = 100;
  int n = 100;

  /* allocate 3D data arrays */
  double*** x = malloc( l * sizeof(double**) );
  double*** y = malloc( l * sizeof(double**) );
  double*** z = malloc( l * sizeof(double**) );
  int i, j, k;
  for (i=0; i<l; i++)  x[i] = malloc( m * sizeof(double*) );
  for (i=0; i<l; i++)  y[i] = malloc( m * sizeof(double*) );
  for (i=0; i<l; i++)  z[i] = malloc( m * sizeof(double*) );
  for (i=0; i<l; i++)  
    for (j=0; j<m; j++) x[i][j] = malloc( n * sizeof(double) );
  for (i=0; i<l; i++)  
    for (j=0; j<m; j++) y[i][j] = malloc( n * sizeof(double) );
  for (i=0; i<l; i++)  
    for (j=0; j<m; j++) z[i][j] = malloc( n * sizeof(double) );

  /* fill in vectors x and y */
  for (i=0; i<l; i++) 
    for (j=0; j<m; j++) 
      for (k=0; k<n; k++) 
	x[i][j][k] = random() / (pow(2.0,31.0) - 1.0);
  for (i=0; i<l; i++)  
    for (j=0; j<m; j++) 
      for (k=0; k<n; k++) 
	y[i][j][k] = random() / (pow(2.0,31.0) - 1.0);

  /* start timer */
  clock_t stime = clock();

  /* call the vector routines a number of times */
  double a;
  for (i=0; i<100; i++) {
    vector_sum(l,m,n,x,y,z);
    vector_scale(l,m,n,2.0,x,z);
    a = vector_infnorm(l,m,n,z);
    a = vector_dotprod(l,m,n,x,y);
  }

  /* stop timer */
  clock_t ftime = clock();
  double runtime = ((double) (ftime - stime))/CLOCKS_PER_SEC;

  /* output solution time */
  printf(" Result from computation = %.12e\n",a);
  printf(" Total run time = %.12e\n",runtime);

  /* free data arrays */
  for (i=0; i<l; i++) {
    for (j=0; j<m; j++) {
      free(x[i][j]);
      free(y[i][j]);
      free(z[i][j]);
    }
    free(x[i]);
    free(y[i]);
    free(z[i]);
  }
  free(x);
  free(y);
  free(z);

} /* end main */

