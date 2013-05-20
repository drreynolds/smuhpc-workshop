Using SMUHPC (Wed. PM)
========================================================

While we've technically been using the SMUHPC cluster throughout this
workshop, in reality we've only been using standard Linux tools as if
SMUHPC were a single computer.  The real value in the SMUHPC cluster
is its ability to run large numbers of simultaneous jobs (a.k.a. *high
throughput computing*) and/or its ability to run jobs that span
multiple processors at once (a.k.a. *high performance computing* or
*parallel computing*).



Basic hardware components
------------------------------------------------------

**UPDATE WITH HELP FROM SYSADMINS**

First, let's familiarize ourselves with the hardware that comprises
the SMUHPC cluster.  We can group the portions of SMUHPC into a few
simple categories: *login nodes*, *batch worker nodes*, *parallel
nodes*, *interactive nodes* and *disk nodes*.


**Login nodes**: 

* smuhpc.smu.edu -- main login node, used for compiling source code,
  setting up job submission files, input files, transferring data
  to/from SMUHPC, etc..  This node *should not* be used for running
  intensive calculations.
* smuhpc2.smu.edu -- clone of smuhpc.smu.edu, with identical usage.
* smuhpc3.smu.edu -- clone of smuhpc.smu.edu, primarily used for??
* smuhpc4.smu.edu -- clone of smuhpc.smu.edu, primarily used for
  interacting with the parallel nodes.


**Batch worker nodes**:

* 107 nodes have 8 cores each (for 856 in total).  Each node has 48 GB
  of RAM and and 250 GB of local disk space.  [wnode1-27, cnode-1-80]

* 56 nodes have 12 cores each (for 672 in total).  Each node has 72 GB
  of RAM and and 500 GB of local disk space.  [cwnode1-56]

* all batch nodes are connected with a gigabit ethernet network.


**Parallel nodes**:

* 16 nodes have 8 cores each (for 128 in total).  Each node has 48 GB
  of RAM and 500 GB of local disk space.  These are connected via a
  `Quadrics <http://en.wikipedia.org/wiki/Quadrics>`_ infiniband
  network.  [inode1-16]

* 32 nodes have 12 cores each (for 384 in total).  Each node has 72 GB 
  of RAM and 500 GB of local disk space.  These are connected via a 
  `QLogic <http://qlogic.com/pages/default.aspx>`_ infiniband
  network.  [iwnode1-32]


**Interactive nodes**:

* 2 high-memory data analysis and shared-memory parallel nodes.  Both
  have 8 cores each, 144 GB of RAM and 3 TB of local disk space.
  [highmem1-2]

* 2 GPU computing nodes:

  * gpu1: 8 CPU cores, 6 GB of RAM and 2 NVIDIA GTX 295 cards.
    Each of these GPU cards has 960 GPU cores and 3585 MB of RAM. 

  * gpu2: 16 CPU cores, 128 GB of RAM and 2 NVIDIA Tesla 2090 cards.
    Each of these GPU cards has 512 GPU cores and 6 GB of RAM.


**Disk nodes**:

* One 320 TB parallel Lustre file system is attached to all nodes, and
  is managed by the nodes diskarraynfs, diskarray, diskarray2-6,
  diskarray2b-6b. 

* Users cannot log into these nodes.


**General SMUHPC information**:

* OS: Scientific Linux 5.5 (64 bit).

* Scheduler: Condor

* The software stack for the full cluster includes a variety of high
  performance mathematics and software libraries, as well as the GNU,
  NAG and PGI compiler suites, as well as Matlab, Mathematica, R and
  python for batch scripting and interactive data analysis.

* `Wiki page <https://wiki.smu.edu/display/smuhpc/SMUHPC>`_ (requires
  SMU login).




Condor and job submission scripts
------------------------------------------------------

In this session we'll focus on the *high throughput* portion of the
SMUHPC cluster, i.e. the portion of the cluster that should be used
for serial (non-parallel) jobs.  This portion of the cluster is
managed by the `Condor <http://research.cs.wisc.edu/htcondor/>`_
scheduler, which is a piece of software designed "to develop,
implement, deploy, and evaluate mechanisms and policies that support
High Throughput Computing (HTC) on large collections of distributively
owned computing resources" [`http://research.cs.wisc.edu/htcondor
<http://research.cs.wisc.edu/htcondor>`_]. 



How Condor works
^^^^^^^^^^^^^^^^^^^^

**FILL THIS IN**




The Condor job submission file
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**UPDATE WITH SYSADMIN INPUT/CORRECTIONS**

The way that a user interacts with Condor is through creating a *job
submission file* that describes the job you want to run.  These
options are case-independent (i.e. "Universe" == "universe" ==
"uNivErSE").  The main options available in this Condor submission
file are as follows:

* **universe** {vanilla, standard, parallel} -- These specify what
  type of computation you plan to run.  Both the "vanilla" and
  "standard" universes correspond to single-node batch processing: in
  the "standard" universe where Condor will checkpoint the code for
  you, and may move it between nodes to accommodate other job
  requests, whereas the "vanilla" universe will run your job to
  completion on the node where it starts.  The "parallel" universe,
  however, corresponds to MPI-based parallel jobs.  

  For example,

  .. code-block:: text

     universe  = vanilla

* **executable** -- The name of your program. For example,

  .. code-block:: text

     executable  = myjob.sh


* **getenv** --   For example,

  .. code-block:: text

     getenv = true

* **arguments** --  These are the command-line arguments that you
  would specify following your executable name if you ran it directly
  at the command line.  For example,

  .. code-block:: text

     arguments = arg1 arg2 arg3

* **output** --  File name indicating where Condor should put the
  standard output from your job.  For example,

  .. code-block:: text

     output = myjob.out

* **error** --  File name indicating where Condor should put the
  standard error from your job.  For example,

  .. code-block:: text

     error = myjob.err

* **log** --  File name indicating where Condor will record
  information about your job's execution.  While it is not required,
  it's usually a good idea to have Condor keep a log in case things go
  wrong.  For example,

  .. code-block:: text

     log = myjob.log

* **machine_count** -- Only available with the "parallel" universe,
  this option tells Condor how many nodes should be allocated to the
  parallel job.  For example,

  .. code-block:: text

     machine_count = 2

* **notification** --   For example,

  .. code-block:: text

     notification = never

* **notify_user** --   For example,

  .. code-block:: text

     notify_user = username@smu.edu

* **requirements** --  For example, to request that you job run on a
  12-core batch node, you could use

  .. code-block:: text

     requirements = regexp("cwnode", Machine)

  or to request that it run on the 8-core-per-node parallel portion of
  the cluster,

  .. code-block:: text

     requirements = regexp("inode", Machine)

  or to run on the 12-core-per-node parallel portion of the cluster,

  .. code-block:: text

     requirements = regexp("iwnode", Machine)


* **queue** -- This should be the last line in your Condor job
  submission file, and indicates to Condor that you are finished
  supplying job-related information and wish to submit the job to the
  queue.  For example,

  .. code-block:: text

     queue


In setting up this file, you have access to the following variables
that may be used, for example, to customize your output, error and log
file names:

* **Process** -- The Condor process ID number for this job.  For
  example,

  .. code-block:: text

     output = myjob.$(Process).out

* **NODE** --   For example,

  .. code-block:: text

     output = myjob.out.$(NODE)



The Condor queue commands
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**FILL THIS IN WITH CONDOR COMMANDS**

* condor_submit

* condor_q

* condor_rm

* condor_compile





Whole node versus shared node jobs
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**UPDATE WITH HELP FROM SYSADMINS**

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

**UPDATE WITH MORE MOTIVATION WHY THIS IS AT ALL USEFUL**

Once your job is running, you can log into it via the commands

.. code-block:: bash

   % source /grid/condor/condor.sh
   % condor_ssh_to_job [job#]

where here ``[job#]`` is the integer ID number for your running job.




Condor resources:
^^^^^^^^^^^^^^^^^^^^^

* :download:`SMU HPC Condor tutorial <files/condor.pdf>`

* `Condor manual (version 7.6.10, HTML)
  <http://research.cs.wisc.edu/htcondor/manual/v7.6/index.html>`_ 

* `Condor manual (version 7.6.10, PDF)
  <http://research.cs.wisc.edu/htcondor/manual/v7.6/condor-V7_6_10-Manual.pdf>`_ 




Condor Examples
-------------------

**FILL IN A SHORT INTRO**

Running a single shared node job
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**FILL THIS IN**

Running a set of shared node jobs
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**FILL THIS IN, HAVE THEM USE BASH SCRIPTS TO SET UP AND LAUNCH**


Running a single whole node job
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**FILL THIS IN**

Running a set of whole node jobs
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**FILL THIS IN**



Exercises
----------------


