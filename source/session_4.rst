More Programming (Tue. PM)
============================



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

   % cp ~dreynolds/SMUHPC_tutorial/session4.tgz .


Unzip/untar this file with the command

.. code-block:: bash

   % tar -zxf session4.tgz

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

   % g++ -c one_norm.cpp

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

   % g++ driver.o one_norm.o vector_difference.o vector_product.o vector_sum.o -lm

This creates an executable file named ``a.out``, which is the default
(entirely non-descriptive) name given by most compilers to the
resulting executable.  The additional argument ``-lm`` is used to tell
``g++`` to link these functions against the built-in math library (so
that we can link with the absolute value function, ``fabs()``, that is
called inside the ``one_norm.cpp`` file.

You can instead give your executable a more descriptive name with the
``-o`` option:

.. code-block:: bash

   % g++ -o driver.exe driver.o one_norm.o vector_difference.o vector_product.o vector_sum.o -lm

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

   % emacs Makefile &

to have ``emacs`` create the file ``Makefile`` in the background, so
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

* list of module functions (e.g. avail, add, swap)

* example: hard way to compile something, then easy way to do the same
  once module is used.

* exercise: set up relevant modules to use a set of packages, then set
  up Makefile to use new environment variables to easily build a piece
  of source code.



Module resources:

* `Main Module page <http://modules.sourceforge.net/>`_

* `Module FAQ <http://sourceforge.net/p/modules/wiki/FAQ/>`_



Version control systems
---------------------------


**FILL THIS IN**



CVS
^^^^^

**FILL THIS IN**

Outline: 

* high-level description, including date it was invented
* listing of main commands


CVS resources:

* `CVS tutorial <http://www-mrsrl.stanford.edu/~brian/cvstutorial/>`_




SVN
^^^^^

**FILL THIS IN**

Outline: 

* high-level description, including date it was invented
* listing of main commands
* listing of free public repositories supporting SVN


SVN resources:

* `SVN tutorial <http://svnbook.red-bean.com/en/1.7/index.html>`_




Git
^^^^^

**FILL THIS IN**

Outline: 

* high-level description, including date it was invented
* listing of main commands
* listing of free public repositories supporting Git


Git resources:

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


Mercurial resources:

* `Main mercurial site <http://mercurial.selenic.com/>`_

* `TortoiseHG -- multi-platform, graphical mercurial client
  <http://tortoisehg.bitbucket.org/>`_ 

* `Mercurial guide <http://hgbook.red-bean.com/>`_

* `Mercurial tutorial <http://mercurial.selenic.com/wiki/Tutorial>`_
