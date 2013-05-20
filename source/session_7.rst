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

* Mention other visualization utilities (visit, paraview) that can be
  used on data, but qualify this portion by stating that neither are
  on SMUHPC (yet).


Hands-on example
----------------------

* Give students another executable to run that generates a single
  file (3D?).  Give them a set of tasks to perform on that data in their
  preferred scripting language, including generating a relevant set of
  statistics, and plotting.

