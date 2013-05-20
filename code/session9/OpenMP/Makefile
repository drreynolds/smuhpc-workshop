###################################################################
#  Makefile for Lab 5
#
#  Daniel R. Reynolds
#  SMU Mathematics
#  Math 6370
#  3 February 2011
###################################################################

CXX = icc
CFLAGS = -O2
OMP = -openmp

driver.exe : driver.cpp vectors.cpp Makefile
	$(CXX) $(OMP) $(CFLAGS) driver.cpp vectors.cpp -o $@

clean :
	\rm -f *.o *.out

realclean : clean
	\rm -f *.exe *~


####### End of Makefile #######
