Introduction to Scripts and Programs (Tue. AM)
========================================================


Linux Processes
--------------------

A process is an executing program identified by a unique PID (process
identifier). To see information about your processes, with their
associated PID and status, type 

.. code-block:: bash

   % ps

A process may be run in the foreground, in the background, or be
suspended. Most Linux jobs are run in the foreground; for those jobs
the shell does not return the command prompt until the process has
finished executing.  

Some processes take a long time to run and hold up the
terminal. Backgrounding a long process has the effect that the UNIX
prompt is returned immediately, and other tasks can be carried out
while the original process continues executing. 


Running background processes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To background a process, type an ``&`` at the end of the command line
when the program is executed. For example, the command ``sleep`` waits
a given number of seconds before continuing. Type 

.. code-block:: bash

   % sleep 10

This will wait 10 seconds before returning the command prompt
``%``. Until the command prompt is returned, you can do nothing except
wait. 

To run ``sleep`` in the background, type

.. code-block:: bash

   % sleep 10 &

   [1] 6259

The ``&`` runs the job in the background and returns the prompt
straight away, allowing you do run other programs while waiting for
that one to finish.  Of course, running ``sleep`` in the background
defeats the purpose of this specific program, but you get the idea.

The first line in the above example is typed in by the user; the next
line, indicating job number and PID, is returned by the machine. The
user is be notified of a job number (numbered from 1) enclosed in
square brackets, together with a PID and is notified when a background
process is finished. Backgrounding is useful for jobs which will take
a long time to complete. 


Backgrounding a current foreground process
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

At the prompt, type

.. code-block:: bash

   % sleep 600

This will not return to the prompt for 10 minutes.  You can suspend
the process running in the foreground by typing ``^Z``, i.e. hold down
the "Ctrl" key and type "z".  This has suspended the job.  To put it
in the background, type  

.. code-block:: bash

   % bg

and hit "return".

Note: do not background terminal-based programs that require user
interaction e.g. ``vi`` or ``nano`` 


Listing suspended and background processes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

When a process is running, backgrounded or suspended, it will be
entered onto a list along with a job number. To examine this list,
type 

.. code-block:: bash

   % jobs

An example of a job list could be

.. code-block:: bash

   [1] Suspended sleep 600
   [2] Running firefox
   [3] Running matlab

To restart (foreground) a suspended or backgrounded processes, type

.. code-block:: bash

   % fg <jobnumber>

For example, to bring ``sleep 600`` to the foreground, type

.. code-block::

   % fg 1

Typing ``fg`` with no job number foregrounds the most-recently
suspended or backgrounded process. 


Killing a process
^^^^^^^^^^^^^^^^^^^

It is sometimes necessary to kill a process (for example, when an
executing program is in an infinite loop).  To kill a job running in
the foreground, type ``^C`` (control c). For example, run 

.. code-block:: bash

   % sleep 100
   ^C

To kill a suspended or background process, type

.. code-block:: bash

   % kill %<jobnumber>

For example, run

.. code-block:: bash

   % sleep 100 &
   % jobs

If ``sleep 100`` is job number 4, type

.. code-block:: bash

   % kill %4

To check whether this has worked, examine the job list again to see if
the process has been removed. 


Alternatively, any process owned by a user (not just one launched from
that terminal) can be killed by finding the process' corresponding ID
numbers (PID) and using ``kill``

.. code-block:: bash

   % sleep 1000 &
   % ps

   PID TT S TIME COMMAND
   20077 pts/5 S 0:05 sleep 1000
   21563 pts/5 T 0:00 firefox
   21873 pts/5 S 0:25 emacs

To kill off the process ``sleep 1000``, type

.. code-block:: bash

   % kill 20077

and then type ``ps`` again to see if it has been removed from the
list. 

If for some reason a process that you own refuses to be killed, you
can try using the ``-9`` option, i.e. type

.. code-block:: bash

   % kill -9 20077

Note: It is not possible to kill off other users' processes!


Summary of commands for interacting with running processes:


======================  ==============================================
Command                 Meaning
======================  ==============================================
*command* &             run *command* in background
^C                      kill the job running in the foreground
^Z                      suspend the job running in the foreground
bg                      background the currently-suspended job
jobs                    list current jobs launched from this shell
fg 1                    foreground job number 1
kill %1                 kill job number 1
ps                      list current processes
kill 26152              kill process number 26152
======================  ==============================================




Writing shell scripts
------------------------------------------------------

A *shell script* is merely a file that contains a set of commands that
you would type at the prompt, but that are grouped together for
repeated use.  In a shell script, your commands are passed on to the
requested Linux shell, which interprets each command in order and
executes them as if they'd been performed interatively.

**FILL THIS IN**


Executing shell scripts
^^^^^^^^^^^^^^^^^^^^^^^^^

**FILL THIS IN**


BASH resources:

* `BASH Programming -- Introductory How-To
  <http://tldp.org/HOWTO/Bash-Prog-Intro-HOWTO.html>`_ 

* `Advanced BASH-Scripting Guide <http://tldp.org/LDP/abs/html/>`_



Python resources:

* `Introductory Python Tutorial <http://www.learnpython.org/>`_

* `The Definitive Python Tutorial <http://docs.python.org/2/tutorial/>`_ 






Compiled programs
------------------------------------------------------

All high-level language code must be converted into a form the
computer understands.  In the above shell scripts, this translation is
handled by the shell itself.  Unfortunately, such *interpreted*
languages that must act on each command one-at-a-time typically run 
much slower than a computer processor is able.  

Alternately, a *compiled program* is one in which a separate program
is used to translate the full set of human-readable commands into an
executable, and in so doing is able to optimize how these commands are
performed.  This translation process is handled by a *compiler*, which
will typically perform a suite of optimizations including grouping
repeated calculations together into *vector operations*, pre-fetching
data from main memory before it is required by the program, or even
re-ordering commands to maximize data reuse within fast cache memory.

For example, C++ language source code is converted into an executable
through the following process.  The human-readable source code is
translated into a lower-level *assembly language*. This assembly
language code is then converted into object code which are fragments
of code which the computer processor understands directly. The final
stage the compiler performs involves linking the object code to code
libraries which contain built-in system functions.  After this linking
stage, the compiler outputs an executable program.  

To do all these steps by hand is complicated and beyond the capability
of the ordinary user. A number of utilities and tools have been
developed for programmers and end-users to simplify these steps. 



Compiling your own codes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**FILL THIS IN**

Fortran resources:

* `Interactive Fortran 90 Programming Course
  <http://www.liv.ac.uk/HPC/HTMLFrontPageF90.html>`_ 

* `Fortran 90 Tutorial
  <http://www.cs.mtu.edu/~shene/COURSES/cs201/NOTES/fortran.html>`_


C++ resources:

* `C++ Language Tutorial <http://www.cplusplus.com/doc/tutorial/>`_ 

* `Interactive C++ Tutorial (focuses on object-oriented programming)
  <http://www.learncpp.com/>`_




Example: compiling an example "hello world" program
""""""""""""""""""""""""""""""""""""""""""""""""""""""

**FILL THIS IN**







Compiling "typical" Linux packages
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

As the number of UNIX variants increased, it became harder to write
programs which would be portable to all variants. Developers
frequently did not have access to every system, and the
characteristics of some systems changed from version to version. The
GNU configure and build system simplifies the building of programs
distributed as source code. All programs are built using a simple,
standardized, two step process. The program builder need not install
any special tools in order to build the program. 

The configure shell script attempts to guess correct values for
various system-dependent variables used during compilation. It uses
those values to create a Makefile in each directory of the package. 

For packages that use this approach, the simplest way to compile a
package is: 

1. ``cd`` to the directory containing the package's source code.

2. Type ``./configure`` to configure the package for your system.

3. Type ``make`` to compile the package.

4. Optionally, type ``make check`` to run any self-tests that come
   with the package. 

5. Type ``make install`` to install the programs and any data
   files and documentation. 

6. Optionally, type ``make clean`` to remove the program binaries
   and object files from the source code directory. 

The configure utility supports a wide variety of options. You can
usually use the ``--help`` option to get a list of interesting
options for a particular configure script. 

The only generic option you are likely to use at first is the
``--prefix`` option.  The directory named by this option will hold
machine independent files such as documentation, data and
configuration files. 


Example: compiling the program "units"
""""""""""""""""""""""""""""""""""""""""

For this example, we will download and compile a piece of free
software that converts between different units of measurements. 


**Downloading source code**

First create a download directory 

.. code-block:: bash

   % mkdir download

Download the software using ``wget`` into your new download directory

.. code-block:: bash

   % cd download
   % wget http://faculty.smu.edu/reynolds/unixtut/units-1.74.tar.gz


**Extracting the source code**

List the contents of your download directory 

.. code-block:: bash

   % ls -l

As you can see, the filename ends in tar.gz. The ``tar`` command turns
several files and directories into one single ".tar" file. This is
then compressed using the ``gzip`` command (to create a ".tar.gz"
file). 

First unzip the file using the ``gunzip`` command. This will create a .tar file

.. code-block:: bash

   % gunzip units-1.74.tar.gz

Then extract the contents of the tar file.  

.. code-block:: bash

   % tar -xvf units-1.74.tar

Again, list the contents of the directory, then go to the ``units-1.74`` sub-directory

.. code-block:: bash

   % ls -l 
   % cd units-1.74


**Configuring and creating the Makefile**

The first thing to do is carefully read the ``README`` and ``INSTALL``
text files (use the ``less`` command). These contain important
information on how to compile and run the software

.. code-block:: bash

   % less README

(use the arrow keys to scroll up/down; hit ``q`` to exit).

The ``units`` package uses the GNU configure system to compile the
source code. We will need to specify the installation directory, since
the default will be the main system area which you do not have write
permissions for. We need to create an install directory in your home
directory

.. code-block:: bash

   % mkdir ~/units-1.7.4

Then run the ``configure`` utility setting the installation path to this

.. code-block:: bash

   % ./configure --prefix=$HOME/units-1.7.4

NOTE: The ``$HOME`` variable is an example of an environment
variable. The value of ``$HOME`` is the path to your home
directory. Type 

.. code-block:: bash

   % echo $HOME 

to show the value of this variable.

If ``configure`` has run correctly, it will have created a
``Makefile`` with all necessary options to compile the program.  You
can view the ``Makefile`` if you wish (use the ``less`` command), but do
not edit the contents of this file unless you know what you are doing.


**Building the package**

Now you can go ahead and build the package by running the ``make`` command

.. code-block:: bash

   % make

After a short while (depending on the speed of the computer), the
executables will be created. You can check to see everything compiled
successfully by typing 

.. code-block:: bash

   % make check

If everything is okay, you can now install the package.  

.. code-block:: bash

   % make install

This will install the files into the ``~/units-1.7.4`` directory you created earlier.


**Running the software**

You are now ready to run the software (assuming everything worked).
Unlike most of the commands you have used so far, the new ``units``
executable is not in your ``PATH``, so you cannot run it from your
current directory:

.. code-block:: bash

   % units

Instead, you must executables that are not in your ``PATH`` by
providing the pathname to the executable.  One option for this is to
provide the path name from your current location, e.g.

.. code-block:: bash

   % ./units-1.7.4/bin/units

Alternately, you can navigate through the directory structure until
you are in the same directory as the executable,

.. code-block:: bash

   % cd ~/units-1.7.4

If you list the contents of the units directory, you will see a number of subdirectories.


==========    ===================================
Directory     Contents
==========    ===================================
bin           The binary executables
info          GNU info formatted documentation
man           Man pages
share         Shared data files
==========    ===================================


To run the program, change to the ``bin`` directory and type 

.. code-block:: bash

   % ./units

As an example, convert 6 feet to meters,

.. code-block:: bash

   You have: 6 feet
   You want: meters 

   * 1.8288

If you get the answer 1.8288, congratulations, it worked. Type ``^c`` to exit the program.

To view what units the program can convert between, view the data file
in the ``share`` directory (the list is quite comprehensive). 

To read the full documentation, change into the ``info`` directory and type 

.. code-block:: bash

   % info --file=units.info
