###################################################################
# Makefile for SMU HPC Workshop, session 5 (Advanced programming)
#
# Daniel R. Reynolds
# SMU HPC Workshop
# 21 May 2013
###################################################################

CXX = g++
OPT = -O0

all : driver1.exe driver2.exe

driver1.exe : driver1.cpp vectors.cpp Makefile
	$(CXX) $(OPT) driver1.cpp vectors.cpp -o driver1.exe -lm

driver2.exe : driver2.cpp tridiag_matvec.cpp Makefile
	$(CXX) $(OPT) driver2.cpp tridiag_matvec.cpp -o driver2.exe -lm


####### End of Makefile #######
