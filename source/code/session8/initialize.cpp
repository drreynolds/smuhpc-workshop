// Daniel R. Reynolds
// SMU HPC Workshop
// 20 May 2013

// Inclusions 
#include <stdlib.h>
#include <math.h>
#include "advection.h"

// Sets the initial conditions into u, v1, v2, v3
int initialize(double *u, double *v1, double *v2, double *v3, 
	       double c, double dx, double dy, int nx, int ny) {

  // set initial condition for solution and derivatives
  double fac = -100.0;
  double y_c, y_f, x_c, x_f;
  for (int j=0; j<ny; j++) {

    // y locations
    y_c = dy*(0.5 + j);
    y_f = dy*j;

    for (int i=0; i<nx; i++) {

      // x locations
      x_c = dx*(0.5 + i);
      x_f = dx*i;

      /* set initial conditions on u_x, u_y [gaussian blob]
            u(0,x,y) = exp(fac*((x-1/3)^2+(y-1/2)^2))
            c*u_x(0,x,y) = 2*fac*c*(x-1/3)*exp(fac*((x-1/3)^2+(y-1/2)^2))
            c*u_y(0,x,y) = 2*fac*c*(y-1/2)*exp(fac*((x-1/3)^2+(y-1/2)^2)) */
      u[ idx(i,j,nx)] = exp( fac*( pow(x_c-1.0/3.0,2.0) + pow(y_c-0.5,2.0) ) );
      v1[idx(i,j,nx)] = 0.0;
      v2[idx(i,j,nx)] = 2.0*fac*c*(x_f-1.0/3.0) *
  	         exp( fac*( pow(x_f-1.0/3.0,2.0) + pow(y_c-0.5,2.0) ) );
      v3[idx(i,j,nx)] = 2.0*fac*c*(y_f-0.5) * 
   	         exp( fac*( pow(x_c-1.0/3.0,2.0) + pow(y_f-0.5,2.0) ) );

    } // for j
  } // for i

  return 0;
} // end initialize

