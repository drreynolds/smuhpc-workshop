:tocdepth: 3


.. _session7:

*******************************************************
Session 7: Parallel Computing on ManeFrame
*******************************************************




General information
=================================================================

As seen in :ref:`tutorial session 5 <session5>`, we may use ManeFrame
for parallel computing (in addition to running serial jobs).  In the
remainder of this tutorial, we will differentiate between running
shared-memory parallel (SMP) programs, typically enabled by OpenMP,
and distributed-memory parallel (DMP) programs, typically enabled by
MPI.  Hybrid MPI+OpenMP programming is also possible on ManeFrame, and
we will end this tutorial session with a brief study of those.

Also as seen in :ref:`tutorial session 5 <session5>`, we have the
choice between GNU and PGI compilers when compiling our codes on
ManeFrame.  In the tutorial below, we will focus on use of the GNU
compilers; use of the PGI compilers is similar.



Partitions, job sizes, sharing and time limits
-----------------------------------------------

There are currently three partitions/queues on ManeFrame:

* **interactive** (7 nodes): Intended for small interactive jobs that
  cannot be run through the batch system.  *This is the default queue
  if none is specified*.

* **highmem** (20 nodes): Intended for batch jobs that require large
  amounts of memory (up to 192 GB RAM per node).

* **parallel** (all remaining nodes, ~1000): Intended for all "normal"
  processing.


Although acquired by a small group of SMU faculty, ManeFrame is a
*shared* resource for all SMU researchers (not your personal/group
research cluster).  As such, it is currently the user's responsibility
to be a good citizen and not monopolize resources.  As a result,
ManeFrame does not currently enforce wall-clock limits on any of these
partitions.  However, *if resources are monopolized, limits will be
created and enforced*. 

We therefore provide suggestions for job size and wall clock limits on
these partitions: 

* interactive -- since this is so small, it is recommended
  that users do not use more than one node at a time, and that user
  jobs not exceed one hour.  All jobs on this queue are forced to be
  :ref:`shared <shared_nodes>`, with up to four shared jobs per node.

* highmem -- again, a small resource.  Since these nodes are requested
  due to their large memory, we recommend that these be requested as
  :ref:`exclusive <exclusive_nodes>` so that two large jobs do not
  share a node and exceed the available memory.  As a result, it is
  recommended that no job use more than 10 nodes; with long-running
  jobs (> 24 hours) using fewer nodes than short jobs.

* parallel -- although this partition is large, one must still be a
  good citizen.  It is recommended that larger jobs (> 100 nodes) run
  for shorter periods (< 48 hours).  Also, it is preferred that no job
  run for longer than one month -- for longer-running jobs please
  strive to break the run into multiple runs of shorter duration.


Lastly, we **strongly recommend** that all user jobs specify a
:ref:`maximum wall-clock time limit <time_limit>`, even if this value
is large (e.g. 2 months).  This helps to ensure that runaway jobs are
eventually killed by SLURM, especially if a user forgets to log in
periodically to monitor the job's progress.



Getting started
------------------------------------

Since we'll be using the GNU compiler throughout this tutorial, and
we'll eventually compile and run MPI-parallelized jobs, load the
``mvapich2/2.0-gcc-4.9.1`` module:

.. code-block:: bash

   $ module load mvapich2/2.0-gcc-4.9.1
   $ module list

You should see at least the two modules, ``gcc/4.9.1`` and
``mvapich2/2.0-gcc-4.9.1`` listed. 


Second, you will need to retrieve sets of files for the OpenMP, MPI
and hybrid MPI+OpenMP portions of this session.  Retrieve the files
for the OpenMP portion by clicking :download:`this link
<code/session7_OpenMP.tgz>` or by copying them on ManeFrame at the
command line: 

.. code-block:: bash

   $ cp ~dreynolds/ManeFrame_tutorial/session7_OpenMP.tgz .

Similarly, retrieve the files for the MPI portion by clicking
:download:`this link <code/session7_MPI.tgz>` or by copying them
on ManeFrame at the command line:

.. code-block:: bash

   $ cp ~dreynolds/ManeFrame_tutorial/session7_MPI.tgz .


Lastly, retrieve the files for the hybrid MPI+OpenMP portion by
clicking :download:`this link <code/session7_Hybrid.tgz>` or by
copying them on ManeFrame at the command line:

.. code-block:: bash

   $ cp ~dreynolds/ManeFrame_tutorial/session7_Hybrid.tgz .





Shared-memory programs
=================================================================

We may run shared-memory programs on any ManeFrame worker node.
All ManeFrame worker nodes have 8 CPU cores.  In my experience,
shared-memory programs rarely benefit from using more execution
threads than the number of physical cores on a node, so I recommend
that SMP jobs use at most 8 threads, though your application may act
differently.


Enabling OpenMP
------------------------------------


.. index:: OpenMP; compiler flags

OpenMP is implemented as an extension to existing programming
languages, and is available for programs written in C, C++, Fortran77
and Fortran90.  These OpenMP extensions are enabled at the compiler
level, with most compilers supporting OpenMP.  In these compilers,
OpenMP is enabled through supplying a flag to the relevant compiler
denoting that you wish for it to allow the OpenMP extensions to the
existing language.  The various compiler flags for well-known
compilers include: 

* PGI: ``-mp``

* GNU: ``-fopenmp``

* Intel: ``-openmp``

* IBM: ``-qsmp``

* Oracle: ``-xopenmp``

* Absoft: ``-openmp``

* Cray: (on by default)

* NAG: ``-openmp``




Compiling with OpenMP
------------------------------------

Before proceeding to the following subsections, unpack the OpenMP
portion of this tutorial using the usual commands:

.. code-block:: bash

   $ tar -zxf session7_OpenMP.tgz
   $ cd session7_OpenMP

In the resulting directory, you will find a number of files, including
``Makefile``, ``driver.cpp`` and ``vectors.cpp``.  


You can compile the executable ``driver.exe`` with the GNU compiler
and OpenMP using the command  

.. code-block:: bash

   $ g++ -fopenmp driver.cpp vectors.cpp -lm -o driver.exe

The compiler option ``-fopenmp`` is the same, no matter which GNU
compiler you are using (``gcc``, ``gfortran``, etc.)


.. note:: The only difference when using the PGI compilers is the compiler
   name and OpenMP flag, e.g.

   .. code-block:: bash

      $ pgc++ -mp driver.cpp vectors.cpp -lm -o driver.exe





Running with OpenMP 
------------------------------------

.. index:: OpenMP; running at the command line

.. _session7-running_OpenMP_commandline:

Running OpenMP programs at the command line
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Run the executable ``driver.exe`` from the command line:

.. code-block:: bash

   $ ./driver.exe

In fact, this executable did not run using more than one thread, since
the default behavior of OpenMP programs on SMU HPC is to only use a
single thread.

.. index:: OpenMP; OMP_NUM_THREADS

To change the number of threads used by our program, we must adjust
the ``OMP_NUM_THREADS`` environment variable. First, verify that this is
set to the default value of 1 (or it may be blank): 

.. code-block:: bash

   $ echo $OMP_NUM_THREADS

The method for re-setting this environment variable will depend on our
login shell.  First, determine which login shell you use:

.. code-block:: tcsh

   $ echo $SHELL

For CSH/TCSH users, you can set your ``OMP_NUM_THREADS`` environment
variable to 2 with the command:

.. code-block:: tcsh

   $ setenv OMP_NUM_THREADS 2

the same may be accomplished by BASH/SH/KSH users with the command:

.. code-block:: bash

   $ export OMP_NUM_THREADS=2

Re-run ``driver.exe`` first using 1 and then using 3 OpenMP
threads.  Notice the speedup when running with multiple threads.  Also
notice that although the result, ``Final rms norm`` is essentially the
same in both runs, the results differ slightly after around the 11th
digit.  The reasoning is beyond the scope of this tutorial, but in
short this results from a combination of floating-point roundoff 
errors and differences in the order of arithmetic operations.  The
punch line being that bitwise identicality between runs is difficult
to achieve in parallel computations, and in any case may not be
necessary in the first place.



.. index:: OpenMP; running batch jobs

.. _session7-running_OpenMP_batch:

Running OpenMP batch jobs
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To run OpenMP-enabled batch job, the steps are identical to those
required for requesting an :ref:`exclusive <exclusive_nodes>` node,
except that we must additionally specify the environment variable
``OMP_NUM_THREADS``.  It is recommended that this variable be supplied
*inside* the batch job submission file to ensure reproducibility of
results. 

Create a batch job submission file:

.. code-block:: bash

   #!/bin/bash
   #SBATCH -J test1          # job name
   #SBATCH -o test1.txt      # output/error file name
   #SBATCH -p parallel       # requested queue
   #SBATCH --exclusive       # do not share the compute node
   #SBATCH -t 1              # maximum runtime in minutes

   # set the desired number of OpenMP threads
   export OMP_NUM_THREADS=7

   # run the code
   ./driver.exe


Recall, the ``--exclusive``  option indicates that we wish to run the
job on an entire node (without sharing that node with others).  This
is critical for SMP jobs, since each SMP job will launch multiple
threads of execution, so we do not want to intrude on other users by
running threads on their CPU cores! 

Furthermore, note that once the job is launched, it will use 7 of the
8 available hardware threads on that node, implying that one core will
remain idle.

.. note:: In fact, each worker node does much more than just run your
	  job (runs the operating system, handles network traffic,
	  etc.), so in many instances SMP jobs *run faster* when using
	  :math:`N-1` threads than when using :math:`N` threads, where
          :math:`N` is the number of CPU cores, since this leaves one
          core to handle all remaining non-job duties.




OpenMP exercise
------------------------------------

Compile the program ``driver.exe`` using the GNU compiler with OpenMP
enabled.

Create a single SLURM submission script that will run the program
``driver.exe`` using 1, 2, 3, ..., 12 OpenMP threads on ManeFrame's
*parallel* partition.  Recall from session 5 that you may embed
multiple commands within your job submission script.

Launch this job, and when it has completed, determine the *parallel
efficiency* (i.e. strong scaling performance) of this code (defined in
session 6, :ref:`parallel_computing_metrics`).  How well does the
program perform?  Is there a maximum number of threads where, beyond
which, additional resources no longer improve the speed?


.. note:: If you finish this early, perform the same experiment but
	  this time using the GNU compiler.  How do your results
	  differ? 





Distributed-memory programs
=================================================================


MPI overview
------------------------------------

Unpack the source files for the MPI portion of this tutorial as usual,

.. code-block:: bash

   $ tar -zxf session7_MPI.tgz


Unlike OpenMP, MPI is implemented as a standalone library.  This means
that MPI merely consists of functions that you may call within your
own programs to perform message passing within a distributed memory
parallel computation.  

Typically written in C (like the Linux kernel, for maximum
portability), MPI libraries typically include interfaces for programs
written in C, C++, Fortran77, Fortran90 and even Python. 

Moreover, since MPI is a library, it does not require any specific
compiler extensions to construct a MPI-enabled parallel program, so as
long as you have any "standard" compiler for these languages, you can
have a functioning MPI installation.




Compiling MPI code
------------------------------------



.. index:: MPI; wrapper scripts

.. _session7-compiling_MPI_programs:

MPI wrapper scripts
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In order to compile a program to use any given software library, a few
key items must be known about how the library was installed on the system:

* Does the library provide header files (C, C++) or modules (F90),
  and where are these located?  This location is important
  because when compiling our own codes, we must tell the compiler
  where to look for these "include files" using the ``-I`` argument.

* If the library was installed in a non-default location, where is
  the resulting ".a" file (static library) or ".so" file (shared
  library) located?  Again, this location is important
  because when linking our own codes, we must tell the compiler where
  to look for these library files using the ``-L`` and ``-l``
  arguments.

For example, the MVAPICH2 MPI library built using the GNU version
4.9.1 compiler, is installed on ManeFrame in the directory
``/grid/software/mvapich2/2.0/gcc-4.9.1/``, with header files located in
``/grid/software/mvapich2/2.0/gcc-4.9.1/include/`` and library files
located in ``/grid/software/mvapich2/2.0/gcc-4.9.1/lib/``.  Without me
telling you that, how easy do you think it would be to find these on
your own?

Finally, because I'm familiar with this package, I know that to
compile an executable I must link against the library files
``libmpich.a`` and ``libmpl.a`` in this library directory location.

As a result, we *could* compile the executable ``driver.exe`` with the
commands 

.. code-block:: bash

   $ g++ driver.cpp -I/grid/software/mvapich2/2.0/gcc-4.9.1/include \
     -L/grid/software/mvapich2/2.0/gcc-4.9.1/lib -lmpich -lmpl -lm -o driver.exe


Clearly, specifying the specific instructions for including and
linking to an MPI library can be nontrivial: 

* You must know where all of the relevant libraries are installed on
  each computer. 

* You must know which specific library files are required for
  compiling a given program. 

* Sometimes, you must even know which order you need to specify these
  specific library files in the linking line. 

Thankfully, MPI library writers typically include *wrapper scripts*
to do most of this work for you. Such scripts are written to encode
all of the above information that is required to use MPI with a given
compiler on a specific system. 

.. index:: 
   single: MPI wrapper scripts; mpicxx
   single: MPI wrapper scripts; mpiCC
   single: MPI wrapper scripts; mpic++
   single: MPI wrapper scripts; openmpicxx
   single: MPI wrapper scripts; mpicc
   single: MPI wrapper scripts; openmpicc
   single: MPI wrapper scripts; mpif90
   single: MPI wrapper scripts; openmpif90
   single: MPI wrapper scripts; mpif77
   single: MPI wrapper scripts; openmpif77

Depending on your programming language and the specific MPI
implementation, these wrapper scripts can have different names. The
typical names for these MPI wrapper scripts for all MPI libraries
installed on ManeFrame are: 

* C++: ``mpicxx`` or ``mpic++``

* C: ``mpicc``

* Fortran 90/95/2003: ``mpif90``

* Fortran 77: ``mpif77`` (typically, the Fortran 90/95 wrapper will also work for these)

In order to use these wrapper scripts on ManeFrame, we must first load
the correct module environment.  Many are available: 

.. code-block:: bash

   $ module avail

Do you see how many of these available modules include the names
``mpich2``, ``mvapich2`` and ``openmpi``?  Each of these modules will
enable the wrapper scripts for a different MPI library and compiler.

As we mentioned at the beginning of this tutorial, today we'll focus
on using the GNU compiler with the Infiniband-optimized MVAPICH2 MPI
library.  Ensure that you still have the module
``mvapich2/2.0-gcc-4.9.1`` loaded:

.. code-block:: bash

   $ module load mvapich2/2.0-gcc-4.9.1
   $ module list

This installation provides the MPI wrapper scripts ``mpicc``,
``mpicxx``, ``mpif90`` and ``mpif77``.


We may now use the corresponding C++ MPI wrapper script, along with
the (much simpler) compilation line

.. code-block:: bash

   $ mpicxx driver.cpp -lm -o driver.exe

to build the executable.



.. _session7-running_MPI_programs:

Running MPI code
------------------------------------


.. index:: MPI; running interactive jobs

.. _session7-running_MPI_command_line:

Running MPI interactive jobs
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

When running jobs on a dedicated parallel cluster (or a single
workstation), parallel jobs and processes are not regulated through a
queueing system. This has some immediate benefits: 

* You never have to wait to start running a program.

* It is trivial to set up and run parallel jobs.

* You have complete control over which processors are used in a parallel computation.

However, dedicated clusters also have some serious deficiencies:

* A single user can monopolize all of the available resources.

* More than one job can be running on a processor at a time, so
  different processes must fight for system resources (giving
  unreliable timings or memory availability). 

* The more users there are, the worse these problems become.

However, running parallel programs on such a system can be very
simple, though the way that you run these jobs will depend on which
MPI implementation you are using. 

Since ManeFrame is a large-scale, shared computing resource, we use
the SLURM queueing system to manage user jobs.  However, in some
instances it may be useful to run MPI jobs interactively on
ManeFrame.  This can be especially useful when debugging or testing a
new code to ensure that it functions correctly, before submitting
larger-scale or longer-running jobs to the queueing system.

On ManeFrame, we should only run interactive MPI jobs by requesting
them through the batch system.  This may be accomplished with the
``srun`` command.  Recall the two :ref:`srun options <srun_options>`,
``-N`` and ``-n``, that request a specified number of nodes and
tasks.  To request an interactive session on an entire *parallel* node
(with up to 8 tasks on that node), for a maximum of 10 minutes, issue
the command:

.. code-block:: bash

   $ srun -I -N1 -n8 -t 10 -p parallel --x11=first --pty $SHELL

.. note:: even though ManeFrame has a special *interactive* partition,
	  it is not recommended that you use that partition for
	  interactive parallel jobs, since these nodes will be shared
	  with other users.  Instead, either the *parallel* or
	  *highmem* partitions should be used since you may reserve
	  all cores on those nodes for your interactive session.  Of
	  those, the probability of reserving a node in *parallel* is
	  much higher than in *highmem*, due to the relative sizes of
	  these partitions.

If this command completes successfully, you should note that you are
in a new shell with a different hostname:

.. code-block:: bash

   $ hostname 

(this should no longer be ``mflogin01.hpc.smu.edu`` or
``mflogin02.hpc.smu.edu``; maybe something like
``mfc0321.hpc.smu.edu``.  Here, we can launch an MPI program
interactively using the program ``mpiexec`` as if we were launching it
on our own workstation.  The calling syntax of ``mpiexec`` is 

.. code-block:: text

   mpiexec [mpiexec_options] program_name [program_options]

The primary ``mpiexec`` option that we use is ``-n #``, where ``#`` is
the desired number of MPI processes to use in running the parallel job.

First, run the program using 1 process: 

.. code-block:: bash

   $ mpiexec -n 1 ./driver.exe

Run the program using 2 processes:

.. code-block:: bash

   $ mpiexec -n 2 ./driver.exe

Run the program using 4 processes:

.. code-block:: bash

   $ mpiexec -n 4 ./driver.exe

All of these will run the MPI processes on separate cores of our
currently-reserved worker node.

Since the ManeFrame nodes have 8 physical CPU cores, we are limited to
requesting at most 8 tasks in the ``srun`` command, and to launching
at most 8 tasks in the subsequent ``mpiexec`` command.  





.. index:: MPI; running batch jobs

.. _session7-running_MPI_batch:

Running MPI batch jobs
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Running MPI batch jobs on ManeFrame is almost identical to running
serial and OpenMP batch jobs. However, when running MPI jobs, we must
tell the queueing system a few additional pieces of information: 

1. How many total nodes we want to reserve on the machine?
2. How many total cores do we want to reserve on the machine?
3. How do you want to distribute tasks on each node?
4. How many MPI tasks do you actually want to run?

We have two key ways to control execution of parallel batch jobs:

* controlling how the job is *reserved*
* controlling how the MPI job is *executed*


MPI batch job reservations
""""""""""""""""""""""""""""

The job *reservation* corresponds with the options specified with the
``#SBATCH`` prefix in the job submission file.  These tell SLURM about
the resources you wish to reserve.  Here, the most relevant options
are: 

.. code-block:: bash

   #SBATCH -N <NumNodes>
   #SBATCH -n <NumTasks>
   #SBATCH --ntasks-per-node=<NumLoad>
   #SBATCH --exclusive

These options signify:

* ``-N <NumNodes>`` -- This requests that ``<NumNodes>`` nodes be reserved
  for this job.  The request should not exceed the total number of
  nodes configured in the partition, otherwise the job will be rejected.

* ``-n <NumTasks>`` or ``--ntasks=<NumTasks>`` -- This requests that
  the job will launch a maximum of ``<NumTasks>`` tasks.

* ``--ntasks-per-node=<NumLoad>`` -- This requests that a maximum of
  ``<NumLoad>`` cores should be used on each node.

* ``--exclusive`` -- This requests that the allocated nodes not be
  shared with other users.

Clearly, if you specify a value of ``NumTasks`` that is more than 8x
larger than your value of ``NumNodes`` it will cause an error, since
you will be requesting more cores tasks than you have requested
physical processes.


MPI batch job execution
""""""""""""""""""""""""""""

The job *execution* corresponds with the command that you actually use
to launch the MPI job.  Here, the most relevant commands are: 

.. code-block:: bash

   srun <executable>

.. code-block:: bash

   mpiexec <executable>

and

.. code-block:: bash

   mpiexec -n <NumProcesses> <executable>

The first two of these are equivalent, and will launch the MPI job to
use all of the reserved resources.

The third option is somewhat different, as it launches the MPI job
to use precisely ``<NumProcesses>`` MPI execution threads.  This value
must not exceed the total reservation size, but it may be smaller.


Perhaps the easiest way to understand these options is through a
series of examples.

MPI batch file examples
""""""""""""""""""""""""""""

**Example 1: specifying the number of MPI tasks**

The simplest way to launch an MPI job with SLURM is to just request a
specific number of MPI tasks with the ``-n`` option:

.. code-block:: bash

   #!/bin/bash
   #SBATCH -n 12                # requested MPI tasks
   #SBATCH -p parallel          # requested queue (batch/parallel)
   #SBATCH -t 1                 # maximum runtime in minutes

   srun ./driver.exe

When running this, my job ran with 12 total MPI tasks on two nodes,
with 8 tasks on the first node and 4 on the second.  Since this does
not specify the ``--exclusive`` option, some of these MPI tasks may be
launched on nodes shared with others; it's even possible that one MPI
task will be launched on each of 12 different nodes that are shared by
others.


**Example 2: specifying the number of MPI tasks (exclusive)**

If we add in only the ``--exclusive`` option, this changes the
behavior slightly:

.. code-block:: bash

   #!/bin/bash
   #SBATCH -n 12                # requested MPI tasks
   #SBATCH -p parallel          # requested queue (batch/parallel)
   #SBATCH -t 1                 # maximum runtime in minutes
   #SBATCH --exclusive          # do not share nodes

   srun ./driver.exe

This job will run always with 12 total MPI tasks on two nodes,
distributed evenly with 6 nodes on each, and the other 2 cores/node
were unused.


**Example 3: filling a specified portion of each node**

For some jobs that require significant amounts of memory, 
8 MPI tasks may require too much memory for each node.  In this case
we may wish to reserve nodes based on memory capacity, and only launch
a few MPI tasks per node.  This is where the ``ntasks-per-node``
option comes in handy:

.. code-block:: bash

   #!/bin/bash
   #SBATCH -N 2                 # requested nodes
   #SBATCH --ntasks-per-node=2  # task load per node
   #SBATCH -p parallel          # requested queue (batch/parallel)
   #SBATCH -t 1                 # maximum runtime in minutes
   #SBATCH --exclusive          # do not share nodes

   srun ./driver.exe

This job will run with 4 total MPI tasks, but now with 2 tasks on each
of the 2 nodes.



**Example 4: filling a specified portion of each node (revisited)**

An alternate way to perform the same kind of run would be to specify
the total number of MPI tasks, along with the load you want on each node:

.. code-block:: bash

   #!/bin/bash
   #SBATCH -n 15                # requested MPI tasks
   #SBATCH --ntasks-per-node=4  # maximum task load per node
   #SBATCH -p parallel          # requested queue (batch/parallel)
   #SBATCH -t 1                 # maximum runtime in minutes
   #SBATCH --exclusive          # do not share nodes

   srun ./driver.exe

As expected, this run uses 15 total MPI tasks, but where these are
allocated so that each node has at most 4 tasks, leading to a
submission in which three nodes run 4 MPI tasks, and a fourth node
runs only 3 MPI tasks.

However, it is recommended that you also specify ``-N`` for such jobs
so that the queueing system knows *at time of submission* how many
total nodes will eventually be needed, e.g.

.. code-block:: bash

   #!/bin/bash
   #SBATCH -N 4                 # requested nodes
   #SBATCH -n 15                # requested MPI tasks
   #SBATCH --ntasks-per-node=4  # maximum task load per node
   #SBATCH -p parallel          # requested queue (batch/parallel)
   #SBATCH -t 1                 # maximum runtime in minutes
   #SBATCH --exclusive          # do not share nodes

   srun ./driver.exe

Interestingly, when running the former of these two approaches, before
the job was launched, ``squeue`` reported that the job would require 2
nodes instead of the full 4.  While the code still ran, I may have
just gotten lucky.






MPI exercise
------------------------------------

Compile the executable ``driver.exe`` using the GNU compilers.  

Set up submission scripts to run this executable using
1, 2, 4, 8, 16, 32 and 64 cores.  For the 1, 2, 4, and 8 processor jobs, just
use one node. Run the 16, 32 and 64 processor jobs using 8 cores per node.

Determine the parallel speedup when running this code using MPI.  Does
it speed up optimally (i.e. by a factor of 64)?









Hybrid Shared/Distributed-memory programs
=================================================================

There is no reason why we cannot mix the two above parallelism
approaches, using MPI to communicate *between* nodes, while using
OpenMP to share computation by CPU cores *within* a node.


Unpack the source files for this portion of the tutorial as usual,

.. code-block:: bash

   $ tar -zxf session7_Hybrid.tgz



.. index:: MPI+OpenMP; compilation

Compiling Hybrid MPI+OpenMP code
------------------------------------

We compile hybrid MPI+OpenMP programs by combining the two previous
compilation strategies: MPI wrapper scripts *plus* OpenMP compiler
flags. 

Ensure that you still have the module ``mvapich2/2.0-gcc-4.9.1`` loaded,

.. code-block:: bash

   $ module load mvapich2/2.0-gcc-4.9.1
   $ module list

and compile the program with the command

.. code-block:: bash

   $ mpicxx -fopenmp driver.cpp -o driver.exe


.. index:: MPI+OpenMP; running batch jobs

Running Hybrid MPI+OpenMP code
------------------------------------

To run a hybrid MPI+OpenMP job, we similarly combine the two previous
approaches: job submission that specifies the number of MPI tasks
*plus* environment variables to specify the number of OpenMP threads
per MPI task.

In setting up these jobs, we want to ensure two things:

1. We clearly specify the number of MPI tasks per node and OpenMP
   threads per node so that we do not overcommit the available
   resources.

2. We evenly balance the MPI tasks so that they are evenly distributed
   among reserved nodes (and not all lumped onto the first few nodes).

We can accomplish both of these goals through techniques that we
already learned for evenly distributing MPI tasks on nodes to balance
memory constraints.

For example, to run our hybrid MPI+OpenMP code using 4 nodes, with
each node running a single MPI task but launching 8 OpenMP threads, we
can use the following submission script:

.. code-block:: bash

   #!/bin/bash
   #SBATCH -N 4                 # requested nodes
   #SBATCH --ntasks-per-node=1  # one MPI task per node
   #SBATCH --exclusive          # do not share nodes
   #SBATCH -p parallel          # requested queue (batch/parallel)
   #SBATCH -t 1                 # maximum runtime in minutes

   export OMP_NUM_THREADS=8
   srun ./driver.exe




Hybrid MPI+OpenMP exercise
------------------------------------

Compile the executable ``driver.exe`` to enable hybrid MPI+OpenMP
parallelism using the GNU compilers.  

Set up submission scripts to run this executable in the following
ways:

* 2 MPI tasks, 8 OpenMP threads each
* 4 MPI tasks, 4 OpenMP threads each
* 8 MPI tasks, 2 OpenMP thread each
* 16 MPI tasks, 1 OpenMP thread each

Set up each of these to run on exactly 2 nodes.

All four of these experiments use the same number of CPU cores.  Do
some approaches outperform others?





.. raw:: html
   :file: counter.html

