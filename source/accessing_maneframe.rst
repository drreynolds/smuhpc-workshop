:tocdepth: 3


.. _accessing_maneframe:

*****************************************************
Accessing ManeFrame
*****************************************************

To log into ManeFrame, you must SSH into one of the login nodes:

* mflogin01.hpc.smu.edu
* mflogin02.hpc.smu.edu
* *(mflogin03.hpc.smu.edu to be added soon)*
* *(mflogin04.hpc.smu.edu to be added soon)*

Your ManeFrame username and password will be associated with your
account, and will be given to you upon account creation.


SSH client programs
-----------------------

There are many SSH client programs that can be used to access any
Linux server (including ManeFrame), but these programs depend on your
operating system.


Linux/OS X
^^^^^^^^^^^^^^

Both Linux and OS X are variants of the UNIX operating system, and
natively support SSH access from the command line.  First, open a
*terminal* to access the command line.  In OS X this is located in
``/Applications/Utilities/Terminal.app`` (I recommend making a shortcut
to this in your dock).  Depending on your Linux variant, this can be
in a variety of locations; if you cannot find it I recommend you
search on the web.

In your terminal, you can SSH into ManeFrame with the command

.. code-block:: bash

   ssh -CX yourusername@mflogin01.hpc.smu.edu

or 

.. code-block:: bash

   ssh -CY yourusername@mflogin02.hpc.smu.edu

.. note:: Replace ``yourusername`` with the username for your account
	  on ManeFrame.  The flag ``C`` enables compression of data
	  across the network (helpful when running graphical
	  programs).  The flags ``X`` and ``Y`` both enable forwarding
	  of graphics from ManeFrame to your computer.

	  Depending on your version of OS X, you may need an auxiliary
	  program to render the graphics that are sent from ManeFrame
	  to your computer.  If you have difficulty viewing graphics
	  (try running ``gedit`` just after logging into ManeFrame),
	  then visit the App Store and install one of the free
	  programs "X11" or "XQuartz".


Windows
^^^^^^^^^^^^^^

Windows does not natively support SSH, but there exist free programs
that you can install.  To set up a full-featured SSH client on your computer:

1. `Putty <http://www.putty.org>`_ is the most popular (and free) SSH
   client available.  Download and install the newest version.

2. In order to view graphics from ManeFrame, you will also need to
   install an X-server.  The most popular (again free) X server in
   Windows is Xming.  To install Xming, download and install the
   newest versions of both the `Xming X server
   <http://sourceforge.net/projects/xming>`_ and the `Xming Fonts
   <http://sourceforge.net/projects/xming/files/Xming-fonts>`_. 

Once these two programs are installed, you can now log into
ManeFrame using the two-step process:

1. Start the X server application from your start up programs. This
   will launch the X server and the following icon will appear in your
   system tray:

   .. figure:: figs/X.png
      :scale: 100%
      :align: center


2. Launch putty.exe. And follow the screens below to login to
   ManeFrame:

   In the "Host Name" field you enter the Linux server you wish to
   log into: ``mflogin01.hpc.smu.edu`` or ``mflogin02.hpc.smu.edu``

   .. figure:: figs/Screen1.png
      :scale: 100%
      :align: center

   Expand the Category "SSH" on the left and select "X11".  Check
   the box next to "Enable X11 Forwarding".  

   In the left panel, click on "Session" at the top.  

   If desired, save these settings (middle right) so
   that you do not need to repeat these steps in the future.

   Click Open (bottom).
   

   .. figure:: figs/Screen2.png
      :scale: 100%
      :align: center

   Enter your username and password in the prompts and you will see
   a similar screen setting up your environment.

   .. figure:: figs/Screen3.png
      :scale: 100%
      :align: center






.. raw:: html
   :file: counter.html

