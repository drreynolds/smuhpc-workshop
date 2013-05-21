// Daniel R. Reynolds
// SMU HPC Workshop
// 20 May 2013

// simple macro to map a 2D index to a 1D address space
#define idx(i,j,nx)  ((j)*(nx)+(i))

// Prototypes
int initialize(double *u, double *v1, double *v2, double *v3, 
	       double c, double dx, double dy, int nx, int ny);

int output(double *u, double t, int nx, int ny, int noutput);
