Postprocessing (Thu. AM)
========================================================

**FILL IN A SHORT INTRO**

Retrieve the set of files for this session either through
:download:`clicking here <code/session7.tgz>` or by copying the
relevant files at the command line:

.. code-block:: bash

   % cp ~dreynolds/SMUHPC_tutorial/session7.tgz .

Unzip this file, and build the executable with ``make``.

Run the executable at the command-line.  You should see a set of
output, ending with the lines:

.. code-block:: text

   writing output file 24, step = 2399, t = 0.48
   writing output file 25, step = 2499, t = 0.5
   total runtime = 2.0000000000000001e-01

List the files in the subdirectory; you should see a new set of files
with the names ``u_sol*.txt``.  These files contain solution data from
the simulation that you just ran, which models the propagation of an
acoustic wave over a periodic, two-dimensional surface, using a coarse
:math:`50\times 50` spatial grid.

In the following sections, we will work on importing these data files
into either a Matlab or Python environment, and then some simple data
analysis.  For the remainder of this session, both Matlab and Python
will be presented, though you may choose to specialize on only your
preferred scripting environment.




Importing data into standard software (Matlab, Python)
--------------------------------------------------------

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
     // Note: we reserve the first set of digits for the MPI process (unused here)
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
     // Note: the two 1's will be used for the MPI process dimensions (unused here)
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
  ``idx()`` macro, defined in ``advection.h``.

* This function is called once every output time; these outputs are
  indexed by the integer ``noutput``, and correspond to the solution
  at the physical time ``t``.

* At each output time, this routine writes two files: 

  * The first is the solution file, that holds the 2D data array,
    printed as one long array with the :math:`x` coordinate the faster
    index.  In this same file, the physical time of the output,
    :math:`t` is also stored.

  * The second is a metadata file, that contains the problem size and
    the total number of outputs that have been written so far in the
    simulation. 


We will first build a Matlab/Python function that will read in the
metadata file.  First. let's view the contents of the metadata file:

.. code-block:: text

   50
   50
   25

Hence we only need to read three numbers in a single column and store
them appropriately.  The relevant Matlab code is in the file
``load_info.m``: 

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
   load u_sol_meta.txt;
   nx = u_sol_meta(1);
   ny = u_sol_meta(2);
   nt = u_sol_meta(3);
   
   return
   % end of function

and the relevant Python code is in the file ``load_info.py``: 

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
             nx,ny,nt = load_info() """
       data = np.loadtxt("u_sol_meta.txt", dtype=int)
       return data

   # end of file


**Continue**



**Mention other data import utilities (e.g. HDF5) used in high-performance computing, and that HDF5 readers already exist in both Matlab and Python.**



Visualization and other processing tasks
--------------------------------------------------------

* Go through visualization scripts in both Matlab and Python.  Add a
  to the scripts to do a bit of analysis before plotting,
  e.g. compute the average value and add it to legend/title.

* Mention other visualization utilities (Mayavi, Visit, Paraview) that can be
  used on data, but qualify this portion by stating that none are installed
  on SMUHPC (yet).


Mayavi
^^^^^^^^^^

Mayavi is a Python plotting package designed primarily for interactive
3D visualization. See:

* `Mayavi Documentation <http://code.enthought.com/projects/mayavi/docs/development/html/mayavi/index.html>`_
* `Mayavi Gallery <http://code.enthought.com/projects/mayavi/docs/development/html/mayavi/auto/examples.html>`_


VisIt
^^^^^^^

`VisIt <https://wci.llnl.gov/codes/visit>`_ is an open source
visualization package being developed at `Lawrence Livermore National
Laboratory <http://www.llnl.gov>`_. It is designed for large-scale
visualization problems (i.e. large data sets, rendered in parallel).
VisIt has a GUI interface, as well as a Python interface for
scripting.  See:

* `VisIt Documentation <https://wci.llnl.gov/codes/visit/doc.html>`_
* `VisIt Gallery <https://wci.llnl.gov/codes/visit/gallery.html>`_
* `VisIt Tutorial <http://www.visitusers.org/index.php?title=Short_Tutorial>`_


ParaView
^^^^^^^^^^^

Like VisIt, `ParaView <http://www.paraview.org>`_ is another open
source package for large-scale visualization developed at the
U.S. Department of Energy National Labs.  It also has both a GUI
interface and a Python interface for scripting.  See:

* `ParaView Documentation
  <http://www.paraview.org/paraview/help/documentation.html>`_ 
* `ParaView Gallery
  <http://www.paraview.org/paraview/project/imagegallery.php>`_ 




Exercise
----------------------

In the set of files for this session, you will find one additional
file that you have not yet used, ``density.txt.gz``.  This is a
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


