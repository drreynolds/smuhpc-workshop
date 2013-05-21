# Defines the functions load_info() and load_data_2d().
#
# Daniel R. Reynolds
# SMU HPC Workshop
# 20 May 2013

# imports
import numpy as np

# set up some helper functions

###
def load_info():
    """Returns the mesh size and total number of output times 
       from the input file 'u_sol_meta.txt'.  Has calling syntax:
          nx,ny,nt = load_info() """
    data = np.loadtxt("u_sol_meta.txt", dtype=int)
    return data

###
def load_data_2d(tstep):
    """Returns the solution over the mesh for a given time snapshot.  
       Has calling syntax:
          t,u = load_snapshot(tstep)
       Input: tstep is an integer denoting which time step output to load.
       Outputs: t is the physical time, and u is the 2D array containing 
                the result at the requested time step."""

    # load the parallelism information
    nx,ny,nt = load_info()

    # check that tstep is allowed
    if (tstep < 0 or tstep > nt):
        print 'load_data_2d error: illegal tstep!'
        return

    # determine data file name
    infile = 'u_sol.' + repr(tstep).zfill(3) + '.txt' 

    # load input file as a long 1-dimensional array
    data = np.loadtxt(infile, dtype=np.double)

    # separate data array from current time
    u1D = data[:len(data)-1]
    t = data[-1];

    # reshape data into 2D mesh
    u = np.reshape(u1D, (nx,ny), order='F')

    # return with results
    return [t,u]


# end of file
