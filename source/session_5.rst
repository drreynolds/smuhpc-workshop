:tocdepth: 3


.. _session5:

*****************************************************
Session 5: Using ManeFrame
*****************************************************



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



.. index:: LUSTRE

Using the LUSTRE filesystem
--------------------------------------------------

[Amit]






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
  submission script (see the next section).  

  It is recommended that all options be specified *inside* the batch
  submission file, to ensure reproducibility of results (i.e. so that
  the same options are specified on each run, and no options are
  accidentally left out).

  Examples:

  .. code-block:: bash

     $ sbatch ./myscript.sh    # submits the batch submission file "myscript.sh" to SLURM


.. index:: SLURM; srun

* ``srun`` -- runs a parallel or interactive job on the worker nodes.
  A full list of options is available `here
  <https://computing.llnl.gov/linux/slurm/srun.html>`_.  The usage
  command (with the most-helpful optional arguments in brackets) is 

  .. code-block:: bash

     $ srun []

  where these options are:

  * ``-a`` or ``--all`` -- Display information about all partitions

  Examples:

  .. code-block:: bash

     $ srun -p parallel /bin/program  # runs the executable /bin/program on the "parallel" partition
     $ srun -p highmem /usr/bin/foo  # runs /usr/bin/foo on the "highmem" partition
     $ srun -N2 -n4  /bin/bar  # runs /bin/bar using 2 nodes 
     $ srun -N2 -n4 --ntasks-per-node=2  /bin/hostname
     $ srun --x11=first --pty emacs


.. index:: SLURM; salloc

* ``salloc`` -- obtains a SLURM job allocation (a set of nodes),
  executes a command, and then releases the allocation when the
  command is finished.  A full list of options is available `here
  <https://computing.llnl.gov/linux/slurm/salloc.html>`_.  The usage
  command (with the most-helpful optional arguments in brackets) is

  .. code-block:: bash

     $ salloc []

  where these options are:

  * ``-a`` or ``--all`` -- Display information about all partitions

  Examples:

  .. code-block:: bash

     $ sinfo --long -p highmem  # long output for all nodes allocated to the "highmem" partition


.. index:: SLURM; scancel

* ``scancel`` -- kills jobs or job steps that are under the control of
  SLURM (and listed by ``squeue``.  A full list of options is available `here
  <https://computing.llnl.gov/linux/slurm/scancel.html>`_.  The usage
  command (with the most-helpful optional arguments in brackets) is

  .. code-block:: bash

     $ scancel []

  where these options are:

  * ``-a`` or ``--all`` -- Display information about all partitions

  Examples:

  .. code-block:: bash

     $ sinfo --long -p highmem  # long output for all nodes allocated to the "highmem" partition








.. index:: condor job submission file

Job submission file
--------------------------------------------------

The way that a user interacts with Condor is through creating a *job
submission file* that describes the job you want to run:

.. index:: condor job submission file; line continuation

* For lengthy lines within the submit description file, ``\`` may be
  used as a line continuation character.  Placing the backslash at
  the end of a line causes the current line's command to be continued
  with the next line of the file. 

.. index:: 
   pair: condor job submission file; comment

* Submit file description files may contain comments, characterized as any
  line beginning with a ``#`` character. 

.. index:: condor job submission file; case-independence

* These submission file options are case-independent (i.e. "Universe" ==
  "uNivErSE"), although any file or path names are not.  


The main condor job submission file options on SMU HPC are as follows: 

.. index:: condor job submission file; arguments 

* **arguments** --  List of arguments to be supplied to the executable
  as part of the command line.  For example, 

  .. code-block:: text

     arguments = "arg1 arg2 arg3"

  Argument rules:

  1. The entire string representing the command line arguments is
     surrounded by double quote marks. This permits the white space
     characters of spaces and tabs to potentially be embedded within a
     single argument. Putting the double quote mark within the
     arguments is accomplished by escaping it with another double
     quote mark. 

  2. The white space characters of spaces or tabs delimit arguments.

  3. To embed white space characters of spaces or tabs within a single
     argument, surround the entire argument with single quote marks. 

  4. To insert a literal single quote mark, escape it within an
     argument already delimited by single quote marks by adding
     another single quote mark. 

.. index:: condor job submission file; environment 

* **environment** -- List of additional environment variables to
  supply to the executable.  For example,

  .. code-block:: text

     environment = "OMP_NUM_THREADS=4 LD_LIBRARY_PATH=/users/dreynolds/sw"

  Environment rules:

  1. Put double quote marks around the entire argument string. This
     distinguishes the new syntax from the old. The old syntax does
     not have double quote marks around it. Any literal double quote
     marks within the string must be escaped by repeating the double
     quote mark. 

  2. Each environment entry has the form ``<name>=<value>``

  3. Use white space (space or tab characters) to separate environment
     entries. 

  4. To put any white space in an environment entry, surround the
     space and as much of the surrounding entry as desired with single
     quote marks. 

  5. To insert a literal single quote mark, repeat the single quote
     mark anywhere inside of a section surrounded by single quote
     marks. 

.. index:: condor job submission file; error file

* **error** --  Path and file name indicating where Condor should put
  the standard error (``stderr``) from running your job.  For example, 

  .. code-block:: text

     error = myjob.err

  * If the file does not begin with a ``/``, the name indicates a
    relative path; otherwise it is an absolute path.  

  * You must have appropriate permissions to write to the supplied file.

  * The default is ``/dev/null``, corresponding to ignoring all error
    messages. 

.. index:: condor job submission file; executable

* **executable** -- The path and file name of your executable
  program. For example,

  .. code-block:: text

     executable  = myjob.sh

  * If the file does not begin with a ``/``, the name indicates a
    relative path; otherwise it is an absolute path.  

  * You must have appropriate permissions to read/execute the supplied file.

.. index:: condor job submission file; getenv

* **getenv** {True, False} -- Propagates the environment variables
  present in your shell upon submitting the job to the job when it
  runs. For example, 

  .. code-block:: text

     getenv = true

  If both **getenv** and **environment** are used, the values supplied
  by **environment** take precedence.

.. index:: condor job submission file; input

* **input** -- File containing any keyboard input values
  (i.e. standard input, ``stdin``) that your program requires.  For
  example,

  .. code-block:: text

     input = 100

  * If not specified, the default value of ``/dev/null`` (i.e. no input)
    is used.

  * You must have appropriate permissions to read from the supplied file.

  * Note that this command does not refer to the command-line arguments
    of the program, which are supplied by the **arguments** command.

.. index:: condor job submission file; log

* **log** --  File name indicating where Condor will record
  information about your job's execution.  While it is not required,
  it's usually a good idea to have Condor keep a log in case things go
  wrong.  For example,

  .. code-block:: text

     log = myjob.log

  * If the file does not begin with a ``/``, the name indicates a
    relative path; otherwise it is an absolute path.  

  * You must have appropriate permissions to write to the supplied file.

  * The default is ``/dev/null``, corresponding to ignoring all log
    messages. 

.. index:: condor job submission file; notification

* **notification** {Always, Complete, Error, Never} -- The set of
  job-related events for which the job owner is sent an email.  The
  default is "Complete", indicating notification when the job
  finishes.  "Error" indicates to notify if the job terminated
  abnormally. For example,

  .. code-block:: text

     notification = Always

.. index:: condor job submission file; notify_user

* **notify_user** -- The email address to which condor will send
  **notification** messages.  For example,

  .. code-block:: text

     notify_user = username@smu.edu

  If left unspecified, condor will send a message to
  ``job-owner@submit-machine-name`` (which ends up going to the system
  administrators, who probably don't really appreciate it).

.. index:: condor job submission file; output

* **output** --  File name indicating where Condor should put the
  standard output (``stdout``) from running your job.  For example,

  .. code-block:: text

     output = myjob.out

  * If the file does not begin with a ``/``, the name indicates a
    relative path; otherwise it is an absolute path.  

  * You must have appropriate permissions to write to the supplied file.

  * The default is ``/dev/null``, corresponding to ignoring all output
    messages. 

.. index:: condor job submission file; universe

* **universe** {vanilla, parallel} -- These specify what
  type of computation you plan to run.  For example,

  .. code-block:: text

     universe  = vanilla

  * The "vanilla" universes corresponds to single-node batch
    processing, in which condor will run your job on the first
    available node to completion.  

  * The "parallel" universe corresponds to MPI-based parallel jobs
    that require multiple compute nodes to run.

.. index:: condor job submission file; machine_count

* **machine_count** -- Only applicable with the "parallel" universe,
  this option tells Condor how many nodes should be allocated to the
  parallel job.  For example,

  .. code-block:: text

     machine_count = 2

.. index:: condor job submission file; requirements

* **requirements** -- Option allowing you to provide additional
  requirements that must be satisfied before launching your job.  This
  typically refers to the type of node you wish to run on.  For
  example, to request that you job run on a 12-core batch node, you
  could use 

  .. code-block:: text

     requirements = regexp("cwnode", Machine)

  or to request that it run on the 8-core-per-node parallel portion of
  the cluster,

  .. code-block:: text

     requirements = regexp("inode", Machine)

  or to run on the 12-core-per-node parallel portion of the cluster,

  .. code-block:: text

     requirements = regexp("iwnode", Machine)

.. index:: condor job submission file; queue

* **queue** -- This places your job into the queue, and should follow
  all arguments that specify how to run the job.  For example,

  .. code-block:: text

     queue

  One condor job file may contain multiple **queue** commands, each
  with different argument lists, allowing for submission of many
  condor jobs at once using the same submission file.



.. index:: condor job submission file; macros

In setting up this file, you have may insert parameterless macros, of
the form ``$(macro_name)``, anywhere in your job submission file.
Custom macros may be defined via the syntax

.. code-block:: text

   <macro_name> = <string>

There are three default macros:

.. index:: condor job submission file; Cluster

* **Cluster** -- the value of the ``ClusterID`` on which the job has
  is queued.

.. index:: condor job submission file; Process

* **Process** -- the Condor process ID number for this job.  For
  example,

  .. code-block:: text

     output = myjob.$(Process).out

.. index:: condor job submission file; Node

* **Node** -- only defined for jobs in the "Parallel" universe, this
  holds the name of the node on which the process is running (useful
  if each node reports different information, e.g. for debugging).
  For example, 

  .. code-block:: text

     output = myjob.out.$(Node)



.. index:: condor; whole node vs shared node

Whole vs shared node (old cluster only)
--------------------------------------------------

When running batch jobs on the cluster, you may request to use a whole
node for your job (the default is to share the node with other users).
Reasons why you may wish to request an entire node for your job
include: 

* Need for reliable timing information.

* Need for all of the memory on the node.

* Use of threads (e.g. OpenMP, Pthreads, Intel Threading Building
  Blocks, MPI, etc.) that will spawn additional processes on top of
  the one that is launched.

* Poor inter-personal skills.


If you wish for your job to use an entire node, you only need to add
two lines to your Condor job submission file.  These lines are
[inappropriately] named "whole machine", even they only refer to a
single node of the larger machine: 

.. code-block:: text

   Requirements = CAN_RUN_WHOLE_MACHINE
   +RequiresWholeMachine = True

If you wish to "require" both a specific node type and a whole node,
you would combine **Requirements** statements, e.g.

.. code-block:: text

   Requirements = CAN_RUN_WHOLE_MACHINE && regexp("iwnode", Machine)


.. index:: condor; ssh to job

Condor SSH to job
--------------------------------------------------

In some instances, you may wish to request a worker node from the
Condor pool for dedicated **interactive** use only.  Since a typical
user is not allowed to SSH directly to a worker node, Condor supplies
a modified SSH executable that will allow users to log into a worker
node that has been dedicated to that user.  This behavior is called
*SSH to job*, and is only allowed when a job has been submitted in
"whole machine" mode as described above.

Once your job is running, you can log into it via the commands

.. code-block:: bash

   $ source /grid/condor/condor.sh
   $ condor_ssh_to_job <processID>

where here ``<processID>`` is the integer ID number for your running job.




.. index:: 
   pair: condor; resources

Condor resources:
--------------------------------------------------

* :download:`SMU HPC Condor tutorial <files/condor.pdf>`

* `Condor manual (version 7.6.10, HTML)
  <http://research.cs.wisc.edu/htcondor/manual/v7.6/index.html>`_ 

* `Condor manual (version 7.6.10, PDF)
  <http://research.cs.wisc.edu/htcondor/manual/v7.6/condor-V7_6_10-Manual.pdf>`_ 




Condor Examples
================================================

In the following, we have a few example Condor usage scenarios to
familiarize you with how to interact with the high-throughput portion
of the SMU HPC cluster.

To do these examples, first retrieve the corresponding set of files
either through :download:`clicking here <code/session5.tgz>` or by copying the
relevant files at the command line:

.. code-block:: bash

   $ cp ~dreynolds/SMUHPC_tutorial/session5.tgz .

Unzip this file, and enter the resulting subdirectory

.. code-block:: bash

   $ tar -zxf session5.tgz
   $ cd session5

Before we can use this example, we need to set up our environment
correctly:

.. code-block:: bash

   $ module load gcc
   $ module load python


.. index:: condor examples; single shared node job

Running a job
--------------------------------------------------

In this example, we'll run the Python scrpit ``myjob.py``, that
performs a simple algorithm for approximating :math:`\pi` using a
composite trapezoidal numerical integration formula to approximate 

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

Before submitting this script to condor, we need to ensure that
``myjob.py`` has "executable" permissions:

.. code-block:: bash

   $ chmod +x ./myjob.py 


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

