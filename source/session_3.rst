Session 3: Introduction to Scripts and Programs
========================================================

*Instructor: Dan Reynolds*

*Assistants: Amit Kumar, Merlin Wilkerson, Doug Tucker*


Getting started
------------------

Retrieve the set of files for this session either through
:download:`clicking here <code/session3.tgz>` or by copying the
relevant files at the command line:

.. code-block:: bash

   $ cp ~dreynolds/SMUHPC_tutorial/session3.tgz .


Unzip/untar this file with the command

.. code-block:: bash

   $ tar -zxf session3.tgz

You should now see a new subdirectory entitled ``session3`` in your
current directory.  This is where we will work for the rest of this
session.




Linux Processes
--------------------

A process is an executing program identified by a unique PID (process
identifier). To see information about your processes, with their
associated PID and status, type 

.. code-block:: bash

   $ ps

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

   $ sleep 10

This will wait 10 seconds before returning the command prompt
``$``. Until the command prompt is returned, you can do nothing except
wait. 

To run ``sleep`` in the background, type

.. code-block:: bash

   $ sleep 10 &

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

   $ sleep 600

This will not return to the prompt for 10 minutes.  You can suspend
the process running in the foreground by typing ``^z``, i.e. hold down
the "control" key and type "z".  This has suspended the job.  To put it
in the background, type  

.. code-block:: bash

   $ bg

and hit "return".

Note: do not background terminal-based programs that require user
interaction e.g. ``vi`` or ``nano`` 


Listing suspended and background processes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

When a process is running, backgrounded or suspended, it will be
entered onto a list along with a job number. To examine this list,
type 

.. code-block:: bash

   $ jobs

An example of a job list could be

.. code-block:: bash

   [1] Suspended sleep 600
   [2] Running firefox
   [3] Running matlab

To restart (foreground) a suspended or backgrounded processes, type

.. code-block:: bash

   $ fg <jobnumber>

For example, to bring ``sleep 600`` to the foreground, type

.. code-block:: bash

   $ fg 1

Typing ``fg`` with no job number foregrounds the most-recently
suspended or backgrounded process. 


Killing a process
^^^^^^^^^^^^^^^^^^^

It is sometimes necessary to kill a process (for example, when an
executing program is in an infinite loop).  To kill a job running in
the foreground, type ``^c`` (control c). For example, run 

.. code-block:: bash

   $ sleep 100
   ^C

To kill a suspended or background process, type

.. code-block:: bash

   $ kill %<jobnumber>

For example, run

.. code-block:: bash

   $ sleep 100 &
   $ jobs

If ``sleep 100`` is job number 4, type

.. code-block:: bash

   $ kill %4

To check whether this has worked, examine the job list again to see if
the process has been removed. 


Alternatively, any process owned by a user (not just one launched from
that terminal) can be killed by finding the process' corresponding ID
numbers (PID) and using ``kill``

.. code-block:: bash

   $ sleep 1000 &
   $ ps

   PID TT S TIME COMMAND
   20077 pts/5 S 0:05 sleep 1000
   21563 pts/5 T 0:00 firefox
   21873 pts/5 S 0:25 gedit

To kill off the process ``sleep 1000``, type

.. code-block:: bash

   $ kill 20077

and then type ``ps`` again to see if it has been removed from the
list. 

If for some reason a process that you own refuses to be killed, you
can try using the ``-9`` option, i.e. type

.. code-block:: bash

   $ kill -9 20077

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

One significant benefit of scripting languages, in contrast with
compiled programs (later in this session) is that because each
instruction is executed in order, development of shell scripts is
easier since the program will proceed up to the point where you have a
bug before stopping, whereas with compiled languages you cannot run
anything until the entire program is somewhat bug-free (i.e. until the
compiler believes it is bug-free).  However, this ease of development
comes at a cost, since scripted programs typically run significant
slower than their compiled equivalents, so it is standard to first
test new algorithms in scripting languages, and then translate them to
more efficient compiled code prior to large simulations.

There are a variety of Linux shells, but the two most popular shells
are currently BASH and TCSH.  As most new accounts on SMUHPC are set
up to use BASH, we'll provide examples for that type of shell here.
Alternately, there are also a variety of specially-designed scripting
languages used throughout scientific computing, such as Python and
Perl.  As we'll be using Python in session 7, we'll provide some
examples of Python scripts here as well.



Scripting vs. using a shell or GUI
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

While it is certainly possible to manually type all commands required
to compile a code, run it in a variety of ways, and even post-process
the results, this makes it hard to reproduce the results unless you
remember exactly the steps that were taken.  Instead, it is
preferrable to write scripts that set all the appropriate input
parameters for your program, run it in the desired manner, and process
the results in such a way that rerunning the scripts will give exactly
the same results.  With some plotting tools such a script can be
automatically generated after you’ve come up with the optimal plot by
fiddling around with the GUI or by typing commands at the prompt.  It
is worth figuring out how to do this most easily for your own tools
and work style.  If you always create a script for each figure, and
then check that it works properly, then you will be able to easily
reproduce the figure again later.  Since *reproducibility* is a
cornerstone of the modern scientific method, this additional effort
can save you later on.  For example, it often happens that the
referees of a journal or members of a thesis committee will suggest
improving a figure by plotting something differently, perhaps as
simple as increasing the font size so that the labels on the axes can
be read. If you have the code that produced the plot this is easy to
do in a few minutes. If you don’t, it may take days (or longer) to
figure out again exactly how you produced that plot to begin with. 

A second, but almost equally important reason for creating scripts is
that you may need to do the same thing (or nearly the same thing)
repeatedly during the course of your experimentation.  This can arise
out of a need to explore a parameter space of simulation inputs, or
when post-processing many experimental outputs.  In such scenarios,
even a moderate amount of effort to create a script can easily pay
dividends if you must do the task repeatedly.  

.. figure:: figs/is_it_worth_the_time.png
   :scale: 100 %

   xkcd comic 1205, `Is It Worth the Time? <http://xkcd.com/1205/>`_



BASH scripts
^^^^^^^^^^^^^^^

Basics of BASH shell scripting:

* The first line of the shell script file should include the line

  .. code-block:: bash

     #!/bin/bash

  to indicate that the script contents should be executed by the BASH
  shell.

* Lines beginning with a ``#`` character are interpreted as comments
  (except for the first line).

* Variables may be defined in-line via setting *variable*=*value*,
  e.g.
 
  .. code-block:: bash

     CXX=g++
     STUDENTS=(Sally Frankie Wally Jenny Ahmad)

  Here, ``CXX`` is a scalar variable, while ``STUDENTS`` is an array.
  Variables may be referenced subsequently in the script via placing a
  dollar-sign in front, e.g.

  .. code-block:: bash

     $CXX driver.cpp -o driver.exe

* Arrays may also be created by merely using the syntax

  .. code-block:: bash

     a[0] = 1
     a[1] = 0
     a[2] = 0

  Entries of an array may be accessed using ``$`` and braces ``{}``, e.g.

  .. code-block:: bash

     ${a[1]}

* Loops may be performed via iteration over a range (version 3.0+):

  .. code-block:: bash

     for i in {1..5}
     do
        echo "The number is $i"
     done

  that gives the output

  .. code-block:: text

     The number is 1
     The number is 2
     The number is 3
     The number is 4
     The number is 5

  or over a range with a user-supplied increment (version 4.0+, not
  installed on SMUHPC):

  .. code-block:: bash

     for i in {1..5..2}
     do
        echo "The number is $i"
     done

  that gives the output

  .. code-block:: text

     The number is 1
     The number is 3
     The number is 5

  More familarly to C, C++ and Java users is the *three-expression*
  loop syntax, e.g.

  .. code-block:: bash

     for ((i=1; i<=5; i+=2))
     do
        echo "The number is $i"
     done

  that gives the output

  .. code-block:: text

     The number is 1
     The number is 3
     The number is 5

  Loops may also iterate over a list, e.g.

  .. code-block:: bash

     for i in Sally Jesse Rafael
     do
        echo "The entry is $i"
     done

  that gives the output

  .. code-block:: text

     The entry is Sally
     The entry is Jesse
     The entry is Rafael

  or even an array-valued variable, e.g.

  .. code-block:: bash
     
     students=(Sally Frankie Wally Jenny Ahmad)
     for i in "${students[@]}"
     do
        echo "The student is $i"
     done

  that gives the output

  .. code-block:: text

     The student is Sally
     The student is Frankie
     The student is Wally
     The student is Jenny
     The student is Ahmad
  
* Loop control statements: the ``break`` statement may be used in a
  loop just as in C and C++, in that it will break out of the smallest
  enclosing loop surrounding the ``break`` statement.  Also similarly
  to C and C++, the ``continue`` statement stops executing the
  statements within that iteration of the loop and jumps to the next
  loop iteration. 

* If-elif-else statements may be performed via the syntax

  .. code-block:: bash
     
     if [condition]
     then
        statements1
     elif [condition]
     then
        statements2
     else
        statements3
     fi

* Functions may defined via the syntax

  .. code-block:: bash
     
     hello()
     {
        echo "Hello world!"
     }

  All function definitions must have an empty set of parentheses
  ``()`` following the function name, and the function statements must
  be enclosed in braces ``{}``.  Function arguments may be accessed
  with the variables ``$1``, ``$2``, etc., where the numeric value
  corresponds to the order in which the argument was passed to the
  function. 

  When called, the ``()`` are not included (see example below).



As an example, consider the following script

.. code-block:: bash

   #!/bin/bash
   # BASH shell script example
   # Dan Reynolds
   # May 2013
   
   # define the "odd" function, takes one argument
   odd() {
      echo "  $1 is odd"
   }
   
   # define the "even" function, takes one argument
   even() {
      echo "  $1 is even"
   }
   
   # define the "other" function, takes one argument
   other() {
      echo "  $1 is neither even nor odd"
   }
   
   # loop over some integers, checking even/odd
   for i in {1..20}; do
      m=$(($i % 2))
      if [ $m -eq 0 ]; then
         even $i
      elif [ $m -eq 1 ]; then
         odd $i
      else
         other $i
      fi
   done

The structure of this example should be obvious from the preceding
short examples, except that there are a few notable exceptions:

* We perform arithmetic: these operations must be of the form
  ``$(( expression ))``.  

* We use the "modulus" operator, ``%``.  Other allowable arithmetic
  operators include ``+``, ``-``, ``*`` and ``/``.

* We perform the logical "equality" operation via ``-eq``.  The
  inequality logical operation is ``-ne``.  The mathematical :math:`<`,
  :math:`\le`, :math:`>` and :math:`\ge` operators are given by
  ``-lt``, ``-le``, ``-gt`` and ``-ge``.

* BASH logic operations may be combined using the standard ``&&``
  (and), ``||`` (or) and ``!`` (not). 

* Function arguments are passed in following the function name; more
  than one function argument may be supplied (though not shown here).




Python scripts
^^^^^^^^^^^^^^^

Basics of Python shell scripting:

* The first line of the shell script file can include the line

  .. code-block:: python

     #!/usr/bin/env python

  to indicate that the script contents should be executed by the BASH
  shell.  However, since Python is installed in different locations on
  many systems, this may be inadvisable, since Python scripts are
  typically run from within a Python environment.

* Lines beginning with a ``#`` character are interpreted as comments
  (except for the first line).

* Variables may be defined in-line via setting *variable*=*value*,
  e.g.
 
  .. code-block:: python

     r = 7
     h = 6
     pi = 3.1415926535897932

  Here, ``N`` is a scalar integer variable and ``pi`` is a scalar
  double-precision variable.  Variables may be referenced subsequently 
  in the script by just writing the variable name, e.g.

  .. code-block:: python

     r = 7
     h = 6
     pi = 3.1415926535897932
     Vol = pi * h * r**2

  Note, Python allows the standard arithmetic operations ``+``, ``-``,
  ``*`` and ``/``, as well as exponentiation via the ``**`` operator.
  Additionally, the ``//`` operator performs division and rounds the
  result down to the nearest integer, while the ``%`` operator
  performs the modulus.

* Python allows a multitude of "array" types, the two most common
  being lists and Numpy's numerical arrays.  A Python *list* is very
  flexible (entries can be anything), but can be very inefficient.
  Lists are declared as a comma-separated list of items enclosed by
  parentheses, e.g.
 
  .. code-block:: python

     mylist = (7, 1.e-4, 'fred')

  Due to this inefficiency, the Numpy extension module to Python was
  created with numerical array types.  Officially called ``ndarray``,
  these are more commonly referred to by the alias ``array`` (these
  differ from the standard Python library ``array`` class).  These may
  be created using a combination of Numpy's ``array`` function and
  square brackets to hold the array values, e.g.

  .. code-block:: python

     from numpy import *
     tols = array([1.e-2, 1.e-4, 1.e-6, 1.e-8])

  In both scenarios (lists and Numpy arrays), array elements may be
  indexed using brackets ``[]``, with indices starting at 0, e.g.

  .. code-block:: python

     from numpy import *
     tols = array([1.e-2, 1.e-4, 1.e-6, 1.e-8])
     print tols[0]

  Lastly, Python allows a simple approach to creating lists of
  equally-spaced values, via the ``range()`` function.  A few
  examples:

  .. code-block:: python

     print range(10)
     print range(5, 10)
     print range(0, 10, 3)
     print range(-10, -100, -30)

  which has output

  .. code-block:: text

     [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
     [5, 6, 7, 8, 9]
     [0, 3, 6, 9]
     [-10, -40, -70]

  Here, when given three arguments, the first is the initial value,
  the second is the upper bound, and the third argument is the
  increment.  When given two arguments, an increment of 1 is
  assumed. When given one argument, a starting value of 0 and an
  increment of 1 are assumed. 

* Loops may be performed via iteration over a list or an array:

  .. code-block:: python

     words = ['platypus', 'orange', 'non sequitur']
     for w in words:
        print w
        print len(w)
     print words

  which has output

  .. code-block:: text

     platypus
     8
     orange
     6
     non sequitur
     12
     ['platypus', 'orange', 'non sequitur']

  Note that to begin a "for" loop, the line must end in a colon
  ``:``.  All statements within the loop must be indented equally, and
  the loop ends with the first statement where that indention is
  broken.

  As a second example, consider

  .. code-block:: python

     for i in range(5):
        print i

  that gives the output

  .. code-block:: text

     0
     1
     2
     3
     4

* Loop control statements: the ``break`` statement may be used in a
  loop just as in C and C++, in that it will break out of the smallest
  enclosing ``for`` or ``while`` loop surrounding the ``break``
  statement.  Also similarly to C and C++, the ``continue`` statement
  stops executing the statements within that iteration of the loop and
  jumps to the next loop iteration.

* If-elif-else statements may be performed via the syntax

  .. code-block:: python
     
     if condition1:
        statements1
     elif condition2:
        statements2
     else:
        statements3

* Functions may defined via the syntax

  .. code-block:: python
     
     def hello():
        echo "Hello world!"

  In Python, there are no braces surrounding a function contents; just
  as with ``if`` statents and ``for`` loops, the contents of a
  function are determined as those statements following the colon
  ``:``, that are indented from the ``def``, and that precede a break
  in that indentation.

  Functions may also allow input and return arguments, e.g.

  .. code-block:: python
     
     def volume(r, h):
        pi = 3.1415926535897932
        Vol = pi * h * r**2
	return Vol

  Similarly, functions can allow multiple return values by enclosing
  them in brackets, e.g.

  .. code-block:: python
     
     def birthday():
        month = March
        day = 24
	return [month, day]


As a more lengthy example (akin to the BASH example above), consider
the following script 

.. code-block:: python

   #!/usr/bin/env python
   # Python shell script example
   # Dan Reynolds
   # May 2013

   # define the "odd" function, takes one argument
   def odd(val):
      print "  ", val, " is odd"

   # define the "even" function, takes one argument
   def even(val):
      print "  ", val, " is even"
   
   # define the "other" function, takes one argument
   def other(val):
      print "  ", val, " is neither even nor odd"
   
   # loop over some integers, checking even/odd
   for i in range(1,21):
      m = i % 2
      if m == 0:
         even(i)
      elif m == 1:
         odd(i)
      else:
         other(i)


As with the previous BASH example, the structure of this example
should be obvious from the preceding explanations, except that there
are a few notable exceptions: 

* We perform the logical "equality" operation via ``==``.  The
  inequality logical operation is ``!=``.  Similarly, ``<``, ``<=``,
  ``>`` and ``>=`` correspond to the the mathematical :math:`<`,
  :math:`\le`, :math:`>` and :math:`\ge` operators.   

* Python logic operations may be combined using ``and``, ``or`` and
  ``not`` (self-explanatory).




Executing shell scripts
^^^^^^^^^^^^^^^^^^^^^^^^^

Shell scripts may be executed in one of two ways.  If the script
already has *execute* permissions (`ls -l` will show an "x" in the
fourth column from the left), then it may be executed like any other
Linux program, through entering the script name at the command prompt.
Alternately, a script may be executed by supplying the file name as an
argument to the appropriate shell.

For example, you may execute the example BASH script from above via

.. code-block:: bash

   $ bash bash_example.sh

Alternately, if the first line was set to 

.. code-block:: bash

   #!/bin/bash

then it may be executed by first changing the file permissions to
"executable" 

.. code-block:: bash

   $ chmod +x bash_example.sh

and then running it like any other Linux program

.. code-block:: bash

   $ ./bash_example.sh

Similarly, you may execute the example Python script from above via

.. code-block:: bash

   $ python python_example.py

It can also be called from an interactive Python session via 

   >>> exefile("python_example.py")

where the ``>>>`` corresponds to the Python prompt (in contrast with
the BASH prompt, ``$``).  Similarly to BASH, if the first line of the
Python file is set to

.. code-block:: python

   #!/usr/bin/env python

then it may be given execute permissions and run directly via

.. code-block:: bash

   $ chmod +x python_example.py
   $ ./python_example.py


Additional resources on both BASH and Python scripting are provided
below. 

BASH resources:

* A Quick Introduction to BASH Programming: `Part 1
  <http://www.codecoffee.com/tipsforlinux/articles2/043.html>`_ and
  `Part 2 <http://www.codecoffee.com/tipsforlinux/articles2/044.html>`_ 

* `BASH Programming -- Introductory How-To
  <http://tldp.org/HOWTO/Bash-Prog-Intro-HOWTO.html>`_ 

* `Advanced BASH-Scripting Guide <http://tldp.org/LDP/abs/html/>`_



Python resources:

* `Python short course
  <http://faculty.washington.edu/rjl/classes/am583s2013/notes/index.html#python>`_

* `Numpy tutorial <http://www.scipy.org/Tentative_NumPy_Tutorial>`_

* `Introductory Python Tutorial <http://www.learnpython.org/>`_

* `The Definitive Python Tutorial <http://docs.python.org/2/tutorial/>`_ 





Scripting exercise
^^^^^^^^^^^^^^^^^^^^^^

Construct your own BASH or Python script that uses the 
`Sieve of Eratosthenes
<https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes>`_ 
to find all of the prime numbers between 2 and 1000:

1. Create an array of candidate primes of length 1001, where all
   entries are initialized to the value 1.

2. Eliminate 0 and 1 from the list of primes by setting the 0th and 1st
   entries of the array to 0.

3. Initialize a variable ``p`` to 2.

4. Starting from ``p``, eliminate all multiples of ``p`` from the list
   by setting the values at those points to 0.

5. Find the first number greater than ``p`` in the list that has not
   been eliminated.  If there is no such number, stop.  Otherwise, let
   ``p`` now equal that number (which is the next prime), and repeat
   from step 4.

6. When the algorithm terminates, all entries that have not been
   eliminated are prime; output that list to the screen.





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

A single session of a week-long workshop is an insufficient amount of
time to teach any compiled programming language, so we'll primarily
discuss how to use codes that you've written within a Linux
environment, and provide some links on tutorial pages for two of most
popular/advanced languages for modern high-performance computing (C++
and Fortran90).



Compiling your own codes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In the ``session3`` directory, you will notice a number of files:

.. code-block:: bash

   $ cd session3
   $ ls
   Makefile         hello.c    hello.f    python_example.py
   bash_example.sh  hello.cpp  hello.f90

We've already seen some of these (``bash_example.sh`` and
``python_example.py``); we'll now investigate the ``hello`` files.
These implement the archetypal "Hello world" program in a variety of
languages prevalent within high-performance computing:

* ``hello.c`` -- written in the C programming language

* ``hello.cpp`` -- written in the C++ programming language

* ``hello.f`` -- written in the Fortran-77 programming language

* ``hello.f90`` -- written in the Fortran-90 programming language

Open the file written in your preferred programming language.  If you
have no preference among these, open the C++ version:

.. code-block:: bash

   $ gedit hello.cpp &

Depending on your language of choice, you should see something similar
to the following

.. code-block:: c++

   // Daniel R. Reynolds
   // SMU HPC Workshop
   // 20 May 2013

   // Inclusions
   #include <iostream>

   // Example "hello world" routine
   int main() {

     // print message to stdout
     std::cout << "Hello World!\n";

     return 0;
   }

For those of you familar to the "Windows" (and even OS X's "Xcode")
approach for programming, you're used to seeing this within an
*Integrated Desktop Environment* (IDE), where you enter your code and
click icons that will handle compilation and execution of your program
for you.  While IDEs exist in the Linux world, they are rarely used in
high-performance computing since the compilation approach on your
laptop typically cannot create code that will execute on the
worker-nodes of a cluster.

Hence, we'll now learn the (rather simple) approach for compiling
codes at the command-line in Linux.  

The first step in compilation is knowing which compiler to use.
Nearly every Linux system is installed with the GNU compiler
collection, `GCC <http://gcc.gnu.org/>`_:

* ``gcc`` -- the GNU C compiler

* ``g++`` -- the GNU C++ compiler

* ``gfortran`` -- the GNU Fortran compiler (handles both F77 and F90)

* ``gcj`` -- the GNU Java compiler

However, if you have a very old version of the GNU compiler suite,
instead of ``gfortran`` you may have ``g77``, that only works with F77
code (no F90).

The GNU compiler suite is open-source (i.e. you can modify it if you
want), free, and is available for all major computer architectures
(even Windows); however, it does not always produce the most efficient
code.  As a result, the `SMU Center for Scientific Computation
<http://www.smu.edu/Academics/CSC>`_ has purchased the `PGI
<http://www.pgroup.com/>`_ compiler suite:

* ``pgcc`` - the PGI C compiler

* ``pgc++`` - the PGI C++ compiler

* ``pgfortran`` - the PGI Fortran compiler (both F77 and F90)

In my experience, with some applications a program compiled with the
PGI compilers can run 50% faster than the same code compiled with the
GNU compilers.  We'll discuss how to use the PGI compiler on SMUHPC in
session 4 later today.

To compile an executable, we merely call the relevant compiler,
followed by the files we wish to compile, e.g. for the C code we'd use

.. code-block:: bash

   $ gcc hello.c
   
or for the F77 code we'd use

.. code-block:: bash

   $ gfortran hello.f
   
Both of these commands produce the same output, a new file in the
directory named ``a.out``.  This is the standard output name for
executables produced by compilers.  However, since a computer on which
every program was named "a.out" would be entirely unusable, it is
typical to name your program something more descriptive.  This is
handled with the command line option ``-o``, e.g.

.. code-block:: bash

   $ g++ hello.cpp -o hello.exe

Compile the program in the language of your choice, naming the
executable ``hello.exe``.  Once this has been compiled, you can run it
just like any other Linux program, via

.. code-block:: bash

   $ ./hello.exe


.. note::

   The extension on executable files in Linux can be anything; I just
   choose ".exe" to provide a sense of familiarity for those coming
   from the Windows world.  In fact, all that actually matters for a
   Linux program is that it has "execute" permissions (and that it was
   compiled correctly).  You can verify that the files generated by
   the compiler have the correct permissions via

   .. code-block:: bash

      $ ls -l hello.exe
      -rwxr-xr-x 1 dreynolds math 8.0K May 22 11:32 hello.exe
 
   The three "x" characters in the string of characters a the left of
   the line states state that the program may be executed by the owner
   (dreynolds), the group (math), and others (anyone on the system),
   respectively. 


For those who would like additional information on learning computing
languages, I'd recommend that you pursue some of the following links,
and look through some of the provided code for this workshop.  The
best ways to learn a new language are through following examples and
practicing; if you'd like some programming "homework" for practice,
ask me after class.  Also, `Google <http://google.com>`_ is a great
resource if you're ever in trouble when programming, since the odds
are good that someone else has had the same questions as you, which
have been answered on public forums.  Just describe your question and
do a web search.


Fortran resources:

* `Fortran short-course
  <http://faculty.washington.edu/rjl/classes/am583s2013/notes/index.html#fortran>`_

* `Interactive Fortran 90 Programming Course
  <http://www.liv.ac.uk/HPC/HTMLFrontPageF90.html>`_ 

* `Fortran 90 Tutorial
  <http://www.cs.mtu.edu/~shene/COURSES/cs201/NOTES/fortran.html>`_



C++ resources:

* `C++ Language Tutorial <http://www.cplusplus.com/doc/tutorial/>`_ 

* `Interactive C++ Tutorial (focuses on object-oriented programming)
  <http://www.learncpp.com/>`_






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

   $ mkdir download

Download the software using ``wget`` into your new download directory

.. code-block:: bash

   $ cd download
   $ wget http://faculty.smu.edu/reynolds/unixtut/units-1.74.tar.gz


**Extracting the source code**

List the contents of your download directory 

.. code-block:: bash

   $ ls -l

As you can see, the filename ends in tar.gz. The ``tar`` command turns
several files and directories into one single ".tar" file. This is
then compressed using the ``gzip`` command (to create a ".tar.gz"
file). 

First unzip the file using the ``gunzip`` command. This will create a .tar file

.. code-block:: bash

   $ gunzip units-1.74.tar.gz

Then extract the contents of the tar file.  

.. code-block:: bash

   $ tar -xvf units-1.74.tar

Again, list the contents of the directory, then go to the ``units-1.74`` sub-directory

.. code-block:: bash

   $ ls -l 
   $ cd units-1.74


**Configuring and creating the Makefile**

The first thing to do is carefully read the ``README`` and ``INSTALL``
text files (use the ``less`` command). These contain important
information on how to compile and run the software

.. code-block:: bash

   $ less README

(use the arrow keys to scroll up/down; hit ``q`` to exit).

The ``units`` package uses the GNU configure system to compile the
source code. We will need to specify the installation directory, since
the default will be the main system area which you do not have write
permissions for. We need to create an install directory in your home
directory

.. code-block:: bash

   $ mkdir ~/units-1.7.4

Then run the ``configure`` utility setting the installation path to this

.. code-block:: bash

   $ ./configure --prefix=$HOME/units-1.7.4

NOTE: The ``$HOME`` variable is an example of an environment
variable. The value of ``$HOME`` is the path to your home
directory. Type 

.. code-block:: bash

   $ echo $HOME 

to show the value of this variable.

If ``configure`` has run correctly, it will have created a
``Makefile`` with all necessary options to compile the program.  You
can view the ``Makefile`` if you wish (use the ``less`` command), but do
not edit the contents of this file unless you know what you are doing.


**Building the package**

Now you can go ahead and build the package by running the ``make`` command

.. code-block:: bash

   $ make

After a short while (depending on the speed of the computer), the
executables will be created. You can check to see everything compiled
successfully by typing 

.. code-block:: bash

   $ make check

If everything is okay, you can now install the package.  

.. code-block:: bash

   $ make install

This will install the files into the ``~/units-1.7.4`` directory you created earlier.


**Running the software**

You are now ready to run the software (assuming everything worked).
Unlike most of the commands you have used so far, the new ``units``
executable is not in your ``PATH``, so you cannot run it from your
current directory:

.. code-block:: bash

   $ units

Instead, you must executables that are not in your ``PATH`` by
providing the pathname to the executable.  One option for this is to
provide the path name from your current location, e.g.

.. code-block:: bash

   $ ./units-1.7.4/bin/units

Alternately, you can navigate through the directory structure until
you are in the same directory as the executable,

.. code-block:: bash

   $ cd ~/units-1.7.4

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

   $ ./units

As an example, convert 6 feet to meters,

.. code-block:: bash

   You have: 6 feet
   You want: meters 

   * 1.8288

If you get the answer 1.8288, congratulations, it worked. Type
``^c`` to exit the program.

To view what units the program can convert between, view the data file
in the ``share`` directory (the list is quite comprehensive). 

To read the full documentation, change into the ``info`` directory and type 

.. code-block:: bash

   $ info --file=units.info
