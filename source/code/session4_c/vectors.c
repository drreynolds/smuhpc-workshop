/* Daniel R. Reynolds
   SMU HPC Workshop
   20 May 2013 */


/* Inclusions */
#include <stdlib.h>   /* malloc(), free() */
#include <stdio.h>    /* printf() */
#include <time.h>     /* time() */
#include <math.h>     /* pow() */
#include <stdint.h>   /* INT32_MAX */


/* Description: 
      Computes the linear combination z = x + y
   
   Arguments:
      l - integer (input), length of vectors
      m - integer (input), length of vectors
      n - integer (input), length of vectors
      x - double (input), vector
      y - double (input), vector
      z - double (output), vector 
*/
void vector_sum(int l, int m, int n, double ***x, double ***y, double ***z) {

  /* loop over the dimensions to compute the vector sum */
  int i, j, k;
  for (k=0; k<n; k++)
    for (j=0; j<m; j++)
      for (i=0; i<l; i++)
	z[i][j][k] = x[i][j][k] + y[i][j][k];

} /* end vector_sum */




/* Description: 
      Computes the scaled vector z = a*x
   
   Arguments:
      l - integer (input), length of vectors
      m - integer (input), length of vectors
      n - integer (input), length of vectors
      a - double (input), scalar
      x - double (input), vector
      z - double (output), vector
*/
void vector_scale(int l, int m, int n, double a, double ***x, double ***z) {

  /* loop over the dimensions to compute the vector sum */
  int i, j, k;
  for (i=0; i<l; i++)
    for (j=0; j<m; j++)
      for (k=0; k<n; k++)
	z[i][j][k] = a*x[i][j][k];

} /* end vector_scale */




/* Description: 
      Computes the infinity-norm, norm = ||x||_{\infty}
   
   Arguments:
      l - integer (input), length of vector
      m - integer (input), length of vector
      n - integer (input), length of vector
      x - double (input), vector
   Ouptut - double (output), norm of vector
*/
double vector_infnorm(int l, int m, int n, double ***x) {

  /* loop over the dimensions to compute the vector sum */
  int i, j, k;
  double norm=0.0;
  for (i=0; i<l; i++)
    for (j=0; j<m; j++)
      for (k=0; k<n; k++)
	norm = (fabs(x[i][j][k]) > norm) ? fabs(x[i][j][k]) : norm;
  return norm;

} /* end vector_infnorm */




/* Description: 
      Computes the dot-product, prod = x \dot y
   
   Arguments:
      l - integer (input), length of vectors
      m - integer (input), length of vectors
      n - integer (input), length of vectors
      x - double (input), vector
      y - double (input), vector
   Output - double (output), dot-product of vectors
*/
double vector_dotprod(int l, int m, int n, double ***x, double ***y) {

  /* loop over the dimensions to compute the vector sum */
  int i, j, k;
  double prod=0.0;
  for (k=0; k<n; k++)
    for (i=0; i<l; i++)
      for (j=0; j<m; j++)
	prod += x[i][j][k] * y[i][j][k];
  return prod;

} /* end vector_dotprod */
