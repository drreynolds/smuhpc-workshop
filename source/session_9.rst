.. _session9:

Session 9: Parallel Computing on SMUHPC
========================================================

*Instructor: Dan Reynolds*

*Assistants: Amit Kumar, Merlin Wilkerson, Doug Tucker*


SMU HPC network
--------------------------------------------------------

**FILL THIS IN**

Outline: 

* gigabit ethernet network connecting disks, login nodes and batch
  workers.

* Quadrics infiniband network for older parallel nodes.

* QLogic infiniband network for newer parallel nodes.

(Do we have any numbers to mention the performance differences of
these networks?)



Compiling and running parallel programs
--------------------------------------------------------

As seen in :ref:`session6`, the SMU HPC cluster is actually currently
comprised of four separate computing clusters: two batch processing
clusters for high-throughput computing, and two smaller parallel
computing clusters.  For future reference, we'll name these:

* *batch1* -- the 107 node,  8 core/node cluster,

* *batch2* -- the 56 node, 12 core/node cluster,

* *parallel1* -- the 16 node, 8 core/node cluster, and 

* *parallel2* -- the 32 node, 12 core/node cluster. 

Each of these components of the cluster is capable of running
parallel programs, albeit of different types and sizes.  As will be
elaborated on below, we differentiate between shared-memory parallel
(SMP) programs, typically enabled by OpenMP, and distributed-memory
parallel (DMP) programs, typically enabled by MPI.  In the table
below, we outline the maximum numbers of cores that may be used in
each paradigm by each portion of the cluster.

|

.. table:: Maximum job sizes that can be run of each type.

   ============  ===============  =================
   Cluster name  Max SMP size     Max DMP size
   ============  ===============  =================
   batch1        8                8
   batch2 	 12 		  12
   parallel1 	 8 		  128
   parallel2 	 12 		  384
   ============  ===============  =================

|

Additionally, we have the choice between GNU and PGI compilers when
compiling our codes for these different execution environments.  In
the tutorial below, we differentiate between different
compiler/cluster/parallelism approaches, providing separate
instructions for each approach.  You need not use all of these
approaches, but they are provided for future reference.

The following links will take you directly to the subsections that
detail each compilation/execution approach:

* Shared-memory programs using OpenMP:

  * :ref:`Compiling with GNU <session9-compiling_OpenMP_GNU>`

  * :ref:`Compiling with PGI <session9-compiling_OpenMP_PGI>`

  * :ref:`Running at the command line <session9-running_OpenMP_commandline>`

  * :ref:`Running on batch1 <session9-running_OpenMP_batch1>`

  * :ref:`Running on batch2 <session9-running_OpenMP_batch2>`

  * :ref:`Running on parallel1 <session9-running_OpenMP_parallel1>`

  * :ref:`Running on parallel2 <session9-running_OpenMP_parallel2>`

* Distributed memory programs using MPI:

  * :ref:`MPI compiler wrappers <session9-compiling_MPI_programs>`

  * The batch1 and batch2 clusters:

    * :ref:`Compiling with GNU <session9-compiling_MPI_GNU_batch>`

    * :ref:`Running with GNU <session9-running_MPI_GNU_batch>`

    * :ref:`Compiling with PGI <session9-compiling_MPI_PGI_batch>`

    * :ref:`Running with PGI <session9-running_MPI_PGI_batch>`
 
  * The parallel1 cluster:

    * :ref:`Compiling with GNU <session9-compiling_MPI_GNU_parallel1>`

    * :ref:`Running with GNU <session9-running_MPI_GNU_parallel1>`

    * :ref:`Compiling with PGI <session9-compiling_MPI_PGI_parallel1>`

    * :ref:`Running with PGI <session9-running_MPI_PGI_parallel1>`

  * The parallel2 cluster:

    * :ref:`Compiling with GNU <session9-compiling_MPI_GNU_parallel2>`

    * :ref:`Running with GNU <session9-running_MPI_GNU_parallel2>`

    * :ref:`Compiling with PGI <session9-compiling_MPI_PGI_parallel2>`

    * :ref:`Running with PGI <session9-running_MPI_PGI_parallel2>`





OpenMP
^^^^^^^^^^

First, retrieve the files for this portion of the session by clicking
:download:`this link <code/session9_OpenMP.tgz>` or by copying them
on SMUHPC at the command line:

.. code-block:: bash

   $ cp ~dreynolds/SMUHPC_workshop/session9_OpenMP.tgz .

Unpack these files as usual

.. code-block:: bash

   $ tar -zxf session9_OpenMP.tgz

In the resulting directory, you will find a number of files, including
``Makefile``, ``driver.cpp`` and ``vectors.cpp``.  


OpenMP is implemented as an extension to existing programming
languages, and is available for programs written in C, C++, Fortran77
and Fortran90.  These OpenMP extensions are enabled at the compiler
level, with most compilers supporting OpenMP and others not.  In all
cases of which I am aware, OpenMP is enabled through supplying a flag
to the relevant compiler denoting that you wish for it to allow the
OpenMP extensions to the existing language.  The various compiler
flags for well-known compilers include:

* GNU: ``-fopenmp``

* PGI: ``-mp``

* Intel: ``-openmp``

* IBM: ``-qsmp``

* Oracle: ``-xopenmp``

* Absoft: ``-openmp``

* Cray: (on by default)

* NAG: ``-openmp``




.. _session9-compiling_OpenMP_GNU:

Compiling OpenMP code with the GNU compilers
"""""""""""""""""""""""""""""""""""""""""""""

You can compile the executable ``driver.exe`` with the GNU compiler and
OpenMP using the command 

.. code-block:: bash

   $ g++ -fopenmp driver.cpp vectors.cpp -lm -o driver.exe

The compiler option ``-fopenmp`` is the same, no matter which GNU
compiler you are using (``gcc``, ``gfortran``, etc.)


.. _session9-compiling_OpenMP_PGI:

Compiling OpenMP code with the PGI compilers
"""""""""""""""""""""""""""""""""""""""""""""

Assuming that you have already loaded the PGI module, you can compile
the executable ``driver.exe`` with the PGI compiler and OpenMP using
the command  

.. code-block:: bash

   $ pgc++ -mp driver.cpp vectors.cpp -lm -o driver.exe

The compiler option ``-mp`` is the same, no matter which PGI
compiler you are using (``pgcc``, ``pgfortran``, etc.)


.. _session9-running_OpenMP_commandline:

Running OpenMP programs at the command line
"""""""""""""""""""""""""""""""""""""""""""""

Run the executable ``driver.exe`` from the command line:

.. code-block:: bash

   $ ./driver.exe

In fact, this executable did not run using more than one thread, since
the default behavior of OpenMP programs on SMU HPC is to only use a
single thread (**verify**). 

To change the number of threads used by our program, we must adjust
the ``OMP_NUM_THREADS`` environment variable. First, verify that this is
set to the default value of 1 (or it may be blank): 

.. code-block:: bash

   $ echo $OMP_NUM_THREADS

Recalling from session 2 of the workshop, the method for re-setting
this environment variable will depend on our login shell.  For CSH/TCSH
users, 
.. code-block:: bash

   $ setenv OMP_NUM_THREADS 2

will adjust this variable to 2; the same may be accomplished in
BASH/SH/KSH users with the command 

.. code-block:: bash

   $ export OMP_NUM_THREADS=2

Re-run ``driver.exe`` first using 1 and then using 2 OpenMP
threads.  Notice the speedup when running with multiple threads. 



.. _session9-running_OpenMP_batch1:

Running OpenMP jobs on batch1
"""""""""""""""""""""""""""""""""""""

To run OpenMP-enabled code on the batch1 cluster, the steps are identical
to those required for requesting an entire compute node, except that
now we must additionally specify the environment variable
``OMP_NUM_THREADS``.  It is recommended that this variable be supplied
as one of the entries in the **environment** option to condor.  

Additionally, we should request that we wish to run on a "wnode" or
"cnode", since those comprise the batch1 portion of the SMU HPC cluster.

For example, if we set our condor job submission file to either

.. code-block:: text

   universe              = vanilla
   getenv                = true
   log                   = OMPtest.log
   error                 = OMPtest.err
   output                = OMPtest.out
   executable            = driver.exe
   environment           = OMP_NUM_THREADS=7
   Requirements          = regexp("wnode", Machine)
   Requirements          = CAN_RUN_WHOLE_MACHINE
   +RequiresWholeMachine = True
   queue

or to
  
.. code-block:: text

   universe              = vanilla
   getenv                = true
   log                   = OMPtest.log
   error                 = OMPtest.err
   output                = OMPtest.out
   executable            = driver.exe
   environment           = OMP_NUM_THREADS=7
   Requirements          = regexp("cnode", Machine)
   Requirements          = CAN_RUN_WHOLE_MACHINE
   +RequiresWholeMachine = True
   queue
  
it will signify to condor that we wish to launch ``driver.exe`` on a
single dedicated node, and that once the job is launched, we will use
7 of the hardware threads on that node (recall, batch1 has 8 cores per
node, so this would entail one core remaining idle).

This job file should be launched from either ``smuhpc.smu.edu`` or
``smuhpc2.smu.edu``. 



.. _session9-running_OpenMP_batch2:

Running OpenMP jobs on batch2
"""""""""""""""""""""""""""""""""""""

To run OpenMP-enabled code on the batch2 cluster, the steps are identical
to those required for requesting an entire compute node, except that
now we must additionally specify the environment variable
``OMP_NUM_THREADS``.  It is recommended that this variable be supplied
as one of the entries in the **environment** option to condor.  

Additionally, we should request that we wish to run on a "cwnode",
since those comprise the batch2 portion of the SMU HPC cluster. 

For example, if we set our condor job submission file to

.. code-block:: text

   universe              = vanilla
   getenv                = true
   log                   = OMPtest.log
   error                 = OMPtest.err
   output                = OMPtest.out
   executable            = driver.exe
   environment           = OMP_NUM_THREADS=11
   Requirements          = regexp("cnode", Machine)
   Requirements          = CAN_RUN_WHOLE_MACHINE
   +RequiresWholeMachine = True
   queue

it will signify to condor that we wish to launch ``driver.exe`` on a
single dedicated node, and that once the job is launched, we will use
11 of the hardware threads on that node (recall, batch2 has 12 cores per
node, so this would entail one core remaining idle).

This job file should be launched from either ``smuhpc.smu.edu`` or
``smuhpc2.smu.edu``. 



.. _session9-running_OpenMP_parallel1:

Running OpenMP jobs on parallel1
"""""""""""""""""""""""""""""""""""""

To run OpenMP-enabled code on the parallel1 cluster, the steps are
identical to those required for requesting an entire compute node,
except for the following changes:

* We must additionally specify the environment variable
  ``OMP_NUM_THREADS``.  It is recommended that this variable be
  supplied as one of the entries in the **environment** 
  option to condor.

* The job *must be launched from* ``smuhpc4.smu.edu``, since that
  manages the parallel clusters.

* We should specify that we only want one node via setting the
  **machine_count** option to 1.

* The **universe** must be set to ``parallel``, indicating that it
  should be run on one of the parallel clusters.

* We should specify that we wish to run on a "inode", since those
  comprise the parallel1 cluster.

For example, if we set our condor job submission file to

.. code-block:: text

   universe              = parallel
   getenv                = true
   log                   = OMPtest.log
   error                 = OMPtest.err
   output                = OMPtest.out
   executable            = driver.exe
   environment           = OMP_NUM_THREADS=5
   machine_count         = 1
   Requirements          = regexp("inode", Machine)
   Requirements          = CAN_RUN_WHOLE_MACHINE
   +RequiresWholeMachine = True
   queue
  
it will signify to condor that we wish to launch ``driver.exe`` on a
single dedicated node, and that once the job is launched, we will use
5 of the hardware threads on that node (recall, parallel1 has 8 cores per
node, so this would entail 3 cores remaining idle).

Again, this job file should be launched from ``smuhpc4.smu.edu``. 



.. _session9-running_OpenMP_parallel2:

Running OpenMP jobs on parallel2
"""""""""""""""""""""""""""""""""""""

To run OpenMP-enabled code on the parallel2 cluster, the steps are
identical to those required for requesting an entire compute node,
except for the following changes:

* We must additionally specify the environment variable
  ``OMP_NUM_THREADS``.  It is recommended that this variable be
  supplied as one of the entries in the **environment** 
  option to condor.

* The job *must be launched from* ``smuhpc4.smu.edu``, since that
  manages the parallel clusters.

* We should specify that we only want one node via setting the
  **machine_count** option to 1.

* The **universe** must be set to ``parallel``, indicating that it
  should be run on one of the parallel clusters.

* We should specify that we wish to run on a "iwnode", since those
  comprise the parallel2 cluster.

For example, if we set our condor job submission file to

.. code-block:: text

   universe              = parallel
   getenv                = true
   log                   = OMPtest.log
   error                 = OMPtest.err
   output                = OMPtest.out
   executable            = driver.exe
   environment           = OMP_NUM_THREADS=10
   machine_count         = 1
   Requirements          = regexp("iwnode", Machine)
   Requirements          = CAN_RUN_WHOLE_MACHINE
   +RequiresWholeMachine = True
   queue
  
it will signify to condor that we wish to launch ``driver.exe`` on a
single dedicated node, and that once the job is launched, we will use
10 of the hardware threads on that node (recall, parallel2 has 12 cores per
node, so this would entail 2 cores remaining idle).

Again, this job file should be launched from ``smuhpc4.smu.edu``. 


OpenMP exercise
"""""""""""""""""

Compile the program ``driver.exe`` using the PGI compiler with OpenMP
enabled.

Create a single condor submission script that will run the program
``driver.exe`` using 1, 2, 3, ..., 12 OpenMP threads on the batch2
portion of the cluster.  Recall from session 6
(:ref:`running_multiple_condor_jobs`), that a single script may launch
multiple jobs by including multiple **queue** statements.

Launch these jobs, and when they have completed, determine the *strong
scaling performance* of this code (defined in session 8,
:ref:`parallel_computing_metrics`).  How well does the program
perform?  Is there a maximum number of threads where, beyond which,
additional resources no longer improve the speed?





MPI
^^^^^^^^^^

Retrieve the files for this portion of the session by clicking
:download:`this link <code/session9_MPI.tgz>` or by copying them
on SMUHPC at the command line:

.. code-block:: bash

   $ cp ~dreynolds/SMUHPC_workshop/session9_MPI.tgz .

Unpack these files as usual,

.. code-block:: bash

   $ tar -zxf session9_MPI.tgz


Unlike OpenMP, MPI is implemented as a standalone library that may be
called by programs wishing to perform message passing to perform a
distributed memory parallel computation.  Typically written in C (for
maximum portability), MPI libraries typically include interfaces for
programs written in C, C++, Fortran77, Fortran90 and Python.

Moreover, since MPI is a library, it does not require any specific
compiler extensions to construct a MPI-enabled parallel program,
although it is typical for highly optimized versions of the MPI
library that you use the same compiler for your program that was used
to construct the library.


.. _session9-compiling_MPI_programs:

Compiling MPI code (MPI wrapper scripts)
""""""""""""""""""""""""""""""""""""""""""""

Typically, in order to compile a program to use a library, a few key
items must be known about how the library was installed on the
system:

* If the library includes header files (C, C++) or modules (F90),
  where are they located (e.g. LIBNAME)?  This location is important
  because when compiling our own codes, we must typically tell the
  compiler where to look for these "include files" using the ``-I``
  argument.

* If the library was installed in a non-default location, where is
  the resulting ".a" file (static library) or ".so" file (shared
  library) located?  Again, this location is important
  because when linking our own codes, we must typically tell the
  compiler where to look for these library files using the ``-L``
  argument.

For example, the PGI-compiled MPI library, MPICH2 version 1.3.2, is
installed on SMU HPC in the directory ``/grid/software/mpich2-1.3.2``,
with header files located in ``/grid/software/mpich2-1.3.2/include``
and library files located in  ``/grid/software/mpich2-1.3.2/lib``.
Finally, because I'm familiar with this package, I know that to
compile an executable I must link against the files ``libmpich.a`` and
``libmpl.a`` in this library directory location.  

As a result, we could compile the executable ``driver.exe`` with the
commands 

  .. code-block:: bash

     $ pgc++ driver.cpp -I/grid/software/mpich2-1.3.2/include \
       -L/grid/software/mpich2-1.3.2/lib -lmpich -lmpl -lm -o driver.exe


Clearly, specifying the specific instructions for including and
linking to an MPI library is not always easy: 

* You must know where all of the relevant libraries are installed on
  each computer. 

* You must know which specific library files are required for
  compiling a given program. 

* Sometimes, you must even know which order you need to specify these
  specific library files in the linking line. 

Thankfully, MPI library writers typically include MPI *wrapper scripts*
to do most of this work for you. Such scripts are written to encode
all of the above information that is required to use MPI with a given
compiler on a specific system. 

Depending on your programming language and the specific MPI
implementation, these wrapper scripts can have different names. The
typical names for these MPI wrapper scripts are below: 

* C++: ``mpicxx``, ``mpiCC``, ``mpic++`` or ``openmpicxx``

* C: ``mpicc`` or ``openmpicc``

* Fortran 90/95: ``mpif90`` or ``openmpif90``

* Fortran 77: ``mpif77`` or ``openmpif77`` (typically, the Fortran
  90/95 wrapper will also work for these)

In order to use these wrapper scripts on SMU HPC, we must first load
the correct module environment.  We'll discuss each of these in the
appropriate context within the following subsections, that focus on
the myriad compilers and clusters we wish to use.



.. _session9-compiling_MPI_GNU_batch:

Compiling MPI code with the GNU compilers for batch1 and batch2
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

Compilation can occur on any SMU HPC login node.

First, load the ``mpich2/1.1.1/gcc`` module,

.. code-block:: bash

   $ module load mpich2/1.1.1/gcc

Second, compile your executable using one of the MPI wrapper scripts:
``mpicc``, ``mpicxx``, ``mpif90`` or ``mpif77``.  For example, we may
compile the example executable as

.. code-block:: bash

   $ mpicxx driver.cpp -lm -o driver_GNU_batch.exe

Note: since the MPI libraries vary based on where we wish to run and
on which compilers we use, I recommend naming the executable
appropriately to distinguish it from other compilation approaches.  Of
course, this is not required.



.. _session9-running_MPI_GNU_batch:

Running MPI code with the GNU compilers on batch1 and batch2
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

You must launch the job from ``smuhpc.smu.edu`` or
``smuhpc2.smu.edu``.

**FILL THIS IN**



.. _session9-compiling_MPI_PGI_batch:

Compiling MPI code with the PGI compilers for batch1 and batch2
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

Compilation can occur on any SMU HPC login node.

First, load the ``mpich2/1.3.2/pgi`` module,

.. code-block:: bash

   $ module load mpich2/1.3.2/pgi

Second, compile your executable using one of the MPI wrapper scripts:
``mpicc``, ``mpicxx``, ``mpif90`` or ``mpif77``.  For example, we may
compile the example executable as

.. code-block:: bash

   $ mpicxx driver.cpp -lm -o driver_PGI_batch.exe

Note: since the MPI libraries vary based on where we wish to run and
on which compilers we use, I recommend naming the executable
appropriately to distinguish it from other compilation approaches.  Of
course, this is not required.



.. _session9-running_MPI_PGI_batch:

Running MPI code with the PGI compilers on batch1 and batch2
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

You must launch the job from ``smuhpc.smu.edu`` or
``smuhpc2.smu.edu``.

**FILL THIS IN**



.. _session9-compiling_MPI_GNU_parallel1:

Compiling MPI code with the GNU compilers for parallel1
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""

Compilation must occur on ``smuhpc4.smu.edu``.

First, load the ``mvapich2/1.9a2/gcc`` module,

.. code-block:: bash

   $ module load mvapich2/1.9a2/gcc

Second, compile your executable using one of the MPI wrapper scripts:
``mpicc``, ``mpicxx``, ``mpif90`` or ``mpif77``.  For example, we may
compile the example executable as

.. code-block:: bash

   $ mpicxx driver.cpp -lm -o driver_GNU_parallel1.exe

Note: since the MPI libraries vary based on where we wish to run and
on which compilers we use, I recommend naming the executable
appropriately to distinguish it from other compilation approaches.  Of
course, this is not required.



.. _session9-running_MPI_GNU_parallel1:

Running MPI code with the GNU compilers on parallel1
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""

**VERIFY THESE INSTRUCTIONS**

You must launch the job from ``smuhpc4.smu.edu``.

The key to launching MPI jobs that utilize more than one node using
either parallel1 or parallel2 is that you must supply an "executable"
to condor that handles the process of launching your program
appropriately.  This Condor/MPI interaction is handled through
incorporating a few specific items into your condor submission script,
along with a customized executable script that handles the launching
of your executable.

This executable script is named ``mvapich2_script``, and is included
in the ``session9_MPI`` directory that you downloaded above.  You
should not need to edit this script file except for more advanced
usage scenarios, which we will not cover during this tutorial.

However, the example condor submission file, ``mpi_condor.sub`` does
contain specific items that you will need to modify for your usage
scenario.  This file is reproduced here:

.. code-block:: bash

   # FILENAME mpi_condor.sub
   # Use this script to submit MPI jobs.
   # Read the instructions carefully and 
   # report any issues to your system admins. 

   ###############################################
   # Edit the following lines to set up your run #
   ###############################################

   # Your actual executable file name along with arguments goes here
   arguments     = "./driver.exe"

   # Total number of nodes you would like to run your code on
   machine_count = 8

   # The particular node type you wish to use,
   # valid values are {inode,iwnode,cnode}
   mynodetype    = "inode"

   # Here you define the specific environment variables
   # _LOAD_MODULE  MPI module required for your job
   # _WAY=Specify  Number of MPI processes to run on each node
   environment   = "_LOAD_MODULE=mvapich2/1.9a2/gcc _WAY=3"

   # Select the appropriate file name for your output files.
   output = out.txt
   error  = err.txt
   log    = log.txt

   ###################################
   # Do not edit the following lines #
   ###################################
   universe     = parallel
   executable   = mvapich2_script
   getenv       = true
   requirements = regexp($(mynodetype), Machine)
   +WantParallelSchedulingGroups = TRUE
   queue

As should be clear from the structure of this file, you only need to
modify the first few blocks of options:

* ``arguments`` -- this should include *both* your executable file
  name and any command-line arguments that it requires.  If more than
  one item is listed (i.e. if your program uses any command-line
  arguments), they should be enclosed in double-quotation marks.

* ``machine_count`` -- this should be the number of nodes that you
  wish to use for your program.  Recall that each node on parallel1
  has 8 cores.

* ``mynodetype`` -- this is the type of node you wish to use, here it
  uses "inode", which is the name of the nodes comprisong parallel1.

* ``environment`` -- in addition to any environment variables you wish
  to specify on your own, you must specify the following two:
 
  * ``_LOAD_MODULE`` -- this is the MPI module required to compile
    your job.  For GNU on parallel1, the module is
    ``mvapich2/1.9a2/gcc``, as entered here.

  * ``_WAY`` -- this is the number of cores on each of your requested
    nodes that you wish to use (1 :math:`\le`  ``_WAY`` :math:`\le`
    8).  For example, if you chose 8 nodes and 3 way, you would run
    with 24 total MPI processes.

* ``output``, ``error`` and ``log`` are as usual.

You should not modify any arguments below the lines

.. code-block:: bash

   ###################################
   # Do not edit the following lines #
   ###################################

Once you have finished setting up this file, you can submit it as
usual (only from the  ``smuhpc4`` login node),

.. code-block:: bash

   $ condor_submit ./mpi_condor.sub





.. _session9-compiling_MPI_PGI_parallel1:

Compiling MPI code with the PGI compilers for parallel1
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""

Compilation must occur on ``smuhpc4.smu.edu``.

First, load the ``mvapich2/1.6/pgi`` module,

.. code-block:: bash

   $ module load mvapich2/1.6/pgi

Second, compile your executable using one of the MPI wrapper scripts:
``mpicc``, ``mpicxx``, ``mpif90`` or ``mpif77``.  For example, we may
compile the example executable as

.. code-block:: bash

   $ mpicxx driver.cpp -lm -o driver_PGI_parallel1.exe

Note: since the MPI libraries vary based on where we wish to run and
on which compilers we use, I recommend naming the executable
appropriately to distinguish it from other compilation approaches.  Of
course, this is not required.


.. _session9-running_MPI_PGI_parallel1:

Running MPI code with the PGI compilers on parallel1
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""

**VERIFY THESE INSTRUCTIONS**

You must launch the job from ``smuhpc4.smu.edu``.

The key to launching MPI jobs that utilize more than one node using
either parallel1 or parallel2 is that you must supply an "executable"
to condor that handles the process of launching your program
appropriately.  This Condor/MPI interaction is handled through
incorporating a few specific items into your condor submission script,
along with a customized executable script that handles the launching
of your executable.

This executable script is named ``mvapich2_script``, and is included
in the ``session9_MPI`` directory that you downloaded above.  You
should not need to edit this script file except for more advanced
usage scenarios, which we will not cover during this tutorial.

However, the example condor submission file, ``mpi_condor.sub`` does
contain specific items that you will need to modify for your usage
scenario.  This file, modified for the PGI/parallel1 usage scenario,
is reproduced here: 

.. code-block:: bash

   # FILENAME mpi_condor.sub
   # Use this script to submit MPI jobs.
   # Read the instructions carefully and 
   # report any issues to your system admins. 

   ###############################################
   # Edit the following lines to set up your run #
   ###############################################

   # Your actual executable file name along with arguments goes here
   arguments     = "./driver.exe"

   # Total number of nodes you would like to run your code on
   machine_count = 8

   # The particular node type you wish to use,
   # valid values are {inode,iwnode,cnode}
   mynodetype    = "inode"

   # Here you define the specific environment variables
   # _LOAD_MODULE  MPI module required for your job
   # _WAY=Specify  Number of MPI processes to run on each node
   environment   = "_LOAD_MODULE=mvapich2/1.6/pgi _WAY=3"

   # Select the appropriate file name for your output files.
   output = out.txt
   error  = err.txt
   log    = log.txt

   ###################################
   # Do not edit the following lines #
   ###################################
   universe     = parallel
   executable   = mvapich2_script
   getenv       = true
   requirements = regexp($(mynodetype), Machine)
   +WantParallelSchedulingGroups = TRUE
   queue

As should be clear from the structure of this file, you only need to
modify the first few blocks of options:

* ``arguments`` -- this should include *both* your executable file
  name and any command-line arguments that it requires.  If more than
  one item is listed (i.e. if your program uses any command-line
  arguments), they should be enclosed in double-quotation marks.

* ``machine_count`` -- this should be the number of nodes that you
  wish to use for your program.  Recall that each node on parallel1
  has 8 cores.

* ``mynodetype`` -- this is the type of node you wish to use, here it
  uses "inode", which is the name of the nodes comprisong parallel1.

* ``environment`` -- in addition to any environment variables you wish
  to specify on your own, you must specify the following two:
 
  * ``_LOAD_MODULE`` -- this is the MPI module required to compile
    your job.  For PGI on parallel1, the module is
    ``mvapich2/1.6/pgi``, as entered here.

  * ``_WAY`` -- this is the number of cores on each of your requested
    nodes that you wish to use (1 :math:`\le`  ``_WAY`` :math:`\le`
    8).  For example, if you chose 8 nodes and 3 way, you would run
    with 24 total MPI processes.

* ``output``, ``error`` and ``log`` are as usual.

You should not modify any arguments below the lines

.. code-block:: bash

   ###################################
   # Do not edit the following lines #
   ###################################

Once you have finished setting up this file, you can submit it as
usual (only from the  ``smuhpc4`` login node),

.. code-block:: bash

   $ condor_submit ./mpi_condor.sub





.. _session9-compiling_MPI_GNU_parallel2:

Compiling MPI code with the GNU compilers for parallel2
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""

Compilation must occur on ``smuhpc.smu.edu``, ``smuhpc2.smu.edu`` or
``smuhpc3.smu.edu``, but **not** on ``smuhpc4``.

First, load the ``mvapich2/1.6/gcc-QL`` module,

.. code-block:: bash

   $ module load mvapich2/1.6/gcc-QL

Second, compile your executable using one of the MPI wrapper scripts:
``mpicc``, ``mpicxx``, ``mpif90`` or ``mpif77``.  For example, we may
compile the example executable as

.. code-block:: bash

   $ mpicxx driver.cpp -lm -o driver_GNU_parallel2.exe

Note: since the MPI libraries vary based on where we wish to run and
on which compilers we use, I recommend naming the executable
appropriately to distinguish it from other compilation approaches.  Of
course, this is not required.


.. _session9-running_MPI_GNU_parallel2:

Running MPI code with the GNU compilers on parallel2
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""

**VERIFY THESE INSTRUCTIONS**

You must launch the job from ``smuhpc4.smu.edu`` (even though you
could not compile it on that node -- sorry).

The key to launching MPI jobs that utilize more than one node using
either parallel1 or parallel2 is that you must supply an "executable"
to condor that handles the process of launching your program
appropriately.  This Condor/MPI interaction is handled through
incorporating a few specific items into your condor submission script,
along with a customized executable script that handles the launching
of your executable.

This executable script is named ``mvapich2_script``, and is included
in the ``session9_MPI`` directory that you downloaded above.  You
should not need to edit this script file except for more advanced
usage scenarios, which we will not cover during this tutorial.

However, the example condor submission file, ``mpi_condor.sub`` does
contain specific items that you will need to modify for your usage
scenario.  This file, modified for the GNU/parallel2 usage scenario,
is reproduced here: 

.. code-block:: bash

   # FILENAME mpi_condor.sub
   # Use this script to submit MPI jobs.
   # Read the instructions carefully and 
   # report any issues to your system admins. 

   ###############################################
   # Edit the following lines to set up your run #
   ###############################################

   # Your actual executable file name along with arguments goes here
   arguments     = "./driver.exe"

   # Total number of nodes you would like to run your code on
   machine_count = 8

   # The particular node type you wish to use,
   # valid values are {inode,iwnode,cnode}
   mynodetype    = "iwnode"

   # Here you define the specific environment variables
   # _LOAD_MODULE  MPI module required for your job
   # _WAY=Specify  Number of MPI processes to run on each node
   environment   = "_LOAD_MODULE=mvapich2/1.6/gcc-QL _WAY=11"

   # Select the appropriate file name for your output files.
   output = out.txt
   error  = err.txt
   log    = log.txt

   ###################################
   # Do not edit the following lines #
   ###################################
   universe     = parallel
   executable   = mvapich2_script
   getenv       = true
   requirements = regexp($(mynodetype), Machine)
   +WantParallelSchedulingGroups = TRUE
   queue

As should be clear from the structure of this file, you only need to
modify the first few blocks of options:

* ``arguments`` -- this should include *both* your executable file
  name and any command-line arguments that it requires.  If more than
  one item is listed (i.e. if your program uses any command-line
  arguments), they should be enclosed in double-quotation marks.

* ``machine_count`` -- this should be the number of nodes that you
  wish to use for your program.  Recall that each node on parallel1
  has 8 cores.

* ``mynodetype`` -- this is the type of node you wish to use, here it
  uses "inode", which is the name of the nodes comprisong parallel1.

* ``environment`` -- in addition to any environment variables you wish
  to specify on your own, you must specify the following two:
 
  * ``_LOAD_MODULE`` -- this is the MPI module required to compile
    your job.  For GNU on parallel2, the module is
    ``mvapich2/1.6/gcc-QL``, as entered here.

  * ``_WAY`` -- this is the number of cores on each of your requested
    nodes that you wish to use (1 :math:`\le`  ``_WAY`` :math:`\le`
    12).  For example, if you chose 8 nodes and 11 way, you would run
    with 88 total MPI processes.

* ``output``, ``error`` and ``log`` are as usual.

You should not modify any arguments below the lines

.. code-block:: bash

   ###################################
   # Do not edit the following lines #
   ###################################

Once you have finished setting up this file, you can submit it as
usual (only from the  ``smuhpc4`` login node),

.. code-block:: bash

   $ condor_submit ./mpi_condor.sub





.. _session9-compiling_MPI_PGI_parallel2:

Compiling MPI code with the PGI compilers for parallel2
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""

Compilation must occur on ``smuhpc.smu.edu``, ``smuhpc2.smu.edu`` or
``smuhpc3.smu.edu``, but **not** on ``smuhpc4``.

First, load the ``mvapich2/1.6/pgi-QL`` module,

.. code-block:: bash

   $ module load mvapich2/1.6/pgi-QL

Second, compile your executable using one of the MPI wrapper scripts:
``mpicc``, ``mpicxx``, ``mpif90`` or ``mpif77``.  For example, we may
compile the example executable as

.. code-block:: bash

   $ mpicxx driver.cpp -lm -o driver_PGI_parallel2.exe

Note: since the MPI libraries vary based on where we wish to run and
on which compilers we use, I recommend naming the executable
appropriately to distinguish it from other compilation approaches.  Of
course, this is not required.


.. _session9-running_MPI_PGI_parallel2:

Running MPI code with the PGI compilers on parallel2
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""


**VERIFY THESE INSTRUCTIONS**

You must launch the job from ``smuhpc4.smu.edu`` (even though you
could not compile it on that node -- sorry).

The key to launching MPI jobs that utilize more than one node using
either parallel1 or parallel2 is that you must supply an "executable"
to condor that handles the process of launching your program
appropriately.  This Condor/MPI interaction is handled through
incorporating a few specific items into your condor submission script,
along with a customized executable script that handles the launching
of your executable.

This executable script is named ``mvapich2_script``, and is included
in the ``session9_MPI`` directory that you downloaded above.  You
should not need to edit this script file except for more advanced
usage scenarios, which we will not cover during this tutorial.

However, the example condor submission file, ``mpi_condor.sub`` does
contain specific items that you will need to modify for your usage
scenario.  This file, modified for the PGI/parallel2 usage scenario,
is reproduced here: 

.. code-block:: bash

   # FILENAME mpi_condor.sub
   # Use this script to submit MPI jobs.
   # Read the instructions carefully and 
   # report any issues to your system admins. 

   ###############################################
   # Edit the following lines to set up your run #
   ###############################################

   # Your actual executable file name along with arguments goes here
   arguments     = "./driver.exe"

   # Total number of nodes you would like to run your code on
   machine_count = 8

   # The particular node type you wish to use,
   # valid values are {inode,iwnode,cnode}
   mynodetype    = "iwnode"

   # Here you define the specific environment variables
   # _LOAD_MODULE  MPI module required for your job
   # _WAY=Specify  Number of MPI processes to run on each node
   environment   = "_LOAD_MODULE=mvapich2/1.6/pgi-QL _WAY=11"

   # Select the appropriate file name for your output files.
   output = out.txt
   error  = err.txt
   log    = log.txt

   ###################################
   # Do not edit the following lines #
   ###################################
   universe     = parallel
   executable   = mvapich2_script
   getenv       = true
   requirements = regexp($(mynodetype), Machine)
   +WantParallelSchedulingGroups = TRUE
   queue

As should be clear from the structure of this file, you only need to
modify the first few blocks of options:

* ``arguments`` -- this should include *both* your executable file
  name and any command-line arguments that it requires.  If more than
  one item is listed (i.e. if your program uses any command-line
  arguments), they should be enclosed in double-quotation marks.

* ``machine_count`` -- this should be the number of nodes that you
  wish to use for your program.  Recall that each node on parallel1
  has 8 cores.

* ``mynodetype`` -- this is the type of node you wish to use, here it
  uses "inode", which is the name of the nodes comprisong parallel1.

* ``environment`` -- in addition to any environment variables you wish
  to specify on your own, you must specify the following two:
 
  * ``_LOAD_MODULE`` -- this is the MPI module required to compile
    your job.  For PGI on parallel2, the module is
    ``mvapich2/1.6/pgi-QL``, as entered here.

  * ``_WAY`` -- this is the number of cores on each of your requested
    nodes that you wish to use (1 :math:`\le`  ``_WAY`` :math:`\le`
    12).  For example, if you chose 8 nodes and 11 way, you would run
    with 88 total MPI processes.

* ``output``, ``error`` and ``log`` are as usual.

You should not modify any arguments below the lines

.. code-block:: bash

   ###################################
   # Do not edit the following lines #
   ###################################

Once you have finished setting up this file, you can submit it as
usual (only from the  ``smuhpc4`` login node),

.. code-block:: bash

   $ condor_submit ./mpi_condor.sub






MPI exercise
"""""""""""""""

Set up submission scripts to run the executable ``driver.exe`` using
1, 2, 4, 8, 16, 32 and 64 cores.  For the 1, 2, 4, and 8 processor jobs, just
use one node. Run the 16, 32 and 64 processor jobs using 8 cores/node.
