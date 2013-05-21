# Defines the function load_info().
#
# Daniel R. Reynolds
# SMU HPC Workshop
# 20 May 2013

# import requisite modules
import numpy as np

def load_info():
    """Returns the mesh size and total number of output times 
       from the input file 'u_sol_meta.txt'.  Has calling syntax:
          nx,ny,nt = load_info() """
    data = np.loadtxt("u_sol_meta.txt", dtype=int)
    return data

# end of file
