###################################################################
#  Makefile for Example: 2D wave equation
#
#  Daniel R. Reynolds
#  SMU HPC Workshop
#  20 May 2013
###################################################################

# compilers/flags 
CXX      = g++
CXXFLAGS = -O2


# makefile targets
advection.exe : advection.cpp output.cpp initialize.cpp
	$(CXX) $(CXXFLAGS) $^ -lm -o $@

clean :
	\rm -f *.o u_sol*

realclean : clean
	\rm -f *.exe *~ 


####### End of Makefile #######
