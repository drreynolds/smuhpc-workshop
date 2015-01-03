# Plotting script for 2D acoustic wave propagation example
# simulation.  This script calls load_info() to determine
# simulation information (grid size and total number of time steps).
# It then calls load_data_2d() to read the solution data from each
# time step, plotting the results (and saving them to disk).
#
# Daniel R. Reynolds
# SMU HPC Workshop
# 20 May 2013

# import the requisite modules
from pylab import *
import numpy as np
from os import *
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
import matplotlib.pyplot as plt
from load_info import load_info
from load_data_2d import load_data_2d

# input general problem information
nx,ny,nt = load_info()

# iterate over time steps
for tstep in range(nt+1):

    # input solution at this time
    t,u = load_data_2d(tstep)

    # set string constants for output plots, current time, mesh size
    pname = 'u_surf.' + repr(tstep).zfill(3) + '.png'
    tstr = repr(round(t,4))
    nxstr = repr(nx)
    nystr = repr(ny)

    # set x and y meshgrid objects
    xspan = np.linspace(0.0, 1.0, nx)
    yspan = np.linspace(0.0, 1.0, ny)
    X,Y = np.meshgrid(xspan,yspan)

    # plot current solution as a surface, and save to disk
    fig = plt.figure(1)
    ax = fig.add_subplot(111, projection='3d')
    ax.plot_surface(X, Y, u, rstride=1, cstride=1, cmap=cm.jet,
                    linewidth=0, antialiased=True, shade=True)
    ax.set_xlabel('y')
    ax.set_ylabel('x')
    title('u(x,y) at t = ' + tstr + ', mesh = ' + nxstr + 'x' + nystr)
    savefig(pname)

    #ion()
    #plt.show()
    #ioff()
    #raw_input('pausing: hit enter to continue')

    plt.close()
    
# end of script
