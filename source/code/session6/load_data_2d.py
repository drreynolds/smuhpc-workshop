# Defines the function load_data_2d().
#
# Daniel R. Reynolds
# SMU HPC Workshop
# 20 May 2013

# import requisite modules
import numpy as np
from load_info import load_info

def load_data_2d(tstep):
    """Returns the solution over the mesh for a given time snapshot.  
       Has calling syntax:
          t,u = load_data_2d(tstep)
       Input: tstep is an integer denoting which time step output to load.
       Outputs: t is the physical time, and u is the 2D array containing 
                the result at the requested time step."""

    # load the parallelism information
    nx,ny,nt = load_info()

    # check that tstep is allowed
    if (tstep < 0 or tstep > nt):
        print 'load_data_2d error: illegal tstep!'
        return

    # determine data file name and load as a long 1-dimensional array
    infile = 'u_sol.' + repr(tstep).zfill(3) + '.txt' 
    data = np.loadtxt(infile, dtype=np.double)

    # separate data array from current time and reshape data into 2D
    u1D = data[:len(data)-1]
    t = data[-1];
    u = np.reshape(u1D, (nx,ny), order='F')

    return [t,u]

# end of file
