:tocdepth: 3


.. _session5:

*****************************************************
Session 5: Using ManeFrame
*****************************************************

Before beginning this session, first retrieve the corresponding set of
files either through :download:`clicking here <code/session5.tgz>` or
by copying the relevant files at the command line:

.. code-block:: bash

   $ cp ~dreynolds/SMUHPC_tutorial/session5.tgz .

Unzip this file, and enter the resulting subdirectory

.. code-block:: bash

   $ tar -zxf session5.tgz
   $ cd session5

Additionally, we need to set up our environment correctly:

.. code-block:: bash

   $ module load python



What is ManeFrame?
================================================

While we've technically been using the ManeFrame cluster throughout
this workshop, in reality we've only been using standard Linux tools
as if ManeFrame were a single computer.  The real value in the
ManeFrame cluster is its ability to run jobs that span multiple
processors at once (a.k.a. *high performance computing* or *parallel
computing*), and/or its ability to run large numbers of simultaneous
jobs (a.k.a. *high throughput computing*).

In fact, SMU has two shared large-scale computing resources, ManeFrame
and SMUHPC; each is optimized for slightly different types of
calculations: 

* SMUHPC -- Originally purchased in 2009 through funding from the `SMU
  Provost's office <http://smu.edu/provost/#1>`_, and subsequently
  added onto through grants to the `Physics
  <http://www.smu.edu/physics>`_ and `Mathematics
  <http://www.smu.edu/math>`_ departments, the SMUHPC cluster is a
  shared resource available to all SMU faculty and
  graduate/undergraduate students involved in scientific computing
  projects.  It is administered by the `SMU Office of Information
  Technology <http://www.smu.edu/BusinessFinance/OIT>`_, and managed
  by the `SMU Center for Scientific Computation
  <http://www.smu.edu/Academics/CSC>`_.

  While SMUHPC has components suited for both parallel computing and
  high-throughput computing, the vast majority of SMUHPC was built
  with a slower network, making it ideally suited for high-throughput
  serial computing.  In addition, SMUHPC utilizes the `Condor
  <http://research.cs.wisc.edu/htcondor/>`_ job scheduler, that was
  originally designed and optimized for high-throughput computing.
  While parallel computing is indeed possible in Condor, it is
  typically much more difficult to use for this purpose than other
  schedulers.

* ManeFrame -- Our newest member of HPC at SMU, `Maneframe
  <https://blog.smu.edu/forum/2014/03/26/smu-welcomes-its-new-supercomputer-maneframe/>`_, 
  was acquired in 2014 through an award by the `US Department of
  Defense HPC Modernization Program <http://www.hpc.mil/index.php>`_
  to a group of faculty from across the SMU campus.

  While ManeFrame may be used for both high-throughput and parallel
  computing, it is built with a high-speed and low-latency
  communication network that connects all computing nodes, making it
  ideally suited for parallel computing.  As a result, ManeFrame
  utilizes the `SLURM <https://computing.llnl.gov/linux/slurm/>`_ job
  scheduler, that was designed from scratch for large-scale parallel
  computing (and is used by many of the most powerful supercomputers
  in the world).

As this workshop focuses on ManeFrame, we will not discuss how to use
SMUHPC.  For further information on using SMUHPC, see the tutorials
from our `2014 SMU HPC Summer Workshop
<http://runge.math.smu.edu/SMUHPC_workshop_Summer14/>`_.




.. index:: ManeFrame; hardware

ManeFrame hardware
-----------------------------------

First, let's familiarize ourselves with the hardware that comprises
the ManeFrame cluster.  We can group the portions of ManeFrame into a few
simple categories: *login nodes*, *worker nodes* and *disk nodes*.


Login nodes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Typically, users only directly interact with the "login" nodes on
ManeFrame.  These are the nodes that users log into when accessing
ManeFrame, and are where users request other resources for running
jobs. 

* The main login nodes are ``mflogin01.hpc.smu.edu`` and
  ``mflogin02.hpc.smu.edu``.  These are used for compiling source
  code, setting up job submission files, input files, transferring
  data to/from ManeFrame, etc..  These nodes **should not** be used
  for running computationally intensive calculations. 
* Additionally, ``mflogin03.hpc.smu.edu`` and
  ``mflogin04.hpc.smu.edu`` are not currently available, but will be
  provisioned in the near future. 


Worker nodes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The worker, or compute, nodes are where jobs should be run on the
ManeFrame cluster:

* The worker nodes are ``mfc0001.hpc.smu.edu`` through ``mfc1104.hpc.smu.edu``:

  * 20 of these are "high memory" nodes, with 192 GB of RAM each.
    Access to these nodes is obtained through requesting a specific
    queue type as described later in this tutorial session.

  * 1084 of these are "normal" nodes, with 24 GB of RAM each.  Access
    to these nodes is obtained through request to a specific queue
    type.

* All nodes have 8-core Intel(R) Xeon(R) CPU X5560 @ 2.80GHz 107
  processors. 

* All nodes are connected by a 20Gbps DDR InfiniBand connection to the
  core backbone InfiniBand network.

* Unless reserved for an interactive job, users *cannot directly log
  in* to the worker nodes.

.. note:: Of the 1104 compute nodes, not all are currently running in
	  production.  Administration efforts are under way to fix the
	  remaining hardware issues and bring them online as
	  available.



Disk nodes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

All ManeFrame compute and login nodes may access a set of network
filesystems, these filesystems reside on a set of separate dedicated
disk nodes. 

* **HOME**: Home directories on ManeFrame reside on a NFS file system,
  located in ``/users`` (e.g. ``/users/dreynolds``).  When you log in
  to ManeFrame your home directory is where you will land in by default.

  This space should be used to write, edit, compile, and browse your
  programs and job submission scripts.  You can use programs in your
  HOME to test them interactively on an interactive node obtained from
  the job scheduler, as described below.

  .. note:: ``$HOME`` space is restricted by quotas, because of
	    limited space (2.5 TB total).  Due to these storage
	    constraints, this space is intended to preserve your
	    important programs and job submission scripts.  You
	    **should not** use this space to store large data/input
	    files that can be reproduced or dowloaded again.  *Please
	    refrain from moving large data files to/from your home
	    directory*, instead move them to ``/scratch``. 

* **SCRATCH**: The largest pool of storage on ManeFrame is located in
  ``/scratch/users/``; all users have a dedicated directory there
  (e.g. ``/scratch/users/dreynolds``).  ManeFrame has approximately
  1.2 PB of scratch space, that uses a high performance parallel
  *Lustre* file system.  

  This space should serve as your default location for storage of
  large files or for large numbers of files used in computation.  

  Additionally, all users have a directory named
  ``/scratch/users/<username>/_small``.  This directory corresponds
  with a smaller (~250 TB) high-speed scratch filesystem.
  Performance-wise, the SATA disks comprising
  ``/scratch/users/<username>`` operate at 400 MB/s, whereas the SAS
  disks comprising ``/scratch/users/<username>/_small`` operate at 450
  MB/s.

  As a high-performance parallel filesystem, Lustre will not perform
  well if misused.  Further information on interacting with Lustre
  will be discussed later in this tutorial.

  .. note:: SCRATCH is a *volatile* file system, meaning we do not
	    guarantee that any of the files stored in SCRATCH can be
	    retrieved or restored in the event of an accidental
	    delete, loss or failure of the filesystem.  Users are
	    therefore encouraged to save their programs, job
	    submission scripts and other non-reproducible files in
	    $HOME or any other secondary storage system.

* **NFSSCRATCH**: ManeFrame additionally has a set of "fast" storage,
  located in ``/nfsscratch/users/``.  These SSD drives have
  approximately ~2.2 TB of storage, and use a high performance NFS
  file system.  Use of this storage space requires an approval from
  the Director of the Center for Scientific Computation, Dr. Thomas
  Hagstrom.  

  .. note:: due to size and the premium nature of this file system,
	    users are required to automatically clean up the storage
	    space after every job has finished running by bundling and
	    moving the resulting files as part of the 'epilog' process
	    of the job.

* **LOCAL_TEMP**: ManeFrame's worker nodes may also access a relative
  large amount of *local* temporary space for use during the executing
  of a job, located in ``/local_temp/users/``.  For example, when
  running the *Gaussian* application, files of size 100-400 GB are
  periodically dumped during the execution of a job.

* **SOFTWARE**: All ManeFrame nodes may access a shared NFS disk that
  holds software, located in ``/grid/software``.  A typical user will
  never need to browse this directly, as the *module* system modifies
  environment variables to point at these installations automatically.

 
Users are encouraged to contact smuhpc-admins@smu.edu with
questions regarding selecting the appropriate storage for their jobs.



.. index:: ManeFrame; general information

General information
--------------------------------------------------

* OS: Scientific Linux 6 (64 bit)

* Scheduler: SLURM

* The software stack on ManeFrame includes a variety of high
  performance mathematics and software libraries, as well as the GNU 
  and PGI compiler suites.  A full listing is always available with
  the ``module avail`` command.

* The ManeFrame `wiki page
  <https://wiki.smu.edu/display/smuhpc/ManeFrame>`_ (requires SMU login)
  has more detailed information on the hardware and software
  configuration of the cluster.



.. index:: LUSTRE

Getting along with LUSTRE
===========================

[Amit]







.. index:: SLURM, job scheduler

The SLURM job scheduler
================================================

In this tutorial we'll focus on running serial jobs (both batch and
interactive) on ManeFrame (we'll discuss parallel jobs in later
tutorial sessions).

In general, a *job scheduler* is a program that manages unattended
background program execution (a.k.a. *batch processing*).  The basic
features of any job scheduler include:

* Interfaces which help to define workflows and/or job dependencies.

* Automatic submission of executions.

* Interfaces to monitor the executions.

* Priorities and/or queues to control the execution order of unrelated
  jobs.

In the context of high-throughput and high-performance computing, the
primary role of a job scheduler is to manage the job queue for all
of the compute nodes of the cluster.  It's goal is typically
to schedule queued jobs so that all of the compute nodes are utilized
to their capacity, yet doing so in a fair manner that gives priority
to users who have used less resources and/or contributed more to the
acquisition of the system.  

Some widely used cluster batch systems are:

.. index:: 
   seealso: SLURM; job scheduler

* `Simple Linux Utility for Resource Management (SLURM)
  <http://slurm.schedmd.com/>`_ -- this is used on ManeFrame

.. index:: 
   seealso: Moab; job scheduler

* `Moab <http://docs.adaptivecomputing.com/mwm/help.htm#topics/0-intro/productOverview.htm>`_

.. index:: 
   seealso: Torque; job scheduler

* `Torque <http://www.adaptivecomputing.com/products/open-source/torque/>`_

.. index:: 
   seealso: LoadLeveler; job scheduler

* `LoadLeveler <http://www-03.ibm.com/systems/software/loadleveler/index.html>`_

.. index:: 
   seealso: condor; job scheduler

* `Condor <http://research.cs.wisc.edu/htcondor/>`_ -- this is used on
  the older SMUHPC cluster

.. index:: 
   seealso: Oracle grid engine; job scheduler

* `Oracle Grid Engine <http://www.oracle.com/us/products/tools/oracle-grid-engine-075549.html>`_

.. index:: 
   seealso: Argent job scheduler; job scheduler

* `Argent Job Scheduler <http://help.argent.com/#product_downloads_job_scheduler>`_

.. index:: 
   seealso: Platform LSF; job scheduler

* `Platform LSF <http://www-03.ibm.com/systems/technicalcomputing/platformcomputing/products/lsf/>`_


.. note::

   While the remainder of this session will focus on using SLURM
   for batch and interactive computing, the ideas represented here
   apply to nearly all of the scheduling systems listed above.  As a
   result, even if you never plan to use SLURM in your research, the
   rest of this lesson will lay a strong foundation for transitioning
   to schedulers on other HPC systems. 


.. index:: SLURM, partitions

ManeFrame's SLURM partitions/queues
--------------------------------------------------

There are currently 3 types of partitions (or queues) set up on
ManeFrame.  As more users move to ManeFrame, we plan to redefine or
create new ones as needed to optimize usage.

1. **interactive**:  This is the *default queue* if none specified.
   Currently 7 compute nodes are defined for interactive use.  Based
   on the usage of the interactive queue and load on the system, more
   resources can be added for interactive use dynamically.  

2. **highmem**: This queue currently has the 20 "high memory" nodes
   (with 192 GB RAM each).

3. **parallel**:  All of the remaining nodes belong to this queue.
   Do not let the name confuse you -- this queue is capable of runnin
   single processor core jobs, multi-core jobs and even
   multi-node-multi-core parallel jobs.




SLURM commands
--------------------------------------------------

While there are a `multitude of SLURM commands
<https://computing.llnl.gov/linux/slurm/documentation.html>`_,
here we'll focus on those applicable to running batch and interactive jobs:

.. index:: SLURM; sinfo

* ``sinfo`` -- displays information about SLURM nodes and partitions
  (queue types).  A full list of options is available `here
  <https://computing.llnl.gov/linux/slurm/sinfo.html>`_.  The usage
  command (with the most-helpful optional arguments in brackets) is

  .. code-block:: bash

     $ sinfo [-a] [-l] [-n <nodes>] [-p <partition>] [-s] [-a] [-a] [-a]

  where these options are:

  * ``-a`` or ``--all`` -- Display information about all partitions

  * ``-l`` or ``--long`` -- Displays more detailed information

  * ``-n <nodes>`` or ``--nodes <nodes>`` -- Displays information only
    about the specified node(s).  Multiple nodes may be comma
    separated or expressed using a node range expression. For example
    ``mfc[1005-1007].hpc.smu.edu`` would indicate three nodes,
    ``mfc1005.hpc.smu.edu`` through ``mfc1007.hpc.smu.edu``.

  * ``-p <partition>`` or ``--partition <partition>`` -- Displays
    information only about the specified partition

  * ``-s`` or ``--summarize`` -- List only a partition state summary
    with no node state details. 
    
  Examples:

  .. code-block:: bash

     $ sinfo --long -p highmem  # long output for all nodes allocated to the "highmem" partition
     $ sinfo -s                 # summarizes output on all nodes on all partitions


.. index:: SLURM; squeue

* ``squeue`` -- views information about jobs located in the SLURM
  scheduling queue.  A full list of options is available `here
  <https://computing.llnl.gov/linux/slurm/squeue.html>`_.  The usage
  command (with the most-helpful optional arguments in brackets) is

  .. code-block:: bash

     $ squeue [-a] [-j] [-l] [-p] [--start] [-u]

  where these options are:

  * ``-a`` or ``--all`` -- Display information about jobs and job
    steps in all partions.

  * ``-j <job_id_list>`` or ``--jobs <job_id_list>`` -- Requests a
    comma separated list of job ids to display. Defaults to all jobs.  

  * ``-l`` or ``--long`` -- Reports more of the available information
    for the selected jobs or job steps, subject to any constraints
    specified.

  * ``-p <part_list>`` or ``--partition <part_list>`` -- Specifies the
    partitions of the jobs or steps to view. Accepts a comma separated
    list of partition names.

  * ``--start`` -- Reports the *expected* start time of pending jobs,
    in order of increasing start time.

  * ``-u <user_list>`` or ``--user <user_list>`` -- Requests jobs or
    job steps from a comma separated list of users. The list can
    consist of user names or user id numbers.  

  Examples:

  .. code-block:: bash

     $ squeue                            # all jobs
     $ squeue -u dreynolds --start       # anticipated start time of dreynolds' jobs
     $ squeue --jobs 12345,12346,12348   # information on only jobs 12345, 12346 and 12348



.. index:: SLURM; sbatch

* ``sbatch`` -- submits a batch script to SLURM.  A full list of options is available `here
  <https://computing.llnl.gov/linux/slurm/sbatch.html>`_.  The usage
  command is

  .. code-block:: bash

     $ sbatch [options] <script> [args]

  where ``<script>`` is a *batch submission script*, and ``[args]``
  are any optional arguments that should be supplied to ``<script>``.
  The ``sbatch`` command accepts a multitude of options; these options
  may be supplied either at the command-line or inside the batch
  submission script.  

  It is recommended that all options be specified *inside* the batch
  submission file, to ensure reproducibility of results (i.e. so that
  the same options are specified on each run, and no options are
  accidentally left out).  Any command-line ``sbatch`` option may
  equivalently be specified within this script (at the top, before any
  executable commands), preceded by the text ``#SBATCH``.

  These options are discussed in the following section,
  :ref:`batch_file`. 

  Examples:

  .. code-block:: bash

     $ sbatch ./myscript.sh    # submits the batch submission file "myscript.sh" to SLURM


.. index:: SLURM; srun

* ``srun`` -- runs a parallel or interactive job on the worker nodes.
  A full list of options is available `here
  <https://computing.llnl.gov/linux/slurm/srun.html>`_.  The usage
  command (with the most-helpful optional arguments in brackets) is 

  .. code-block:: bash

     $ srun [-D <path>] [-e <errf>] [--epilog=<executable>] [-o <outf>] [-p <part>] [--pty] [--x11] <executable>

  where these options are:

  * ``-D <path>`` or ``--chdir=<path>`` -- have the remote processes
    change directories ``<path>`` before beginning execution. The default is to
    change to the current working directory of the ``srun`` process.  

  * ``-e <errf>`` or ``--error=<errf>`` -- redirects stderr to the file ``<errf>``

  * ``--epilog=<executable>`` -- run ``executable`` just after the job
    completes.  The command line arguments for ``executable`` will be
    the command and arguments of the job itself.  If ``executable`` is
    "none", then no epilog will be run. 

  * ``-I`` or ``--immediate[=secs]`` -- exit if requested resources
    not available in "secs" seconds (useful for interactive jobs).

  * ``-o <outf>`` or ``--output=<outf>`` -- redirects stdout to the file ``<outf>``

  * ``-p <part>`` or ``--partition=<part>`` -- requests that the job
    be run on the requested partition.

  * ``--pty`` -- requests that the task be run in a pseudo-terminal

  * ``-t <min>`` or ``--time=<min>`` -- sets a limit on the total run
    time of the job. The default/maximum time limit is defined on a
    per-partition basis.

  * ``--x11=[batch|first|last|all]`` -- exports the X11 display from
    the first|last|all allocated node(s), so that graphics displayed
    by this process can be forwarded to your screen.

  * ``<executable>`` -- the actual program to run.

  Examples:

  .. code-block:: bash

     $ srun -p parallel /bin/program  # runs executable /bin/program on "parallel" partition
     $ srun --x11=first --pty emacs  # runs "emacs" and forwards graphics
     $ srun --x11=first --pty $SHELL # runs a the user's current shell and forwards graphics


.. index:: SLURM; salloc

* ``salloc`` -- obtains a SLURM job allocation (a set of nodes),
  executes a command, and then releases the allocation when the
  command is finished.  A full list of options is available `here
  <https://computing.llnl.gov/linux/slurm/salloc.html>`_.  The usage
  command is

  .. code-block:: bash

     $ salloc [options] <command> [command args]

  where ``<command> [command args]`` specifies the command (and any
  arguments) to run.  Available options are almost identical to
  ``srun``, including:

  * ``-D <path>`` or ``--chdir=<path>`` -- change directory to
    ``<path>`` before beginning execution.

  * ``-I`` or ``--immediate[=secs]`` -- exit if requested resources
    not available in "secs" seconds (useful for interactive jobs).

  * ``-p <part>`` or ``--partition=<part>`` -- requests that the job
    be run on the requested partition.

  * ``-t <min>`` or ``--time=<min>`` -- sets a limit on the total run
    time of the job. The default/maximum time limit is defined on a
    per-partition basis.

  * ``--x11=[batch|first|last|all]`` -- exports the X11 display from
    the first|last|all allocated node(s), so that graphics displayed
    by this process can be forwarded to your screen.



.. index:: SLURM; scancel

* ``scancel`` -- kills jobs or job steps that are under the control of
  SLURM (and listed by ``squeue``.  A full list of options is available `here
  <https://computing.llnl.gov/linux/slurm/scancel.html>`_.  The usage
  command (with the most-helpful optional arguments in brackets) is

  .. code-block:: bash

     $ scancel [-i] [-n <job_name>] [-p <part>] [-t <state>] [-u <uname>] [jobid]

  where these options are:

  * ``-i`` or ``--interactive`` -- require response from user for each
    job (used when cancelling multiple jobs at once)

  * ``-n <job_name>`` or ``--name=<job_name>`` -- cancel only on jobs
    with the specified name.

  * ``-p <part>`` or ``--partition=<part>`` -- cancel only on jobs in
    the specified partition.

  * ``-t <state>`` or ``--state=<state>`` -- cancel only on jobs in
    the specified state.  Valid job states are ``PENDING``,
    ``RUNNING`` and ``SUSPENDED``

  * ``-u <uname>`` or ``--user=<uname>`` -- cancel only on jobs of
    the specified user (note: normal users can only cancel their own
    jobs).

  * ``jobid`` is the numeric job identifier (as shown by ``squeue``)
    of the job to cancel.

  Examples:

  .. code-block:: bash

     $ scancel 1234  # cancel job number 1234
     $ scancel -u dreynolds  # cancel all jobs owned by user "dreynolds"
     $ scancel -t PENDING -u joe  # cancel all pending jobs owned by user "joe"





Example: running interactive jobs
======================================

In this example, we'll interactively run the Python scrpit
``myjob.py``, that performs a simple algorithm for approximating
:math:`\pi` using a composite trapezoidal numerical integration
formula to approximate 

.. math::

   \int_0^1 \frac{4}{1+x^2}\,\mathrm dx

This script accepts a single integer-valued command-line argument,
corresponding to the number of subintervals to use in the
approximation, with the typical tradeoff that *the harder you work, the
better your answer*.

While you can run this at the command line:

.. code-block:: bash

   $ python ./myjob.py 50

as we increase the number of subintervals to obtain a more accurate
approximation it can take longer to run, so as "good citizens" we
should instead run it on dedicated compute nodes instead of the shared
login nodes.  

Before running this script on a compute node, we need to ensure that
``myjob.py`` has "executable" permissions:

.. code-block:: bash

   $ chmod +x ./myjob.py 

We'll use ``srun`` to run this script interactively for interval
values of {50,500,5000,50000}.  For each run, we'll direct the output
to a separate file:

.. code-block:: bash

   $ srun -o run_50.txt ./myjob.py 50
   $ srun -o run_500.txt ./myjob.py 500
   $ srun -o run_5000.txt ./myjob.py 5000
   $ srun -o run_50000.txt ./myjob.py 50000

Upon completion you should have the files ``run_50.txt``,
``run_500.txt``, ``run_5000.txt`` and ``run_50000.txt`` in your
directory.  View the results to ensure that things ran properly:

.. code-block:: bash

   $ cat run_*

.. note:: in the above commands we do not need to directly specify to
	  run on the "interactive" SLURM partition, since that is the
	  default partition.





.. index:: SLURM job submission file

.. _batch_file:

Batch job submission file
===============================

The standard way that a user submits batch jobs to run on SLURM is
through creating a *job submission file* that describes (and executes)
the job you want to run.  This is the ``<script>`` file specified to
the ``sbatch`` command.

A batch submission script is just that, a shell script.  You are
welcome to use your preferred shell scripting language; in this
tutorial we'll use BASH (as we used in :ref:`tutorial session 2
<session2>`).  As a result, the script typically starts with the line

.. code-block:: bash

   #!/bin/bash

The following lines (before any executable commands) contain the
options to be supplied to the ``sbatch`` command.  Each of these
options must be prepended with the text ``#SBATCH``, e.g.

.. code-block:: bash

   #!/bin/bash
   #SBATCH -J my_program       # job name to display in squeue
   #SBATCH -o output-%j.txt    # standard output file
   #SBATCH -e error-%j.txt     # standard error file
   #SBATCH -p parallel         # requested partition
   #SBATCH -t 180              # maximum runtime in minutes

Since each of these ``sbatch`` options begins with the character
``#``, they are treated as comments by the BASH shell; however
``sbatch`` parses the file to find these and supply them as options
for the job.

After all of the requested options have been specified, you can supply
any number of executable lines, variable definitions, and even
functions, as with any other BASH script.

Unlike general BASH scripts, there are a few SLURM replacement symbols
(variables) that may be used within your script:

* ``%A`` -- the master job allocation number (only meaningful for job
  arrays (advanced usage))
* ``%a`` -- the job array ID (index) number (also only meaningful for job arrays)
* ``%j`` -- the job allocation number (the number listed by ``squeue``)
* ``%N`` -- the node name. If running a job on multiple nodes, this will 
  map to only the first node on the job (i.e. the node that actually runs the
  script).
* ``%u`` -- your username

The available options to ``sbatch`` are `numerous
<https://computing.llnl.gov/linux/slurm/sbatch.html>`_.  Here we list
the most useful options for running serial batch jobs.

* ``-D <dir>`` or ``--workdir=<dir>`` -- sets the working directory
  where the batch script should be run, e.g. 

  .. code-block:: bash

     #SBATCH -D /scratch/users/ezekiel/test_run
 
* ``-J <name>`` or ``--job-name=<name>`` -- sets the job name as
  output by the ``squeue`` command, e.g.  

  .. code-block:: bash

     #SBATCH -J test_job
 
* ``-o <fname>`` -- sets the output file name for stdout and stderr
  (if stderr is left unspecified).  The default standard output is
  directed to a file of the name ``slurm-%j.out``, where ``%j``
  corresponds to the job ID number.  You can do something similar,
  e.g. 

  .. code-block:: bash

     #SBATCH -o output-%j.txt

* ``-e <fname>`` -- sets the output file name for stderr only.  The
  default is to combine this with stdout.  An example similar to
  ``-o`` above would be

  .. code-block:: bash

     #SBATCH -e error-%j.txt

* ``-i <fname>`` or ``--input=<fname>`` -- sets the standard input
  stream for the running job.  For example, if an executable program
  will prompt the user for text input, these inputs may be placed in a
  file ``inputs.txt`` and specified to the script via

  .. code-block:: bash

     #SBATCH -i inputs.txt

* ``-p <part>`` -- tells SLURM on which partition it should submit the
  job.  The options are "interactive", "highmem" or "parallel".  For
  example, so submit a batch job to a high-memory node you would use

  .. code-block:: bash

     #SBATCH -p parallel

* ``-t <num>`` -- tells SLURM the maximum runtime to be allowed for
  the job (in minutes).  For example, to allow a job to run for up to
  3 hours you would use

  .. code-block:: bash

     #SBATCH -t 180

* ``--exclusive`` -- tells SLURM that the job can not share nodes with
  other running jobs.

* ``-s`` or ``--share`` -- tells SLURM that the job can share nodes
  with other running jobs. This is the opposite of ``--exclusive``,
  whichever option is seen last on the command line will be used.
  This option may result the allocation being granted sooner than if
  the ``--share`` option was not set and allow higher system
  utilization, but application performance will likely suffer due to
  competition for resources within a node.

* ``--mail-user <email address>`` -- tells SLURM your email address if 
  you'd like to receive job-related email notifications, e.g.

  .. code-block:: bash

     #SBATCH --mail-user peruna@smu.edu

* ``--mail-type=<flag>`` -- tells SLURM which types of email
  notification messages you wish to receive.  Options include:

  * ``BEGIN`` -- send a message when the run starts
  * ``END`` -- send a message when the run ends
  * ``FAIL`` -- send a message if the run failed for some reason
  * ``REQUEUE`` -- send a message if and when the job is requeued
  * ``ALL`` -- send a message for all of the above

  For example,
  
  .. code-block:: bash

     #SBATCH --mail-type=all






Example: running batch jobs
======================================

In this example we'll use the second script provided in the
``session5`` directory, ``myjob.sh``.  This is a BASH script that uses
the same basic approach that you used in :ref:`session 2 <session2>`
to determine prime numbers.  The script requires a single command-line
argument, specifying how many prime numbers we wish to find before
exiting the program.


[resume changes here]


Create a new job submission file, ``test1.job`` using the editor of
your choice (e.g. ``gedit`` or ``emacs``), and fill in the arguments

.. code-block:: text

   universe     = vanilla
   getenv       = true
   log          = test1.log
   error        = test1.err
   output       = test1.out
   notification = always
   notify_user  = username@smu.edu
   executable   = myjob.py
   arguments    = 5000000
   queue

Submit this to the condor scheduler with the command

.. code-block:: bash

   $ condor_submit test1.job

View your jobs in the queue by supplying your username to
``condor_q``, e.g.

.. code-block:: bash

   $ condor_q dreynolds

(if nothing shows up, it's because the job already finished)

When the job finishes, you should see the files ``test1.log``,
``test1.err`` and ``test1.out`` in your directory.  Open these files
and view their contents.  If everything ran correctly, the error file
should be empty, the log file should have some general condor-related
information, and the output file should have our desired results.



.. index:: condor examples; multiple shared node jobs

.. _running_multiple_condor_jobs:

Running many jobs
--------------------------------------------------

Suppose now that we wanted to run this script multiple times with
different arguments, in order to experimentally measure how rapidly
the approximation to :math:`\pi` converges as we change the number of
subintervals.  

To this end, we have a few options:

1. Write separate job files for each command line argument (here, the
   number of subintervals), and submit each to condor separately.
   This has the benefit of creating a reproducible set of tests, where
   the inputs for each test are quite clear, but can take quite some
   time to set up.  

2. Reuse our existing job file, but when calling ``condor_submit`` we can
   use the ``-append`` option to modify the command line argument and
   output/log/error file names.  

   The problems with this approach are that (a) we may forget the
   command-line arguments we had to use for the different calls,
   making our results more difficult to reproduce, and (b) all results
   would be written to the same output files, obliterating results
   from all but the last run.  

   However, this could be automated by creating a BASH
   script that calls ``condor_submit`` for us multiple times, with the
   customized calls hard-coded into the script.  This would again
   allow for reproducibility.  Additionally, the **output** condor
   argument could use the **Process** macro to create separate output
   files for each run.

3. We could write a single job file that has separate blocks of
   options, each separated by a different **queue** command, allowing
   us to run multiple tests with a single submission file.  

All of the above approaches are equally valid, but we'll choose option
3 since it requires the least typing.  

Create a new condor job submission file, ``test2.job`` with the contents

.. code-block:: text

   universe     = vanilla
   getenv       = true
   log          = test2a.log
   error        = test2a.err
   output       = test2a.out
   notification = always
   notify_user  = username@smu.edu
   executable   = myjob.py
   arguments    = 500
   queue

   log        = test2b.log
   error      = test2b.err
   output     = test2b.out
   arguments  = 5000
   queue

   log        = test2c.log
   error      = test2c.err
   output     = test2c.out
   arguments  = 50000
   queue

   log        = test2d.log
   error      = test2d.err
   output     = test2d.out
   arguments  = 500000
   queue

   log        = test2e.log
   error      = test2e.err
   output     = test2e.out
   arguments  = 5000000
   queue

Note that only the first block specifies the **universe**, **getenv**
**executable**, **notification** and **notify_user**; since these will
be reused for all of our runs we do not need to change them for each
subsequent job.

Launch these jobs as before, with the command

.. code-block:: bash

   $ condor_submit test2.job

To view our results in a single command, use

.. code-block:: bash

   $ cat test2*.out



.. index:: condor examples; single whole node job

Running on a whole node 
--------------------------------------------------

All of our above tests were performed on nodes where other users' jobs
could also be running.  As previously discussed, sometimes our
computational experiments cannot be run on shared resources, e.g. if
we need reliable timings, if we need to use more than 2 GB of
RAM, or if our job will spawn additional threads as it runs to fill up
all the cores on a given node.  In such situations, we wish to request
that our job run on a node that is dedicated to our one job.

This is accomplished by adding a small number of additional arguments
to our earlier job submission file.  Let's run one of these, wherein
we will now run the executable ``myjob.sh`` on a dedicated node.  This
script also requires a command-line argument, e.g. ``n``, and it then
computes the first ``n`` prime numbers using a simplistic version of
the *trial division* algorithm.

Before submitting this script to condor, we need to ensure that
``myjob.sh`` has "executable" permissions:

.. code-block:: bash

   $ chmod +x ./myjob.sh

Create a new condor job submission file, ``test3.job`` with the contents

.. code-block:: text

   universe              = vanilla
   getenv                = true
   log                   = test3.log
   error                 = test3.err
   output                = test3.out
   executable            = myjob.sh
   arguments             = 5000
   Requirements          = CAN_RUN_WHOLE_MACHINE
   +RequiresWholeMachine = True
   queue

and launch it as usual,

.. code-block:: bash

   $ condor_submit test3.job


.. index:: sed

After the run finishes, find the 4324th prime number (on line 4326 of
``test3.out`` because of the two extra lines that condor adds to the
top of the output file) with the command

.. code-block:: bash

   $ sed -n 4326p test3.out








.. raw:: html
   :file: counter.html

