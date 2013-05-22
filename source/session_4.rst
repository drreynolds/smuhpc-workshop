Session 4: More Programming
============================

*Instructor: Dan Reynolds*

*Assistants: Amit Kumar, Merlin Wilkerson, Doug Tucker*



Makefiles
--------------

The ``make`` command allows programmers to easily manage programs with
large numbers of files.  It aids in developing large programs by
encoding instructions on how to build the program, keeping track of
which portions of the entire program have been changed, and compiling
only those parts of the program which have changed since the last
compile.

The ``make`` program gets its set of compile rules from a text file
called ``Makefile`` which resides in the same directory as the source
files. It contains information on how to compile the software,
e.g. the compiler to use, the optimization level, whether to include
debugging info in the executable, etc.. It also contains information
on where to install the finished compiled binaries (executables),
manual pages, data files, dependent library files, configuration
files, etc..  For example, when we built the ``units`` program in the
previous session, the ``configure`` program automatically created a 
``Makefile`` for building ``units``, so that we did not need to
compile everything manually.


Retrieve the set of files for this session either through
:download:`clicking here <code/session4.tgz>` or by copying the
relevant files at the command line:

.. code-block:: bash

   $ cp ~dreynolds/SMUHPC_tutorial/session4.tgz .


Unzip/untar this file with the command

.. code-block:: bash

   $ tar -zxf session4.tgz

You should now see a new subdirectory entitled ``session4`` in your
current directory.  This is where we will work for the rest of this
section.  Inside this directory you will see a number of files: 

.. code-block:: bash

   driver.cpp
   one_norm.cpp
   vector_difference.cpp
   vector_product.cpp
   vector_sum.cpp

Here, the main program is held in the file ``driver.cpp``, and
supporting subroutines are held in the remaining files. To compile
these on SMUHPC, it takes a number of steps. 

Let's first compile and assemble the auxiliary subroutine
``one_norm.cpp``:

.. code-block:: bash

   $ g++ -c one_norm.cpp

This calls the GNU C++ compiler, ``g++``, to create an object file, named
``one_norm.o``, that contains compiler-generated CPU instructions on how
to execute the function in the file ``one_norm.cpp``. 

Use similar instructions to create the object files ``driver.o``,
``vector_difference.o``, ``vector_product.o`` and ``vector_sum.o`` in
a similar fashion.  

You should now have the files ``driver.o``, ``one_norm.o``,
``vector_difference.o``, ``vector_product.o`` and ``vector_sum.o`` in
your directory. The final stage in creating the executable is to link
these files together. We may call ``g++`` one more time to do this
(which itself calls the system-dependent linker), supplying all of the
object files as arguments so that ``g++`` knows which files to link
together: 

.. code-block:: bash

   $ g++ driver.o one_norm.o vector_difference.o vector_product.o \
     vector_sum.o -lm

This creates an executable file named ``a.out``, which is the default
(entirely non-descriptive) name given by most compilers to the
resulting executable.  The additional argument ``-lm`` is used to tell
``g++`` to link these functions against the built-in math library (so
that we can link with the absolute value function, ``fabs()``, that is
called inside the ``one_norm.cpp`` file.

You can instead give your executable a more descriptive name with the
``-o`` option:

.. code-block:: bash

   $ g++ driver.o one_norm.o vector_difference.o vector_product.o \
     vector_sum.o -lm -o driver.exe 

This will create the same executable, but with the more descriptive name ``driver.exe``. 

While you may find it to be quite enjoyable to compile every source
file by hand, and then manually link them together into an executable,
the process can be completely automated by using a ``Makefile``.  

A few rules about ``Makefiles``:

* The ``make`` program will look for any of the files:
  ``GNUmakefile``, ``makefile``, and ``Makefile`` (in that order) for
  build instructions.  Most people consider the name ``Makefile`` as
  best practice, though any are acceptable.  

* Inside the ``Makefile``, lines beginning with the ``#`` character
  are treated as comments, and are ignored. 

* Blank lines are ignored.

* You specify a *target* for ``make`` to build using the syntax,

  .. code-block:: makefile

     target : dependencies
           build command 1
           build command 2
           build command 3

  where each of the lines following the ``target :`` line must begin
  with a ``[Tab]`` character.  Each of these lines are executed when
  ``make`` is called.  These lines are executed as if they were typed
  directly at the command line (as with a shell script). 

* More than one *target* may be included in any ``Makefile``.

* If you just type ``make`` at the command line, only the first
  *target* is run.

As an example, examine the Makefile from session 3.  Here, all of the
lines are either blank or are comment lines except for the two: 

.. code-block:: makefile

   main.exe : main.cpp
         g++ -o main.exe main.cpp

Here, the build target is ``main.exe`` (it is traditional to give the
target the same name as the output of the build commands). The
executable depends on the source code file ``main.cpp``. The ``make``
program then builds ``main.exe`` by issuing the command ``g++ -o
main.exe main.cpp``, which does the compilation, assembly and linking
all in one step (since there is only one source code file).  

Alternatively, this Makefile could have been written:

.. code-block:: makefile

   main.exe : main.cpp
         g++ -c main.cpp
         g++ -o main.exe main.o

or even as

.. code-block:: makefile

   main.exe :
         g++ -c main.cpp
         g++ -o main.exe main.o

(which ignores the dependency on ``main.cpp``).

Create a ``Makefile`` to compile the executable ``driver.exe`` out of
the files ``driver.cpp``, ``one_norm.cpp``, ``vector_difference.cpp``,
``vector_product.cpp`` and ``vector_sum.cpp``.  This should encode all
of the commands that we earlier needed to do by hand. Start out with
the command 

.. code-block:: bash

   $ gedit Makefile &

to have ``gedit`` create the file ``Makefile`` in the background, so
that while you edit the ``Makefile`` you can still use the terminal
window to try out ``make`` as you add commands.

You can incorparate additional targets into your ``Makefile``.  The
first target in the file will be executed by a ``make`` command
without any arguments.  Any other targets may be executed through the
command ``make target``, where ``target`` is the name you have
specified for a target in the ``Makefile``.  For example, a standard
``Makefile`` target is to clean up the temporary files created during
compilation of the executable, typically entitled ``clean``.  In our 
compilation process, we created the temporary files ``driver.o``,
``one_norm.o``, ``vector_product.o``, ``vector_sum.o`` and
``vector_difference.o``.  These could be cleaned up with the single
command ``make clean`` if we add the following lines to the
``Makefile``, after your commands to create the executable: 

.. code-block:: makefile

   clean :
         rm -f *.o

Now type ``make clean`` in the terminal -- all of the temporary build
files have been removed. 

``Makefiles`` can be much more complicated than those outlined here,
but for our needs in this tutorial these commands should suffice. For
additional information on the ``make`` system, see the PDF manual
listed below.

Make resources:

* `GNU Make manual
  <http://dreynolds.math.smu.edu/Courses/Math6370_Spring13/make.pdf>`_ 




Modules
-----------

**FILL THIS IN**

Outline:

* links to documentation on our Module system (if it exists)

* list of module highlights

* high-level description of how module works



Module functions:

* ``module avail`` -- displays a list of all available modules on the
  system, e.g.

  .. code-block:: bash

     $ module avail

     ----------- /grid/software/Modules/smu-modules/applications -----------
     R                                  meep/1.2
     ROOT/5.32                          namd/2.9/x86_64/infiniband/non-smp
     abinit                             namd/2.9/x86_64/infiniband/smp
     mathematica                        namd/2.9/x86_64/multicore/CUDA
     meep/1.1.1
     
     ------------ /grid/software/Modules/smu-modules/libraries -------------
     fftw                mpich2/1.1.1/gcc    mvapich2/1.6/pgi
     gsl/1.15            mpich2/1.3.2/pgi    mvapich2/1.6/pgi-QL
     gsl/1.9             mvapich2/1.6/gcc    mvapich2/1.9a2/gcc
     hdf5/1.8.3          mvapich2/1.6/gcc-QL
     
     ------------ /grid/software/Modules/smu-modules/compilers -------------
     g95/0.92/32bit g95/0.92/64bit pgi/10.5/64bit pgi/13.2/64bit
     
     ------------- /grid/software/Modules/smu-modules/physics --------------
     ATLASLocalRootBase clhep/2.0.4.7
     clhep/2.0.4.5      clhep/2.1.2.3

* ``module list`` -- lists all currently loaded modules in your
  working environment.  At first, we have none:

  .. code-block:: bash

     $ module list
     No Modulefiles Currently Loaded.

* ``module add`` and ``module load`` -- loads a module into your
  working environment.  For example, at the moment the PGI C compiler
  is not in our PATH:

  .. code-block:: bash

     $ pgcc
     -bash: pgcc: command not found

  but once we load the ``pgi`` module, it is now in our path

  .. code-block:: bash

     $ module load pgi
     $ pgcc
     pgcc-Warning-No files to process

  and it is listed as being loaded in our environment

  .. code-block:: bash

     $ module list
     Currently Loaded Modulefiles:
       1) pgi/13.2/64bit

* ``module rm`` and ``module unload`` -- undoes a previous "add" or
  "load" command, removing the module from your working environment,
  e.g.

  .. code-block:: bash

     $ module load mathematica
     $ module list
     Currently Loaded Modulefiles:
       1) pgi/13.2/64bit   2) mathematica
     $ module unload mathematica
     $ module list
     Currently Loaded Modulefiles:
       1) pgi/13.2/64bit

* ``module switch`` and ``module swap`` -- this does a combination
  unload/load, swapping out one module for another, e.g.

  .. code-block:: bash

     $ module load mvapich2/1.6/gcc
     $ module list
     Currently Loaded Modulefiles:
       1) pgi/13.2/64bit     2) mvapich2/1.6/gcc
     $ module swap mvapich2/1.6/gcc mvapich2/1.6/gcc-QL
     $ module list
     Currently Loaded Modulefiles:
       1) pgi/13.2/64bit        2) mvapich2/1.6/gcc-QL

* ``module display`` and ``module show`` -- this shows detaled
  information about how a specific module affects your environment,
  e.g.
 
  .. code-block:: bash

     $ module show R
     -------------------------------------------------------------------
     /grid/software/Modules/smu-modules/applications/R:
     
     module-whatis	 loads R executables in current environment 
     setenv		 R_HOME /grid/software/R-2.10.0 
     prepend-path	 PATH /grid/software/R-2.10.0/bin 
     prepend-path	 LD_LIBRARY_PATH /grid/software/R-2.10.0/lib64 
     -------------------------------------------------------------------

* ``module help`` -- This displays a set of descriptive information
  about the module (what it does, the version number of the
  software, etc.), e.g.

  .. code-block:: bash

     $ module help fftw
     
     ----------- Module Specific Help for 'fftw' -----------------------
     
        Loads FFTW - 'A free collection of fast C routines forcomputing 
         the Discrete Fourier Transform in one or more dimensions'

        This adds /grid/software/fftw-3.2.2/* to several of the
        environment variables.
     
        FFTW Version 3.2.2
     
     
        fftw-wisdom-to-conf
        fftw-wisdom




Module example
^^^^^^^^^^^^^^^^^^^

As a simple example, let's compare how to do the same task first
without, and then with, the module system.  Returning to our previous
example on using Makefiles, we can compile that code using the PGI C++
compiler by using the command

.. code-block:: bash

   $ /grid/software/pgi-13.2/linux86-64/13.2/bin/pgc++  driver.cpp \
     one_norm.cpp vector_difference.cpp vector_product.cpp \
     vector_sum.cpp -lm -o driver.exe

While this certainly works, it requires us to know the global PATH to
the ``pgc++`` compiler.  Using the module system, this simplifies to

.. code-block:: bash

   $ module load pgi
   $ pgc++  driver.cpp one_norm.cpp vector_difference.cpp \
     vector_product.cpp vector_sum.cpp -lm -o driver.exe

Even for this simple example where we only
need to add something to our PATH, the module system can be invaluable
since it is rare that you know the global location of a file *a
priori*.  



Module exercise
^^^^^^^^^^^^^^^^^^

Run Mathematica on SMUHPC, using it to integrate the function
:math:`f(x) = \log(x^3-2)`.  Once in Mathematica, you can use the
"Help"->"Documentation Center" menu and search for "Integration".
*Hint: to execute a Mathematica command, after entering the command
you should press [shift]-[enter]*. 



Module resources:
^^^^^^^^^^^^^^^^^^

* `Main Module page <http://modules.sourceforge.net/>`_

* `Module FAQ <http://sourceforge.net/p/modules/wiki/FAQ/>`_



Version control systems
---------------------------

(adapted from `A visual guide to version control
<http://betterexplained.com/articles/a-visual-guide-to-version-control/>`_) 


Version Control (aka *revision control* or *source control*) lets you
track the history of your files over time. Why do you care? So when
you mess up you can easily get back to a previous version that worked. 

You've probably invented your own simple version control system in the
past without realizing it. Do you have an directories with files like this? 

* my_function.c

* my_function2.c

* my_function3.c

* my_function4.c

* my_function_old.c

* my_function_older.c

* my_function_even_older.c

It's why we use "Save As". You want to save the new file without
obliterating the old one.  It's a common problem, and solutions are
usually like this: 

* Make a *single backup copy* (e.g. Document.old.txt).

* If we're clever, we add a *version number* or *date*:
  e.g. Document_V1.txt, DocumentMarch2012.txt.

* We may even use a *shared folder* so other people can see and edit
  files without sending them over email.  Hopefully they rename the 
  file after they save it. 


So Why Do We Need A Version Control System (VCS)?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Our shared folder/naming system is fine for class projects or one-time
papers.  But software projects?

Do you imagine that the Windows source code sits in a shared folder
like "Windows7-Latest-New", for anyone to edit?  Or that every
programmer just works on different files in the same folder?

Large, fast-changing projects with multiple authors need a Version Control
System (think: "file database") to track changes and avoid
general chaos. A good VCS does the following: 

* *Backup and Restore* -- files are saved as they are edited, and you
  can jump to any moment in time.  Need that file as it was on March
  8?  No problem.

* *Synchronization* -- Allows people to share files and stay
  up to date with the latest version. 

* *Short-term undo* -- Did you try to "fix" a file and just mess it
  up?  Throw away your changes and go back to the last "correct"
  version in the database.

* *Long-term undo* -- Sometimes we mess up bad. Suppose you made a
  change a year ago, and it had a bug that you never caught until
  now. Jump back to the old version, and see what change was made that
  day.  Maybe you can fix that one bug and not have to undo your work
  for the whole year?

* *Track Changes* -- As files are updated, you can leave messages
  explaining why the change happened (these are stored in the VCS, not
  the file).  This makes it easy to see how a file is evolving over time,
  and why it was changed. 

* *Track Ownership* -- A VCS tags every change with the name of the
  person who made it, which can be hepful for laying blame *or* giving
  credit.

* *Sandboxing* (i.e. insurance against yourself) -- Plan to make a big
  change?  You can make temporary changes in an isolated area, test and
  work out the kinks before "checking in" your set of changes. 

* *Branching and merging* -- A larger sandbox. You can branch a copy
  of your code into a separate area and modify it in isolation
  (tracking changes separately). Later, you can merge your work back
  into the common area. 

Shared folders are quick and simple, but can't beat these features.



General definitions
^^^^^^^^^^^^^^^^^^^^^

Most version control systems involve the following concepts, though
the labels may be different. 

Basic setup:

* *Repository (repo)* -- The database storing the files.

* *Server* -- The computer storing the repo.

* *Client* -- The computer connecting to the repo.

* *Working Set/Working Copy* -- Your local directory of files, where
  you make changes. 

* *Trunk/Main* -- The primary location for code in the repo.  Think of
  code as a family tree — the trunk is the main line. 


Basic actions:

* *Add* -- Put a file into the repo for the first time, i.e. begin
  tracking it with Version Control. 

* *Revision* -- What version a file is on (v1, v2, v3, etc.).

* *Head/Tip* -- The latest revision in the repo.

* *Check out* -- Download a file from the repo.

* *Check in* -- Upload a file to the repository (if it has
  changed). The file gets a new revision number, and people can "check
  out" the latest one. 

* *Checkin Message* -- A short message describing what was changed.

* *Changelog/History* -- A list of changes made to a file since it was
  created.

* *Update/Sync* -- Synchronize your files with the latest from the
  repository. This lets you grab the latest revisions of all files.

* *Revert* -- Throw away your local changes and reload the latest
  version from the repository.


More advanced actions

* *Branch* -- Create a separate copy of a file/folder for private use
  (bug fixing, testing, etc). Branch is both a verb ("branch the
  code") and a noun ("Which branch is it in?").

* *Diff/Change/Delta* -- Finding the differences between two
  files. Useful for seeing what changed between revisions.

* *Merge/Patch* -- Apply the changes from one file to another, to
  bring it up-to-date. For example, you can merge features from one
  branch into another.

* *Conflict* -- When pending changes to a file contradict each other
  (both changes cannot be applied automatically).

* *Resolve* -- Fixing the changes that contradict each other and
  checking in the final version.

* *Locking* -- Taking control of a file so nobody else can edit it
  until you unlock it. Some version control systems use this to avoid
  conflicts.

* *Breaking the lock* -- Forcibly unlocking a file so you can edit
  it. It may be needed if someone locks a file and goes on vacation.

* *Check out for edit* -- Checking out an "editable" version of a
  file. Some VCSes have editable files by default, others require an
  explicit command.



A typical scenario goes like this:

* Alice adds a file (ShoppingList.txt) to the repository. 

* Alice checks out the file, makes a change (puts "milk" on the list),
  and checks it back in with a checkin message ("Added required
  item."). 

* The next morning, Bob updates his local working set and sees the
  latest revision of ShoppingList.txt, which contains "milk".

* Bob adds "eggs" to the list, while Alice also adds "bread" to the
  list.

* Bob checks the list in.

* Alice updates her copy of the list before checking it in, and
  notices that there is a conflict.  Realizing that the order of items
  doesn't matter, she merges the changes by putting both "eggs" and
  "bread" on the list, and checks in the final version.




CVS
^^^^^

**FILL THIS IN**

Outline: 

* high-level description, including date it was invented
* listing of main commands


CVS resources:

* `Main CVS site <http://cvs.nongnu.org/>`_

* `CVS tutorial <http://www-mrsrl.stanford.edu/~brian/cvstutorial/>`_




SVN
^^^^^

**FILL THIS IN**

Outline: 

* high-level description, including date it was invented
* listing of main commands
* listing of free public repositories supporting SVN


SVN resources:

* `Main SVN site <https://subversion.apache.org/>`_

* `SVN tutorial <http://svnbook.red-bean.com/en/1.7/index.html>`_




Git
^^^^^

**FILL THIS IN**

Outline: 

* high-level description, including date it was invented
* listing of main commands
* listing of free public repositories supporting Git


Git resources:

* `Main Git site <http://git-scm.com/>`_

* `Git tutorials <http://www.atlassian.com/git/tutorial>`_

* `Git book chapters <http://git-scm.com/book>`_



Mercurial
^^^^^^^^^^^^

**FILL THIS IN**


Outline: 

* high-level description, including date it was invented
* listing of main commands
* listing of free public repositories supporting Mercurial
* example of cloning a repository, adding a file, checking it in,
  pushing back to main repository.


Mercurial example
""""""""""""""""""""

We'll get a little experience with using Mercurial to "collaborate" on
a shared project.  The first step in using an version control system
on an existing repository it to do the initial download of the code
from the main repository.  This repository can often be on a
standalone server, on the web, or it can even reside in someone else's
home directory.  Here, we'll use one that I've set up for this class.
In Mercurial, the initial download of the code uses the ``clone``
command:

.. code-block:: bash

   $ hg clone ~dreynolds/smuhpc-workshop-example

When the command completes, you should have a new directory named
``smuhpc-workshop-example``.  Enter that directory,

.. code-block:: bash

   $ cd smuhpc-workshop-example
   $ ls
   driver.cpp  one_norm.cpp  vector_difference.cpp  vector_product.cpp vector_sum.cpp

You should notice the files we used earlier in this session.  Since
Mercurial is a *distributed* version control system (so is Git), this
directory is now a new source code repository of your own. 

In this directory, add a new file of the form *lastname.txt*
containing your first name, e.g.

.. code-block:: bash

   $ echo "Daniel" > Reynolds.txt

To see what has changed in comparison with the last saved state of the
repository, you can use the ``status`` command:

.. code-block:: bash

   $ hg status
   ? Reynolds.txt

The "?" indicates that there is a new file in the directory that the
repository does not yet know about.  We can add these files to the 
repository with the ``add`` command: 

.. code-block:: bash

   $ hg add Reynolds.txt

Re-running ``status``, we see that the repository now knows about the
file:

.. code-block:: bash

   $ hg status
   A Reynolds.txt

where, the "A" denotes that the file has been added to the
repository.  Other keys include:

* "M" -- the file has been modified

* "!" -- the file has been deleted

* "R" -- the file has been removed from the repository

To save this change into the repository, we must ``commit`` the
changes:

.. code-block:: bash

   $ hg commit -m "added a file with my name" Reynolds.txt

The string following the ``-m`` indicates a log message describing the
changes that were made.  Once this command has completed, we see that
the local directory is current with our local repository:

.. code-block:: bash

   $ hg status

(note that nothing is listed).  To share these changes with everyone
else, you can ``push`` your modifications back to the original
repository that you'd initially cloned.  In order to be a good
citizen, before you push your changes up to the shared location, you
should first retrieve all changes that have been pushed by others:

.. code-block:: bash

   $ hg pull
   $ hg update

If the ``update`` command complains about changes needing to be merged
(meaning that someone else checked things in, so your changes need to
be merged with his/hers), then you can ``merge`` via

.. code-block:: bash

   $ hg merge

Assuming that your modifications do not collide with anyone else's,
this should be successful, in which case you need to check in the
merge

.. code-block:: bash

   $ hg commit -m "merged to tip"

Once you're certain that you have finished retrieving and merging all
changes from the shared repository, you ``push`` via

.. code-block:: bash

   $ hg push


.. note::

   Typically this process is not so challenging as it is today in
   class, because in reality most of your collaborators will not be
   checking in and pushing things at the same moment as you are.  If
   you choose to skip the ``push`` portion of the above steps today,
   that's fine. 



Mercurial resources:

* `Main mercurial site <http://mercurial.selenic.com/>`_

* `TortoiseHG -- multi-platform, graphical mercurial client
  <http://tortoisehg.bitbucket.org/>`_ 

* `Mercurial guide <http://hgbook.red-bean.com/>`_

* `Mercurial tutorial <http://mercurial.selenic.com/wiki/Tutorial>`_
