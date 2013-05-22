Session 9: Parallel Computing on SMUHPC
========================================================

*Instructor: Dan Reynolds*

*Assistants: Amit Kumar, Merlin Wilkerson, Doug Tucker*


SMUHPC network
--------------------------------------------------------

**FILL THIS IN**



Compiling and running parallel programs
--------------------------------------------------------

**FILL THIS IN WITH AN INTRODUCTION TO WHICH PARTS OF SMUHPC CAN RUN
WHAT**



OpenMP
^^^^^^^^^^

**Update for SMUHPC**


Retrieve the files for this portion of the session by clicking
:download:`this link <code/session9_OpenMP.tgz>` or by copying them
on SMUHPC at the command line:

.. code-block:: bash

   $ cp ~dreynolds/SMUHPC_workshop/session9_OpenMP.tgz .

Unpack these files as usual

.. code-block:: bash

   $ tar -zxf session9_OpenMP.tgz

In the resulting directory, you will find a number of files, including
``Makefile``, ``driver.cpp``, ``vectors.cpp``.  You may have noticed
that in listing your set of modules, the g++ compiler was not
listed. Although it is available on Stampede by default, we will take
this opportunity to use a different compiler. The compiler icc is the
C++ compiler by Intel, which is the default on Stampede. 

You can compile the executable driver.exe with the Intel compiler and
OpenMP using the command 

.. code-block:: bash

   $ icc -openmp driver.cpp vectors.cpp -lm -o driver.exe

or by typing ``make`` on the command line (you will notice the above
lines in the ``Makefile``). This builds a threaded version of the code
using OpenMP directives that have been added on to a set of
otherwise-serial vector arithmetic operations. We note that different
compilers use different flags to enable OpenMP: the PGI compilers use
the ``-mp`` flag, and the GNU compilers use the ``-fopenmp`` flag, instead of
Intel's ``-openmp`` flag.

Open the file ``vectors.cpp`` with your editor of choice
(e.g. ``gedit``). You will notice that the loops in each subroutine in
this file have been parallelized with OpenMP directives. 


Running OpenMP programs
"""""""""""""""""""""""""""

Run the executable ``driver.exe`` from the command line:

.. code-block:: bash

   $ ./driver.exe

In fact, this executable did not run using more than one thread, since
the default behavior of OpenMP programs on Stampede is to only use a
single thread. 

To change the number of threads used by our program, we must adjust
the ``OMP_NUM_THREADS`` environment variable. First, verify that this is
set to the default value of 1 (or it may be blank): 

.. code-block:: bash

   $ echo $OMP_NUM_THREADS

Recalling from session 2 of the workshop, the method for re-setting
this environment variable will depend on our login shell. For CSH/TCSH
users, 
.. code-block:: bash

   $ setenv OMP_NUM_THREADS 2

will adjust this variable to 2; the same may be accomplished in BASH/SH/KSH users with the command

.. code-block:: bash

   $ export OMP_NUM_THREADS=2

Run ``driver.exe`` first using 1 and then using 2 OpenMP
threads. Notice the speedup when running with multiple threads. 


Setting up batch submission scripts
"""""""""""""""""""""""""""""""""""""

We executed the above runs on one of the login nodes of Stampede, you
can see which one you are on with 

.. code-block:: bash

   $ echo $HOSTNAME

However, this login node is shared by a large number of users, you can
see them all with the command 

.. code-block:: bash

   $ finger

As a result of their increased load, it may not be possible to trust
the timings reported by running programs on the login nodes of a
shared cluster (and more to the point, the system administrators prefer
that you don't tie them up with computations).

We may instead request a dedicated node of the cluster to run our
executable, without any interference by other users. Since many users
may be requesting nodes of the cluster to run their jobs (and since
many of those jobs may require hundreds of nodes at a time), their
access is regulated by a queueing system. Stampede uses the SLURM
batch environment to manage its job queue. Instead of merely running
an executable, you submit your executable to the batch system using a
batch submission script. 

Sample batch submission scripts are located in the files t1.job and
t8.job. The first line of each file specifies that the shell script
commands contained therein conform to the BASH shell. Lines beginning
with #SBATCH denote arguments that are specified to the SLURM batch
system. Of the ones in these files, students in this class should
leave all entries alone except the following: 

* ``-J`` -- specifies the name that you want to give to your job. A
  unique name here for each of your jobs can be helpful when you list
  your jobs waiting in the queue. 
* ``-o`` -- specifies the name of the output file where you want to
  write everything that would typically be displayed on the
  screen. You should specify unique names here for every run you want
  to perform, so that your tests do not overwrite one another. Note my
  use of the %j variable, which appends the ID of each job as a
  suffix, this is quite helpful in keeping the files from being
  overwritten. 
* ``-p`` -- specifies the queue to be used on the job. For OpenMP-only
  programs, do not edit this line. We will revisit this topic when we
  get to MPI. 
* ``-t`` -- specifies the run time limit you wish to specify for your
  program. You should over-estimate the required run time so that
  Stampede does not kill your program prematurely. However, you
  shouldn't over-estimate it by too much, or the queueing system will
  force your job to wait longer before running it. 

The remaining lines contain shell commands that we would use to set up
and run our executable. We note that in these files, we set up the
systems to run either 1 or 8 OpenMP threads. Moreover, we run our
executable in the program ``ibrun``, which handles submission of
parallel jobs. Note: any path names that you specify in this file
should be relative to the directory where you submit the job, hence
our use of ``./driver.exe`` .

More information on Stampede's batch system is available `here
<http://www.tacc.utexas.edu/user-services/user-guides/stampede-user-guide#running>`_. 


Running tests
"""""""""""""""""

Create input scripts that will run the program ``driver.exe`` using 1,
2, 3, ..., and 8 OpenMP threads. Each of these should have a unique
name, and should have the relevant variables set appropriately. 

You submit a job to the scheduler with the ``sbatch`` command. For
example, we can submit the job specified by the input script
``t8.job`` with the command 

.. code-block:: bash

   $ sbatch t8.job

This submits your request to the batch system. You may submit many
jobs to the queue at once -- the queueing system will keep track of
them, running each one as specified in your input script. 

You can check the status of your jobs by either looking in your run
directory (if the output files for your job have appeared, the job has
started), or by using the ``squeue`` command, followed by the flag
``-u <username>`` . For example, I could check on my queued jobs with
the command 

.. code-block:: bash

   $ squeue -u tg457291

A typical output from squeue will be something like:

.. code-block:: bash

   JOBID	PARTITION	NAME	USER	ST	TIME	NODES	NODELIST(REASON)
   35543	serial	driver.e	tg457291	R	0:02    1	c557-202

Which signifies that the job named ``driver.exe`` (abbreviated to
``driver.e`` in the output from ``squeue`` above) has process ID
35543, was run by the user ``tg457291``, is currently running in the
queue ``serial`` (i.e. no MPI), will require 1 node, and has currently
been running for 2 seconds. 

We note that if a job is designed to use any part of a node, Stampede
will dedicate the entire 16-core node to the job.  Hence both of these
jobs, which utilize 1 and 8 cores, respectively, will be charged as if
they were using 16 cores at once. 

If for any reason you wish to remove a job from the queue, you may use
the ``scancel`` command. For example, if we wished to remove the job
``driver.e`` from the queue above, we would use the command 

.. code-block:: bash

   $ scancel 35543

Once your code runs, the results that were typically written to the
screen ("Final rms norm" and "Total run time") will be included in the
file you specified in your job submission script with the ``-o``
argument.





MPI
^^^^^^^^^^


**Update for SMUHPC**


Retrieve the files for this portion of the session by clicking
:download:`this link <code/session9_MPI.tgz>` or by copying them
on SMUHPC at the command line:

.. code-block:: bash

   $ cp ~dreynolds/SMUHPC_workshop/session9_MPI.tgz .

Unpack these files as usual

.. code-block:: bash

   $ tar -zxf session0_MPI.tgz


Compiling MPI Programs on SMUHPC (hard way)
"""""""""""""""""""""""""""""""""""""""""""""""

Unlike OpenMP, MPI is a library and is not built into a compiler. This
means that someone has written a set of functions/subroutines to take
care of all message passing capabilities that are available in MPI. As
with any program that uses external functions/subroutines, we must
link them in during compilation of our program. 

Open the file ``driver.cpp``. Notice that the first place where our
program uses MPI is in line 10, where we have the statement 

.. code-block:: c

   #include "mpi.h"

This tells the C++ compiler that we will include a file named
``mpi.h``, that describes the interfaces by which C++ programs access
these MPI functions, datatypes, and reserved variables
(e.g. ``MPI_COMM_WORLD`` on line 22). 

If we were to compile our program manually, we would need to tell the
``g++`` compiler where to look for this module file, using the
compiler argument ``-I<include_path>``. On SMUHPC, we use the MPICH
implementation of MPI, which is installed in the
``/usr/local/mpich2-1.4.1p1`` directory, hence the compile command
would be: 

.. code-block:: bash

   $ g++ driver.cpp -lm -o driver.exe -I/usr/local/mpich2-1.4.1p1/include

Unfortunately, when you try that out, the compiler still doesn't know
how to link with the internal MPI functions and subroutines that we
call in our routine. We therefore need to also specify the libraries
to link in. Clearly, this process can become quite tedious, since the
MPICH library itself can depend on other libraries to link with when
producing a final executable... 


Compiling MPI Programs on SMUHPC (easy way)
"""""""""""""""""""""""""""""""""""""""""""""

Specifying the specific instructions for including and linking to an
MPI library is not always easy: 

* You must know where all of the relevant libraries are installed on
  each computer. 

* You must know which specific library files are required for
  compiling a given program. 

* Sometimes, you must even know which order you need to specify these
  specific library files in the linking line. 

Thankfully, MPI library writers typically include MPI wrapper scripts
to do most of this work for you. Such scripts are written to encode
all of the above information that is required to use MPI with a given
compiler on a specific system. 

Depending on your programming language and the specific MPI
implementation, these wrapper scripts can have different names. The
typical names for these MPI wrapper scripts are below: 

* C++: ``mpicxx`` or ``mpiCC`` or ``mpic++`` or ``openmpicxx``

* C: ``mpicc`` or ``openmpicc``

* Fortran 90/95: ``mpif90`` or ``openmpif90``

* Fortran 77: ``mpif77`` or ``openmpif77``

Compile your program ``driver.cpp`` on SMUHPC with the ``mpicxx`` wrapper script:

.. code-block:: bash

   $ mpicxx driver.cpp -lm -o driver.exe

This is much easier than doing it all by hand, don't you think?

Depending on the MPI installation, these wrapper scripts may be in
your default ``PATH`` or not. If not, you will need to find the
directory where these are installed; on SMUHPC these are in the
directory ``/usr/local/bin``, so they should already be in your path,
though that is not necessarily true for all systems. Once you find
these on a system (if they exist), you should use these to compile, as
opposed to finding the library names, module names, and relevant paths
for each to compile your MPI programs. 

I would strongly recommend that if the ``/usr/local/bin`` directory is
not already in your ``PATH`` on SMUHPC, you should add it.  Look up
how in session 2 from this workshop.


Running MPI Programs on Zeno
""""""""""""""""""""""""""""""

When running jobs on a dedicated parallel cluster (like zeno),
parallel jobs and processes are not regulated through a queueing
system. This has some immediate benefits: 

* You never have to wait to run a program.

* It is easy to set up and run parallel jobs.

* You have complete control over which processors are used in a
  parallel computation. 

However, dedicated clusters also have some serious deficiencies:

* A single user can hog all of the resources.

* More than one job can be running on a processor at a time, so
  different processes must fight for system resources (giving
  unreliable timings or memory availability).

* The more users there are, the worse these problems become.

However, running parallel programs on such a system can be very
simple, though the way that you run these jobs will depend on which
MPI implementation you are using.

On zeno, we use MPICH version 2, which gives us an available 16
physical CPU cores, though in fact each of these floating-point cores
is attached to two integer processing units, which means that the OS
thinks of zeno as having 32 processors (though unfortunately this
rarely ever gives speedups over 16x). Parallel jobs are run with the
MPICH script mpiexec. The calling syntax of mpiexec is 

.. code-block:: bash

   $ mpiexec <mpiexec_options> <program_name> <program_options>

The primary mpiexec option that we will use on zeno is ``-n p``, that
tells ``mpiexec`` how many processors (p) to use in running the
parallel job. 

Run the program ``driver.exe`` using 1 process:

.. code-block:: bash

   $ mpiexec -n 1 ./driver.exe

Note: if you did not yet add ``/usr/local/bin`` to your ``PATH``, you will need to run the program with the full path name, ``/usr/local/bin/mpiexec``.

Run the program ``driver.exe`` using 2 process:

.. code-block:: bash

   $ mpiexec -n 2 ./driver.exe

Run the program ``driver.exe`` using 4 processes:

.. code-block:: bash

   $ mpiexec -n 4 ./driver.exe

All of these will run the MPI processes as separate threads on Zeno,
since it is a shared-memory server. 

Although zeno has 16 floating-point cores, because it is a shared
resource that also acts as the Math department file server, you should
NOT run any MPI jobs on zeno using more than 8 processes; for
long-running jobs (e.g. over 30 minutes), you should limit yourself to
using at most 4 processes per job.


Compiling on SMUHPC
"""""""""""""""""""""""

Fortunately, Stampede already has the MPI compiler script mpicxx in
your default path. However, there it uses the Intel compilers by
default (whereas zeno uses the GNU compilers by default). Compile the
program ``driver.cpp``: 

.. code-block:: bash

   $ mpicxx driver.cpp -lm -o driver.exe

Because Stampede has a queueing system to regulate who uses the
computing resources, and how much individuals can use, you cannot run
your program using mpiexec. Instead, for parallel MPI jobs you must
use a batch submission file (as we did when running OpenMP jobs). 


More Advanced Batch Submission Scripts
"""""""""""""""""""""""""""""""""""""""""

Stampede consists of over 6,400 nodes, each with 16 Intel Xeon E5 SMP
cores and 32 GB of RAM, providing for a total of over 100,000 CPU
cores and 200 TB of memory. In addition, most of these nodes also have
an Intel Xeon Phi Coprocessor. While we will not have a session using
these Phi coprocessors, it is highly recommended that you look into
these further, potentially making use of them within your projects. 

All of our previous work on Stampede has been on one node at a time,
using anywhere from 1 to 16 of the available SMP cores on the
node. With MPI, we may now (theoretically) use up to the full 100,000
CPU cores on the machine (do not attempt to do this). 

Running MPI jobs on stampede is almost identical to running OpenMP
batch jobs. However, when running MPI jobs, we must tell the queueing
system a few relelvant pieces of information, in addition to what we
had previously specified: 

* How many total nodes we want to use on the machine?

* How many total MPI tasks do we want to use?

These two pieces of information are specified on the two lines in the batch submission script,

.. code-block:: bash

   #SBATCH -N NUM
   #SBATCH -n num

Here, the two numbers ``NUM`` and ``num`` specify:

* How many nodes we want to use on the machine (``NUM``), and

* How many total MPI tasks we wish to use on the machine (``num``).

Clearly, if you specify a value of ``num`` that is more than 16x
larger than your value of ``NUM`` it will not work properly, since you
will be requesting more MPI tasks than you have requested physical
processes. 

Let us consider a few examples:

1. num = 1 and NUM = 1

   This approach will use one node, and one core on that node, i.e. it
   will run one MPI process. We will be charged for 16 cores, i.e. if
   we run for one hour, we will be charged for 16. This is how we've
   been running our OpenMP jobs 

2. num = 2 and NUM = 1

   This approach will use one node, and two cores on that node,
   i.e. it will run two MPI processes. We will be charged for 16
   cores. 

3. num = 2 and NUM = 2

   This approach will use two nodes, and one core on each node,
   i.e. it will also run two MPI processes. We will be charged for 32
   cores. 

4. num = 48 and NUM = 6

   This approach will use six nodes forty-eight MPI processes,
   resulting in using 8 cores on each node. Though since we are using
   6 nodes, we will be charged for 6*16=96 cores. 

A relevant question is, why would anyone use anything less than the
full 16 cores per node for a job, especially since you will be charged
for using all 16 cores on each node anyway? There are a number of
reasons why you might do this: 

* You may need more than 2 GB of memory per MPI process, so you
  couldn't even fit 16 processes on a single node. 

* Your algorithm may be memory bandwidth limited, so you may not be
  able to effectively use more than, say 4 cores/node. However, since
  each node is it's own computer (with local cache and memory bus),
  adding on more nodes results in more effective cache and memory
  bandwidth. 

* You may wish to run your program using a hybrid of MPI and OpenMP,
  so even though you launch fewer MPI tasks than you have cores
  available, these could be filled up with threads launched by each
  MPI process. 

Look at the three files ``p1.job``, ``p2a.job`` and ``p2b.job``. These
run the first three examples discussed above. In these, we have added
in the following options: 

* ``#SBATCH --mail-type=ALL``

  This flag tells the system to email us when the job begins, ends, aborts or is suspended.

* ``#SBATCH -mail-user=user@address.edu``

  This flag tells the system which email address to use in sending the above emails.

You will also notice that, unlike our earlier submission scripts, we
have removed the line specifying OMP_NUM_THREADS. This makes sense
because we are no longer using OpenMP. 

Set up submission scripts to run the executable driver.exe using 1, 2,
4, 8, 16, 32 and 64 cores. For the 1, 2, 4, and 8 processor jobs, just
use one node. For the 16 processor job, try it both using 16 cores on
one node and 8 cores on 2 nodes. Run the 32 and 64 processor jobs
using 16 cores/node.
