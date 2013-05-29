.. _session6:

Session 6: Using SMU HPC
========================================================

*Instructor: Dan Reynolds*

*Assistants: Amit Kumar, Merlin Wilkerson, Doug Tucker*


What is SMU HPC?
------------------

While we've technically been using the SMU HPC cluster throughout this
workshop, in reality we've only been using standard Linux tools as if
``smuhpc`` were a single computer.  The real value in the SMU HPC
cluster is its ability to run large numbers of simultaneous jobs
(a.k.a. *high throughput computing*) and/or its ability to run jobs
that span multiple processors at once (a.k.a. *high performance
computing* or *parallel computing*).

Originally purchased in 2009 through funding from the `SMU Provost's
office <http://smu.edu/provost/#1>`_, and subsequently added onto
through grants to the `Physics <http://www.smu.edu/physics>`_ and
`Math <http://www.smu.edu/math>`_ departments, the SMU HPC cluster is
a shared resource available to all SMU faculty and graduate and
undergraduate students involved in scientific computing projects.  It
is administered by the `SMU Office of Information Technology
<http://www.smu.edu/BusinessFinance/OIT>`_, and managed by the 
`SMU Center for Scientific Computation
<http://www.smu.edu/Academics/CSC>`_.




Basic hardware components
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

First, let's familiarize ourselves with the hardware that comprises
the SMU HPC cluster.  We can group the portions of SMU HPC into a few
simple categories: *login nodes*, *batch worker nodes*, *parallel
nodes*, *interactive nodes* and *disk nodes*.


Login nodes
""""""""""""""""

* ``smuhpc.smu.edu`` -- main login node, used for compiling source code,
  setting up job submission files, input files, transferring data
  to/from ``smuhpc``, etc..  This node *should not* be used for running
  intensive calculations.
* ``smuhpc2.smu.edu`` -- clone of ``smuhpc.smu.edu``, with identical usage.
* ``smuhpc3.smu.edu`` -- interactive login node, used for interactive
  computations, such as using Matlab, Mathematica, Python or R.
* ``smuhpc4.smu.edu`` -- clone of ``smuhpc.smu.edu``, primarily used for
  interacting with the parallel nodes.


Batch worker nodes
""""""""""""""""""""

* 107 nodes have 8 cores each (for 856 in total).  Each node has 48 GB
  of RAM and and 250 GB of local disk space.  [``wnode1-27``, ``cnode1-80``]

* 56 nodes have 12 cores each (for 672 in total).  Each node has 72 GB
  of RAM and and 500 GB of local disk space.  [``cwnode1-56``]

* all batch nodes are connected with a gigabit ethernet network.


Parallel nodes
""""""""""""""""""""

* 16 nodes have 8 cores each (for 128 in total).  Each node has 48 GB
  of RAM and 500 GB of local disk space.  These are connected via a
  `Quadrics <http://en.wikipedia.org/wiki/Quadrics>`_ infiniband
  network.  [``inode1-16``]

* 32 nodes have 12 cores each (for 384 in total).  Each node has 72 GB 
  of RAM and 500 GB of local disk space.  These are connected via a 
  `QLogic <http://qlogic.com/pages/default.aspx>`_ infiniband
  network.  [``iwnode1-32``]


Interactive nodes
"""""""""""""""""""""

* 2 high-memory data analysis and shared-memory parallel nodes.  Both
  have 8 cores each, 144 GB of RAM and 3 TB of local disk space.
  [``highmem1`` and ``highmem2``]

* 2 GPU computing nodes:

  * ``gpu1``: 8 CPU cores, 6 GB of RAM and 2 NVIDIA GTX 295 cards.
    Each of these GPU cards has 960 GPU cores and 3585 MB of RAM. 

  * ``gpu2``: 16 CPU cores, 128 GB of RAM and 2 NVIDIA Tesla 2090 cards.
    Each of these GPU cards has 512 GPU cores and 6 GB of RAM.


Disk nodes
"""""""""""""""

* One 320 TB parallel Lustre file system is attached to all nodes, and
  is managed by the nodes diskarraynfs, diskarray, diskarray2-6,
  diskarray2b-6b. 


General SMU HPC information
"""""""""""""""""""""""""""""""

* OS: Scientific Linux 5.5 (64 bit).

* Scheduler: Condor

* The software stack for the full cluster includes a variety of high
  performance mathematics and software libraries, as well as the GNU,
  NAG and PGI compiler suites, as well as Matlab, Mathematica, R and
  python for batch scripting and interactive data analysis.

* To log into any of ``gpu1``, ``gpu2``, ``highmem1`` or ``highmem2``,
  you must first log into one of the login nodes (``smuhpc``,
  ``smuhpc2``, ``smuhpc3`` or ``smuhpc4``) and then SSH from there to
  the relevant machine.

* Users may not log directly into any of the worker or disk nodes.

* `Wiki page <https://wiki.smu.edu/display/smuhpc/SMUHPC>`_ (requires
  SMU login).




Condor and job submission scripts
------------------------------------------------------

In this session we'll focus on the *high throughput* portion of the
SMU HPC cluster, i.e. the portion of the cluster that should be used
for serial (non-parallel) jobs.  This portion of the cluster is
managed by the `Condor <http://research.cs.wisc.edu/htcondor/>`_
job scheduler, which is a piece of software designed "to develop,
implement, deploy, and evaluate mechanisms and policies that support
High Throughput Computing (HTC) on large collections of distributively
owned computing resources" [`http://research.cs.wisc.edu/htcondor
<http://research.cs.wisc.edu/htcondor>`_]. 

More generally, a *job scheduler* is a program that manages unattended
background program execution (a.k.a. *batch processing*).  The basic
features of any job scheduler include:

* Interfaces which help to define workflows and/or job dependencies.

* Automatic submission of executions.

* Interfaces to monitor the executions.

* Priorities and/or queues to control the execution order of unrelated
  jobs.

In the context of high-throughput and high-performance computing, the
primary role of a job scheduler is to managing the job queue for all
of the compute nodes of the cluster.  It's primary goal is typically
to schedule queued jobs so that all of the compute nodes are utilized
to their capacity, yet doing so in a fair manner that gives priority
to users who have used less resources and/or contributed more to the
acquisition of the system.  

Some widely used cluster batch systems are:

* `Simple Linux Utility for Resource Management (SLURM) <http://slurm.schedmd.com/>`_

* `Moab <http://docs.adaptivecomputing.com/mwm/help.htm#topics/0-intro/productOverview.htm>`_

* `Torque <http://www.adaptivecomputing.com/products/open-source/torque/>`_

* `LoadLeveler <http://www-03.ibm.com/systems/software/loadleveler/index.html>`_

* `Condor <http://research.cs.wisc.edu/htcondor/>`_

* `Oracle Grid Engine <http://www.oracle.com/us/products/tools/oracle-grid-engine-075549.html>`_

* `Argent Job Scheduler <http://help.argent.com/#product_downloads_job_scheduler>`_

* `Platform LSF <http://www-03.ibm.com/systems/technicalcomputing/platformcomputing/products/lsf/>`_




The Condor queue commands
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

While there are a `multitude of condor commands
<http://research.cs.wisc.edu/htcondor/manual/v7.6/9_Command_Reference.html>`_,
only some are of value to a new user:

* ``condor_submit`` -- this is the main interface between a user and
  the condor scheduler, that queues jobs for execution.  The usage
  command (with the most-helpful optional arguments on SMU HPC in
  brackets) is 

  .. code-block:: bash

     $ condor_submit [-verbose] [-debug] [-append command ... ] [job file]

  where these options are:

  * ``-verbose`` -- Verbose output about the created job

  * ``-debug`` -- Cause debugging information to be sent to
    ``stderr``, based on the value of the configuration variable
    ``TOOL_DEBUG``.  

  * ``-append command`` -- Augment the commands in the submit
    description file with the given command. This command will be
    considered to immediately precede the ``Queue`` command within the
    job file, and come after all other previous commands.

    Multiple commands may be specified by using the ``-append`` option
    multiple times. 

    Commands with spaces in them must be enclosed in double quote marks. 

  * job file -- The pathname to the condor job submission file
    (described in the next section). If this optional argument is
    missing or equal to ``-``, then the commands are taken from
    standard input.

* ``condor_q`` -- displays information about jobs in the condor
  queue.  The usage command with the most helpful arguments is

  .. code-block:: bash

     $ condor_q [-help] [-run] [-hold] [-long] [{processID | username} ]

  where the options are:

  * ``-help`` -- returns a brief description of the supported options 

  * ``-run`` -- returns information about running jobs. 

  * ``-hold`` -- returns information about jobs in the hold
    state. Also displays the time the job was placed into the hold
    state and the reason why the job was placed in the hold state.  

  * ``-long`` -- displays job information in long format 

  * ``processID`` -- limits output to only the condor process ID for a specific job

  * ``username`` -- limits output to only jobs submitted by a specific
    user

* ``condor_rm`` -- removes jobs from the condor queue.  The usage
  command with the most typical arguments is

  .. code-block:: bash

     condor_rm [-help] {processID | username}

  where the options are:

  * ``-help`` -- displays usage information 

  * ``processID`` -- removes a job with a specific process ID

  * ``username`` -- removes all jobs launched by a user (you can only
    remove your own)





The Condor job submission file
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The way that a user interacts with Condor is through creating a *job
submission file* that describes the job you want to run.  

For lengthy lines within the submit description file, ``\`` may be
used as a line continuation character.  Placing the backslash at
the end of a line causes the current line's command to be continued
with the next line of the file. 

Submit file description files may contain comments, characterized as any
line beginning with a ``#`` character. 

These submission file options are case-independent (i.e. "Universe" ==
"uNivErSE"), although any file or path names are not.  

The main options on SMU HPC are as follows: 

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

* **error** --  Path and file name indicating where Condor should put
  the standard error (``stderr``) from your job.  For example,

  .. code-block:: text

     error = myjob.err

  If the file does not begin with a ``/``, the name indicates a
  relative path; otherwise it is an absolute path.  You must have
  appropriate permissions to write to the supplied file.

  The default is ``/dev/null``, corresponding to ignoring all error
  messages. 

* **executable** -- The path and file name of your executable
  program. For example,

  .. code-block:: text

     executable  = myjob.sh

  If the file does not begin with a ``/``, the name indicates a
  relative path; otherwise it is an absolute path.  You must have
  appropriate permissions to read/execute the supplied file.

* **getenv** {True, False} -- Propagates the environment variables
  present in your shell upon submitting the job to the job when it
  runs. For example, 

  .. code-block:: text

     getenv = true

  If both **getenv** and **environment** are used, the values supplied
  by **environment** take precedence.

* **input** -- File containing any keyboard input values
  (i.e. standard input, ``stdin``) that your program requires.  For
  example,

  .. code-block:: text

     input = 100

  If not specified, the default value of ``/dev/null`` (i.e. no input)
  is used.

  Note that this command does not refer to the command-line arguments
  of the program, which are supplied by the **arguments** command.

* **log** --  File name indicating where Condor will record
  information about your job's execution.  While it is not required,
  it's usually a good idea to have Condor keep a log in case things go
  wrong.  For example,

  .. code-block:: text

     log = myjob.log

* **notification** {Always, Complete, Error, Never} -- The set of
  job-related events for which the job owner is sent an email.  The
  default is "Complete", indicating notification when the job
  finishes.  "Error" indicates to notify if hte job terminated
  abnormally. For example,

  .. code-block:: text

     notification = Always

* **notify_user** -- The email address to which condor will send
  **notification** messages.  For example,

  .. code-block:: text

     notify_user = username@smu.edu

  If left unspecified, condor will send a message to
  ``job-owner@submit-machine-name`` (which goes nowhere).

* **output** --  File name indicating where Condor should put the
  standard output (``stdout``) from your job.  For example,

  .. code-block:: text

     output = myjob.out

  If the file does not begin with a ``/``, the name indicates a
  relative path; otherwise it is an absolute path.  You must have
  appropriate permissions to write to the supplied file.

  The default is ``/dev/null``, corresponding to ignoring all output
  messages. 

* **universe** {vanilla, parallel} -- These specify what
  type of computation you plan to run.  The "vanilla" universes
  corresponds to single-node batch processing, in which condor will
  run your job on the first available node to  completion.  The
  "parallel" universe, however, corresponds to MPI-based parallel
  jobs.

  For example,

  .. code-block:: text

     universe  = vanilla

* **machine_count** -- Only available with the "parallel" universe,
  this option tells Condor how many nodes should be allocated to the
  parallel job.  For example,

  .. code-block:: text

     machine_count = 2

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

* **queue** -- This places your job into the queue, and should follow
  all arguments that specify how to run the job.  For example,

  .. code-block:: text

     queue

  One condor job file may contain multiple **queue** commands, each
  with different argument lists, allowing for submission of many
  condor jobs at once.



In setting up this file, you have may insert parameterless macros, of
the form ``$(macro_name)``, anywhere in your job submission file.
Custom macros may be defined via the syntax

.. code-block:: text

   <macro_name> = <string>

There are three default macros:

* **Cluster** -- the value of the ``ClusterID`` on which the job has
  is queued.

* **Process** -- the Condor process ID number for this job.  For
  example,

  .. code-block:: text

     output = myjob.$(Process).out

* **Node** -- only defined for jobs in the "Parallel" universe, this
  holds the name of the node on which the process is running (useful
  if each node reports different information, e.g. for debugging).
  For example, 

  .. code-block:: text

     output = myjob.out.$(Node)




Whole node versus shared node jobs
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

When running batch jobs on the cluster, you may request to use a whole
node for your job (otherwise you will share the node with other
users).  Reasons why you may wish to request an entire node for your
job include:

* Need for reliable timing information

* Need for all of the memory on the node

* Use of threads (e.g. OpenMP, Pthreads, Intel Threading Building
  Blocks, MPI, etc.) that will spawn additional processes on top of
  the one that is launched.


If you wish for your job to use an entire node, you only need to add
two lines to your Condor job submission file.  These lines are
[inappropriately] named "whole machine", even they only refer to a
single node on the larger machine: 

.. code-block:: text

   Requirements =  CAN_RUN_WHOLE_MACHINE
   +RequiresWholeMachine = True



Condor SSH to job
^^^^^^^^^^^^^^^^^^^^

In some instances, you may wish to request a worker node from the
Condor pool for dedicated use.  Since a typical user is not allowed to
SSH directly to a worker node, Condor supplies a modified SSH
executable that will allow users to log into a worker node that has
been dedicated to that user.  This behavior is called *SSH to job*,
and is only allowed when a job has been submitted in "whole machine"
mode as described above.

Once your job is running, you can log into it via the commands

.. code-block:: bash

   $ source /grid/condor/condor.sh
   $ condor_ssh_to_job <processID>

where here ``<processID>`` is the integer ID number for your running job.




Condor resources:
^^^^^^^^^^^^^^^^^^^^^

* :download:`SMU HPC Condor tutorial <files/condor.pdf>`

* `Condor manual (version 7.6.10, HTML)
  <http://research.cs.wisc.edu/htcondor/manual/v7.6/index.html>`_ 

* `Condor manual (version 7.6.10, PDF)
  <http://research.cs.wisc.edu/htcondor/manual/v7.6/condor-V7_6_10-Manual.pdf>`_ 




Condor Examples
-------------------

We will perform this session of the workshop on the ``smuhpc2`` login
node, so log in there to begin.

In the following, we have a few example Condor usage scenarios to
familiarize you with how to interact with the high-throughput portion
of the SMU HPC cluster.

To do these examples, first retrieve the corresponding set of files
either through :download:`clicking here <code/session6.tgz>` or by copying the
relevant files at the command line:

.. code-block:: bash

   $ cp ~dreynolds/SMUHPC_tutorial/session6.tgz .

Unzip this file, and enter the resulting subdirectory

.. code-block:: bash

   $ tar -zxf session6.tgz
   $ cd session6


Running a single shared node job
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

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

   $ python ./myjob.py 500

as we increase the number of subintervals to obtain a more accurate
approximation it can take longer to run, so as "good citizens" we
should instead run it on dedicated compute nodes instead of the shared
login nodes.  

Create a new job submission file, ``test1.job`` using the editor of
your choice (e.g. ``gedit`` or ``emacs``), and fill in the arguments

.. code-block:: text

   universe   = vanilla
   getenv     = true
   log        = test1.log
   error      = test1.err
   output     = test1.out
   executable = myjob.py
   arguments  = 5000000
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



.. _running_multiple_condor_jobs:

Running a set of shared node jobs
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Suppose now that we wanted to run this script multiple times with
different arguments, in order to experimentally measure how rapidly
the approximation to :math:`pi` converges as we change the number of
subintervals.  

To this end, we have a few options:

1. Write separate job files for each number of subintervals, and
   submit each to condor.  This has the benefit of creating
   a reproducible set of tests, where the inputs for each test are
   quite clear.  

2. Reuse our existing job file, but when calling ``condor_submit`` we can
   use the ``-append`` option to modify the command line argument and
   output/log/error file names.  

   The challenge with this approach is
   that we may forget the command-line arguments we had to use for
   the different calls, making our results more difficult to
   reproduce.  

   However, this could be automated by creating a BASH
   script that calls ``condor_submit`` for us multiple times, with the
   customized calls hard-coded into the script.  This would again
   allow for reproducibility.

3. We could write a single job file that has separate blocks of
   options, each separated by a different **queue** command, allowing
   us to run multiple tests with a single submission file.  

All of the above approaches are equally valid, but we'll choose option
3 since it requires the least typing.  

Create a new condor job submission file, ``test2.job`` with the contents

.. code-block:: text

   universe   = vanilla
   getenv     = true
   log        = test2a.log
   error      = test2a.err
   output     = test2a.out
   executable = myjob.py
   arguments  = 500
   queue

   universe   = vanilla
   getenv     = true
   log        = test2b.log
   error      = test2b.err
   output     = test2b.out
   executable = myjob.py
   arguments  = 5000
   queue

   universe   = vanilla
   getenv     = true
   log        = test2c.log
   error      = test2c.err
   output     = test2c.out
   executable = myjob.py
   arguments  = 50000
   queue

   universe   = vanilla
   getenv     = true
   log        = test2d.log
   error      = test2d.err
   output     = test2d.out
   executable = myjob.py
   arguments  = 500000
   queue

   universe   = vanilla
   getenv     = true
   log        = test2e.log
   error      = test2e.err
   output     = test2e.out
   executable = myjob.py
   arguments  = 5000000
   queue

Note that all of the blocks use many of the same options, but that we
have set up unique log, error and output file names for each, and have
varied the value of **arguments** to specify different numbers of
subintervals. 

Launch these jobs as before, with the command

.. code-block:: bash

   $ condor_submit test2.job

To view our results in a single command, use

.. code-block:: bash

   $ cat test2*.out



Running a single whole node job
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

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

After the run finishes, find the 4326th prime number (on line 4326 of
test3.out) with the command

.. code-block:: bash

   $ sed -n 4326p test3.out





Exercises
----------------

**FILL THIS IN**
