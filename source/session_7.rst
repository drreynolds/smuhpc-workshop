Postprocessing (Thu. AM)
========================================================

**FILL IN A SHORT INTRO**

* Give students an executable to run that will generate a set of files
  (e.g. advection example from class, run in serial).

* Split my advection post-processing script into two: first a function
  that will just do the import, and second a script that will call
  that function and then do the analysis.



Importing data into standard software (Matlab, Python)
--------------------------------------------------------

* Go through data import functions, both in Matlab and Python.

* Mention other data import utilities (e.g. HDF5) used in
  high-performance computing, and that HDF5 readers already exist in
  both Matlab and Python.


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

* Give students another executable to run that generates a single
  file (3D?).  Give them a set of tasks to perform on that data in their
  preferred scripting language, including generating a relevant set of
  statistics, and plotting.

