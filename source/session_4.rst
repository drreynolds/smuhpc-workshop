:tocdepth: 3


.. _session4:

*****************************************************
Session 4: More Programming
*****************************************************

*Instructor: Dan Reynolds*

*Assistants: Amit Kumar, Merlin Wilkerson, Doug Tucker*


We will perform this session of the workshop on the ``smuhpc3`` login
node, so log in there to begin.


.. index:: ! Makefile

Makefiles
================================================

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

   driver.cpp      vector_difference.cpp    vector_sum.cpp
   one_norm.cpp    vector_product.cpp


Here, the main program is held in the file ``driver.cpp``, and
supporting subroutines are held in the remaining files. To compile
these on SMUHPC, it takes a number of steps. 

Let's first compile and assemble the auxiliary subroutine
``one_norm.cpp``:

.. code-block:: bash

   $ g++ -c one_norm.cpp

This calls the GNU C++ compiler, ``g++``, to create an :index:`object
file`, named ``one_norm.o``, that contains compiler-generated CPU
instructions on how to execute the function in the file ``one_norm.cpp``. 

Use similar instructions to create the object files ``driver.o``,
``vector_difference.o``, ``vector_product.o`` and ``vector_sum.o`` in
a similar fashion.  

You should now have the files ``driver.o``, ``one_norm.o``,
``vector_difference.o``, ``vector_product.o`` and ``vector_sum.o`` in
your directory. The final stage in creating the executable is to
:index:`link <linking>` these files together. We may call ``g++`` one
more time to do this (which itself calls the system-dependent linker),
supplying all of the object files as arguments so that ``g++`` knows
which files to link together: 

.. code-block:: bash

   $ g++ driver.o one_norm.o vector_difference.o vector_product.o \
     vector_sum.o -lm

This creates an executable file named ``a.out``, which is the
:index:`default <a.out>` (entirely non-descriptive) name given by most
compilers to the resulting executable.  The additional argument
``-lm`` is used to tell ``g++`` to link these functions against the
built-in math library (so that we can use the absolute value function,
``fabs()``, that is called inside the ``one_norm.cpp`` file. 

You can instead give your executable a more descriptive name with the
``-o`` option:

.. code-block:: bash

   $ g++ driver.o one_norm.o vector_difference.o vector_product.o \
     vector_sum.o -lm -o driver.exe 

This will create the same executable, but with the more descriptive
name ``driver.exe``.  

While you may find it to be quite enjoyable to compile every source
file by hand, and then manually link them together into an executable,
the process can be completely automated by using a ``Makefile``.  

A few rules about ``Makefiles``:

.. index:: Makefile
   seealso: GNUmakefile; Makefile
   seealso: makefile; Makefile

* The ``make`` program will look for any of the files:
  ``GNUmakefile``, ``makefile``, and ``Makefile`` (in that order) for
  build instructions.  Most people consider the name ``Makefile`` as
  best practice, though any are acceptable.  

* Inside the ``Makefile``, lines beginning with the ``#`` character
  are treated as :index:`comments <pair: Makefile; comment>`, and are
  ignored.

* Blank lines are ignored.

* You :index:`specify <pair: Makefile; target>` a *target* for
  ``make`` to build using the syntax, 

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
lines are either blank or are comment lines except for the four sets: 

.. code-block:: makefile

   hello_cpp.exe : hello.cpp
           g++ hello.cpp -o hello_cpp.exe

   hello_c.exe : hello.c
           gcc hello.c -o hello_c.exe

   hello_f90.exe : hello.f90
           gfortran hello.f90 -o hello_f90.exe

   hello_f77.exe : hello.f
           gfortran hello.f -o hello_f.exe

Here, we have four build targets, ``hello_cpp.exe``,
``hello_c.exe``, ``hello_f90.exe`` and ``hello_f77.exe`` (it is
traditional to give the target the same name as the output of the
build commands).  

Each of these targets :index:`depend <pair: Makefile; dependencies>` a
source code file listed to the right of the colon; here these are
``hello.cpp``, ``hello.c``, ``hello.f90`` and ``hello.f``, respectively.  

The :index:`indented <pair: Makefile; build commands>` lines (each
require a single [tab] character) under each target contain the
instructions on how to build that executable.  For example, ``make``
will build ``hello_cpp.exe`` by issuing the command ``g++ hello.cpp -o
hello_cpp.exe``, which does the compilation, assembly and linking all
in one step (since there is only one source code file). 

Alternatively, this Makefile could have been written:

.. code-block:: makefile

   hello_cpp.exe : hello.cpp
           g++ -c hello.cpp
           g++ hello.o -o hello_cpp.exe

   hello_c.exe : hello.c
           gcc -c hello.c
           gcc hello.o -o hello_c.exe

   hello_f90.exe : hello.f90
           gfortran -c hello.f90
           gfortran hello.o -o hello_f90.exe

   hello_f77.exe : hello.f
           gfortran -c hello.f
           gfortran hello.o -o hello_f.exe

or even as

.. code-block:: makefile

   hello_cpp.exe : 
           g++ hello.cpp -o hello_cpp.exe

   hello_c.exe : 
           gcc hello.c -o hello_c.exe

   hello_f90.exe : 
           gfortran hello.f90 -o hello_f90.exe

   hello_f77.exe : 
           gfortran hello.f -o hello_f.exe

(which ignores the dependency on the source code files ``main.cpp``).

Create a ``Makefile`` to compile the executable ``driver.exe`` for
session 4, out of the files ``driver.cpp``, ``one_norm.cpp``,
``vector_difference.cpp``, ``vector_product.cpp`` and
``vector_sum.cpp``.  This should encode all of the commands that we
earlier needed to do by hand. Start out with the command 

.. code-block:: bash

   $ gedit Makefile &

to have ``gedit`` create the file ``Makefile`` in the background, so
that while you edit the ``Makefile`` you can still use the terminal
window to try out ``make`` as you add commands.

As with the example from session 3, you can incorporate more than one
target into your ``Makefile``.  The first target in the file will be
executed by a ``make`` command without any arguments.  All other
targets may be executed through the command ``make target``, where
``target`` is the name you have specified for a target in the
``Makefile``.  

.. index:: make clean

For example, a standard ``Makefile`` target is to clean up the
temporary files created during compilation of the executable,
typically entitled ``clean``.  In our compilation process, we created
the temporary files ``driver.o``, ``one_norm.o``,
``vector_product.o``, ``vector_sum.o`` and ``vector_difference.o``.
These could be cleaned up with the single command ``make clean`` if we
add the following lines to the ``Makefile``, after your commands to
create ``driver.exe``:

.. code-block:: makefile

   clean :
         rm -f *.o

Now type ``make clean`` in the terminal -- all of the temporary build
files have been removed. 

``Makefiles`` can be much more complicated than those outlined here,
but for our needs in this tutorial these commands should suffice. For
additional information on the ``make`` system, see the PDF manual
listed below.

.. index::
   pair: Makefile; resources

Make resources:

* `GNU Make manual
  <http://dreynolds.math.smu.edu/Courses/Math6370_Spring13/make.pdf>`_ 




.. index:: ! module

Modules
================================================

The *module* system is a command-line tool to help users manage their
Linux environment variables (e.g. PATH, LD_LIBRARY_PATH).  It works by
grouping related environment variable settings together based on
various usage scenarios, such as

* Adding executables to a user's ``PATH``

* Adding the location of specific software libraries to a user's ``LD_LIBRARY_PATH``

* Adding documentation manual pages "man pages" to a user's ``MANPATH``

* Creating custom environment variables to define the global path
  where a specific package is installed, e.g. ``FFTWHOME``

These modules may be added/removed dynamically, allowing
a user to have a great amount of control over her/his environment.

Possibly one of the greatest assets of the module system is that it
provides a simple user interface, and can be queried to learn all of
the available modules on a system, making it easier to know which
packages are or aren't already installed on a system.  


The module system operates through the Linux executable, ``module``,
followed by the desired command.  The primary module commands are as
follows: 

.. index:: module; avail

* ``module avail`` -- displays a list of all available modules on the
  system, e.g. 

  .. code-block:: bash

     $ module avail
     Rebuilding cache file, please wait ... done.
     
     --------------- /grid/software/modulefiles/applications ---------------
        R-2.10.0          meep/1.2                            (D)
        R-2.15.3          mercurial-2.6.1
        R-3.0.0           namd/2.9/x86_64/infiniband/non-smp
        ROOT/5.32         namd/2.9/x86_64/infiniband/smp      (D)
        abinit            namd/2.9/x86_64/multicore/CUDA
        java-1.7          python-2.6.5
        mathematica       python-2.7.5
        meep/1.1.1
     
     ---------------- /grid/software/modulefiles/compilers -----------------
        g95/0.92/32bit          gcc/4.5.1          pgi/10.5-64bit
        g95/0.92/64bit  (D)     gcc/4.8.0  (D)     pgi/13.2-64bit  (D)
     
     ---------------- /grid/software/modulefiles/libraries -----------------
        fftw                      mvapich2/1.6/gcc-QL
        gsl/1.9                   mvapich2/1.6/gcc
        gsl/1.15          (D)     mvapich2/1.6/pgi-QL
        hdf5/1.8.3                mvapich2/1.6/pgi     (D)
        mpich2/1.1.1/gcc          mvapich2/1.9a2/gcc
        mpich2/1.3.2/pgi
     
     ----------------- /grid/software/modulefiles/physics ------------------
        clhep/2.0.4.5       clhep/2.0.4.7       clhep/2.1.2.3  (D)
     
       Where:
        (D):  Default Module
     
     Use "module spider" to find all possible modules. 
     Use "module keyword key1 key2 ..." to search for all possible modules 
     matching any of the "keys". 


.. index:: module; list

* ``module list`` -- lists all currently loaded
  modules in your working environment.  At first, we have none:

  .. code-block:: bash

     $ module list
     Rebuilding cache file, please wait ... done.
     
     
     Lmod Warning: No modules installed

.. index:: 
   single: module; add
   single: module; load

* ``module add`` and ``module load`` -- loads
  a module into your working environment.  For example, at the moment
  the PGI C compiler is not in our PATH:

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

     Currently Loaded Modules:
       1) pgi/13.2-64bit

.. index:: 
   single: module; rm
   single: module; unload

* ``module rm`` and ``module unload`` -- undoes
  a previous "add" or "load" command, removing the module from your
  working environment, e.g.

  .. code-block:: bash

     $ module load fftw
     $ module list

     Currently Loaded Modules:
       1) pgi/13.2-64bit    2) fftw
     $ module unload fftw
     $ module list

     Currently Loaded Modules:
       1) pgi/13.2-64bit

.. index:: 
   single: module; switch
   single: module; swap

* ``module switch`` and ``module swap`` -- this
  does a combination unload/load, swapping out one module for another,
  e.g. 

  .. code-block:: bash

     $ module load mvapich2/1.6/gcc
     $ module list
     Currently Loaded Modules:
       1) pgi/13.2/64bit     2) mvapich2/1.6/gcc
     $ module swap mvapich2/1.6/gcc mvapich2/1.6/gcc-QL
     $ module list
     Currently Loaded Modules:
       1) pgi/13.2/64bit        2) mvapich2/1.6/gcc-QL

.. index:: 
   single: module; display
   single: module; show

* ``module display`` and ``module show`` -- this
  shows detaled information about how a specific module affects your
  environment, e.g.
 
  .. code-block:: bash

     $ module show R-3.0.0
     Rebuilding cache file, please wait ... done.

     ------------------------------------------------------------
        /grid/software/modulefiles/applications/R-3.0.0.lua:
     ------------------------------------------------------------
     whatis("loads R executables in current environment")
     setenv("R_HOME", "/grid/software/R-3.0.0")
     prepend_path("PATH", "/grid/software/R-3.0.0/bin:/grid/software/gcc-4.8
     .0/bin")
     prepend_path("MANPATH", "/grid/software/R-3.0.0/share/man")
     prepend_path("LD_LIBRARY_PATH", "/grid/software/R-3.0.0/lib64:/grid/sof
     tware/R-3.0.0/lib64:/grid/software/gcc-4.8.0/lib64:/grid/software/gcc-4
     .8.0/lib:/grid/software/gmp-5.1.1/lib:/grid/software/mpfr-3.1.2/lib:/gr
     id/software/mpc-1.0.1/lib")

.. index:: module; help

* ``module help`` -- This displays a set of
  descriptive information about the module (what it does, the version
  number of the software, etc.).  This only applies to packages where
  their "help" pages have been installed (none yet on SMU HPC).



Module example
--------------------------------------------------

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

Even for this simple example where we only need to add something to
our PATH, the module system can be invaluable since it is rare that
you know the global location of a file *a priori*.  



Module exercise
--------------------------------------------------

Run Mathematica on SMUHPC, using it to integrate the function
:math:`f(x) = \log(x^3-2)`.  

Hints:

* Find/load the appropriate module.

* Use ``mathematica`` at the command-line.

* Click "Notebook".

* Once in Mathematica, use the "Help"->"Documentation Center" menu and
  search for "Integration". 

* At the Mathematica prompt, after entering a Mathematica command it
  may be executed with [shift]-[enter]. 



.. index:: 
   pair: module; resources

Module resources:
--------------------------------------------------

* `Main Module page <http://modules.sourceforge.net/>`_

* `Module FAQ <http://sourceforge.net/p/modules/wiki/FAQ/>`_



.. index:: version control systems
   seealso: VCS; version control systems

Version control systems
================================================

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

It's why we use "Save As"; you want to save the new file without
writing over the old one.  It's a common problem, and solutions are
usually like this: 

* Make a *single backup copy* (e.g. Document.old.txt).

* If we're clever, we add a *version number* or *date*:
  e.g. Document_V1.txt, DocumentMarch2012.txt.

* We may even use a *shared folder* so other people can see and edit
  files without sending them by email.  Hopefully they rename the 
  file after they save it. 


Why use a VCS?
--------------------------------------------------

Our shared folder/naming system is fine for class projects or one-time
papers, but is exceptionally bad for software projects.  Do you
imagine that the Windows source code sits in a shared folder named
something like "Windows7-Latest-New", for anyone to edit?  Or that
every programmer just works on different files in the same folder?

For projects that are large, fast-changing, or have multiple authors,
a Version Control System (VCS) is critical.  Think of a VCS as a "file
database", that helps to track changes and avoid
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

Shared folders are quick and simple, but can't provide these critical
features. 



General definitions
--------------------------------------------------

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
  and checks it back in with a checkin message ("Added delicious beverage."). 

* The next morning, Bob updates his local working set and sees the
  latest revision of ShoppingList.txt, which contains "milk".

* Bob adds "donuts" to the list, while Alice also adds "eggs" to the
  list.

* Bob checks the list in, with a checking message `"Mmmmm, donuts"
  <https://www.youtube.com/watch?v=8-4P1WPE-Qg>`_. 

* Alice updates her copy of the list before checking it in, and
  notices that there is a conflict.  Realizing that the order of items
  doesn't matter, she merges the changes by putting both "donuts" and
  "eggs" on the list, and checks in the final version.



Standard VCS programs
--------------------------------------------------

.. index:: cvs

CVS
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Originally developed in 1990, `CVS
<https://en.wikipedia.org/wiki/Concurrent_Versions_System>`_ is one of
the oldest version systems still in use today.  It follows a
client-server approach, in which all repository duties are handled by
a server, to which clients connect to "check out" and "check in"
files.

The primary CVS commands are:

.. index:: cvs; add

* ``cvs add`` --  adds a new file/directory to the repository

.. index:: cvs; admin

* ``cvs admin`` -- administration front end for the underlying
  revision control system

.. index:: cvs; checkout

* ``cvs checkout`` -- checkout sources for editing

.. index:: cvs; commit

* ``cvs commit`` -- checks files into the repository

.. index:: cvs; diff

* ``cvs diff`` -- checks for differences between revisions

.. index:: cvs; history

* ``cvs history`` -- shows status of files and users

.. index:: cvs; import

* ``cvs import`` -- import sources into CVS

.. index:: cvs; remove

* ``cvs remove`` -- removes an entry from the repository

.. index:: cvs; status

* ``cvs status`` -- status info on the revisions

.. index:: cvs; tag

* ``cvs tag`` -- add a tag to checked out version

.. index:: cvs; update

* ``cvs update`` -- brings work tree in sync with repository

While there are many `criticisms of CVS
<https://en.wikipedia.org/wiki/Concurrent_Versions_System#Criticism>`_,
it's longevity has resulted in `CVS support by a large number 
of *Integrated Desktop Environments* (IDEs)
<https://en.wikipedia.org/wiki/Concurrent_Versions_System#IDEs_with_support_for_CVS>`_
on all major operating systems with native support for CVS-hosted projects.


.. index:: 
   pair: cvs; resources

CVS resources:

* `Main CVS site <http://cvs.nongnu.org/>`_

* `CVS tutorial <http://www-mrsrl.stanford.edu/~brian/cvstutorial/>`_




.. index:: svn

SVN
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

`Apache Subversion <https://en.wikipedia.org/wiki/Apache_Subversion>`_
(SVN) was initially released in 2000, as an effort to write an
open-source version control system that behaved similarly to CVS, but
with a variety of bug fixes and feature improvements.  Resultingly,
SVN similarly relies on a client-server approach, and it's commands
are quite similar to those for CVS.

The primary SVN commands include:

.. index:: svn; help

* ``svn help`` -- provides a summary of the available commands.

.. index:: 
   single: svn; checkout
   single: svn; co

* ``svn checkout`` or ``svn co`` -- pulls an SVN tree from the server
  (you should only need to do this once).

.. index:: svn; add

* ``svn add`` -- adds a newly-created file or directory to the repository.

.. index:: 
   single: svn; delete
   single: svn; del
   single: svn; remove
   single: svn; rm

* ``svn delete`` or ``svn del`` or ``svn remove`` or ``svn rm`` --
  deletes the local file immediately, and notifies the repository that
  on the next commit, the file should be deleted from there as well.

.. index:: 
   single: svn; status
   single: svn; stat

* ``svn status`` or ``svn stat`` --  displays the status of working directories and files.

.. index:: 
   single: svn; update
   single: svn; up

* ``svn update`` or ``svn up`` -- synchronizes your local version of
  the code with the server. If you have made local changes, it will
  try and merge any changes on the server with your changes on your
  machine. 

.. index:: 
   single: svn; commit
   single: svn; ci

* ``svn commit`` or ``svn ci`` -- recursively sends your changes to
  the SVN server.

  * If called with specific files/directories as arguments, it will
    send only those.

  * If given no arguments it will send all changes.

  * The ``-m`` option should always be used to pass a log message to the command.

.. index:: svn; diff

* ``svn diff`` -- shows all changes between the local version of a
  file and the version in the repository.  May also be used to see
  changes between specific versions of the file with the syntax ``svn diff -r
  revision1:revision2 FILENAME``

.. index:: 
   single: svn; move
   single: svn; mv
   single: svn; rename
   single: svn; ren

* ``svn move SRC DEST`` or ``svn mv SRC DEST`` or ``svn rename SRC
  DEST`` or ``svn ren SRC DEST`` --  moves a file from one directory
  to another or renames a file in your local directory immediately,
  and performs the same changes on the server upon committing.

.. index:: svn; revert

* ``svn revert`` -- replaces a local file(s) with the one in the repository.

.. index:: svn; log

* ``svn log`` –- displays the log messages from checkins to to the repository.

.. index:: svn; resolve

* ``svn resolve`` -- if an update showed a conflict (a file marked
  with a "C"), then once you have manually merged the two versions of
  file, this command will set the file's status to "resolved".


As with any project, SVN also has a number of `criticisms
<https://en.wikipedia.org/wiki/Apache_Subversion#Limitations_and_problems>`_,
but again since it has been widely used for over a decade, subversion
support has been integrated into a variety of `GUI front-ends and IDEs
<https://en.wikipedia.org/wiki/List_of_software_that_uses_Subversion>`_.


.. index:: 
   pair: svn; web hosting

In addition, there are a number of web sites that will host
open-source SVN-based software projects free of charge, including:

* `Google code <http://code.google.com/hosting/>`_

* `SourceForge <http://sourceforge.net/>`_

* `CloudForge <http://info.cloudforge.com/freeplan.html>`_

* `Bounty Source <http://www.bountysource.com/>`_

* `Assembla
  <http://offers.assembla.com/free-subversion-hosting/?affiliate=ianterrell>`_

* `BerliOS Developer <http://developer.berlios.de/>`_



.. index:: 
   pair: svn; web resources

SVN resources:

* `Main SVN site <https://subversion.apache.org/>`_

* `SVN tutorial <http://svnbook.red-bean.com/en/1.7/index.html>`_




.. index:: git

Git
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Originally released in 2005 (by `Linus Torvalds
<https://en.wikipedia.org/wiki/Linus_Torvalds>`_ himself!), `Git
<https://en.wikipedia.org/wiki/Git_(software)>`_ was one of the first
version control systems that followed a *distributed revision control*
model (DRCS), in which it is no longer required to have a single
server that all clients connect with.  Instead, DRCS follows a
peer-to-peer approach. in which each peer's working copy of the
codebase is a fully-functional repository. These work by exchanging
patches (sets of changes) between peers, resulting in some `key
benefits over previous centralized systems 
<https://en.wikipedia.org/wiki/Distributed_revision_control#Distributed_vs._centralized>`_ 

.. index:: git; commands

The `commands
<https://confluence.atlassian.com/display/STASH/Basic+Git+commands>`_
used for interacting with Git are nearly identical to those for SVN,
with a few additions/exceptions: 

.. index:: git; clone

* ``git clone`` -- this is the primary mechanism for retrieving a
  local copy of a Git repository.  Unlike the CVS and SVN ``checkout``
  commands, the result is a full repository that may act as a server
  for other client repositories.

.. index:: git; pull

* ``git pull`` -- this fetches and merges changes on the remote server
  to your working repository.

.. index:: git; push

* ``git push`` -- the opposite of ``pull``, this sends all changes in
  your local repository to a remote repository.


.. index:: 
   pair: git; web hosting

While distributed version control systems no longer require a main
server, it is often useful to have a centralized, "agreed-upon" main
repository that all users can access.  As with subversion, there are a
number of web sites that will host open-source Git-based software
projects free of charge, including: 

* `Bitbucket <https://bitbucket.org/dashboard/overview>`_

* `GitHub <https://github.com/>`_

* `Gitorious <https://gitorious.org/>`_

* `CloudForge <http://www.cloudforge.com/>`_

* `ProjectLocker <http://projectlocker.com/>`_

* `Assembla <http://offers.assembla.com/free-git-hosting/>`_


.. index:: 
   pair: git; resources

Git resources:

* `Main Git site <http://git-scm.com/>`_

* `Git tutorials <http://www.atlassian.com/git/tutorial>`_

* `Git book chapters <http://git-scm.com/book>`_



.. index:: hg
   seealso: mercurial; hg

Mercurial
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

(my favorite)

Like Git, `Mercurial
<https://en.wikipedia.org/wiki/Mercurial_(software)>`_ was first
released in 2005, and is a widely-used distributed revision control
system.  It is primarily implemented using Python, and is available on
all major operating systems.  

.. index:: hg; commands

Again, like Git, Mercurial commands are similar to CVS and SVN, with a
few notable exceptions (note that ``hg`` is the chemical symbol for
mercury): 

.. index:: hg; clone

* ``hg clone`` -- the primary mechanism for retrieving a local copy of
  a mercurial repository; the result of which is a full repository
  that may act as a server for other client repositories.

.. index:: hg; pull

* ``hg pull`` -- this fetches all changes on the remote server and
  adds them to your working repository, but *unlike Git it does not
  merge them in*, allowing you control over which remote changesets
  are incorporated into your local sandbox, and which are not.

.. index:: hg; up

* ``hg up`` -- this is the command that updates your local sandbox
  with changes that have been pulled into your working repository.

.. index:: hg; push

* ``hg push`` -- like Git, this command sends all changes in
  your local repository to a remote repository.


.. index:: 
   pair: hg; web hosting

As with Git and Subversion, there are a variety of web sites that will
host open-source Mercurial repositories free of charge, including: 

* `Bitbucket <https://bitbucket.org/dashboard/overview>`_

* `Assembla <http://www.assembla.com/>`_

* `Google code <http://code.google.com/projecthosting/>`_

* `SourceForge <http://sourceforge.net/>`_

* `CodePlex <http://www.codeplex.com/>`_

* `Pikacode <http://pikacode.com/>`_

* `Beanstalk <http://beanstalkapp.com/>`_

* `Kiln <http://www.fogcreek.com/kiln/>`_


.. index:: 
   pair: hg; resources

Mercurial resources:

* `Main mercurial site <http://mercurial.selenic.com/>`_

* `TortoiseHG -- multi-platform, graphical mercurial client
  <http://tortoisehg.bitbucket.org/>`_ 

* `Mercurial guide <http://hgbook.red-bean.com/>`_

* `Mercurial tutorial <http://mercurial.selenic.com/wiki/Tutorial>`_


Mercurial example
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

We'll get a little experience with using Mercurial to "collaborate" on
a shared project.  We'll first need to load the Mercurial module:

.. code-block:: bash

   $ module load mercurial-2.6.1

The first step in using a version control system
on an existing repository is to do the initial download of the code
from the main repository.  This repository can often be on a
standalone server, on a public a web site, or it can even reside in
someone else's home directory.  Here, we'll use one that I've set up
for this class on the public web server `bitbucket.org
<http://bitbucket.org>`_.   In Mercurial, the initial download of the
code uses the ``clone`` command:

.. code-block:: bash

   $ hg clone ssh://hg@bitbucket.org/drreynolds/smuhpc-workshop-example

When the command completes, you should have a new directory named
``smuhpc-workshop-example``.  Enter that directory,

.. code-block:: bash

   $ cd smuhpc-workshop-example
   $ ls
   driver.cpp     vector_difference.cpp   vector_sum.cpp
   one_norm.cpp   vector_product.cpp 

You should notice the files we used earlier in this session.  Since
Mercurial is a *distributed* version control system, this
directory is now a new repository of your own. 

In this directory, add a new file of the form *lastname.txt*
containing your first name, e.g.

.. code-block:: bash

   $ echo "Daniel" > Reynolds.txt

.. index:: hg; status

To see which files have changed in comparison with the last saved
state of the repository, you can use the ``status`` command:

.. code-block:: bash

   $ hg status
   ? Reynolds.txt


.. index:: hg; add

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


.. index:: hg; diff

If you want to see the specific changes that have been made to all of
the Mercurial-tracked files, you can use the ``diff`` command:

.. code-block:: bash

   $ hg diff
   diff -r ad44a3024020 Reynolds.txt
   --- /dev/null	Thu Jan 01 00:00:00 1970 +0000
   +++ b/Reynolds.txt	Fri May 31 13:46:17 2013 -0500
   @@ -0,0 +1,1 @@
   +Daniel

where we see that there is a new line "Daniel" (denoted by the ``+``)
that has been added.

.. index:: hg; commit

To save this change into the repository, we must ``commit`` the
changes.  To do so, we must supply both a log message using the ``-m``
flag, and our name (in order to give credit and/or lay blame) with the
``-u`` flag.  For example, my commit message could be something like
this: 

.. code-block:: bash

   $ hg commit -u dreynolds -m "added a file with my name" Reynolds.txt

Once this command has completed, we see that the local directory is
current with our local repository: 

.. code-block:: bash

   $ hg status

(note that nothing is listed).  

When working on a project with others, you will eventually wish to
share your code by "pushing" it back up to a shared repository.  This
can also be quite helpful if you develop your project on different
computers, so that instead of copying the files manually by email,
``rsync`` or ``scp``, you can just push your changes up to the
repository from one computer, and clone/pull them down to another.

The command to push files back to the main repository is ``push``.  We
will not do so here, since in order to push to `bitbucket.org
<http://bitbucket.org>`_ you must first set up a Bitbucket account.

However, if you did have a Bitbucket account, prior to pushing your
code, you should always retrieve any changes that your collaborators
have made to the repository by using a "pull" and an "update" (and
possibly a "merge" if necessary).  To retrieve these changes:

.. code-block:: bash

   $ hg pull
   $ hg update

If the ``update`` command returns successfully, then you can push your
changes back to my example repository with the command

.. code-block:: bash

   $ hg push

.. index:: hg; merge

However, if the ``update`` command complained about changes needing to
be merged (meaning that someone else checked things in, so your
changes need to be merged with his/hers), then you can ``merge`` via

.. code-block:: bash

   $ hg merge

Assuming that your modifications do not collide with anyone else's,
this should be successful, in which case you need to check in the
merge

.. code-block:: bash

   $ hg commit -u dreynolds -m "merged to tip"

Once you're certain that you have finished retrieving and merging all
changes from the shared repository, you ``push`` via

.. code-block:: bash

   $ hg push


.. note::

   Typically this process is not difficult, since typically you will
   be editing different files than your collaborators.


