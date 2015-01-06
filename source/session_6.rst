:tocdepth: 3


.. _session6:

*****************************************************
Session 6: Postprocessing
*****************************************************




Getting started
================================================

Scientific simulation is worthless unless the results of those
simulations can be analyzed and understood.  Unfortunately, most
classes on scientific computing focus almost exclusively on how to
create the methods for performing the simulations, with little time
(if any) dedicated to the analysis of those results.  While one
session of a workshop is insufficient to fully rectify this situation,
we'll try to get get you started down the right path, by first
focusing on how to input simulation data into more interactive
computing environments, and then how to postprocess and visualize that
data.  While there are many available interactive computing
environments that can be used for these purposes, we'll focus on two
of the most popular options, Matlab and Python.


Retrieve the set of files for this session either through
:download:`clicking here <code/session6.tgz>` or by copying the
relevant files at the command line:

.. code-block:: bash

   $ cp ~dreynolds/SMUHPC_tutorial/session6.tgz .

Unzip this file, enter the resulting directory, and build the
executable with ``make``.

Run the executable at the command-line.

.. code-block:: bash

   $ ./advection.exe

You should see a set of output, ending with lines similar to:

.. code-block:: text

   writing output file 24, step = 2399, t = 0.48
   writing output file 25, step = 2499, t = 0.5
   total runtime = 3.1000000000000000e-01

List the files in the subdirectory; you should see a new set of files
with the names ``u_sol.###.txt``.  These files contain solution data from
the simulation that you just ran, which models the propagation of an
acoustic wave over a periodic, two-dimensional surface, using a coarse
:math:`50\times 50` spatial grid.

In the following sections, we will work on importing these data files
into either a Matlab or Python environment, and then performing some
simple data analysis.  For the remainder of this session, both Matlab
and Python will be presented, though you may choose to specialize in
only your preferred interactive environment.




Importing/exporting data
================================================

Before we can understand how to load data into Matlab or Python, we
must understand how it was written from the program.  Here is the C++
function used to output the two-dimensional data array ``u``:

.. code-block:: c++

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


A few contextual notes about this code to better understand what is
happening (we'll discuss in greater detail during class):

* ``u`` holds a two-dimensional array of size ``nx`` by ``ny``, stored
  in a one-dimensional index space of length ``nx*ny``.  The mapping
  between the 2D physical space and 1D index space is handled by the
  ``idx()`` macro, defined in ``advection.h``, of the form

  .. code-block:: c++

     // simple macro to map a 2D index to a 1D address space
     #define idx(i,j,nx)  ((j)*(nx)+(i))

* This function is called once every output time; these outputs are
  indexed by the integer ``noutput``, and correspond to the solution
  at the physical time ``t``.

* At each output time, this routine writes two files: 

  * The first file is the solution file (``u_sol.###.txt``), that
    holds the 2D data array, printed as one long array with the
    :math:`x` coordinate the faster index.  In this same file, after
    ``u`` is stored, the physical time of the output, ``t`` is
    also stored. 

  * The second file is a metadata file (``u_sol_meta.txt``), that
    contains the problem size and the total number of outputs that
    have been written so far in the simulation. 


We will first build Matlab and Python functions that can read in the
metadata file.  First. let's view the contents of the metadata file:

.. code-block:: text

   $ cat u_sol_meta.txt 
   50
   50
   25

Here the first "50" corresponds to ``nx``, the second "50" corresponds
to ``ny``, and the "25" corresponds to the total number of solutions
that have been output (i.e. the final value for ``noutput``).  

Due to this file's simple structure, we we only need to read three
numbers in a single column and store them appropriately.  The relevant
Matlab code is in the file ``load_info.m``, and relies on the built-in
Matlab function ``load``:

.. index:: 
   pair: load_info(); Matlab

.. code-block:: matlab

   function [nx,ny,nt] = load_info()
   % Usage: [nx,ny,nt] = load_info()
   %
   % Outputs: nx,ny are the grid size, and nt is the total number of
   % time steps that have been output to disk.
   %
   % Daniel R. Reynolds
   % SMU HPC Workshop
   % 20 May 2013

   % input general problem information
   load u_sol_meta.txt;   % reads values from disk, storing in a vector
   nx = u_sol_meta(1);    % unpack vector to name each output
   ny = u_sol_meta(2);
   nt = u_sol_meta(3);
   
   return
   % end of function

The corresponding Python code is in the file ``load_info.py``, which
similarly relies on the built-in Numpy function ``loadtxt``:

.. index:: 
   pair: load_info(); Python

.. code-block:: python

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
             nx,ny,nt = load_info(). """
       
       # reads integer values from disk, storing in a vector
       data = np.loadtxt("u_sol_meta.txt", dtype=int)
       return data     # return entire vector

   # end of file

In both of these scripts, the data in the file ``u_sol_meta.txt`` is
input and converted to a one-dimensional array of numbers.  In the
Matlab code we name these and return each separately.  In the Python
code we merely return the array, leaving unpacking and naming to the
calling routine. 

.. note::

   In the R package for interactive statistical data analysis, the
   corresponding command to Matlab's ``load`` and Python/Numpy's
   ``loadtxt`` is the R function ``read.table``, e.g. 

   .. code-block:: text

      > read.table("u_sol_meta.txt")
        V1
      1 50
      2 50
      3 25

   However, since I do not know how to use R all of the following
   examples will only be in Matlab or Python.  Of course, if you are
   more familiar with R, you are welcome to attempt the remainder of
   this session with that instead of Matlab or Python.

Now that we've seen a simple approach for loading an array into Matlab
and Python, we can move on to functions for reading the larger
``u_sol.###.txt`` files.  As with the above functions, since the data
is output in a single (but very long) column of numbers, we may use
``load`` or ``loadtxt`` to input the data.  Once this data has been
read in, however, we will then split it into the solution component,
``u``, and the current time, ``t``.  Since ``u`` holds a
two-dimensional array, but is stored in a flattened one-dimensional
format, we can use ``reshape`` (the same command in both Matlab and
Python) to convert it from the one-dimensional to the two-dimensional
representation.

First, the Matlab code, ``load_data_2d.m``:

.. index:: 
   pair: load_data_2d(); Matlab

.. code-block:: matlab

   function [t,u] = load_data_2d(tstep)
   % Usage: [t,u] = load_data_2d(tstep)
   %
   % Input: tstep is an integer denoting which time step output to load
   % 
   % Outputs: t is the physical time, and u is the 2D array containing
   % the result at the requested time step 
   %
   % Daniel R. Reynolds
   % SMU HPC Workshop
   % 20 May 2013
   
   % input general problem information
   [nx,ny,nt] = load_info();
   
   % ensure that tstep is allowable
   if (tstep < 0 || tstep > nt) 
      error('load_data_2d error: illegal tstep')
   end
   
   % set filename string and load as a long 1-dimensional array
   infile = sprintf('u_sol.%03i.txt',tstep);
   data = load(infile);
         
   % separate data array from current time, and reshape data into 2D
   u1D = data(1:end-1);
   t = data(end);
   u = reshape(u1D, [nx, ny]);      
  
   return

and here is the corresponding Python code, ``load_data_2d.py``:

.. index:: 
   pair: load_data_2d(); Python

.. code-block:: python

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


How these work:

* These routines take as input an integer, ``tstep``, that corresponds
  to the desired time step output file (the ``###`` in the file
  name). 

* They then call the corresponding ``load_info`` function to find out
  the two-dimensional domain size and the total number of time steps
  written to disk, and perform a quick check to see whether ``tstep``
  is an allowable time step index.
  
  * *Matlab:* The function namespace for Matlab corresponds to all
    ".m" files in the current folder, followed by all built-in
    functions.  So as long as both of the scripts  ``load_info.m`` and
    ``load_data_2d.m`` are in the same folder, the ``load_data_2d``
    function can call the ``load_info`` function automatically.

  * *Python:* Since Python protects the namespace by default, any
    non-built-in Python functions from other files must be loaded before
    they may be executed.  As a result, ``load_data_2d.py`` must import
    the ``load_info`` function from the ``load_data.py`` file before it
    may be used (*note: the ".py" extension for the ``load_data.py``
    file is assumed, and should not be added to the "from" portion of
    the ``import`` command).

* The routine then combines the time step index into a string that
  represents the correct file name (e.g. ``u_sol.006.txt``), and calls
  the relevant ``load`` or ``loadtxt`` routine to input the data.

* The routine then splits the data into the one-dimensional version of
  ``u`` (called ``u1D``) and ``t``, before reshaping ``u1D`` into a
  two-dimensional version of the solution, before returning the values.

.. note:: 

   .. index:: C vs Fortran ordering

   In the Python version, we must specify that the data is
   ordered in "Fortran" style, i.e. that the first index is the fastest
   (as opposed to "C" style, where it is the slowest).  Fortran
   ordering is the default in Matlab, whereas C ordering is the default
   in Python.

These data input routines can be used by Matlab or Python scripts to
first read in the data, before either performing analysis or plotting.

A few general comments on the above approach:

* By storing the values as raw text, these files are larger than
  necessary.  In this example the files are not too large (~58 KB
  each), but in more realistic simulations it would be preferred to
  store data in a more compressed format.  Two approaches for this are
  to:

  a. Zip each file after it is written to disk, through using library
     routines (e.g. ``libz``, ``libzip``, ``libgzip``), and the
     uncompress them when reading.  If the file is compressed with
     ``gzip``, Numpy's ``loadtxt`` routine will automatically unzip as
     it reads.

  b. Write the data to disk in binary format.

* Performance-wise, it is best to write out data in the
  order in which it is stored in memory during the simulation.  In
  this example, the data is stored with the ``x`` index being the
  fastest, hence the "Fortran" ordering of the data file.

.. index:: HDF5, netCDF

High-quality alternatives to such manual I/O approaches abound.  Two
popular I/O libraries in high-performance computing are `HDF5
<http://www.hdfgroup.org/HDF5/>`_ and `netCDF
<http://www.unidata.ucar.edu/software/netcdf/>`_.  Both of these
libraries have the following benefits over doing things manually:

* Natively output in binary format for smaller file sizes.

* Allow you to output descriptive information in addition to just the
  data (e.g. units of each field, version of the code).

* Allow you to output multiple items to the same file (e.g. density,
  momentum, energy).

* Support parallel computing, allowing many MPI tasks to write to the
  same file.

* Professional visualization utilities typically have readers built-in
  for these file types.

* Have data input utilities in both Matlab and Python:

  * Matlab/HDF5: ``h5create``, ``h5disp``, ``h5info``, ``h5read``,
    ``h5readatt``, ``h5write``, ``h5writeatt``.  All are built into
    Matlab (see `this Matlab help page
    <http://www.mathworks.com/help/matlab/high-level-functions.html>`_
    for information).

  * Matlab/netCDF: although not built into Matlab, there are
    contributed versions of netCDF readers on `Matlab Central
    <http://www.mathworks.com/matlabcentral/fileexchange/15177-netcdf-reader>`_. 

  * Python/HDF5: the Python module ``h5py`` contains a full Pythonic
    interface to the HDF5 data format (`click here for more
    information on h5py <https://code.google.com/p/h5py/>`_).

  * Python/netCDF: the Python module ``netcdf4-python`` contains
    interfaces to the majority of netCDF (`click here for more
    information on netcdf4-python
    <https://code.google.com/p/netcdf4-python/>`_). 

* Last but not least: someone else writes and debugs the code,
  allowing you to focus on your work instead of spending your time
  fiddling with I/O.



Post-processing 
================================================

We will now use the above data input routines to do some
post-processing of these simulated results.  For this example, we'll
create surface plots of the field ``u``, one for each time step, and
write them to the disk.  Of course, once the data is available in our
preferred scripting environment (Matlab, Python, etc.), we can easily
perform additional data analysis, as will be included in the hands-on
exercise at the end of this session.

As we did earlier, we'll first show the code and then go through the
steps.  You may focus on your preferred computing environment, since
both scripts are functionally equivalent.

First the Matlab code, ``plot_solution.m``:

.. index:: 
   pair: plot_solution(); Matlab

.. code-block:: matlab

   % Plotting script for 2D acoustic wave propagation example
   % simulation.  This script inputs the file u_sol_meta.txt to determine
   % simulation information (grid size and total number of time steps).
   % It then calls load_data_2d() to read the solution data from each
   % time step, plotting the results (and saving them to disk).
   %
   % Daniel R. Reynolds
   % SMU HPC Workshop
   % 20 May 2013
   clear
   
   % input general problem information
   [nx,ny,nt] = load_info();
   
   % loop over time steps
   for tstep = 0:nt
   
      % load time step data
      [t,u] = load_data_2d(tstep);
   
      % plot current solution (and save to disk)
      xvals = linspace(0,1,nx);
      yvals = linspace(0,1,ny);
      h = surf(yvals,xvals,u);
      shading flat
      view([50 44])
      axis([0, 1, 0, 1, -1, 1])
      xlabel('x','FontSize',14), ylabel('y','FontSize',14)
      title(sprintf('u(x,y) at t = %g, mesh = %ix%i',t,nx,ny),'FontSize',14)
      pfile = sprintf('u_surf.%03i.png',tstep);
      saveas(h,pfile);
      
      %disp('pausing: hit enter to continue')
      %pause
   end

and then the Python code, ``plot_solution.py``:

.. index:: 
   pair: plot_solution(); Python

.. code-block:: python

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


How these work:

* These first call ``load_info`` to determine the simulation grid size
  and total number of time steps that have been output to disk.

* These then loop over each time step, and:

  * Call ``load_data_2d`` to read the simulation time and solution
    array. 

  * Create arrays for the :math:`x` and :math:`y` coordinates of each
    solution data point.

  * Plot ``u`` at that time step as a 2D surface plot, setting the
    plot labels and title appropriately.

  * Save the plot to disk in files of the form ``u_surf.###.png``.

  * (Commented out) Pause the loop until the user hits "enter".


Run this code as usual, using either Matlab,

.. code-block:: bash

   $ module load matlab
   $ matlab -r plot_solution

or Python,

.. code-block:: bash

   $ module load gcc
   $ module load python
   $ python ./plot_solution.py

You should then see a set of ``.png`` images in the directory:

.. code-block:: bash

   $ ls
   Makefile          plot_solution.m   u_sol.012.txt  u_sol_meta.txt  u_surf.013.png
   advection.cpp     plot_solution.py  u_sol.013.txt  u_surf.000.png  u_surf.014.png
   advection.exe     u_sol.000.txt     u_sol.014.txt  u_surf.001.png  u_surf.015.png
   advection.h       u_sol.001.txt     u_sol.015.txt  u_surf.002.png  u_surf.016.png
   density.txt       u_sol.002.txt     u_sol.016.txt  u_surf.003.png  u_surf.017.png
   initialize.cpp    u_sol.003.txt     u_sol.017.txt  u_surf.004.png  u_surf.018.png
   input.txt         u_sol.004.txt     u_sol.018.txt  u_surf.005.png  u_surf.019.png
   load_data_2d.m    u_sol.005.txt     u_sol.019.txt  u_surf.006.png  u_surf.020.png
   load_data_2d.py   u_sol.006.txt     u_sol.020.txt  u_surf.007.png  u_surf.021.png
   load_data_2d.pyc  u_sol.007.txt     u_sol.021.txt  u_surf.008.png  u_surf.022.png
   load_info.m       u_sol.008.txt     u_sol.022.txt  u_surf.009.png  u_surf.023.png
   load_info.py      u_sol.009.txt     u_sol.023.txt  u_surf.010.png  u_surf.024.png
   load_info.pyc     u_sol.010.txt     u_sol.024.txt  u_surf.011.png  u_surf.025.png
   output.cpp        u_sol.011.txt     u_sol.025.txt  u_surf.012.png



You can view these plots on SMUHPC with the command, e.g.

.. code-block:: bash

   $ display u_surf.009.png

Alternately, you can open them all and cycle through them by
right-clicking and selecting "Next":

.. code-block:: bash

   $ display u_surf.*.png





Advanced visualization
================================================


A few difficulties with using either Matlab or Python for data
visualization include:

* Difficulty dealing with three-dimensional plotting: while slices and
  projections are simple, 3D data sets require much more interactive
  visualization, including isocontour surface plots, moving slices,
  rotating, etc.

* Difficulty dealing with data output from parallel simulations: you
  need to read in each processor's data file and glue them together
  manually, and such in-core processing is impossible when the data
  sets grow too large.

As a result, there are a variety of high-quality visualization
packages that are designed for interactive 3D visualization, as
discussed below.  None of these are installed on SMUHPC at present,
though all are freely-available and open-source, so if you need/want
one you should make a request to the SMUHPC system administrators.


.. index:: Mayavi

Mayavi
--------------------------------------------------

Mayavi is a Python plotting package designed primarily for interactive
3D visualization. See:

* `Mayavi Documentation <http://code.enthought.com/projects/mayavi/docs/development/html/mayavi/index.html>`_
* `Mayavi Gallery <http://code.enthought.com/projects/mayavi/docs/development/html/mayavi/auto/examples.html>`_


.. index:: VisIt

VisIt
--------------------------------------------------

`VisIt <https://wci.llnl.gov/codes/visit>`_ is an open source
visualization package being developed at `Lawrence Livermore National
Laboratory <http://www.llnl.gov>`_. It is designed for large-scale
visualization problems (i.e. large data sets, rendered in parallel).
VisIt has a GUI interface, as well as a Python interface for
scripting.  See:

* `VisIt Documentation <https://wci.llnl.gov/codes/visit/doc.html>`_
* `VisIt Gallery <https://wci.llnl.gov/codes/visit/gallery.html>`_
* `VisIt Tutorial <http://www.visitusers.org/index.php?title=Short_Tutorial>`_


.. index:: ParaView

ParaView
--------------------------------------------------

Like VisIt, `ParaView <http://www.paraview.org>`_ is another open
source package for large-scale visualization developed at the
U.S. Department of Energy National Labs.  It also has both a GUI
interface and a Python interface for scripting.  See:

* `ParaView Documentation
  <http://www.paraview.org/paraview/help/documentation.html>`_ 
* `ParaView Gallery
  <http://www.paraview.org/paraview/project/imagegallery.php>`_ 




Exercise
================================================

In the set of files for this session, you will find one additional
file that you have not yet used, ``density.txt``.  This is a
snapshot of a three-dimensional cosmological density field at a
redshift of approximately :math:`z = 9`.  Unlike the previous
example, this file contains only the data field itself, with no
auxiliary metadata.  Like the previous example, this data is stored in
a single column, with :math:`x` being the fastest index and :math:`z`
the slowest.  The three-dimensional grid is uniform in each direction,
(i.e. it has size :math:`N\times N\times N`) so the total number of
lines in the file should equal :math:`N^3`. 

Create a Matlab or Python script that accomplishes the following
tasks:

1. Determine the maximum density over the domain, and where it occurs.

2. Determine the minimum density over the domain, and where it occurs.

3. Determine the average density over the domain.

4. Generate the following two-dimensional plots, and save each to disk:
 
   * Slice through the center of the domain parallel to the
     :math:`xy` plane. 

   * Slice through the center of the domain parallel to the
     :math:`xz` plane. 

   * Slice through the center of the domain parallel to the
     :math:`yz` plane. 

   * Plot a projection of the density onto the :math:`xy` plane
     (i.e. add all entries in the :math:`z` direction to collapse the
     3D set to 2D).

   * Plot a projection of the density onto the :math:`xz` plane.

   * Plot a projection of the density onto the :math:`yz` plane.


*Hints*: 

* If you plot the :math:`log` of the density, you will get more
  interesting pictures.  In both Matlab and Python/Numpy, this is
  easily computed using whole-array operations, e.g. ``logd = log(d)``.

* Both Matlab and Python allow array slicing to extract a plane from
  a 3D data set, e.g. 
   
  * Matlab: ``dslice = squeeze(d(:,:,2))`` -- here, the ``squeeze``
    command may be used to eliminate the now-trivial 3rd dimension
    that has length 1.

  * Python/Numpy: ``dslice = d[:][:][2]``  or even ``dslice = d[:,:,2]``
    (both forms of syntax are equivalent for Numpy arrays).

* Both Matlab and Python/Numpy have a ``sum`` command that will add
  all values of a multi-dimensional array along a specified
  dimension.  Read their documentation to see how this works (it will
  help with the average value and with the projection plots).

* Both Matlab and Python/Numpy have ``max`` and ``min`` commands that
  can be applied to array-valued data.  Read their documentation to
  see how this works (it will help with the maximum and minimum
  values). 








.. raw:: html
   :file: counter.html

